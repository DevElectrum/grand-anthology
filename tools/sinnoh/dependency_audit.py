#!/usr/bin/env python3
"""Audit actual Sinnoh map-script symbol use against the destination engine.

No source files are copied, linked, or rewritten by this tool.
"""
import argparse
import json
import re
from collections import defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent
MANIFEST = HERE / "pinned_source_inventory.json"
TOKEN = re.compile(
    r"\b(?:FLAG|VAR|TRAINER|MUS|SE|OBJ_EVENT_GFX|MAPSEC|LAYOUT|MAP|ITEM|"
    r"SPECIES|MOVE|SPECIAL|LOCALID)_[A-Z0-9_]+\b"
)
BADGE = re.compile(r"FLAG_BADGE0[1-8]_GET$")
CATEGORIES = ("FLAG", "VAR", "TRAINER", "MUS", "SE", "OBJ_EVENT_GFX",
              "MAPSEC", "LAYOUT", "MAP", "ITEM", "SPECIES", "MOVE",
              "SPECIAL", "LOCALID")

def tokens(text):
    return set(TOKEN.findall(text))

def family(value):
    if value.startswith("OBJ_EVENT_GFX_"):
        return "OBJ_EVENT_GFX"
    for name in CATEGORIES:
        if value.startswith(name + "_"):
            return name
    return "other"

def scan(upstream, target):
    inventory = json.loads(MANIFEST.read_text(encoding="utf-8"))
    source_records = [item for item in inventory["files"] if item["kind"] == "map"]
    records = {}
    for item in source_records:
        text = (upstream / item["path"]).read_text(encoding="utf-8")
        used = tokens(text)
        if item["path"].endswith("/map.json"):
            doc = json.loads(text)
            used.update(tokens(json.dumps(doc)))
        records[item["path"]] = sorted(used)

    references = defaultdict(set)
    for path, names in records.items():
        for name in names:
            references[name].add(path)

    # Discover existing constants without interpreting their numeric values.
    # A defined name in the target might still mean a DIFFERENT regional event.
    existing = set()
    const_dir = target / "include/constants"
    for path in sorted(const_dir.glob("*.h")):
        existing.update(tokens(path.read_text(encoding="utf-8", errors="replace")))
    for path in (target / "include").glob("*.h"):
        existing.update(tokens(path.read_text(encoding="utf-8", errors="replace")))

    upstream_map_names = set()
    for group in inventory["group_import"]:
        for name in group["maps"]:
            upstream_map_names.add(name)
    source_maps = set()
    for path in records:
        if path.endswith("/map.json"):
            doc = json.loads((upstream / path).read_text(encoding="utf-8"))
            source_maps.add(doc["id"])
    grouped = defaultdict(list)
    for name in sorted(references):
        grouped[family(name)].append(name)

    explicit_unsafe = sorted(
        name for name in references
        if BADGE.match(name)
        or name == "VAR_STARTER_MON"
        or name.startswith("FLAG_BADGE")
    )
    existing_flag_var = sorted(
        name for name in references
        if name in existing and family(name) in ("FLAG", "VAR")
    )
    missing_symbols = defaultdict(list)
    for name in sorted(references):
        cat = family(name)
        # New MAP/LAYOUT symbols and local IDs intentionally come from this import.
        if cat in ("MAP", "LAYOUT", "LOCALID"):
            continue
        if name not in existing:
            missing_symbols[cat].append(name)

    return {
        "source_commit": inventory["upstream"]["commit"],
        "source_map_files": len(source_records),
        "source_maps": len(upstream_map_names),
        "source_map_ids": len(source_maps),
        "unique_references": len(references),
        "symbol_counts": {key: len(values) for key, values in sorted(grouped.items())},
        "dangerous_hoenn_badge_or_starter_aliases": explicit_unsafe,
        "existing_destination_flags_or_vars_needing_semantic_review": existing_flag_var,
        "not_found_in_destination_header_scan": dict(sorted(missing_symbols.items())),
        "reference_sites": {
            name: sorted(files)
            for name, files in sorted(references.items())
        }
    }

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--upstream", required=True, type=Path)
    parser.add_argument("--target", type=Path, default=HERE.parents[1])
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    data = scan(args.upstream.resolve(), args.target.resolve())
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(data, indent=2, sort_keys=True) + "\n")
    view = {k: v for k, v in data.items() if k != "reference_sites"}
    print(json.dumps(view, indent=2, sort_keys=True))
    if data["source_maps"] != 93 or data["source_map_files"] != 186:
        raise SystemExit("ERROR: incomplete Sinnoh map source")

if __name__ == "__main__":
    main()
