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


if __name__ == "__main__":
    unittest.main()
