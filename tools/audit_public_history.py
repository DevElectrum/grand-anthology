#!/usr/bin/env python3
"""Read-only scan of all history blobs reachable from local refs.

Requires a full Git clone. Redacts secrets and local home-directory names.
Not a legal review or a replacement for specialized secret scanning.
"""
import argparse
from collections import Counter
import hashlib
import re
import subprocess
import sys

def git(*args):
    return subprocess.check_output(["git", *args], stderr=subprocess.PIPE)

PATS = {
 "github_token":rb"\b(?:gh[pousr]_[A-Za-z0-9_]{20,}|github_pat_[A-Za-z0-9_]{20,})\b",
 "openai_key":rb"\bsk-(?:proj-)?[A-Za-z0-9_-]{28,}\b",
 "aws_key":rb"\b(?:AKIA|ASIA)[A-Z0-9]{16}\b",
 "private_key":rb"-----BEGIN (?:RSA |EC |OPENSSH |DSA )?PRIVATE KEY-----",
 "google_key":rb"\bAIza[0-9A-Za-z_-]{35}\b",
 "slack_token":rb"\bxox(?:b|p|a|s)-[0-9A-Za-z-]{20,}\b",
 "generic_assignment":rb"(?im)^\s*(?:export\s+)?[A-Z0-9_]*(?:PASSWORD|PASSWD|API_KEY|ACCESS_TOKEN|AUTH_TOKEN|SECRET_KEY)\s*[:=]\s*[\x22\x27]?[A-Za-z0-9_+/=.-]{20,}",
}
PATTERNS={k:re.compile(v) for k,v in PATS.items()}
ROM_SAVE=re.compile(r"\.(?:gba|gbc|gb|nds|3ds|cia|sav|sgm|sa1|ss[0-9]|sna)$",re.I)
SENSITIVE_NAME=re.compile(r"(?:^|/)(?:\.env(?:\..*)?|id_rsa|id_ed25519|credentials?\.[^/]+|tokens?\.[^/]+|[^/]+\.(?:pem|p12|pfx|kdbx))$",re.I)
ARCHIVE=re.compile(r"\.(?:zip|7z|rar|tar|tgz)$",re.I)
HOME=re.compile(rb"(?<![\w])/(?:Users|home)/([A-Za-z0-9_.-]+)(?:/|$)")

def fid(path):
    return hashlib.sha256(path.encode("utf8","replace")).hexdigest()[:12]

