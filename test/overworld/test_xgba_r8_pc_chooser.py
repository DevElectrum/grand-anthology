#!/usr/bin/env python3
"""Source-safety checks only. A real controller-driven PC test is still required."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
CLASSIC = (ROOT / "src" / "pokemon_storage_system.c").read_text()
SWSH = (ROOT / "src" / "swsh_storage_system.c").read_text()
FIELD_SPECIALS = (ROOT / "src" / "field_specials.c").read_text()
SAVE = (ROOT / "src" / "save.c").read_text()
HEADER = (ROOT / "include" / "xgba_a2_game.h").read_text()


class R8PcChooserSourceTests(unittest.TestCase):
    def test_classic_reopen_allocation_failure_is_safe(self):
        start = CLASSIC.index("static void CB2_ReturnToPokeStorage(void)\n{")
        end = CLASSIC.index("static void ResetAllBgCoords(void)", start)
        body = CLASSIC[start:end]
        self.assertIn("if (sStorage == NULL)", body)
        self.assertIn("if (sCurrentBoxOption == OPTION_SELECT_MON)", body)
        self.assertNotIn("if (sStorage->boxOption == OPTION_SELECT_MON)", body)

    def test_global_preview_api_is_validated_and_ram_only(self):
        start = SAVE.index("u8 *XgbaA2GameGlobalBoxName(u16 globalBox)")
        end = SAVE.index("// The UI must only request a transfer", start)
        body = SAVE[start:end]
        self.assertIn("XgbaA2GameCollectionReady()", body)
        self.assertIn("globalBox >= XGBA_A2_PC_PAGES * TOTAL_BOXES_COUNT", body)
        self.assertIn("sA2Pager.ramPages", body)
        self.assertIn("gPokemonStoragePtr", body)
        self.assertNotIn("XgbaA2Journal", body)
        self.assertNotIn("sA2Io.", body)
        self.assertIn("s16 XgbaA2GameCountGlobalBoxMons", HEADER)

    def test_classic_global_jump_keeps_carried_mon_guard(self):
        self.assertIn("limit = XGBA_A2_PC_PAGES * TOTAL_BOXES_COUNT", CLASSIC)
        self.assertIn("sChooseBoxMenu->globalMode && JOY_NEW(L_BUTTON | R_BUTTON)", CLASSIC)
        self.assertIn("sChooseBoxMenu->globalMode = XgbaA2GameCollectionReady()", CLASSIC)
        self.assertIn("!IsMonBeingMoved() && !IsMovingItem()", CLASSIC)
        self.assertIn("XgbaA2GameSelectGlobalBox(globalBox)", CLASSIC)
        self.assertIn("SetPokeStorageTask(Task_R8ReopenBox)", CLASSIC)

    def test_global_deposit_source_guards_and_both_pc_engines(self):
        start = SAVE.index("bool8 XgbaA2GameDepositPartyMonGlobal(")
        end = SAVE.index("// Swap is likewise preflighted", start)
        body = SAVE[start:end]
        self.assertIn("CountPartyAliveNonEggMonsExcept(partyPosition)", body)
        self.assertIn("ItemIsMail(", body)
        self.assertIn("position == IN_BOX_COUNT", body)
        self.assertIn("memcpy(target, &deposited.box", body)
        self.assertIn("ZeroMonData(party)", body)
        self.assertLess(body.index("if (position == IN_BOX_COUNT)"),
                        body.index("memcpy(target, &deposited.box"))
        self.assertIn("XgbaA2GameDepositPartyMonGlobal(sCursorPosition, boxId, &slot)", CLASSIC)
        self.assertIn("boxId %= TOTAL_BOXES_COUNT;", CLASSIC)
        self.assertIn("XgbaA2GameDepositPartyMonGlobal(sCursorPosition, boxId, NULL)", SWSH)
        self.assertIn("sStorage->state = 18;", SWSH)

    def test_c3b_remote_withdraw_preflights_both_ends(self):
        start = SAVE.index("bool8 XgbaA2GameWithdrawGlobalBoxMon(")
        end = SAVE.index("// Swap is likewise preflighted", start)
        body = SAVE[start:end]
        self.assertIn("XgbaA2GameGlobalBoxMon(globalBox, sourcePosition)", body)
        self.assertIn("GetBoxMonData(source, MON_DATA_SPECIES)", body)
        self.assertIn("ItemIsMail(", body)
        self.assertIn("CalculatePlayerPartyCount()", body)
        self.assertIn("partyCount >= PARTY_SIZE", body)
        self.assertIn("position < PARTY_SIZE", body)
        self.assertIn("BoxMonToMon(&snapshot, &withdrawn)", body)
        self.assertIn("memcmp(&withdrawn.box, &snapshot", body)
        self.assertIn("ZeroBoxMonData(source)", body)
        self.assertIn("gPartiesCount[B_TRAINER_PLAYER] = partyCount + 1", body)
        self.assertLess(body.index("partyCount >= PARTY_SIZE"), body.index("ZeroBoxMonData(source)"))
        self.assertLess(body.index("BoxMonToMon(&snapshot"), body.index("ZeroBoxMonData(source)"))
        self.assertIn("XgbaA2GameWithdrawGlobalBoxMon", HEADER)

    def test_c3c_controller_route_is_walkable(self):
        # The original direct walk crossed the impassable row at (16,8).
        # Check every tile in the actual 25 x 18 Region Hub binary layout.
        import struct

        path = ROOT / "data" / "layouts" / "RegionHub" / "map.bin"
        data = path.read_bytes()
        self.assertEqual(len(data), 25 * 18 * 2)
        waypoints = [(16, 6), (6, 6), (6, 13), (17, 13), (17, 10)]
        points = [(16, 7)] + waypoints
        for i in range(1, len(points)):
            x1, y1 = points[i - 1]
            x2, y2 = points[i]
            self.assertTrue(x1 == x2 or y1 == y2)
            x, y = x1, y1
            while True:
                tile = struct.unpack_from("<H", data, 2 * (25 * y + x))[0]
                self.assertEqual((tile >> 10) & 3, 0, f"blocked at ({x},{y})")
                if x == x2 and y == y2:
                    break
                if x < x2:
                    x += 1
                elif x > x2:
                    x -= 1
                elif y < y2:
                    y += 1
                else:
                    y -= 1
        script = (ROOT / "tools" / "xgba_r8_pc_controller.lua").read_text()
        self.assertIn("local waypoints = {{16, 6}, {6, 6}, {6, 13}, {17, 13}, {17, 10}}", script)

    def test_c3c_jump_save3_cold_verifies_page_and_records(self):
        # Production validation must not be relaxed to accept a page mismatch.
        self.assertIn("XgbaR8VerifyTestPokemonAtCounter(sA2Committed.nativeCounter)", SAVE)
        runtime = (ROOT / "src" / "xgba_r8_runtime_test.c").read_text()
        self.assertIn("counter == 2", runtime)
        self.assertIn("counter == 3", runtime)
        self.assertIn("VerifyTestPokemonRecords()", runtime)
        self.assertIn("XgbaA2GamePage() == 1", runtime)
        self.assertIn("StorageGetCurrentBox() == 0", runtime)
        self.assertIn("XgbaR8VerifyTestPokemonAtCounter(u32 counter)", HEADER)
        classic = CLASSIC.split("static void Task_PCMainMenu(u8 taskId)")[1]
        swsh = SWSH.split("static void Task_PCMainMenu(u8 taskId)")[1]
        self.assertIn("gXgbaR8PcUiTrace = 11;", classic)
        self.assertIn("gXgbaR8PcUiTrace = 12;", swsh)
        controller = (ROOT / "tools" / "xgba_r8_pc_controller.lua").read_text()
        self.assertIn('action == "jump-save"', controller)
        self.assertIn("verifyChooser(29, 2)", controller)
        self.assertIn("verifyChooser(14, 2)", controller)
        self.assertIn("R8_PC_CONTROLLER_JUMP_SAVE3_PASS", controller)

    def test_c3c_graphical_probe_is_test_only(self):
        self.assertIn("gXgbaR8PcUiTrace = 1;", CLASSIC)
        self.assertIn("gXgbaR8PcUiTrace = 2;", CLASSIC)
        self.assertIn("gXgbaR8PcUiTrace = 3;", SWSH)
        self.assertIn("gXgbaR8PcUiTrace = 4;", CLASSIC)
        self.assertIn("gXgbaR8PcUiTrace = 5;", SWSH)
        self.assertIn("gXgbaR8PcUiTrace = 6;", FIELD_SPECIALS)
        self.assertIn("#if defined(XGBA_R8_TEST) && XGBA_R8_TEST", CLASSIC)
        self.assertIn("#if defined(XGBA_R8_TEST) && XGBA_R8_TEST", SWSH)

    def test_c3c_jump_preview_has_independent_live_controller_witness(self):
        controller = (ROOT / "tools" / "xgba_r8_pc_controller.lua").read_text()
        workflow = (ROOT / ".github" / "workflows" / "R8-PC-Controller.yml").read_text()
        symbols = (ROOT / "tools" / "xgba_a2_symbol_env.py").read_text()
        fixture = (ROOT / "src" / "xgba_r8_runtime_test.c").read_text()
        self.assertIn("gXgbaR8PcChooserTrace", fixture)
        self.assertIn("A2_R8_PC_CHOOSER_TRACE", symbols)
        self.assertIn("gXgbaR8PcUiTrace = 7;", CLASSIC)
        self.assertIn("gXgbaR8PcUiTrace = 8;", SWSH)
        self.assertIn("gXgbaR8PcUiTrace = 9;", CLASSIC)
        self.assertIn("gXgbaR8PcUiTrace = 10;", SWSH)
        for frontend in (CLASSIC, SWSH):
            self.assertIn("gXgbaR8PcChooserTrace = 0x80000000U", frontend)
            self.assertIn("gXgbaR8PcChooserTrace = 0x40000000U", frontend)
        self.assertIn("verifyChooser(14)", controller)
        self.assertIn("verifyChooser(0)", controller)
        self.assertIn("R8_PC_CONTROLLER_PREVIEW_CANCEL_PASS", controller)
        self.assertIn("R8_PC_ACTION: preview-cancel", workflow)
        self.assertIn('action == "jump-commit"', controller)
        self.assertIn("verifyChooser(14)", controller)
        self.assertIn("R8_PC_CONTROLLER_JUMP_COMMIT_PASS", controller)
        self.assertIn("jump_committed_wrong_resident_box", controller)
        self.assertIn("R8_PC_ACTION: jump-commit", workflow)
        self.assertIn("diff -u /tmp/r8-pc-before.txt /tmp/r8-pc-jump-audit.txt", workflow)
        self.assertIn("diff -u /tmp/r8-pc-before.txt /tmp/r8-pc-preview-audit.txt", workflow)

    def test_swsh_uses_paged_14_cell_preview_without_switching_on_hover(self):
        self.assertIn("sChooseBoxMenu->previewPage", SWSH)
        self.assertIn("if (sChooseBoxMenu->globalMode && JOY_NEW(L_BUTTON | R_BUTTON))", SWSH)
        self.assertIn("for (boxId = 0; boxId < TOTAL_BOXES_COUNT; boxId++)", SWSH)
        self.assertIn("XgbaA2GameCountGlobalBoxMons", SWSH)
        self.assertIn("XgbaA2GameGlobalBoxName", SWSH)
        self.assertIn("!IsMonBeingMoved() && !IsMovingItem()", SWSH)
        self.assertIn("sCursorMode != CURSOR_MODE_MULTI_MOVE", SWSH)
        self.assertIn("SetPokeStorageTask(Task_A2ReopenPage)", SWSH)


if __name__ == "__main__":
    unittest.main()
