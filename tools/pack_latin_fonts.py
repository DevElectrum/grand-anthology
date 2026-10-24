#!/usr/bin/env python3
"""Deduplicate identical 16-byte tiles in all twelve GBA Latin font tables.

The original .latfont files are retained as intermediate inputs. The generated
byte dictionary and 12-bit per-tile index table reconstruct every input exactly.
No glyph or font style is dropped and no RAM decompression cache is necessary.
"""
import argparse
from pathlib import Path
import tempfile

TILE_BYTES = 16
FONT_BYTES = 0x8000
TILES_PER_FONT = FONT_BYTES // TILE_BYTES
FONT_COUNT = 12


def pack(fonts):
    if len(fonts) != FONT_COUNT:
        raise ValueError("Need exactly twelve Latin fonts")
    dictionary, positions, indexes = [], {}, []
    for number, source in enumerate(fonts):
        if len(source) != FONT_BYTES:
            raise ValueError(f"Font {number}: expected {FONT_BYTES} bytes, got {len(source)}")
        for offset in range(0, FONT_BYTES, TILE_BYTES):
            tile = source[offset:offset + TILE_BYTES]
            if tile not in positions:
                positions[tile] = len(dictionary)
                dictionary.append(tile)
            indexes.append(positions[tile])
    if len(dictionary) > 4096:
        raise ValueError("Unique Latin font tiles overflow 12-bit index")
    # Two 12-bit offsets in 3 bytes, little-endian; 12*2048 is even.
    packed = bytearray()
    for left, right in zip(indexes[::2], indexes[1::2]):
        packed.extend((left & 255, (left >> 8) | ((right & 15) << 4), right >> 4))
    return b"".join(dictionary), bytes(packed)


def unpack(tiles, indexes):
    if len(indexes) != FONT_COUNT * TILES_PER_FONT * 3 // 2:
        raise ValueError("Wrong index count")
    idx = []
    for pos in range(0, len(indexes), 3):
        first, middle, last = indexes[pos:pos+3]
        idx.extend((first | ((middle & 15) << 8),
                    (middle >> 4) | (last << 4)))
    return [b"".join(tiles[i*TILE_BYTES:(i+1)*TILE_BYTES]
                     for i in idx[n*TILES_PER_FONT:(n+1)*TILES_PER_FONT])
            for n in range(FONT_COUNT)]


def self_test():
    parts = [bytes([i])*TILE_BYTES for i in range(53)]
    fonts = [b"".join(parts[(i*(n+1)+n) % len(parts)]
                      for i in range(TILES_PER_FONT)) for n in range(FONT_COUNT)]
    dictionary, indexes = pack(fonts)
    assert unpack(dictionary, indexes) == fonts
    assert len(dictionary) == 53 * TILE_BYTES
    assert len(indexes) == FONT_COUNT * TILES_PER_FONT * 3 // 2
    for invalid in ([], [b""]*FONT_COUNT, fonts[:-1]):
        try:
            pack(invalid)
        except ValueError:
            continue
        raise AssertionError("Accepted invalid font collection")
    print("PASS: deterministic lossless font tile packing and input guards")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--self-test", action="store_true")
    parser.add_argument("--output-dir", type=Path)
    parser.add_argument("fonts", type=Path, nargs="*")
    args = parser.parse_args()
    if args.self_test:
        self_test()
        return
    if args.output_dir is None or len(args.fonts) != FONT_COUNT:
        parser.error("Expected --output-dir and twelve .latfont files in Makefile order")
    fonts = [path.read_bytes() for path in args.fonts]
    tiles, indexes = pack(fonts)
    if unpack(tiles, indexes) != fonts:
        raise RuntimeError("Font roundtrip mismatch")
    args.output_dir.mkdir(parents=True, exist_ok=True)
    for name, data in (("tiles.bin", tiles), ("indexes.bin", indexes)):
        dest = args.output_dir / name
        with tempfile.NamedTemporaryFile(dir=dest.parent, delete=False) as tmp:
            tmp.write(data)
            tempname = tmp.name
        Path(tempname).replace(dest)
    print(f"Latin tiles: {sum(map(len, fonts))} -> {len(tiles)+len(indexes)} bytes, "
          f"saved {sum(map(len,fonts))-len(tiles)-len(indexes)} bytes "
          f"({len(tiles)//TILE_BYTES} unique tiles)")


if __name__ == "__main__":
    main()
