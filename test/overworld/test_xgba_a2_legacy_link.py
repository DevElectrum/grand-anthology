"""Source-level safety contract for experimental A2 legacy link entrypoints."""
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]


def section(path, start, end):
    source = (ROOT / path).read_text()
    return source[source.index(start):source.index(end, source.index(start))]


class TestA2LegacyLinkSafety(unittest.TestCase):
    def test_legacy_incremental_entrypoints_reject_instead_of_succeeding(self):
        source = (ROOT / "src/save.c").read_text()
        method_names = [
            "LinkFullSave_Init",
            "LinkFullSave_WriteSector",
            "LinkFullSave_ReplaceLastSector",
            "LinkFullSave_SetLastSectorSignature",
            "WriteSaveBlock2",
            "WriteSaveBlock1Sector",
        ]
        for name in method_names:
            with self.subTest(name=name):
                body = source[source.index("bool8 " + name + "(void)"):].split(
                    "#endif", 1
                )[0]
                self.assertIn("#if defined(XGBA_A2) && XGBA_A2", body)
                self.assertIn("return A2RejectIncrementalSave();", body)
                self.assertNotIn("return TRUE;", body)
        guard = section(
            "src/save.c",
            "static bool8 A2RejectIncrementalSave(void)",
            "\nbool8 LinkFullSave_Init(void)",
        )
        self.assertIn("sA2Ready = FALSE;", guard)
        self.assertIn("gSaveAttemptStatus = SAVE_STATUS_ERROR;", guard)
        self.assertIn("DoA2UnsupportedLinkScreen();", guard)

    def test_link_trade_is_blocked_before_pokemon_exchange(self):
        source = (ROOT / "src/trade.c").read_text()
        for function, next_part in [
            ("void CB2_StartCreateTradeMenu(void)", "static void CB2_CreateTradeMenu(void)"),
            ("void CB2_LinkTrade(void)", "void InitTradeSequenceBgGpuRegs(void)"),
        ]:
            with self.subTest(function=function):
                start = source.index(function + "\n{")
                section = source[start:source.index("#endif", start)]
                self.assertIn("CloseLink();", section)
                self.assertIn("DoA2UnsupportedLinkScreen();", section)
                self.assertIn("return;", section)

    def test_unsupported_link_is_not_reported_as_success(self):
        screen = (ROOT / "src/save_failed_screen.c").read_text()
        self.assertIn("void DoA2UnsupportedLinkScreen(void)", screen)
        self.assertIn("sA2UnsupportedLink = TRUE;", screen)
        self.assertIn("sText_A2UnsupportedLink", screen)
        self.assertIn("gGameContinueCallback = NULL;", screen)


if __name__ == "__main__":
    unittest.main()
