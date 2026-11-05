#!/usr/bin/env python3
"""Deterministic Sinnoh source import from a pinned local GBA Platinum checkout.

Audit and stage are non-destructive. Install copies only missing files; it never
overwrites a Grand Anthology resource. Registry activation is a separate gate.
"""
import argparse
import hashlib
import json
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MANIFEST = Path(__file__).with_name("pinned_source_inventory.json")

def blob_hash(data):
    return hashlib.sha1(b"blob " + str(len(data)).encode("ascii") + b"\0" + data).hexdigest()

def read_json(path):
    return json.loads(path.read_text(encoding="utf-8"))

def git_head(folder):
    result = subprocess.run(["git", "-C", str(folder), "rev-parse", "HEAD"],
                            capture_output=True, text=True, check=True)
    return result.stdout.strip()

def safe_child(root, path):
    relative = Path(path)
    if relative.is_absolute() or not relative.parts or ".." in relative.parts:
        raise ValueError("Unsafe path: " + str(path))
    result = root / relative
    if not result.resolve().is_relative_to(root.resolve()):
        raise ValueError("Path escapes checkout: " + str(path))
    return result

def check_existing_world(target, inventory):
    current = read_json(target / "data/maps/map_groups.json")
    old_groups = inventory["target"]["legacy_groups"]
    if current["group_order"][:len(old_groups)] != old_groups:
        raise ValueError("Legacy group-order modification")
    old_count = sum(len(current[g]) for g in old_groups)
    if old_count != inventory["target"]["legacy_map_count"]:
        raise ValueError("Legacy map-count modification")
    original_names = set()
    for group in old_groups:
        original_names.update(current[group])
    source_names = []
    for group in inventory["group_import"]:
        source_names.extend(group["maps"])
        if group["upstream_group"] in current and current[group["upstream_group"]] != group["maps"]:
            raise ValueError("Existing group has different maps: " + group["upstream_group"])
    if len(source_names) != 93 or len(set(source_names)) != 93:
        raise ValueError("Source map inventory not unique or incomplete")
    if original_names.intersection(source_names):
        raise ValueError("Sinnoh map already occupies a legacy slot")
    current_layouts = read_json(target / "data/layouts/layouts.json")
    layouts_by_id = {layout["id"]: layout for layout in current_layouts["layouts"]}
    for layout in inventory["layout_import"]:
        if layout["id"] in layouts_by_id:
            raise ValueError("New layout ID already in target: " + layout["id"])
    return current, current_layouts

def chosen_files(inventory, assets):
    for record in inventory["files"]:
        if record["kind"] in ("map", "layout"):
            yield record
        elif assets and record["kind"] == "asset":
            yield record

def check_source(upstream, inventory, assets):
    source_revision = git_head(upstream)
    if source_revision != inventory["upstream"]["commit"]:
        raise ValueError("Wrong upstream revision: " + source_revision)
    records = list(chosen_files(inventory, assets))
    for record in records:
        path = safe_child(upstream, record["path"])
        if path.is_symlink() or not path.is_file():
            raise ValueError("Missing or linked source: " + record["path"])
        content = path.read_bytes()
        if blob_hash(content) != record["git_blob"]:
            # Git attributes convert .pal/.ps1 to CRLF in the checkout. Hash
            # the canonical, clean-filtered blob, not the worktree bytes.
            check = subprocess.run(
                ["git", "-C", str(upstream), "hash-object",
                 "--path=" + record["path"], "--", record["path"]],
                capture_output=True, text=True, check=True,
            )
            if check.stdout.strip() != record["git_blob"]:
                raise ValueError("Source hash mismatch: " + record["path"])
    return records

def map_summary(inventory):
    return {
        "map_groups": len(inventory["group_import"]),
        "maps": sum(len(g["maps"]) for g in inventory["group_import"]),
        "layouts": len(inventory["layout_import"]),
        "asset_candidates": sum(r["kind"] == "asset" for r in inventory["files"]),
        "upstream": inventory["upstream"]["commit"],
    }

