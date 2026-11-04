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
META_STRIDE = 0x80
HEADER = struct.Struct("<8sHHBBHIIII")
PAGE_MAGIC = b"XGBAA2P\x00"
META_MAGIC = b"XGBAA2M\x00"
FORMAT_VERSION = 2
META_PAYLOAD = struct.Struct("<16sIBB2x8I")
EMPTY = b"\xFF" * 8

assert len(SUPER_HEADER) == 24
assert HEADER.size == 32
assert META_PAYLOAD.size == 56
assert META_PAYLOAD.size + HEADER.size <= META_STRIDE
assert PC_BYTES == sp.PK_SIZE
assert PC_BYTES + HEADER.size <= PC_STRIDE
assert META_BASE + META_STRIDE * 2 < SUPER_BACKUP
assert PC_PAGES * BOXES_PER_PAGE * BOX_SLOTS == 3360
# The native PokemonStorage ABI has 420 BoxPokemon × 80 bytes and 4
# full Pokemon × 100 bytes. Names/wallpaper are NOT zero-terminated strings.
BOX_REGION_START = 4
BOX_REGION_END = BOX_REGION_START + BOXES_PER_PAGE * BOX_SLOTS * 80
FUSION_REGION_START = PC_BYTES - 4 * 100
assert BOX_REGION_END == 0x8344
assert FUSION_REGION_START == 0x83D0


class A2Error(ValueError):
    pass


def fingerprint(native, native_slot):
    if native_slot not in (0, 1):
        raise A2Error('Native slot must be 0 or 1')
    size = sp.SECTORS_PER_SLOT * sp.SECTOR_SIZE
    start = native_slot * size
    return hashlib.sha256(native[start:start + size]).digest()[:16]


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
    """Resolve metadata, native slot and exact PC generations as one cohort."""
    sp.require_pins()
    path = Path(path)
    size = path.stat().st_size
    if size not in (SAVE_BYTES, SAVE_BYTES + 16):
        raise A2Error(f"Wrong E6 image size {size}")
    with path.open("rb") as stream:
        stream.seek(SUPER_PRIMARY)
        first = stream.read(24)
        stream.seek(SUPER_BACKUP)
        last = stream.read(24)
        if first != SUPER_HEADER or last != SUPER_HEADER:
            raise A2Error("Both emulator E6 superblocks must match")
        stream.seek(0)
        native = stream.read(NATIVE_BYTES)
        metadata = []
        for slot in (0, 1):
            row = candidate(stream, meta_offset(slot), META_MAGIC, 0,
                            META_PAYLOAD.size)
            if row is not None:
                metadata.append((row[0], slot, row[1]))
        if not metadata:
            raise A2Error("No valid A2 metadata commit")
        metadata.sort(key=lambda item: item[0], reverse=True)
        newest = metadata[0][0]

    for meta_generation, meta_slot, payload in metadata:
        digest, counter, selected, native_slot, *generations = META_PAYLOAD.unpack(payload)
        if (selected >= PC_PAGES or native_slot not in (0, 1)
                or any(generation == 0 for generation in generations)):
            continue
        if digest != fingerprint(native, native_slot):
            continue
        try:
            save = sp.self_test(str(path), slot=native_slot)
            sp.require_intact_slot(save)
        except SystemExit:
            continue
        if save.slots[native_slot]["status"] != "OK":
            continue
        if save.slots[native_slot]["counter"] != counter:
            continue
        if save.sb2[sp.V["SB2.saveVersion"]] != sp.SAVE_FORMAT_VERSION:
            continue
        if save.u16(save.sb2, sp.V["SB2.regionChecksum"]) != save.region_checksum():
            continue

        pages = []
        counts = []
        with path.open("rb") as stream:
            for page in range(PC_PAGES):
                valid = []
                for slot in (0, 1):
                    row = candidate(stream, page_offset(page, slot),
                                    PAGE_MAGIC, page, PC_BYTES)
                    if row is not None:
                        valid.append(row)
                counts.append(len(valid))
                selected_record = next((row[1] for row in valid
                                        if row[0] == generations[page]), None)
                if selected_record is None:
                    break
                pages.append(selected_record)
        if len(pages) != PC_PAGES:
            continue
        if pages[selected] != bytes(save.pk):
            continue

        return {
            "size": size,
            "version": save.sb2[sp.V["SB2.saveVersion"]],
            "counter": counter,
            "native_slot": native_slot,
            "active_page": selected + 1,
            "pc_boxes": PC_PAGES * BOXES_PER_PAGE,
            "slots": PC_PAGES * BOXES_PER_PAGE * BOX_SLOTS,
            # Exact 80-byte BoxPokemon bytes for all 3,360 slots across
            # all eight cohort-resolved pages. Selected-box metadata,
            # wallpapers and currentBox are intentionally excluded so a
            # GUI-only Jump cannot legitimately change this digest.
            "collection_boxes_sha256": hashlib.sha256(
                b"".join(page[BOX_REGION_START:BOX_REGION_END]
                         for page in pages)
            ).hexdigest(),
            "healthy_page_copies": counts,
            "healthy_metadata_copies": len(metadata),
            "metadata_generation": meta_generation,
            "page_generations": list(generations),
            "recovery_required": (meta_generation != newest
                                  or save.save_counter != counter),
        }
    raise A2Error("No coherent native save slot and referenced PC-page generation")



