#!/usr/bin/env python3
"""Source-safety checks only. A real controller-driven PC test is still required."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
CLASSIC = (ROOT / "src" / "pokemon_storage_system.c").read_text()
SWSH = (ROOT / "src" / "swsh_storage_system.c").read_text()
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
