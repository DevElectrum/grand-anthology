#!/usr/bin/env python3
"""Prepare a fresh-history source candidate; never publish or touch Git refs.

Examples:
  python3 tools/public_source_snapshot.py --report /tmp/public-candidate.json
  python3 tools/public_source_snapshot.py --export /tmp/pga-clean-source
The output is a *working tree*, not a Git clone and not a release authorization.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
ROOT_FILES = {
    ".gitattributes", ".gitignore", "ANTHOLOGY_CHANGELOG.md",
    "CHANGELOG.md", "CONTRIBUTING.md", "CREDITS.md", "FEATURES.md",
    "INSTALL.md", "Makefile", "README.md", "THIRD_PARTY_NOTICES.md",
    "audio_rules.mk", "charmap.txt", "check_history.sh",
    "compile_flags.txt", "config.mk", "graphics_file_rules.mk",
    "json_data_rules.mk", "ld_script_modern.ld", "ld_script_test.ld",
    "make_tools.mk", "map_data_rules.mk", "porymap.project.cfg",
    "spritesheet_rules.mk", "trainer_rules.mk",
}
BUILD_DIRS = {
    "asm", "constants", "data", "graphics", "include", "libagbsyscall",
    "sound", "src", "test", "tools",
}
PUBLIC_DOCS = {"docs/PUBLIC_STATUS.md", "docs/story-progress/README.md"}
PUBLIC_WORKFLOWS = {
    ".github/workflows/Gameplay-P0.yml",
    ".github/workflows/P0-mGBA-Release-Smoke.yml",
}
PUBLIC_ARTWORK = {".github/assets/grand-anthology-readme-banner.jpg"}
NEVER_FILES = {
    "AGENTS.md", "RELEASING.md", "tools/audit_actions_privacy.py", "tools/audit_git_history_privacy.py",
    "tools/audit_history_privacy.py", "tools/audit_historical_privacy.py",
    "test/overworld/MakeMigrationFixtures.sh", "tools/mgba/mgba-rom-test",
}
PRIVATE_PARTS = {"maintenance", "native", "evidence", ".git", "__pycache__", ".pytest_cache", ".venv"}
PRIVATE_SUFFIXES = {
    ".gba", ".sav", ".srm", ".sgm", ".state", ".log", ".exe", ".dll",
    ".dylib", ".so", ".o", ".a", ".elf", ".map", ".zip", ".7z",
    ".rar", ".tar", ".tgz", ".gz", ".bak", ".core", ".pyc",
}
REQUIRED = {
    "README.md", "CREDITS.md", "INSTALL.md", "FEATURES.md", "THIRD_PARTY_NOTICES.md",
    "Makefile", "make_tools.mk", "config.mk", "charmap.txt",
    "data/maps/map_groups.json",
    "test/overworld/mgba/0001-headless-video-buffer-save-and-rtc.patch",
    "test/overworld/GenLuaSymbols.py", "test/overworld/lua/WorldDiancieStatic.lua",
    "tools/public_source_snapshot.py",
    *PUBLIC_DOCS, *PUBLIC_WORKFLOWS, *PUBLIC_ARTWORK,
}
CREDENTIAL_PATTERNS = (
    re.compile(rb"gh[pousr]_[A-Za-z0-9]{20,}"),
    re.compile(rb"github_pat_[A-Za-z0-9_]{20,}"),
    re.compile(rb"sk-(?:proj-)?[A-Za-z0-9_-]{24,}"),
    re.compile(rb"AKIA[A-Z0-9]{16}"),
    re.compile(rb"-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----"),
    re.compile(rb"xox[baprs]-[A-Za-z0-9-]{20,}"),
)
HOME = re.compile(rb"(?:/(?:Users|home)/|(?:[A-Za-z]:[/\\\\]|/mnt/[a-z]/)Users[/\\\\])([A-Za-z0-9._-]{2,})[/\\\\]")
LINK = re.compile(r"!?\[[^\]]*\]\(([^)]+)\)")
MARKDOWN_ENTRYPOINTS = {
    "README.md", "FEATURES.md", "INSTALL.md", "CONTRIBUTING.md",
    "THIRD_PARTY_NOTICES.md",
    "CREDITS.md", "ANTHOLOGY_CHANGELOG.md", "docs/PUBLIC_STATUS.md",
    "docs/story-progress/README.md",
}


def classify(path):
    p = PurePosixPath(path)
    if (path in NEVER_FILES or p.parts[0] in PRIVATE_PARTS
            or any(x in PRIVATE_PARTS for x in p.parts)
            or p.suffix.lower() in PRIVATE_SUFFIXES or p.name.endswith(".tar.xz")
            or p.name.startswith(".DS_Store")):
        return "private-or-generated"
    if path in ROOT_FILES or path in PUBLIC_DOCS or path in PUBLIC_WORKFLOWS or path in PUBLIC_ARTWORK:
        return "include"
    if p.parts[0] == "docs" and len(p.parts) > 2 and p.parts[1] == "story-progress" and p.suffix == ".md":
        return "include"
    if p.parts[0] in BUILD_DIRS:
        return "include"
    return "not-allowlisted"


def git_files():
    result = subprocess.run(
        ["git", "-C", str(ROOT), "ls-files", "-z"],
        check=True, stdout=subprocess.PIPE,
    ).stdout
    return sorted(x.decode("utf-8") for x in result.split(b"\0") if x)


def scan_text(path, data):
    issues = []
    if b"\0" in data[:8192]:
        return issues
    if path.endswith(".md") and b"github.com/DevElectrum/pokemon-grand-anthology" in data:
        issues.append("private-development-repository-link")
    for pattern in CREDENTIAL_PATTERNS:
        if pattern.search(data):
            issues.append("credential-signature")
            break
    for match in HOME.finditer(data):
        if match.group(1).lower() not in (b"runner", b"user", b"username", b"example"):
            issues.append("local-home-path")
            break
    return issues


def link_failures(included):
    missing = []
    for path in sorted(MARKDOWN_ENTRYPOINTS & included):
        base = PurePosixPath(path).parent
        data = (ROOT / path).read_text(encoding="utf-8", errors="replace")
        for raw in LINK.findall(data):
            target = raw.split("#", 1)[0].split("?", 1)[0].strip().split(" ", 1)[0]
            if not target or "://" in target or target.startswith(("mailto:", "/", "data:")):
                continue
            resolved = os.path.normpath(str(base / target)).replace("\\", "/")
            if resolved not in included:
                missing.append({"document": path, "target": target})
    return missing


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--export", type=Path, help="new EMPTY directory outside checkout")
    parser.add_argument("--report", type=Path, help="write safe JSON audit and sha256 manifest")
    args = parser.parse_args()
    paths = git_files()
    selected = set()
    excluded = {}
    issues = []
    manifest = []
    for path in paths:
        reason = classify(path)
        if reason != "include":
            excluded[reason] = excluded.get(reason, 0) + 1
            continue
        src = ROOT / path
        if src.is_symlink() or not src.is_file():
            issues.append({"file": path, "kind": "symlink-or-unreadable"})
            continue
        data = src.read_bytes()
        for problem in scan_text(path, data):
            issues.append({"file": path, "kind": problem})
        if data[:4] in (b"\x7fELF", b"PK\x03\x04") or data[:2] == b"MZ":
            issues.append({"file": path, "kind": "embedded-executable-or-archive"})
        selected.add(path)
        manifest.append({
            "path": path, "bytes": len(data),
            "sha256": hashlib.sha256(data).hexdigest(),
        })
    missing = sorted(REQUIRED - selected)
    for path in missing:
        issues.append({"file": path, "kind": "missing-required-build-or-credit-input"})
    links = link_failures(selected)
    for problem in links:
        issues.append({"file": problem["document"], "kind": "broken-relative-link", "target": problem["target"]})
    notices = sorted(p for p in selected if PurePosixPath(p).name.lower() in
                     ("license", "license.md", "license.txt", "copying", "copying.md", "copying.txt"))
    if len(notices) < 9:
        issues.append({"file": "(multiple)", "kind": "fewer-than-nine-third-party-license-notices"})
    report = {
        "source": "tracked working tree, no Git history",
        "tracked_count": len(paths), "included_count": len(selected),
        "excluded": excluded, "license_notices": notices,
        "required_missing": missing, "relative_links_missing": links,
        "issues": issues, "manifest": manifest,
    }
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2, ensure_ascii=False) + "\n")
    print(json.dumps({k: v for k, v in report.items() if k not in ("manifest", "license_notices")},
                     indent=2))
    if issues:
        print("STOP: candidate failed review; no export was written", file=sys.stderr)
        return 1
    if args.export:
        dest = args.export.resolve()
        if dest == ROOT or ROOT in dest.parents or dest in ROOT.parents:
            parser.error("--export must be outside the source checkout")
        if dest.exists() and any(dest.iterdir()):
            parser.error("--export destination must be empty")
        dest.mkdir(parents=True, exist_ok=True)
        for path in sorted(selected):
            target = dest / path
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(ROOT / path, target)
        print(f"Exported {len(selected)} tracked source files to {dest} with no .git history")
    return 0


if __name__ == "__main__":
    sys.exit(main())