def main():
    cli=argparse.ArgumentParser(description=__doc__)
    cli.add_argument("--strict",action="store_true")
    cli.add_argument("--triage",action="store_true",
                     help="Print redacted file classifications, never matched values or home names")
    args=cli.parse_args()
    strict=args.strict
    triage=args.triage
    try:
        if git("rev-parse","--is-shallow-repository").strip()!=b"false":
            print("ERROR: shallow checkout; history scan incomplete");return 2
        items=git("rev-list","--objects","--all").splitlines()
    except subprocess.CalledProcessError:
        print("ERROR: cannot enumerate Git history");return 2
    files={}
    for line in items:
        sha,sep,name=line.partition(b" ")
        if sep: files.setdefault(sha.decode("ascii"),set()).add(name.decode("utf8","replace"))
    issues=set();stats=Counter()
    summaries={}
    baseline_objects=set()
    if triage:
        # Was a blob reachable from the inherited, unchanged master baseline?
        # This attribution matters more than treating every finding as new.
        try:
            baseline_objects={row.split(b" ",1)[0].decode("ascii")
                              for row in git("rev-list","--objects","master").splitlines()}
        except subprocess.CalledProcessError:
            print("WARNING: master baseline unavailable; origin shown as unknown")
    def record(kind, path, oid, size, detail):
        key=(kind,fid(path))
        issues.add(key)
        if triage:
            top=path.split("/",1)[0] if "/" in path else "(root)"
            if top not in ("test","tools","docs","maintenance","src","data",
                           "graphics","sound","include","native",".github"):
                top="(other)"
            origin=("inherited" if oid in baseline_objects
                    else "anthology_or_unknown")
            summaries[key]=(top, size, origin, detail)
    proc=subprocess.Popen(["git","cat-file","--batch"],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.DEVNULL,bufsize=1048576)
    assert proc.stdin and proc.stdout
    try:
        for sha,names in files.items():
            proc.stdin.write((sha+"\n").encode("ascii"));proc.stdin.flush()
            hdr=proc.stdout.readline().split()
            if len(hdr)<3 or hdr[1]==b"missing":raise RuntimeError("missing object")
            typ,size=hdr[1],int(hdr[2])
            body=proc.stdout.read(size+1)
            if len(body)!=size+1 or not body.endswith(b"\n"):raise RuntimeError("short read")
            if typ!=b"blob":continue
            stats["blobs"]+=1
            for path in names:
                if ROM_SAVE.search(path):
                    suffix=path.rsplit(".",1)[-1].lower()
                    # A game-data filename without spaces, @, or punctuation
                    # is safe to print in a PRIVATE triage log. Never print
                    # matched blob bytes or developer-machine home paths.
                    label=(" name="+path.rsplit("/",1)[-1]
                           if path.startswith("data/")
                           and len(path)<128
                           and re.fullmatch(r"[A-Za-z0-9_./-]+",path)
                           else "")
                    record("ROM_or_save",path,sha,size,
                           "extension="+suffix+label)
                if SENSITIVE_NAME.search(path) and "graphics/items/" not in path:
                    record("sensitive_filename",path,sha,size,"sensitive_filename")
                if ARCHIVE.search(path):
                    record("archive",path,sha,size,"archive")
            if size>5242880:
                stats["unscanned_oversize"]+=1;continue
            stats["inspected"]+=1
            binary=b"\x00" in body[:8192]
            if binary:stats["binary"]+=1
            else:
                users={x.group(1).lower() for x in HOME.finditer(body)}
                non_system=users-{b"runner",b"ubuntu",b"root",b"oai"}
                if non_system:
                    src=min(names)
                    os_style=("mac" if b"/Users/" in body else "linux")
                    record("local_home_path",src,sha,size,
                           "host_style="+os_style+" identities="+str(len(non_system)))
            for kind,pattern in PATTERNS.items():
                if binary and kind=="generic_assignment":continue
                if pattern.search(body):
                    record(kind,min(names),sha,size,"credential_signature")
    except Exception as error:
        print("ERROR: incomplete history scan:",type(error).__name__);return 2
    finally:
        proc.stdin.close();proc.stdout.close();proc.terminate()
        try:proc.wait(timeout=3)
        except subprocess.TimeoutExpired:proc.kill();proc.wait()
    counts=Counter(kind for kind,_ in issues)
    print("FULL-REF HISTORICAL GIT BLOB SCAN")
    print("Git history objects:",len(items),"path-bearing hashes:",len(files))
    print("Blob objects:",stats["blobs"],"inspected:",stats["inspected"],"binary:",stats["binary"])
    print("Too-large blobs uninspected:",stats["unscanned_oversize"])
    for kind in sorted(counts):
        print(kind,counts[kind],"path hashes:",",".join(sorted(path for k,path in issues if k==kind)[:8]))
    if triage:
        print("REDACTED HISTORICAL FINDING TRIAGE")
        for (kind,path_hash),(top,size,origin,detail) in sorted(summaries.items()):
            print(f"  {kind} id={path_hash} group={top} blob_bytes={size} "
                  f"origin={origin} {detail}")
    print("No matched tokens or home names printed; privately review findings before publication.")
    blocking=set(PATTERNS)|{"ROM_or_save","sensitive_filename","local_home_path"}
    return 1 if strict and (stats["unscanned_oversize"] or blocking.intersection(counts)) else 0

if __name__=="__main__":
    sys.exit(main())
