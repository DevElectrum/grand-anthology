#!/usr/bin/env python3
"""A2: offline, create-only Anthology Flash -> XGBA E6 collection image.

This does NOT activate an in-game A2 storage runtime. Close the emulator first.
It never modifies the source and refuses an existing output. Only intact
current-version v13 saves can be migrated. The E6 emulator superblock and
Anthology's A2 collection record format are independent.
"""
import argparse
import hashlib
import os
from pathlib import Path
import struct
import sys
import tempfile
import zlib

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "test" / "overworld"))
import SavePatch as sp

NATIVE_BYTES = 0x20000
SAVE_BYTES = 0x1000000
SUPER_PRIMARY = 0x20000
SUPER_BACKUP = 0xFFF000
SUPER_HEADER = bytes.fromhex("5847424145365300020000000000020000000001b02b5b12")
PC_BASE = 0x200000
PC_PAGES = 8
BOXES_PER_PAGE = 14
BOX_SLOTS = 30
PC_BYTES = 34144
PC_STRIDE = 0x9000
META_BASE = PC_BASE + PC_PAGES * 2 * PC_STRIDE
META_STRIDE = 0x40
HEADER = struct.Struct("<8sHHBBHIIII")
PAGE_MAGIC = b"XGBAA2P\x00"
META_MAGIC = b"XGBAA2M\x00"
FORMAT_VERSION = 1
META_PAYLOAD = struct.Struct("<16sIB3x")
EMPTY = b"\xFF" * 8

assert len(SUPER_HEADER) == 24
assert HEADER.size == 32
assert META_PAYLOAD.size == 24
assert PC_BYTES == sp.PK_SIZE
assert PC_BYTES + HEADER.size <= PC_STRIDE
assert META_BASE + META_STRIDE * 2 < SUPER_BACKUP
assert PC_PAGES * BOXES_PER_PAGE * BOX_SLOTS == 3360


class A2Error(ValueError):
    pass


def fingerprint(native):
    return hashlib.sha256(native).digest()[:16]


def crc(data):
    return zlib.crc32(data) & 0xFFFFFFFF


def page_offset(page, slot):
    if not 0 <= page < PC_PAGES or slot not in (0, 1):
        raise A2Error("Page must be 0..7, copy must be 0 or 1")
    return PC_BASE + (page * 2 + slot) * PC_STRIDE


def meta_offset(slot):
    if slot not in (0, 1):
        raise A2Error("Metadata copy must be 0 or 1")
    return META_BASE + slot * META_STRIDE


def make_record(magic, page, sequence, payload):
    if not 0 < sequence <= 0xFFFFFFFF:
        raise A2Error("Invalid record generation")
    prefix = struct.pack(
        "<8sHHBBHIII", magic, FORMAT_VERSION, 0,
        page, 0, 0, sequence, len(payload), crc(payload)
    )
    return prefix + struct.pack("<I", crc(prefix)) + payload


def candidate(source, offset, magic, page, expected_size):
    source.seek(offset)
    header = source.read(HEADER.size)
    if len(header) != HEADER.size or header[:8] == EMPTY:
        return None
    (found_magic, version, reserved, found_page, flags, padding,
     sequence, length, payload_crc, header_crc) = HEADER.unpack(header)
    if (found_magic != magic or version != FORMAT_VERSION or reserved
            or found_page != page or flags or padding or not sequence
            or length != expected_size or crc(header[:28]) != header_crc):
        return None
    data = source.read(expected_size)
    if len(data) != expected_size or crc(data) != payload_crc:
        return None
    return (sequence, data)


def best_record(source, offsets, magic, page, size):
    found = []
    for offset in offsets:
        result = candidate(source, offset, magic, page, size)
        if result is not None:
            found.append(result)
    if not found:
        raise A2Error(f"Page/metadata {page}: both copies missing or invalid")
    found.sort(key=lambda x: x[0], reverse=True)
    return found[0][1], len(found)


def verify_native(path):
    sp.require_pins()
    raw = Path(path).read_bytes()
    if len(raw) not in (NATIVE_BYTES, NATIVE_BYTES + 16):
        raise A2Error("Source must be native 128 KiB save, optionally with 16 RTC bytes")
    try:
        save = sp.self_test(str(path))
        sp.require_intact_slot(save)
    except SystemExit as exc:
        raise A2Error(f"Native save failed validation: {exc}") from exc
    if save.slots[save.active]["status"] != "OK":
        raise A2Error("Active native save slot must be fully healthy")
    version = save.sb2[sp.V["SB2.saveVersion"]]
    if version != sp.SAVE_FORMAT_VERSION:
        raise A2Error(f"Save format {version} != current v{sp.SAVE_FORMAT_VERSION}")
    if save.u16(save.sb2, sp.V["SB2.regionChecksum"]) != save.region_checksum():
        raise A2Error("Region progress checksum mismatch: refusing migration")
    if len(save.pk) != PC_BYTES:
        raise A2Error("PokemonStorage ABI mismatch")
    return raw, save


