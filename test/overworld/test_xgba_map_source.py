"""Preprocessor ordering regression: only the active XGBA layout may be opened."""
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "tools" / "xgba_select_map_layout.py"


class TestXgbaMapSource(unittest.TestCase):
    def test_selects_only_one_active_layout_in_real_map_source(self):
        original = (ROOT / "data/maps.s").read_text()
        self.assertEqual(original.count('.include "data/layouts/layouts.inc"'), 1)
        self.assertNotIn("build/xgba-a1-release/xgba-a1-layouts.inc", original)
        self.assertNotIn("build/xgba-a2-release/xgba-a1-layouts.inc", original)
        with tempfile.TemporaryDirectory() as folder:
            folder = Path(folder)
            for variant in ("xgba-a1-release", "xgba-a2-release"):
                layout = folder / variant / "xgba-a1-layouts.inc"
                layout.parent.mkdir()
                layout.write_text(".byte 1\n")
                output = layout.parent / "maps_selected.s"
                run = subprocess.run(
                    [sys.executable, str(SCRIPT), str(ROOT / "data/maps.s"),
                     str(layout), str(output)],
                    capture_output=True, text=True,
                )
                self.assertEqual(run.returncode, 0, run.stderr)
                result = output.read_text()
                self.assertIn('.include "' + layout.as_posix() + '"', result)
                self.assertNotIn('.include "data/layouts/layouts.inc"', result)
                self.assertEqual(result.count("xgba-a1-layouts.inc"), 1)
                self.assertEqual(len(result.splitlines()), len(original.splitlines()))

    def test_does_not_modify_source_or_accept_missing_layout(self):
        with tempfile.TemporaryDirectory() as folder:
            folder = Path(folder)
            source = folder / "maps.s"
            source.write_text('.include "data/layouts/layouts.inc"\n')
            result = subprocess.run(
                [sys.executable, str(SCRIPT), str(source),
                 str(folder / "missing.inc"), str(folder / "selected.s")],
                capture_output=True, text=True,
            )
            self.assertNotEqual(result.returncode, 0)
            self.assertFalse((folder / "selected.s").exists())
            self.assertEqual(source.read_text(), '.include "data/layouts/layouts.inc"\n')


if __name__ == "__main__":
    unittest.main()
