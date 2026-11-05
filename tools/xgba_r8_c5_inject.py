#!/usr/bin/env python3
"""Create-only R8 C5 fault fixtures from a disposable, audited Save2.

This never edits its source or any player file. Exit nonzero unless damaged
media selects the exact previous cohort (or refuses both broken copies).
"""
import argparse
import os
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import xgba_a2_storage as a2


def find_copy(image, kind, page, generation):
    magic = a2.META_MAGIC if kind == "metadata" else a2.PAGE_MAGIC
    size = a2.META_PAYLOAD.size if kind == "metadata" else a2.PC_BYTES
    for slot in (0, 1):
        off = a2.meta_offset(slot) if kind == "metadata" else a2.page_offset(page, slot)
        # Verify the header/CRC before selecting a corruption target.
        from io import BytesIO
        candidate = a2.candidate(BytesIO(image), off, magic,
                                 0 if kind == "metadata" else page, size)
        if candidate is not None and candidate[0] == generation:
            return off
    raise a2.A2Error(f"Missing {kind} page {page} generation {generation}")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("source")
    parser.add_argument("destination")
    parser.add_argument("fault", choices=("torn-page7", "torn-metadata", "both-metadata"))
    args = parser.parse_args()
    src = Path(args.source).resolve()
    dst = Path(args.destination).absolute()
    if src == dst or not src.is_file() or dst.exists() or dst.is_symlink():
        raise a2.A2Error("Require distinct existing source and nonexistent destination")
    if any(parent.is_symlink() for parent in (dst.parent, *dst.parent.parents)):
        raise a2.A2Error("Destination traverses a symlink")
    baseline = a2.audit(src)
    if baseline["counter"] != 2 or baseline["metadata_generation"] < 2:
        raise a2.A2Error("Expected independently audited game-generated Save2")
    image = bytearray(src.read_bytes())
    if args.fault == "torn-page7":
        off = find_copy(image, "page", 7, baseline["page_generations"][7])
        image[off + a2.HEADER.size + 200] ^= 0xFF
    elif args.fault == "torn-metadata":
        off = find_copy(image, "metadata", 0, baseline["metadata_generation"])
        image[off + a2.HEADER.size + 16] ^= 0xFF
    else:
        for slot in (0, 1):
            off = a2.meta_offset(slot)
            image[off + a2.HEADER.size + 16] ^= 0xFF
    # O_EXCL prevents overwriting any file; no copy/hardlink aliases the input.
    fd = os.open(dst, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
    try:
        with os.fdopen(fd, "wb") as output:
            output.write(image)
            output.flush()
            os.fsync(output.fileno())
        if args.fault == "both-metadata":
            try:
                a2.audit(dst)
            except a2.A2Error:
                print("R8_C5_FAIL_CLOSED both_metadata_invalid")
                return 0
            raise a2.A2Error("Both invalid metadata copies were wrongly accepted")
        recovered = a2.audit(dst)
        if recovered["counter"] != 1:
            raise a2.A2Error(f"Recovery selected {recovered['counter']} instead of Save1")
        if recovered["collection_boxes_sha256"] == baseline["collection_boxes_sha256"]:
            raise a2.A2Error("Fault did not roll back the expected collection")
        print(f"R8_C5_RECOVERY_VERIFIED {args.fault} counter=1 source_counter=2")
        return 0
    except Exception:
        dst.unlink(missing_ok=True)
        raise


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (a2.A2Error, OSError, ValueError) as exc:
        print(f"R8_C5_INJECTION_REFUSED {exc}", file=sys.stderr)
        raise SystemExit(1)
