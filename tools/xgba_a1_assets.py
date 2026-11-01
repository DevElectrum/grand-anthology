#!/usr/bin/env python3
"""A1: deterministic XGBA map relocation, native symbol generation and ROM packer.

The ordinary linker remains within 32 MiB. The generated MapLayout metadata
references high-ROM pointers for *all* map and border files. No save changes.
"""
import argparse
import hashlib
import json
import re
import subprocess
import struct
import sys
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LAYOUTS = ROOT / "data/layouts/layouts.json"
ROM_BYTES = 0x10000000
ROM_NATIVE_LIMIT = 0x02000000
XGBA_CPU_BASE = 0x10000000
LANES = {
    "emerald": (0x04000000, 0x08000000),
    "frlg": (0x08000000, 0x0C000000),
    "johto": (0x0C000000, 0x0F000000),
    "hub": (0x0FF00000, ROM_BYTES - 0x100),
}
MAGIC = b"XGBA256"
PROBE_PREFIX = b"XGBAA1!!"
PROBE_OFFSET = ROM_BYTES - 16
LABEL_RE = re.compile(
    r'(?m)^([A-Za-z_][A-Za-z0-9_]*)::\n[ \t]*\.incbin[ \t]+"([^"]+)"[ \t]*\n'
)


def align(value, alignment=4):
    return (value + alignment - 1) & ~(alignment - 1)


def manifest_plan():
    definition = json.loads(LAYOUTS.read_text())
    layouts = definition["layouts"]
    if len(layouts) < 1000:
        raise ValueError("A1 requires all existing regional map layouts")
    next_offset = {lane: bounds[0] for lane, bounds in LANES.items()}
    used_symbols = set()
    result = []
    for layout in layouts:
        name = layout["name"]
        lane = "hub" if layout["id"] == "LAYOUT_REGION_HUB" else layout["layout_version"]
        if lane not in LANES:
            raise ValueError(f"Unknown layout format: {name} ({lane})")
        width = layout["width"]
        height = layout["height"]
        if width <= 0 or height <= 0 or width > 65535 or height > 65535:
            raise ValueError(f"Invalid map dimensions for {name}")
        for suffix, source_key in (("Border", "border_filepath"), ("Blockdata", "blockdata_filepath")):
            label = name + "_" + suffix
            if label in used_symbols:
                raise ValueError(f"Duplicate linker symbol: {label}")
            used_symbols.add(label)
            source = ROOT / layout[source_key]
            if not source.is_file():
                raise ValueError(f"Missing layout source: {source}")
            size = source.stat().st_size
            # Some original 1x1/odd-area layouts include assembler word padding.
            # Keep the exact source blob, but allow only zero-padded 4-byte alignment.
            expected = width * height * 2
            if suffix == "Blockdata" and size not in (expected, align(expected)):
                raise ValueError(f"{label} has {size} bytes, expected {expected} or its word-padded size")
            if suffix == "Blockdata" and size > expected:
                with source.open("rb") as src:
                    src.seek(expected)
                    if any(src.read()):
                        raise ValueError(f"Nonzero map padding in {label}")
            if size % 2:
                raise ValueError(f"{label} is not 16-bit aligned in size")
            offset = align(next_offset[lane])
            reserved = max(size, 2)
            end = offset + reserved
            if end > LANES[lane][1]:
                raise ValueError(f"{lane} lane overflow at {label}: {end:#x}")
            next_offset[lane] = end
            result.append({
                "label": label,
                "path": layout[source_key],
                "lane": lane,
                "offset": offset,
                "address": XGBA_CPU_BASE + offset,
                "size": size,
                "sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
            })
    ranges = sorted((x["offset"], x["offset"] + max(2, x["size"]), x["label"]) for x in result)
    for previous, following in zip(ranges, ranges[1:]):
        if previous[1] > following[0]:
            raise ValueError(f"Overlapping assets: {previous[2]} and {following[2]}")
    return {
        "format": "XGBA_A1_MAPS_V1",
        "count": len(layouts),
        "assets": result,
        "probe": PROBE_OFFSET,
        "lanes": {x: {"start": LANES[x][0], "used": next_offset[x] - LANES[x][0], "limit": LANES[x][1]} for x in LANES},
    }


