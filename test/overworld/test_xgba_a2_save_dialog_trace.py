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
        game_save = (ROOT / "src/save.c").read_text()
        self.assertIn("EWRAM_DATA volatile u32 gXgbaA2GameSaveTrace = 0;", game_save)
        self.assertIn("A2_SAVE_TRACE(0xE6, result)", game_save)
        self.assertIn("A2_SAVE_TRACE(0xE8, result)", game_save)
        self.assertIn("A2_SAVE_TRACE(0xE5, 0)", game_save)
        self.assertIn("A2_XGBA_GAME_SAVE_TRACE_CHANGE", lua)
        self.assertIn("A2_XGBA_DIAG_LAST_NONZERO_SAVE_TRACE", lua)
        pager = (ROOT / "src/xgba_a2_pager.c").read_text()
        self.assertIn("extern volatile uint32_t gXgbaA2PagerTrace;", pager)
        self.assertIn("PAGER_TRACE(2, status);", pager)
        self.assertIn("PAGER_TRACE(4, status);", pager)
        self.assertIn("PAGER_TRACE(6, status);", pager)
        self.assertIn("A2_XGBA_DIAG_PAGER_TRACE", lua)
        self.assertIn("A2_XGBA_JOURNAL_COMMIT_ERROR_STAGE_", lua)
        self.assertIn('"gXgbaA2PagerTrace": "A2_PAGER_TRACE"', elf)
        self.assertIn('"gXgbaA2NativeWriteTrace": "A2_NATIVE_WRITE_TRACE"', elf)
        self.assertIn('"gXgbaA2FlashWriteTrace": "A2_FLASH_WRITE_TRACE"', elf)
        self.assertIn("A2_XGBA_DIAG_NATIVE_WRITE_TRACE", lua)
        self.assertIn("A2_XGBA_FLASH_WRITE_TRACE_CHANGE", lua)
        self.assertIn("A2_NATIVE_TRACE(0xE4, gSaveCounter);", game_save)
        self.assertIn("A2_NATIVE_TRACE(0xE6, result);", game_save)
        self.assertIn("A2_FLASH_TRACE(0xE0 | (sector / NUM_SECTORS_PER_SLOT)", game_save)
        tx = (ROOT / "src/xgba_a2_transaction.c").read_text()
        self.assertIn("extern volatile uint32_t gXgbaA2TransactionTrace;", tx)
        self.assertIn("TX_TRACE(0xE3, prepareStatus);", tx)
        self.assertIn("TX_TRACE(0xE4, nextCounter);", tx)
        self.assertIn("TX_TRACE(0xE7, selectStatus);", tx)
        self.assertIn('A2_XGBA_TRANSACTION_TRACE_CHANGE', lua)
        self.assertIn('A2_XGBA_DIAG_TRANSACTION_TRACE', lua)
        self.assertIn('"gXgbaA2TransactionTrace": "A2_TRANSACTION_TRACE"', elf)
        self.assertIn('"gXgbaA2GameSaveTrace": "A2_GAME_SAVE_TRACE"', elf)


if __name__ == "__main__":
    unittest.main()
