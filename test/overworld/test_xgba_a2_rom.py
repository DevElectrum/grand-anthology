#!/usr/bin/env python3
"""No ROM files are distributed or needed for A2 header qualification."""
import sys
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))

import xgba_a1_assets as a1
import xgba_a2_rom as a2


class A2HeaderTests(unittest.TestCase):
    def test_only_e6_revision_and_checksum_change(self):
        header = bytearray((i * 17 + 13) & 255 for i in range(0xC0))
        header[0xB5:0xBC] = a1.MAGIC
        header[0xBC] = 0
        header[0xBD] = a1.checksum_header(header)
        tagged = a2.stamp_header(header)
        self.assertEqual(tagged[0xB5:0xBC], a1.MAGIC)
        self.assertEqual(tagged[0xBC], 0xE6)
        self.assertEqual(tagged[0xBD], a1.checksum_header(tagged))
        changed = [i for i in range(len(header)) if tagged[i] != header[i]]
        self.assertEqual(changed, [0xBC, 0xBD])
        with self.assertRaises(a2.A2RomError):
            a2.stamp_header(tagged)

    def test_bad_input_magic_checksum_or_length_refused(self):
        header = bytearray(0xC0)
        header[0xB5:0xBC] = a1.MAGIC
        header[0xBD] = a1.checksum_header(header)
        cases = [header[:10], header + b"!",
                 header[:0xB5] + b"WRONG!!" + header[0xBC:],
                 header[:0xBD] + bytes([header[0xBD] ^ 0xFF]) + header[0xBE:]]
        for candidate in cases:
            with self.subTest(length=len(candidate)):
                with self.assertRaises(a2.A2RomError):
                    a2.stamp_header(candidate)


if __name__ == "__main__":
    unittest.main()