def stage(upstream, destination, records):
    if destination.exists() and not destination.is_dir():
        raise ValueError("Stage destination is not a directory")
    for record in records:
        path = safe_child(destination, record["path"])
        original = safe_child(upstream, record["path"])
        if path.exists():
            if path.read_bytes() != original.read_bytes():
                raise ValueError("Stage conflict: " + record["path"])
            continue
        path.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(original, path)

def install_files(upstream, target, records):
    """Copy only brand-new paths; never overwrite any existing target file.

    Verify every collision before modifying the worktree.
    """
    creates = []
    already = []
    for record in records:
        source = safe_child(upstream, record["path"])
        destination = safe_child(target, record["path"])
        if destination.exists():
            if destination.is_file() and blob_hash(destination.read_bytes()) == record["git_blob"]:
                already.append(record["path"])
                continue
            raise ValueError("REFUSING shared-path overwrite: " + record["path"])
        creates.append((source, destination))
    for source, destination in creates:
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, destination)
    return len(creates), len(already)

def generate_registries(inventory, current_groups, current_layouts):
    result_groups = json.loads(json.dumps(current_groups))
    for group in inventory["group_import"]:
        name = group["upstream_group"]
        if name not in result_groups:
            result_groups["group_order"].append(name)
            result_groups[name] = group["maps"]
    result_layouts = json.loads(json.dumps(current_layouts))
    for layout in inventory["layout_import"]:
        source = dict(layout)
        source["border_width"] = 0
        source["border_height"] = 0
        source["layout_version"] = "emerald"
        result_layouts["layouts"].append(source)
    return result_groups, result_layouts

def write_registry_proposal(target, output, inventory, world, layouts):
    """Propose actual registry modifications, without enabling incomplete maps."""
    groups, merged_layouts = generate_registries(inventory, world, layouts)
    output.mkdir(parents=True, exist_ok=True)
    (output / "map_groups.json").write_text(json.dumps(groups, indent=2) + "\n")
    (output / "layouts.json").write_text(json.dumps(merged_layouts, indent=2) + "\n")
    # The proposal must be reviewed with symbol, collision, map and resource audits.
    return len(groups["group_order"]), len(merged_layouts["layouts"])

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--upstream", type=Path, required=True)
    parser.add_argument("--target", type=Path, default=ROOT)
    parser.add_argument("--mode", choices=("audit", "stage", "install"), default="audit")
    parser.add_argument("--assets", action="store_true",
                        help="Include source-only art/tilesets/audio/animation candidates")
    parser.add_argument("--output", type=Path,
                        help="Stage path (default target/build/sinnoh-stage)")
    args = parser.parse_args()
    upstream = args.upstream.resolve()
    target = args.target.resolve()
    inventory = read_json(MANIFEST)
    world, layouts = check_existing_world(target, inventory)
    records = check_source(upstream, inventory, args.assets)
    report = map_summary(inventory)
    report["verified_files"] = len(records)
    report["mode"] = args.mode
    if args.mode == "stage":
        destination = (args.output or (target / "build/sinnoh-stage")).resolve()
        if destination == target or destination == upstream:
            raise ValueError("Invalid stage destination")
        stage(upstream, destination, records)
        proposal = destination / "_registry_proposal"
        count, layout_count = write_registry_proposal(
            target, proposal, inventory, world, layouts
        )
        report.update({"stage": str(destination),
                       "proposed_groups": count, "proposed_layouts": layout_count})
    elif args.mode == "install":
        # Deliberately DO NOT replace the target registry or the save/battle engine.
        # These files stay unregistered until symbol/graphics/state mapping is safe.
        created, already = install_files(upstream, target, records)
        report.update({"created": created, "already_identical": already,
                       "activated": False})
    print(json.dumps(report, indent=2, sort_keys=True))

if __name__ == "__main__":
    try:
        main()
    except (ValueError, OSError, subprocess.CalledProcessError) as error:
        print("Sinnoh import rejected:", error, file=sys.stderr)
        sys.exit(1)