def audit(path):
    sp.require_pins()
    path = Path(path)
    size = path.stat().st_size
    if size not in (SAVE_BYTES, SAVE_BYTES + 16):
        raise A2Error(f"Wrong E6 image size {size}")
    with path.open("rb") as src:
        src.seek(SUPER_PRIMARY)
        first = src.read(24)
        src.seek(SUPER_BACKUP)
        last = src.read(24)
        if first != SUPER_HEADER or last != SUPER_HEADER:
            raise A2Error("Both emulator E6 superblocks must match")
        src.seek(0)
        native = src.read(NATIVE_BYTES)
        payload, meta_copies = best_record(
            src, [meta_offset(0), meta_offset(1)],
            META_MAGIC, 0, META_PAYLOAD.size
        )
        digest, counter, selected = META_PAYLOAD.unpack(payload)
        if digest != fingerprint(native) or selected >= PC_PAGES:
            raise A2Error("Native Flash and A2 metadata are not the same generation")
        page_counts = []
        for page in range(PC_PAGES):
            _, valid_copies = best_record(
                src, [page_offset(page, 0), page_offset(page, 1)],
                PAGE_MAGIC, page, PC_BYTES
            )
            page_counts.append(valid_copies)
    try:
        sv = sp.self_test(str(path))
        sp.require_intact_slot(sv)
    except SystemExit as exc:
        raise A2Error(f"Invalid native sectors in E6 image: {exc}") from exc
    if sv.slots[sv.active]["status"] != "OK":
        raise A2Error("Incomplete native save slot")
    if sv.save_counter != counter:
        raise A2Error("Native save counter differs from A2 metadata")
    with path.open("rb") as src:
        page_zero, _ = best_record(src, [page_offset(0, 0), page_offset(0, 1)],
                                   PAGE_MAGIC, 0, PC_BYTES)
    if page_zero != bytes(sv.pk):
        raise A2Error("Original 14-box PC is not byte-identical in A2 page one")
    return {
        "size": size,
        "version": sv.sb2[sp.V["SB2.saveVersion"]],
        "counter": counter,
        "active_page": selected + 1,
        "pc_boxes": PC_PAGES * BOXES_PER_PAGE,
        "slots": PC_PAGES * BOXES_PER_PAGE * BOX_SLOTS,
        "healthy_page_copies": page_counts,
        "healthy_metadata_copies": meta_copies,
    }


def prepare(source, target):
    source = Path(source).expanduser()
    target = Path(target).expanduser()
    if not source.is_file() or source.resolve() == target.resolve():
        raise A2Error("Need distinct existing input and new output path")
    if target.exists():
        raise A2Error("Refusing to overwrite existing output")
    if not target.parent.is_dir():
        raise A2Error("Output directory does not exist")
    native, save = verify_native(source)
    fd, temp = tempfile.mkstemp(prefix=".xgba-a2-", dir=target.parent)
    try:
        with os.fdopen(fd, "w+b") as out:
            blank = b"\xFF" * 0x100000
            for _ in range(SAVE_BYTES // len(blank)):
                out.write(blank)
            out.seek(0)
            out.write(native[:NATIVE_BYTES])
            out.seek(SUPER_PRIMARY)
            out.write(SUPER_HEADER)
            out.seek(SUPER_BACKUP)
            out.write(SUPER_HEADER)
            for page in range(PC_PAGES):
                payload = bytes(save.pk) if page == 0 else bytes(PC_BYTES)
                for slot, seq in ((0, 1), (1, 2)):
                    out.seek(page_offset(page, slot))
                    out.write(make_record(PAGE_MAGIC, page, seq, payload))
            meta = META_PAYLOAD.pack(fingerprint(native[:NATIVE_BYTES]),
                                     save.save_counter, 0)
            for slot, seq in ((0, 1), (1, 2)):
                out.seek(meta_offset(slot))
                out.write(make_record(META_MAGIC, 0, seq, meta))
            if len(native) > NATIVE_BYTES:
                out.seek(SAVE_BYTES)
                out.write(native[NATIVE_BYTES:])
            out.flush()
            os.fsync(out.fileno())
        result = audit(temp)
        # Atomic create-only link: even a racing writer cannot be overwritten.
        os.link(temp, target)
        return result
    finally:
        Path(temp).unlink(missing_ok=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    init = sub.add_parser("init", help="Create a NEW E6 image from intact v13 native save")
    init.add_argument("source")
    init.add_argument("destination")
    check = sub.add_parser("audit", help="Verify E6 headers and protected A2 PC records")
    check.add_argument("save")
    sub.add_parser("layout", help="Print reserved A2 collection-storage geometry")
    args = parser.parse_args()
    try:
        if args.command == "init":
            result = prepare(args.source, args.destination)
            print(f"Created {args.destination} (original unchanged)")
        elif args.command == "audit":
            result = audit(args.save)
        else:
            result = {
                "e6_bytes": SAVE_BYTES, "native_bytes": NATIVE_BYTES,
                "superblocks": [SUPER_PRIMARY, SUPER_BACKUP],
                "pc_start": PC_BASE, "pc_end": META_BASE + 2 * META_STRIDE,
                "pages": PC_PAGES, "boxes": PC_PAGES * BOXES_PER_PAGE
            }
        for key, value in result.items():
            print(f"{key}: {value}")
        return 0
    except (A2Error, OSError, ValueError) as exc:
        print(f"A2 storage refused: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
