#!/usr/bin/env python3
"""A2 crash-point matrix. Every file is synthetic and disposable."""
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
from test_xgba_a2_storage import native_fixture


def atomic_partial(data, offset, record, budget):
    """Model the same write order as src/xgba_a2_journal.c."""
    chunks = (
        (offset, bytes(8)),
        (offset + a2.HEADER.size, record[a2.HEADER.size:]),
        (offset + 8, record[8:a2.HEADER.size]),
        (offset + 1, record[1:8]),
        (offset, record[:1]),
    )
    for pos, content in chunks:
        if budget <= 0:
            return
        written = min(len(content), budget)
        data[pos:pos + written] = content[:written]
        budget -= written


def make_new_native_slot(original, new_pc):
    """Write all 14 valid logical sectors of next native slot, counter 3."""
    sectors = []
    for sector in range(sp.SECTORS_PER_SLOT):
        start = sector * sp.SECTOR_SIZE
        raw = bytearray(original[start:start + sp.SECTOR_SIZE])
        offset, count = sp.SLOT_LAYOUT[sector]
        if sector >= sp.ID_PK_START:
            raw[:count] = new_pc[offset:offset + count]
        checksum = sp.calc_checksum(raw, count)
        struct.pack_into("<HHII", raw, sp.SECTOR_DATA_SIZE + sp.SB3_CHUNK,
                         sector, checksum, sp.SIGNATURE, 3)
        sectors.append(bytes(raw))
    return sectors


class TransactionFaultMatrix(unittest.TestCase):
    def setUp(self):
        self.work = tempfile.TemporaryDirectory()
        self.addCleanup(self.work.cleanup)
        root = Path(self.work.name)
        source = root / "old-native.sav"
        self.test_file = root / "candidate.sav"
        native_fixture(source)
        a2.prepare(source, self.test_file)
        self.base = self.test_file.read_bytes()
        native_save = sp.self_test(str(source))
        pc = bytearray(native_save.pk)
        pc[4] ^= 0x5A
        self.new_pc = bytes(pc)
        self.page_record = a2.make_record(a2.PAGE_MAGIC, 0, 3, self.new_pc)
        self.new_sectors = make_new_native_slot(self.base, self.new_pc)
        self.staged = bytearray(self.base)
        atomic_partial(self.staged, a2.page_offset(0, 0),
                       self.page_record, 34184)
        for i, block in enumerate(self.new_sectors):
            pos = (sp.SECTORS_PER_SLOT + i) * sp.SECTOR_SIZE
            self.staged[pos:pos + sp.SECTOR_SIZE] = block
        new_native = self.staged[:a2.NATIVE_BYTES]
        payload = a2.META_PAYLOAD.pack(
            a2.fingerprint(new_native, 1), 3, 0, 1,
            3, 2, 2, 2, 2, 2, 2, 2
        )
        self.meta_record = a2.make_record(a2.META_MAGIC, 0, 3, payload)

    def inspect(self, data):
        self.test_file.write_bytes(data)
        return a2.audit(self.test_file)

    def test_page_write_crashes_never_expose_uncommitted_mons(self):
        for limit in (0, 1, 7, 8, 9, 31, 100, 3000, 34000,
                      34152, 34176, 34183, 34184):
            with self.subTest(bytes_written=limit):
                data = bytearray(self.base)
                atomic_partial(data, a2.page_offset(0, 0),
                               self.page_record, limit)
                saved = self.inspect(data)
                self.assertEqual(saved["counter"], 2)
                self.assertEqual(saved["native_slot"], 0)
                self.assertEqual(saved["page_generations"][0], 2)

    def test_native_slot_write_crashes_keep_committed_old_cohort(self):
        for sectors_written in (0, 1, 2, 5, 7, 13, 14):
            with self.subTest(sectors=sectors_written):
                data = bytearray(self.base)
                atomic_partial(data, a2.page_offset(0, 0),
                               self.page_record, 34184)
                for i in range(sectors_written):
                    offset = (sp.SECTORS_PER_SLOT + i) * sp.SECTOR_SIZE
                    data[offset:offset + sp.SECTOR_SIZE] = self.new_sectors[i]
                saved = self.inspect(data)
                self.assertEqual(saved["counter"], 2)
                self.assertEqual(saved["native_slot"], 0)
                self.assertEqual(saved["page_generations"][0], 2)
                if sectors_written == 14:
                    self.assertTrue(saved["recovery_required"])

    def test_metadata_commit_is_only_global_visibility_boundary(self):
        for limit in (0, 1, 7, 8, 9, 32, 70, 88, 95, 96):
            with self.subTest(bytes_written=limit):
                data = bytearray(self.staged)
                atomic_partial(data, a2.meta_offset(0),
                               self.meta_record, limit)
                saved = self.inspect(data)
                if limit < 96:
                    self.assertEqual(saved["counter"], 2)
                    self.assertEqual(saved["native_slot"], 0)
                    self.assertEqual(saved["page_generations"][0], 2)
                    self.assertTrue(saved["recovery_required"])
                else:
                    self.assertEqual(saved["counter"], 3)
                    self.assertEqual(saved["native_slot"], 1)
                    self.assertEqual(saved["page_generations"][0], 3)
                    self.assertFalse(saved["recovery_required"])

    def test_after_commit_corrupt_newest_page_rolls_back_consistently(self):
        data = bytearray(self.staged)
        atomic_partial(data, a2.meta_offset(0), self.meta_record, 96)
        page = a2.page_offset(0, 0) + a2.HEADER.size + 200
        data[page] ^= 0xFF
        saved = self.inspect(data)
        self.assertEqual(saved["counter"], 2)
        self.assertEqual(saved["page_generations"][0], 2)
        self.assertTrue(saved["recovery_required"])


if __name__ == "__main__":
    unittest.main()
