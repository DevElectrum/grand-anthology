"""Guard the A2 save-failure path against legacy Flash erasure/retry behavior."""
import unittest
from pathlib import Path


class TestA2SaveFailureScreen(unittest.TestCase):
    def test_a2_failure_never_wipes_or_retries(self):
        source = (Path(__file__).resolve().parents[2] / "src" / "save_failed_screen.c").read_text()
        start = source.index("static void CB2_WipeSave(void)\n{")
        end = source.index("\nstatic void CB2_GameplayCannotBeContinued(void)", start)
        function = source[start:end]
        a2_begin = function.index("#if defined(XGBA_A2) && XGBA_A2")
        a2_end = function.index("\n#else", a2_begin)
        branch = function[a2_begin:a2_end]

        self.assertNotIn("WipeSectors(", branch)
        self.assertNotIn("ProgramFlashByte(", branch)
        self.assertNotIn("HandleSavingData(", branch)
        self.assertNotIn("gText_SaveComplete", branch)
        self.assertIn("gGameContinueCallback = NULL;", branch)
        self.assertIn("SetMainCallback2(CB2_FadeAndReturnToTitleScreen);", branch)
        self.assertIn("SaveFailedScreenTextPrint(sText_A2SaveFailed", branch)

    def test_original_legacy_failure_code_remains_separate(self):
        source = (Path(__file__).resolve().parents[2] / "src" / "save_failed_screen.c").read_text()
        start = source.index("static void CB2_WipeSave(void)\n{")
        end = source.index("\nstatic void CB2_GameplayCannotBeContinued(void)", start)
        function = source[start:end]
        self.assertIn("#else\n    u8 wipeTries = 0;", function)
        self.assertIn("WipeSectors(gDamagedSaveSectors)", function)
        self.assertIn("HandleSavingData(sSaveFailedType)", function)


if __name__ == "__main__":
    unittest.main()