def rewrite_layout_assembly(original, plan):
    assets = {entry["label"]: entry for entry in plan["assets"]}
    seen = set()

    def convert(match):
        label, path = match.groups()
        if label not in assets:
            raise ValueError(f"Unexpected .incbin label in generated layouts: {label}")
        entry = assets[label]
        if path != entry["path"]:
            raise ValueError(f"Generator/manifest path mismatch for {label}: {path}")
        if label in seen:
            raise ValueError(f"Repeated generated map label: {label}")
        seen.add(label)
        return f".global {label}\n.set {label}, 0x{entry['address']:08X}\n"

    result = LABEL_RE.sub(convert, original)
    missing = assets.keys() - seen
    if missing:
        raise ValueError(f"Map generator omitted {len(missing)} assets; first: {sorted(missing)[0]}")
    if ".incbin" in result:
        raise ValueError("Unrelocated map data still exists in A1 layout assembly")
    return result


def generate(args):
    plan = manifest_plan()
    source = Path(args.source_asm)
    if not source.is_file():
        raise ValueError(f"Run mapjson first: {source}")
    rewritten = rewrite_layout_assembly(source.read_text(), plan)
    destination = Path(args.asm)
    manifest = Path(args.manifest)
    destination.parent.mkdir(parents=True, exist_ok=True)
    manifest.parent.mkdir(parents=True, exist_ok=True)
    destination.write_text(rewritten)
    manifest.write_text(json.dumps(plan, indent=2) + "\n")
    print(f"A1 generated {plan['count']} layout headers, {len(plan['assets'])} absolute high-ROM pointers")
    for name, lane in plan["lanes"].items():
        print(f"  {name}: {lane['used']:,} used of {lane['limit'] - lane['start']:,} bytes")


def checksum_header(header):
    return (-0x19 - sum(header[0xA0:0xBD])) & 255


def probe_bytes():
    prefix = PROBE_PREFIX + struct.pack("<I", PROBE_OFFSET)
    return prefix + struct.pack("<I", zlib.crc32(prefix))


def tagged_native(native):
    if len(native) > ROM_NATIVE_LIMIT:
        raise ValueError(f"Resident game exceeds 32 MiB: {len(native):,}")
    if len(native) < 0xC0:
        raise ValueError("Truncated resident game")
    data = bytearray(native)
    data[0xB5:0xBC] = MAGIC
    data[0xBC] = 0  # A1 is ROM-only; E5/E6 is SAVE opt-in and must not activate
    data[0xBD] = checksum_header(data)
    if data[0xBD] != checksum_header(data):
        raise ValueError("GBA header checksum mismatch")
    return data


def verify_manifest(plan):
    current = manifest_plan()
    if current != plan:
        raise ValueError("A1 manifest is stale or inconsistent with source assets")
    return current



def verify_symbols(plan, elf_path, nm="arm-none-eabi-nm"):
    """Assert ALL generated A1 pointer symbols resolve to actual high addresses.

    Linker garbage collection/LTO and declaration mismatches must never permit
    a seemingly correct host pack with native code still reading old blobs.
    """
    result = subprocess.run(
        [nm, "--defined-only", str(elf_path)],
        capture_output=True,
        text=True,
        check=True,
    )
    symbols = {}
    for line in result.stdout.splitlines():
        match = re.fullmatch(r"\\s*([0-9a-fA-F]+)\\s+([A-Za-z])\\s+(\\S+)\\s*", line)
        if match and match.group(2).upper() == "A":
            symbols[match.group(3)] = int(match.group(1), 16)

    for entry in plan["assets"]:
        actual = symbols.get(entry["label"])
        if actual != entry["address"]:
            if actual is None:
                raise ValueError(f"Missing absolute ELF symbol: {entry['label']}")
            raise ValueError(
                f"ELF pointer mismatch {entry['label']}: "
                f"{actual:#x}, expected {entry['address']:#x}"
            )
    print(f"A1 LINKED SYMBOLS VERIFIED: {len(plan['assets'])} high-address absolute pointers")


