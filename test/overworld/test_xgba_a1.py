"""A1 ROM mapping contract regression tests; no emulator or full ROM needed."""
import importlib.util
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location("a1", ROOT / "tools/xgba_a1_assets.py")
a1 = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(a1)


class A1Tests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.plan = a1.manifest_plan()

    def test_full_existing_source_inventory(self):
        self.assertEqual(self.plan["count"], 1043)
        self.assertEqual(len(self.plan["assets"]), 2086)
        self.assertEqual(len(set(x["label"] for x in self.plan["assets"])), 2086)
        self.assertEqual(set(x["lane"] for x in self.plan["assets"]), set(a1.LANES))

    def test_all_assets_outside_native_and_inside_capacity_lanes(self):
        for asset in self.plan["assets"]:
            start, end = a1.LANES[asset["lane"]]
            self.assertGreaterEqual(asset["offset"], start)
            self.assertGreaterEqual(asset["offset"], a1.ROM_NATIVE_LIMIT)
            self.assertLessEqual(asset["offset"] + max(asset["size"], 2), end)
            self.assertEqual(asset["address"], a1.XGBA_CPU_BASE + asset["offset"])
            self.assertEqual(asset["offset"] % 4, 0)
        self.assertEqual(
            [x["lane"] for x in self.plan["assets"] if x["label"].startswith("RegionHub_Layout_")],
            ["hub", "hub"]
        )

    def test_rewriter_replaces_every_literal_blob_and_keeps_metadata(self):
        rows = []
        for item in self.plan["assets"]:
            rows.append(item["label"] + '::\n\t.incbin "' + item["path"] + '"\n')
        rows.append("TestMetadata::\n\t.4byte RegionHub_Layout_Blockdata\n")
        result = a1.rewrite_layout_assembly("\n".join(rows), self.plan)
        self.assertEqual(result.count(".set "), 2086)
        self.assertNotIn(".incbin", result)
        self.assertIn("TestMetadata::", result)
        self.assertIn(".4byte RegionHub_Layout_Blockdata", result)
        with self.assertRaisesRegex(ValueError, "omitted"):
            a1.rewrite_layout_assembly(rows[0], self.plan)

    def test_header_tag_is_rom_only_and_complement_correct(self):
        native = bytearray([0xFF] * 0xC0)
        native[0xAC:0xB0] = b"BPEE"
        tagged = a1.tagged_native(native)
        self.assertEqual(tagged[0xB5:0xBC], b"XGBA256")
        self.assertEqual(tagged[0xBC], 0)
        self.assertEqual(tagged[0xBD], a1.checksum_header(tagged))
        self.assertEqual(tagged[0xAC:0xB0], b"BPEE")
        with self.assertRaisesRegex(ValueError, "Truncated"):
            a1.tagged_native(b"hi")

    def test_probe_reaches_final_byte_and_is_offset_bound(self):
        self.assertEqual(a1.PROBE_OFFSET + len(a1.probe_bytes()), a1.ROM_BYTES)
        self.assertEqual(len(a1.probe_bytes()), 16)
        self.assertEqual(a1.probe_bytes()[:8], b"XGBAA1!!")
        with tempfile.TemporaryFile() as file:
            file.truncate(a1.ROM_BYTES)
            file.seek(a1.PROBE_OFFSET)
            file.write(a1.probe_bytes())
            file.seek(a1.ROM_BYTES - 16)
            self.assertEqual(file.read(16), a1.probe_bytes())

    def test_build_has_no_save_or_registry_changes(self):
        makefile = (ROOT / "Makefile").read_text()
        maps = (ROOT / "data/maps.s").read_text()
        self.assertIn("XGBA_A1=1", makefile)
        self.assertIn('BUILD_NAME=xgba-a1', makefile)
        self.assertIn("xgba_a1_assets.py generate", makefile)
        self.assertIn("xgba_a1_assets.py pack", makefile)
        self.assertIn('build/xgba-a1-release/xgba-a1-layouts.inc', maps)
        self.assertIn('data/layouts/layouts.inc', maps)


if __name__ == "__main__":
    unittest.main()
