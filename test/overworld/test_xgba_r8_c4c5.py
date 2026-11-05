#!/usr/bin/env python3
"""R8 C5 crash-consistency tests for two distant dirty PC groups.

All 16 MiB saves are generated inside a disposable temporary directory.
The scenarios deliberately publish the same native-slot/page/meta sequence
used by the production journal; no game code, player saves or emulator
state is changed by these tests.
"""
from pathlib import Path
import struct
import sys
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "test" / "overworld"))

import xgba_a2_storage as a2
from test_xgba_a2_transactions import TransactionFaultMatrix, atomic_partial


class C5MultiGroupRecovery(unittest.TestCase):
    def setUp(self):
        # Use the already audited authentic native-slot layout and a
        # metadata-pinned counter-2 cohort, never handwave Flash geometry.
        self.fixture = TransactionFaultMatrix(
            "test_page_write_crashes_never_expose_uncommitted_mons")
        self.fixture.setUp()
        self.addCleanup(self.fixture.doCleanups)

        self.staged = bytearray(self.fixture.staged)
        old_page7 = a2.page_offset(7, 1)
        start = old_page7 + a2.HEADER.size
        page7 = bytearray(self.fixture.base[start:start + a2.PC_BYTES])
        self.remote_slot = a2.BOX_REGION_START + 419 * 80
        self.assertEqual(page7[self.remote_slot:self.remote_slot + 80], bytes(80))
        # A distant Box112 slot30 record at the upper edge of the collection.
        self.remote_mon = bytes((0xA0 + (i % 33) for i in range(80)))
        page7[self.remote_slot:self.remote_slot + 80] = self.remote_mon
        self.page7_record = a2.make_record(a2.PAGE_MAGIC, 7, 3, bytes(page7))
        self.staged[a2.page_offset(7, 0):
                    a2.page_offset(7, 0) + len(self.page7_record)] = self.page7_record

        native = self.staged[:a2.NATIVE_BYTES]
        payload = a2.META_PAYLOAD.pack(
            a2.fingerprint(native, 1), 3, 0, 1, 3, 2, 2, 2, 2, 2, 2, 3)
        self.meta = a2.make_record(a2.META_MAGIC, 0, 3, payload)

    def examine(self, image):
        return self.fixture.inspect(image)

    def records(self, image):
        result = self.examine(image)
        return result, a2.verified_collection_box_bytes(self.fixture.test_file, result)

    def committed(self):
        state = bytearray(self.staged)
        atomic_partial(state, a2.meta_offset(0), self.meta, len(self.meta))
        return state

    def test_two_distant_groups_publish_as_one_coherent_generation(self):
        previous, old = self.records(self.fixture.base)
        current, new = self.records(self.committed())
        self.assertEqual(previous["counter"], 2)
        self.assertEqual(current["counter"], 3)
        self.assertFalse(current["recovery_required"])
        self.assertEqual(current["page_generations"], [3, 2, 2, 2, 2, 2, 2, 3])
        self.assertEqual(len(old), 3360 * 80)
        self.assertEqual(len(new), 3360 * 80)
        different = [i for i in range(3360)
                     if old[i * 80:(i + 1) * 80] != new[i * 80:(i + 1) * 80]]
        self.assertEqual(different, [0, 3359])
        self.assertEqual(new[-80:], self.remote_mon)
        self.assertEqual(new[80:-80], old[80:-80])

    def test_torn_page7_or_metadata_never_exposes_half_collection(self):
        old_report, original = self.records(self.fixture.base)
        self.assertEqual(old_report["counter"], 2)
        for budget in (0, 1, 7, 8, 9, 31, 32, 100,
                       len(self.page7_record) - 1):
            with self.subTest(remote_page_bytes=budget):
                state = bytearray(self.fixture.staged)
                atomic_partial(state, a2.page_offset(7, 0),
                               self.page7_record, budget)
                report, recovered = self.records(state)
                self.assertEqual(report["counter"], 2)
                self.assertEqual(recovered, original)
        for budget in (0, 1, 7, 8, 9, 31, 32, 50, len(self.meta) - 1):
            with self.subTest(meta_bytes=budget):
                state = bytearray(self.staged)
                atomic_partial(state, a2.meta_offset(0), self.meta, budget)
                report, recovered = self.records(state)
                self.assertEqual(report["counter"], 2)
                self.assertEqual(recovered, original)

    def test_corrupt_newest_remote_page_recovers_entire_old_cohort(self):
        _, old = self.records(self.fixture.base)
        corrupt = self.committed()
        corrupt[a2.page_offset(7, 0) + a2.HEADER.size + 150] ^= 0xFF
        report, recovered = self.records(corrupt)
        self.assertEqual(report["counter"], 2)
        self.assertTrue(report["recovery_required"])
        self.assertEqual(recovered, old)

    def test_both_remote_copies_corrupt_refuses_to_synthesize_empty_boxes(self):
        corrupt = self.committed()
        for slot in (0, 1):
            corrupt[a2.page_offset(7, slot) + a2.HEADER.size + 200] ^= 0xFF
        with self.assertRaises(a2.A2Error):
            self.examine(corrupt)


if __name__ == "__main__":
    unittest.main()