def pack(args):
    plan = json.loads(Path(args.manifest).read_text())
    verify_manifest(plan)
    native_path = Path(args.native)
    output_path = Path(args.output)
    if native_path.resolve() == output_path.resolve():
        raise ValueError("Cannot overwrite the resident source ROM")
    if output_path.exists():
        raise ValueError("Refusing to overwrite an existing candidate ROM")
    native = tagged_native(native_path.read_bytes())
    output_path.parent.mkdir(parents=True, exist_ok=True)
    try:
        with output_path.open("xb") as out:
            out.write(native)
            for entry in plan["assets"]:
                data = (ROOT / entry["path"]).read_bytes()
                if hashlib.sha256(data).hexdigest() != entry["sha256"]:
                    raise ValueError(f"Changed asset during A1 build: {entry['label']}")
                out.seek(entry["offset"])
                out.write(data if data else b"\x00\x00")
            out.seek(PROBE_OFFSET)
            out.write(probe_bytes())
            out.truncate(ROM_BYTES)
        verify(args)
    except Exception:
        output_path.unlink(missing_ok=True)
        raise


def verify(args):
    plan = json.loads(Path(args.manifest).read_text())
    verify_manifest(plan)
    path = Path(args.output)
    if path.stat().st_size != ROM_BYTES:
        raise ValueError("A1 ROM must be exactly 256 MiB")
    with path.open("rb") as rom:
        rom.seek(0xB5)
        if rom.read(8) != MAGIC + b"\x00":
            raise ValueError("XGBA256 magic/revision not ROM-only")
        rom.seek(0xA0)
        header = rom.read(0x20)
        if header[0x1D] != checksum_header(b"\x00" * 0xA0 + header):
            raise ValueError("Invalid GBA header complement")
        for entry in plan["assets"]:
            rom.seek(entry["offset"])
            payload = rom.read(entry["size"])
            if hashlib.sha256(payload).hexdigest() != entry["sha256"]:
                raise ValueError(f"Wrong packed asset: {entry['label']} at {entry['offset']:#x}")
        rom.seek(PROBE_OFFSET)
        if rom.read(16) != probe_bytes():
            raise ValueError("Last ROM byte / final A1 marker invalid")
    if args.native:
        native = tagged_native(Path(args.native).read_bytes())
        with path.open("rb") as rom:
            if rom.read(len(native)) != native:
                raise ValueError("Native game changed outside A1 cartridge header")
    print(f"A1 PASS: exact 256 MiB, {plan['count']} maps, {len(plan['assets'])} high-address geometry pointers, 128 KiB native Flash")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("command", choices=("generate", "pack", "verify", "check-plan", "check-symbols"))
    parser.add_argument("--source-asm", default="data/layouts/layouts.inc")
    parser.add_argument("--asm", default="build/xgba-a1/native/xgba-a1-layouts.inc")
    parser.add_argument("--manifest", default="build/xgba-a1/native/a1-manifest.json")
    parser.add_argument("--native", default="")
    parser.add_argument("--elf", default="")
    parser.add_argument("--nm", default="arm-none-eabi-nm")
    parser.add_argument("--output", default="xgba-anthology-a1.gba")
    args = parser.parse_args()
    if args.command == "generate":
        generate(args)
    elif args.command == "check-plan":
        plan = manifest_plan()
        print(f"A1 manifest source valid: {plan['count']} layouts, {len(plan['assets'])} binaries")
    elif args.command == "check-symbols":
        if not args.elf:
            parser.error("check-symbols requires --elf")
        plan = json.loads(Path(args.manifest).read_text())
        verify_manifest(plan)
        verify_symbols(plan, args.elf, args.nm)
    elif args.command == "pack":
        if not args.native:
            parser.error("pack requires --native")
        pack(args)
    else:
        verify(args)


if __name__ == "__main__":
    main()
