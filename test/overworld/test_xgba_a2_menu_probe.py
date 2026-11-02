"""Keep disposable graphical Save-menu introspection read-only and A2-only."""
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


class TestA2MenuProbe(unittest.TestCase):
    def test_probe_exposed_only_in_a2_and_tracks_menu_lifetime(self):
        code = (ROOT / "src" / "unbound_start_menu.c").read_text()
        marker = "gXgbaA2UsmStateProbe"
        self.assertEqual(code.count(marker), 3)
        declaration = "EWRAM_DATA struct Usm_State * volatile " + marker + " = NULL;"
        self.assertIn(declaration, code)
        self.assertIn(
            "sUsmState = &sUsmMemory->state;\n"
            "#if defined(XGBA_A2) && XGBA_A2\n"
            "    gXgbaA2UsmStateProbe = sUsmState;\n"
            "#endif", code
        )
        self.assertIn(
            "static void Usm_ExitStartMenu(void)\n{\n"
            "    A2_USM_TRACE(7);\n"
            "#if defined(XGBA_A2) && XGBA_A2\n"
            "    gXgbaA2UsmStateProbe = NULL;\n"
            "#endif", code
        )

    def test_live_probe_required_from_linked_elf(self):
        resolver = (ROOT / "tools" / "xgba_a2_symbol_env.py").read_text()
        runner = (ROOT / "tools" / "xgba_a2_runtime_save.lua").read_text()
        self.assertIn('"gXgbaA2UsmStateProbe": "A2_USM_STATE_PTR"', resolver)
        self.assertIn('addr("A2_USM_STATE_PTR")', runner)
        self.assertIn("emu:read32(location)", runner)
        self.assertIn("if icon == 7 then", runner)
        self.assertIn('phase = "intro"', runner)
        self.assertIn('A2_XGBA_MENU_NOT_OPEN', runner)
        self.assertIn('A2_XGBA_COLD_RESUME_COUNTER_MISMATCH', runner)
        self.assertIn('A2_EXPECTED_SAVE_COUNTER', runner)
        self.assertIn('A2_XGBA_GAME_NATIVE_COUNTER_ADVANCED=', runner)
        self.assertIn('A2_XGBA_DIAG_USM_TRACE=', runner)
        self.assertIn('A2_XGBA_MENU_TRACE_CHANGE', runner)
        self.assertIn('A2_XGBA_DIALOG_TRACE_CHANGE', runner)
        self.assertIn('A2_XGBA_NATIVE_COUNTER_CHANGE', runner)
        self.assertIn('A2_XGBA_SAVE_DIALOG_DISAPPEARED_POSSIBLE_SOFT_RESET', runner)
        self.assertIn('A2_XGBA_TOUR_CLEARED_AND_REAL_MENU_STARTED', runner)
        self.assertIn('A2_XGBA_HUB_INTRO_NEVER_RELEASED_FIELD_INPUT', runner)
        self.assertIn('if (emu:read32(usmTraceAddr) & 3) ~= 3 then', runner)
        self.assertIn('phase = "tour-advance"', runner)
        self.assertIn('A2_USM_TRACE', (ROOT / "tools/xgba_a2_symbol_env.py").read_text())
        menu = (ROOT / "src/unbound_start_menu.c").read_text()
        self.assertIn("EWRAM_DATA volatile u32 gXgbaA2UsmTrace = 0;", menu)
        self.assertIn("#define A2_USM_TRACE(n) ((void)0)", menu)
        for bit in (1, 2, 3, 4, 5, 6, 7):
            self.assertIn("A2_USM_TRACE(" + str(bit) + ")", menu)
        self.assertIn('A2_XGBA_AWAITING_INITIAL_E6_BOOTSTRAP', runner)
        self.assertIn('A2_XGBA_INITIAL_SAVE_BOOTSTRAP_TIMEOUT', runner)
        self.assertIn('A2_XGBA_BOOTSTRAP_IN_PROGRESS', runner)
        self.assertIn('mode == "save-again"', runner)
        self.assertIn('A2_XGBA_SECOND_SAVE_COLD_LOAD_VERIFIED', runner)
        self.assertEqual(runner.count('A2_XGBA_SECOND_SAVE_GENERATION_MISMATCH'), 2)
        self.assertIn('saveStage ~= 8', runner)
        workflow = (ROOT / ".github/workflows/A2-XGBA-Boot-Probe.yml").read_text()
        self.assertIn('A2_ACTION: save-again', workflow)
        self.assertIn('metadata_generation:', workflow)
        self.assertIn("grep -q '^native_slot: 0$'", workflow)
        self.assertIn('A2_XGBA_COLD_RESUME_HUB_PASS counter=2', workflow)
        self.assertEqual(runner.count('saveStage == 6 or saveStage == 8'), 2)
        self.assertIn('The native counter increments BEFORE the metadata commit', runner)
        self.assertIn('if phase == "await-commit" then', runner)
        self.assertIn('(emu:read32(saveDialogTraceAddr) & 256) ~= 0', runner)
        self.assertIn('if counter > 0 and (saveStage == 6 or saveStage == 8) then', runner)


if __name__ == "__main__":
    unittest.main()
