#!/usr/bin/env python3
"""Convert Grand Anthology's 240x160 indexed PNG to GBA background resources."""
from collections import defaultdict, deque
from pathlib import Path
import struct
import zlib

ROOT = Path(__file__).resolve().parents[1]
DIRECTORY = ROOT / "graphics" / "title_screen" / "world"
SOURCE = DIRECTORY / "grand_anthology_title.png"


def read_png(path):
    data = path.read_bytes()
    assert data[:8] == b"\x89PNG\r\n\x1a\n", "Not a PNG"
    offset = 8
    palette = None
    compressed = bytearray()
    width = height = bit_depth = color_type = interlace = 0
    while offset < len(data):
        length = struct.unpack_from(">I", data, offset)[0]
        kind = data[offset + 4:offset + 8]
        chunk = data[offset + 8:offset + 8 + length]
        offset += length + 12
        if kind == b"IHDR":
            width, height, bit_depth, color_type, _, _, interlace = struct.unpack(">IIBBBBB", chunk)
        elif kind == b"PLTE":
            palette = [tuple(chunk[i:i + 3]) for i in range(0, len(chunk), 3)]
        elif kind == b"IDAT":
            compressed.extend(chunk)
        elif kind == b"IEND":
            break
    assert (width, height, bit_depth, color_type, interlace) == (240, 160, 8, 3, 0), "Expected non-interlaced indexed 240x160 PNG"
    assert palette is not None and len(palette) <= 256
    raw = zlib.decompress(compressed)
    pixels = bytearray()
    offset = 0
    previous = bytearray(width)
    for _ in range(height):
        filter_type = raw[offset]
        row = bytearray(raw[offset + 1:offset + 1 + width])
        offset += width + 1
        for i in range(width):
            left = row[i - 1] if i else 0
            above = previous[i]
            upper_left = previous[i - 1] if i else 0
            if filter_type == 1:
                row[i] = (row[i] + left) & 255
            elif filter_type == 2:
                row[i] = (row[i] + above) & 255
            elif filter_type == 3:
                row[i] = (row[i] + ((left + above) // 2)) & 255
            elif filter_type == 4:
                predict = left + above - upper_left
                distances = (abs(predict - left), abs(predict - above), abs(predict - upper_left))
                choice = (left, above, upper_left)[distances.index(min(distances))]
                row[i] = (row[i] + choice) & 255
            else:
                assert filter_type == 0, f"Unsupported PNG filter {filter_type}"
        pixels.extend(row)
        previous = row
    assert offset == len(raw)
    assert max(pixels) <= 239, "Reserve palette bank 15 / indices 240-255"
    assert 0 not in pixels, "Palette index zero is reserved for transparency"
    palette += [(0, 0, 0)] * (256 - len(palette))
    return pixels, palette


def compress_lz10(data):
    output = bytearray((0x10, len(data) & 255, len(data) >> 8 & 255, len(data) >> 16 & 255))
    seen = defaultdict(deque)
    position = 0
    while position < len(data):
        flags_position = len(output)
        output.append(0)
        flags = 0
        for bit in range(8):
            if position >= len(data):
                break
            best_length = 0
            best_distance = 0
            if position + 2 < len(data):
                candidates = seen[data[position:position + 3]]
                while candidates and position - candidates[0] > 4096:
                    candidates.popleft()
                for old in reversed(candidates):
                    match_length = 3
                    maximum = min(18, len(data) - position)
                    while match_length < maximum and data[old + match_length] == data[position + match_length]:
                        match_length += 1
                    if match_length > best_length:
                        best_length = match_length
                        best_distance = position - old - 1
                        if match_length == 18:
                            break
            if best_length >= 3:
                flags |= 0x80 >> bit
                match_code = (best_length - 3) << 12 | best_distance
                output.extend((match_code >> 8, match_code & 255))
                advance = best_length
            else:
                output.append(data[position])
                advance = 1
            for i in range(advance):
                p = position + i
                if p + 2 < len(data):
                    candidates = seen[data[p:p + 3]]
                    candidates.append(p)
                    while candidates and p - candidates[0] > 4096:
                        candidates.popleft()
            position += advance
        output[flags_position] = flags
    while len(output) % 4:
        output.append(0)
    return output


def main():
    pixels, palette = read_png(SOURCE)
    tiles = [bytes(64)]
    dedup = {tiles[0]: 0}
    tilemap = bytearray(2048)
    for ty in range(20):
        for tx in range(30):
            tile = b"".join(pixels[(ty * 8 + row) * 240 + tx * 8:(ty * 8 + row) * 240 + tx * 8 + 8]
                            for row in range(8))
            if tile not in dedup:
                dedup[tile] = len(tiles)
                tiles.append(tile)
            struct.pack_into("<H", tilemap, (ty * 32 + tx) * 2, dedup[tile])
    raw_tiles = b"".join(tiles)
    assert len(raw_tiles) < 30 * 2048, "Tiles cannot overlap screen block 30"
    gba_palette = bytearray(512)
    for index, (red, green, blue) in enumerate(palette):
        value = (red >> 3) | (green >> 3) << 5 | (blue >> 3) << 10
        struct.pack_into("<H", gba_palette, index * 2, value)
    (DIRECTORY / "anthology_title_tiles.bin.lz").write_bytes(compress_lz10(raw_tiles))
    (DIRECTORY / "anthology_title_map.bin").write_bytes(tilemap)
    (DIRECTORY / "anthology_title_palette.bin").write_bytes(gba_palette)
    print(f"Grand Anthology title: {len(tiles)} native BG tiles, {len(raw_tiles)} bytes, 240x160, palette indexed")


if __name__ == "__main__":
    main()
