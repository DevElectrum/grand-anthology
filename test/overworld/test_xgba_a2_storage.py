#!/usr/bin/env python3
"""A2 save migration regression suite: all fixtures synthetic, no user saves."""
from pathlib import Path
import struct
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "test" / "overworld"))
import xgba_a2_storage as a2
import SavePatch as sp


def native_fixture(path, version=sp.SAVE_FORMAT_VERSION, rtc=False):
    data = bytearray(b"\xFF" * sp.FLASH_SIZE)
    for sector_id in range(sp.SECTORS_PER_SLOT):
        start = sector_id * sp.SECTOR_SIZE
        block = bytearray(sp.SECTOR_SIZE)
        if sector_id == 0:
            block[sp.V["SB2.saveVersion"]] = version
        if sector_id == sp.ID_PK_START:
            block[4:12] = bytes.fromhex("0102030405060708")
        size = sp.SLOT_LAYOUT[sector_id][1]
        checksum = sp.calc_checksum(block, size)
        struct.pack_into("<HHII", block, sp.SECTOR_DATA_SIZE + sp.SB3_CHUNK,
                         sector_id, checksum, sp.SIGNATURE, 2)
        data[start:start + sp.SECTOR_SIZE] = block
    with path.open("wb") as f:
        f.write(data)
        if rtc:
            f.write(bytes(range(16)))
    return bytes(data)


class A2Tests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.src = Path(self.tmp.name) / "native.sav"
        self.dst = Path(self.tmp.name) / "a2.sav"

    def test_layout_and_emulator_header(self):
        self.assertEqual(a2.PAGE_MAGIC, b"XGBAA2P\x00")
        self.assertEqual(a2.HEADER.size, 32)
        self.assertEqual(a2.META_PAYLOAD.size, 24)
        self.assertEqual(a2.PC_BYTES, 34144)
        self.assertEqual(a2.page_offset(7, 1) + a2.PC_STRIDE, a2.META_BASE)
        self.assertLess(a2.META_BASE + 2 * a2.META_STRIDE, a2.SUPER_BACKUP)
        self.assertEqual(a2.SUPER_HEADER[:8], b"XGBAE6S\x00")
        self.assertEqual(a2.SUPER_HEADER[8:12], b"\x02\x00\x00\x00")
        self.assertEqual(a2.SAVE_BYTES, 0x1000000)

    def test_create_only_preserves_every_native_byte(self):
        before = native_fixture(self.src)
        result = a2.prepare(self.src, self.dst)
        self.assertEqual(result["pc_boxes"], 112)
        self.assertEqual(result["slots"], 3360)
        self.assertEqual(result["healthy_page_copies"], [2] * 8)
        self.assertEqual(self.src.read_bytes(), before)
        self.assertEqual(self.dst.read_bytes()[:a2.NATIVE_BYTES], before)
        with self.assertRaises(a2.A2Error):
            a2.prepare(self.src, self.dst)

    def test_rtc_preserved_in_new_trailer(self):
        before = native_fixture(self.src, rtc=True)
        a2.prepare(self.src, self.dst)
        with self.dst.open("rb") as f:
            f.seek(a2.SAVE_BYTES)
            self.assertEqual(f.read(), bytes(range(16)))
        self.assertEqual(self.src.read_bytes(), before + bytes(range(16)))

    def test_single_corrupt_copy_recovers_from_older(self):
        native_fixture(self.src)
        a2.prepare(self.src, self.dst)
        with self.dst.open("r+b") as f:
            f.seek(a2.page_offset(7, 1) + a2.HEADER.size + 17)
            f.write(b"\x99")
        stats = a2.audit(self.dst)
        self.assertEqual(stats["healthy_page_copies"][7], 1)

    def test_double_corruption_fails_closed(self):
        native_fixture(self.src)
        a2.prepare(self.src, self.dst)
        with self.dst.open("r+b") as f:
            for slot in (0, 1):
                f.seek(a2.page_offset(0, slot) + a2.HEADER.size + 18)
                f.write(b"\x99")
        with self.assertRaisesRegex(a2.A2Error, "both copies"):
            a2.audit(self.dst)

    def test_native_generation_mismatch_fails(self):
        native_fixture(self.src)
        a2.prepare(self.src, self.dst)
        with self.dst.open("r+b") as f:
            f.seek(100)
            f.write(b"\xA4")
        with self.assertRaises(a2.A2Error):
            a2.audit(self.dst)

    def test_old_save_refused(self):
        native_fixture(self.src, version=12)
        with self.assertRaisesRegex(a2.A2Error, "Save format"):
            a2.prepare(self.src, self.dst)
        self.assertFalse(self.dst.exists())

    def test_partial_save_refused(self):
        native_fixture(self.src)
        with self.src.open("r+b") as f:
            f.seek(5 * sp.SECTOR_SIZE + 20)
            f.write(b"\xBE")
        with self.assertRaises(a2.A2Error):
            a2.prepare(self.src, self.dst)
        self.assertFalse(self.dst.exists())

    def test_bad_emulator_superblock_refused(self):
        native_fixture(self.src)
        a2.prepare(self.src, self.dst)
        with self.dst.open("r+b") as f:
            f.seek(a2.SUPER_BACKUP)
            f.write(b"!")
        with self.assertRaisesRegex(a2.A2Error, "superblocks"):
            a2.audit(self.dst)

    def test_page_one_must_match_original_pc(self):
        native_fixture(self.src)
        a2.prepare(self.src, self.dst)
        with self.dst.open("r+b") as f:
            for slot, seq in ((0, 1), (1, 2)):
                altered = bytearray(a2.PC_BYTES)
                altered[4] = 77
                f.seek(a2.page_offset(0, slot))
                f.write(a2.make_record(a2.PAGE_MAGIC, 0, seq, altered))
        with self.assertRaisesRegex(a2.A2Error, "byte-identical"):
            a2.audit(self.dst)

    def test_c_format_contract_and_save_abi(self):
        header = (ROOT / "include/xgba_a2_storage_format.h").read_text()
        self.assertIn("#define XGBA_A2_PC_BYTES 34144", header)
        self.assertIn("#define XGBA_A2_PC_BASE 0x200000", header)
        sp.require_pins()


if __name__ == "__main__":
    unittest.main()