def verified_collection_box_bytes(path, report):
    """Read only the cohort's selected CRC-verified PC page generations."""
    image = bytearray()
    with Path(path).open("rb") as stream:
        for page in range(PC_PAGES):
            generation = report["page_generations"][page]
            payload = None
            for copy in (0, 1):
                record = candidate(stream, page_offset(page, copy),
                                   PAGE_MAGIC, page, PC_BYTES)
                if record is not None and record[0] == generation:
                    payload = record[1]
                    break
            if payload is None:
                raise A2Error(f"Page {page} is missing committed generation {generation}")
            image.extend(payload[BOX_REGION_START:BOX_REGION_END])
    return bytes(image)


def audit_roundtrip(before_path, after_path):
    """Independently prove the graphical Box 15 -> Box 43 save-3 transfer.

    Exact 80-byte encrypted records and all unrelated 3,358 slots are
    compared against the committed Save2 cohort, never against volatile RAM.
    """
    before_report = audit(before_path)
    after_report = audit(after_path)
    if (before_report["counter"] != 2 or after_report["counter"] != 3
            or before_report["active_page"] != 3 or after_report["active_page"] != 2):
        raise A2Error("Roundtrip requires coherent Save2 Box30 -> Save3 Box15")
    original = verified_collection_box_bytes(before_path, before_report)
    updated = verified_collection_box_bytes(after_path, after_report)
    width = 80
    source = (14 * BOX_SLOTS + 5) * width
    destination = (42 * BOX_SLOTS + 1) * width
    empty = bytes(width)
    squirtle = original[source:source + width]
    if squirtle == empty:
        raise A2Error("Source Squirtle is absent from committed Save2")
    if original[destination:destination + width] != empty:
        raise A2Error("Destination Box43 slot2 is occupied in Save2")
    expected = bytearray(original)
    expected[source:source + width] = empty
    expected[destination:destination + width] = squirtle
    if updated != bytes(expected):
        mismatch = next((i for i, (a, b) in enumerate(zip(updated, expected))
                         if a != b), None)
        raise A2Error(f"Committed collection differs from exact expected transfer at byte {mismatch}")
    if before_report["collection_boxes_sha256"] == after_report["collection_boxes_sha256"]:
        raise A2Error("Collection digest remained unchanged despite transfer")
    return {
        "roundtrip": "verified exact 80-byte Box15/slot6 -> Box43/slot2",
        "unchanged_other_slots": PC_PAGES * BOXES_PER_PAGE * BOX_SLOTS - 2,
        "before_counter": before_report["counter"],
        "after_counter": after_report["counter"],
        "source_cleared": True,
        "destination_matches_source": True,
    }


def empty_page_template(source_pc):
    """Preserve valid GBA-encoded box names and wallpaper, clear all Mons."""
    if len(source_pc) != PC_BYTES:
        raise A2Error("Cannot initialize boxes from an unknown storage ABI")
    empty = bytearray(source_pc)
    empty[0] = 0
    empty[BOX_REGION_START:BOX_REGION_END] = bytes(BOX_REGION_END - BOX_REGION_START)
    empty[FUSION_REGION_START:PC_BYTES] = bytes(PC_BYTES - FUSION_REGION_START)
    return bytes(empty)


