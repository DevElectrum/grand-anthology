"""A2-only Save-dialog telemetry must not affect saved game data or A1."""
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


class TestA2SaveDialogTrace(unittest.TestCase):
    def test_trace_is_a2_only_and_not_part_of_save_blocks(self):
        code = (ROOT / "src/save_dialog.c").read_text()
        self.assertIn("#if defined(XGBA_A2) && XGBA_A2", code)
        self.assertIn("EWRAM_DATA volatile u32 gXgbaA2SaveDialogTrace = 0;", code)
        self.assertIn("#define A2_TRACE_SAVE_DIALOG(n) ((void)0)", code)
        self.assertIn("gXgbaA2SaveDialogTrace = 1u;", code)
        self.assertNotIn("gXgbaA2SaveDialogTrace", (ROOT / "include/global.h").read_text())

    def test_callback_reachability_bits_are_pinned(self):
        code = (ROOT / "src/save_dialog.c").read_text()
        callbacks = [
            "SaveConfirmSaveCallback", "SaveYesNoCallback",
            "SaveConfirmInputCallback", "SaveFileExistsCallback",
            "SaveConfirmOverwriteCallback", "SaveOverwriteInputCallback",
            "SaveSavingMessageCallback", "SaveDoSaveCallback",
            "SaveSuccessCallback", "SaveErrorCallback",
        ]
        for bit, callback in enumerate(callbacks, 1):
            with self.subTest(callback=callback):
                needle = "static u8 " + callback + "(void)\n{\n    A2_TRACE_SAVE_DIALOG(" + str(bit) + ");"
                self.assertIn(needle, code)

    def test_elf_and_lua_use_linked_trace_symbol(self):
        elf = (ROOT / "tools/xgba_a2_symbol_env.py").read_text()
        lua = (ROOT / "tools/xgba_a2_runtime_save.lua").read_text()
        self.assertIn('"gXgbaA2SaveDialogTrace": "A2_SAVE_DIALOG_TRACE"', elf)
        self.assertIn('addr("A2_SAVE_DIALOG_TRACE")', lua)
        self.assertIn("A2_XGBA_DIAG_SAVE_DIALOG_TRACE", lua)


if __name__ == "__main__":
    unittest.main()
