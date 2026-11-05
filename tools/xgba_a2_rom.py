#!/usr/bin/env python3
"""Developer-only: retag a separately compiled 256 MiB A2 shadow ROM E6.

Never converts saves, never modifies a source ROM, and refuses overwrites.
The input MUST originate from a separately compiled XGBA_A2=1 native build;
this tool validates the cartridge wrapper, not the game's save behavior.
"""
import argparse
import os
from pathlib import Path
import sys

from xgba_a1_assets import MAGIC, ROM_BYTES, PROBE_OFFSET, probe_bytes, checksum_header

R8_OFFSET = ROM_BYTES - 0x100
R8_DESCRIPTOR = b"XGBARAM8" + (1).to_bytes(4, "little") + (0x400000).to_bytes(4, "little")


class A2RomError(ValueError):
    pass


def stamp_header(header):
    if len(header) != 0xC0:
        raise A2RomError("GBA header must contain exactly 0xC0 bytes")
    if header[0xB5:0xBC] != MAGIC or header[0xBC] != 0:
        raise A2RomError("Input is not an original XGBA256 revision-zero ROM")
    if header[0xBD] != checksum_header(header):
        raise A2RomError("Input GBA header checksum invalid")
    tagged = bytearray(header)
    tagged[0xBC] = 0xE6
    tagged[0xBD] = checksum_header(tagged)
    return bytes(tagged)


def verify(path, r8=False):
    path = Path(path)
    if not path.is_file() or path.is_symlink() or path.stat().st_size != ROM_BYTES:
        raise A2RomError("E6 ROM must be a regular 256 MiB file")
    with path.open("rb") as stream:
        head = stream.read(0xC0)
        if head[0xB5:0xBC] != MAGIC or head[0xBC] != 0xE6:
            raise A2RomError("Not an XGBA256 E6 revision cartridge")
        if head[0xBD] != checksum_header(head):
            raise A2RomError("Invalid E6 GBA header checksum")
        stream.seek(PROBE_OFFSET)
        if stream.read(len(probe_bytes())) != probe_bytes():
            raise A2RomError("Missing final A1 high-ROM probe marker")
        stream.seek(R8_OFFSET)
        present = stream.read(len(R8_DESCRIPTOR))
        if r8 and present != R8_DESCRIPTOR:
            raise A2RomError("Missing or incompatible R8 4 MiB descriptor")
        if not r8 and present == R8_DESCRIPTOR:
            raise A2RomError("R8 cartridge must be verified explicitly with --r8")
    return {"bytes": ROM_BYTES, "revision": "E6", "header": "valid",
            "r8_ram": "4 MiB" if r8 else "disabled",
            "high_rom_probe": "valid"}


def tag(source, destination, r8=False):
    source = Path(source).expanduser()
    destination = Path(destination).expanduser()
    if not source.is_file() or source.is_symlink() or source.stat().st_size != ROM_BYTES:
        raise A2RomError("Source must be a regular 256 MiB A1-layout ROM")
    if source.resolve() == destination.resolve() or os.path.lexists(destination):
        raise A2RomError("Refusing to modify or overwrite a ROM")
    if not destination.parent.is_dir():
        raise A2RomError("Output directory does not exist")
    with source.open("rb") as origin:
        stamped = stamp_header(origin.read(0xC0))
        origin.seek(PROBE_OFFSET)
        if origin.read(len(probe_bytes())) != probe_bytes():
            raise A2RomError("Source has no final extended ROM probe")
        origin.seek(0xC0)
        try:
            with destination.open("xb") as out:
                out.write(stamped)
                while True:
                    block = origin.read(0x100000)
                    if not block:
                        break
                    out.write(block)
                if r8:
                    out.seek(R8_OFFSET)
                    out.write(R8_DESCRIPTOR)
                out.flush()
                os.fsync(out.fileno())
            return verify(destination, r8=r8)
        except BaseException:
            destination.unlink(missing_ok=True)
            raise


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("tag", "verify"))
    parser.add_argument("path", help="Input ROM for tag, E6 ROM for verify")
    parser.add_argument("destination", nargs="?", help="New output ROM for tag")
    parser.add_argument("--r8", action="store_true", help="Require and stamp separate 4 MiB R8 hardware capability")
    args = parser.parse_args()
    try:
        if args.command == "tag":
            if not args.destination:
                parser.error("tag requires a separate destination")
            result = tag(args.path, args.destination, r8=args.r8)
        else:
            result = verify(args.path, r8=args.r8)
        for key, value in result.items():
            print(f"{key}: {value}")
        return 0
    except (OSError, A2RomError) as exc:
        print(f"A2 developer ROM refused: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