def verify_blank(path):
    """Assert the entire disposable E6 seed contains NO user or game data."""
    path = Path(path)
    if not path.is_file() or path.is_symlink() or path.stat().st_size != SAVE_BYTES:
        raise A2Error("Blank E6 template must be a regular 16 MiB file")
    chunk_size = 0x100000
    with path.open("rb") as stream:
        for offset in range(0, SAVE_BYTES, chunk_size):
            actual = stream.read(chunk_size)
            expected = bytearray(b"\xFF" * chunk_size)
            for marker in (SUPER_PRIMARY, SUPER_BACKUP):
                if offset <= marker < offset + chunk_size:
                    index = marker - offset
                    expected[index:index + len(SUPER_HEADER)] = SUPER_HEADER
            if actual != expected:
                raise A2Error(f"Unexpected data in blank E6 template at block 0x{offset:X}")
        if stream.read(1):
            raise A2Error("Blank E6 image contains trailing data")
    return {"size": SAVE_BYTES, "state": "erased E6", "native_slots": "both blank",
            "a2_metadata": "absent", "pc_pages": "all blank"}


def create_blank(target):
    """Create a strict first-run E6 seed without modifying any existing save."""
    target = Path(target).expanduser().absolute()
    if os.path.lexists(target):
        raise A2Error("Refusing to overwrite an existing save or symlink")
    if not target.parent.is_dir():
        raise A2Error("Destination directory does not exist")
    for parent in (target.parent, *target.parent.parents):
        if parent.is_symlink():
            raise A2Error("Refusing symlinked destination directory")
    fd, temp = tempfile.mkstemp(prefix=".xgba-a2-blank-", dir=target.parent)
    try:
        with os.fdopen(fd, "w+b") as stream:
            chunk = b"\xFF" * 0x100000
            for _ in range(SAVE_BYTES // len(chunk)):
                stream.write(chunk)
            stream.seek(SUPER_PRIMARY)
            stream.write(SUPER_HEADER)
            stream.seek(SUPER_BACKUP)
            stream.write(SUPER_HEADER)
            stream.flush()
            os.fsync(stream.fileno())
        result = verify_blank(temp)
        # Same-directory hard link is create-only even if another process
        # creates the destination between validation and publication.
        os.link(temp, target, follow_symlinks=False)
        return result
    finally:
        Path(temp).unlink(missing_ok=True)


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
            empty_page = empty_page_template(bytes(save.pk))
            for page in range(PC_PAGES):
                payload = bytes(save.pk) if page == 0 else empty_page
                for slot, seq in ((0, 1), (1, 2)):
                    out.seek(page_offset(page, slot))
                    out.write(make_record(PAGE_MAGIC, page, seq, payload))
            for slot, seq in ((0, 1), (1, 2)):
                meta = META_PAYLOAD.pack(
                    fingerprint(native[:NATIVE_BYTES], save.active),
                    save.slots[save.active]["counter"],
                    0, save.active, *([seq] * PC_PAGES)
                )
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
    blank = sub.add_parser("blank", help="Create a NEW erased E6 first-run template, never overwrite")
    blank.add_argument("destination")
    blank_check = sub.add_parser("audit-blank", help="Prove E6 seed contains no save data")
    blank_check.add_argument("save")
    check = sub.add_parser("audit", help="Verify E6 headers and protected A2 PC records")
    check.add_argument("save")
    transfer = sub.add_parser("audit-roundtrip", help="Verify exact committed C3D Box15 -> Box43 transfer")
    transfer.add_argument("before")
    transfer.add_argument("after")
    sub.add_parser("layout", help="Print reserved A2 collection-storage geometry")
    args = parser.parse_args()
    try:
        if args.command == "init":
            result = prepare(args.source, args.destination)
            print(f"Created {args.destination} (original unchanged)")
        elif args.command == "blank":
            result = create_blank(args.destination)
            print(f"Created disposable E6 seed {args.destination}")
        elif args.command == "audit-blank":
            result = verify_blank(args.save)
        elif args.command == "audit":
            result = audit(args.save)
        elif args.command == "audit-roundtrip":
            result = audit_roundtrip(args.before, args.after)
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
