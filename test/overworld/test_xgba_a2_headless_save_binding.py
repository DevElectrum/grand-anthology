"""Headless XGBA must explicitly bind a pre-existing 16MiB E6 save."""
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


class TestA2HeadlessSaveBinding(unittest.TestCase):
    def test_binding_is_create_never_and_header_verified(self):
        script = (ROOT / "tools/xgba_a2_bind_save.lua").read_text()
        self.assertIn('assert(os.getenv("A2_E6_SAVE_PATH")', script)
        self.assertIn('io.open(path, "rb")', script)
        self.assertIn('size == 16777216', script)
        self.assertIn('emu:loadSaveFile(path, false)', script)
        self.assertIn('emu:read8(0x0F000000 + offset)', script)
        self.assertIn('A2_XGBA_E6_SAVE_BOUND', script)

    def test_every_scripted_game_process_binds_save_first(self):
        workflow = (ROOT / ".github/workflows/A2-XGBA-Boot-Probe.yml").read_text()
        self.assertIn('A2_E6_SAVE_PATH: build/a2-ci-runtime/a2-boot.sav', workflow)
        self.assertEqual(workflow.count('--script tools/xgba_a2_bind_save.lua'), 3)
        self.assertIn(
            '--script tools/xgba_a2_bind_save.lua \\\n'
            '            --script tools/xgba_a2_runtime_boot.lua', workflow,
        )
        self.assertEqual(
            workflow.count(
                '--script tools/xgba_a2_bind_save.lua \\\n'
                '            --script tools/xgba_a2_runtime_save.lua'
            ), 2
        )


if __name__ == "__main__":
    unittest.main()
