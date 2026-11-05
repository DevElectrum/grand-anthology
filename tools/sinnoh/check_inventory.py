#!/usr/bin/env python3
"""Check a pinned Sinnoh source inventory without copying or linking assets."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

MANIFEST = Path(__file__).with_name("pinned_source_inventory.json")


def git_blob_sha(data):
    header = ("blob " + str(len(data))).encode("ascii") + bytes([0])
    return hashlib.sha1(header + data).hexdigest()


def load_json(path):
    with path.open("r", encoding="utf-8") as handle:
        return json.load(handle)


def get_head(root):
    result = subprocess.run(
        ["git", "-C", str(root), "rev-parse", "HEAD"],
        capture_output=True,
        text=True,
        check=True,
    )
    return result.stdout.strip()


def check_legacy_groups(manifest, target):
    path = target / "data/maps/map_groups.json"
    current = load_json(path)
    expected = manifest["target"]["legacy_groups"]
    assert current["group_order"][:len(expected)] == expected, "Legacy map-group IDs changed"
    count = 0
    for name in expected:
        count += len(current[name])
    assert count == manifest["target"]["legacy_map_count"], "Legacy map counts changed"


def run(upstream, target, all_files):
    manifest = load_json(MANIFEST)
    assert get_head(upstream) == manifest["upstream"]["commit"], "Wrong upstream commit"
    check_legacy_groups(manifest, target)
    groups = manifest["group_import"]
    assert len(groups) == 13, "Expected 13 source map groups"
    count = 0
    names = set()
    for group in groups:
        for map_name in group["maps"]:
            count += 1
            assert map_name not in names, "Duplicate map " + map_name
            names.add(map_name)
    assert count == 93, "Expected 93 source maps"
    assert len(manifest["layout_import"]) == 64, "Expected 64 new layouts"
    records = manifest["files"]
    verified = 0
    for record in records:
        if not all_files and record["kind"] == "asset":
            continue
        path = Path(record["path"])
        assert not path.is_absolute() and ".." not in path.parts, "Unsafe source path"
        source_path = upstream / path
        assert source_path.is_file(), "Missing source " + str(path)
        raw = source_path.read_bytes()
        if git_blob_sha(raw) != record["git_blob"]:
            # .gitattributes may normalize CRLF palettes on checkout.
            normalized = subprocess.run(
                ["git", "-C", str(upstream), "hash-object",
                 "--path=" + str(path), "--", str(path)],
                capture_output=True, text=True, check=True,
            )
            assert normalized.stdout.strip() == record["git_blob"], "Blob changed: " + str(path)
        verified += 1
    print("PASS: pinned Sinnoh inventory, legacy IDs, and", verified, "source blobs")
    print("Nothing was copied or modified.")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--upstream", type=Path, required=True)
    parser.add_argument("--target", type=Path, required=True)
    parser.add_argument("--all-files", action="store_true")
    args = parser.parse_args()
    run(args.upstream, args.target, args.all_files)


if __name__ == "__main__":
    main()
