#!/usr/bin/env python3
"""P0 source regression gate: the three World Transit gates retain the user's team.

This is a structural check, not a substitute for a live emulator save/party test.
"""
from pathlib import Path
root = Path(__file__).resolve().parents[2]
c = (root / "src/region_switch.c").read_text()
s = (root / "data/maps/RegionHub/scripts.inc").read_text()
h = (root / "include/regions.h").read_text()
f = (root / "FEATURES.md").read_text()

def require(ok, msg):
    if not ok:
        raise AssertionError(msg)

body = c.split("void RegionHub_ScrEnterRegion(struct ScriptContext *ctx)", 1)[1].split("\n}", 1)[0]
require("SetCurrentRegion(target);" in body, "persist selected region")
require("SetRegionArrivalRespawn(target);" in body, "re-arm region whiteout destination")
require("DepositPartyToPC" not in c + s + h, "do not auto-box the party")
require("RegionHub_ScrCanDepositParty" not in c + s, "do not gate region travel on free PC/mail space")
for name in ("Kanto", "Johto", "Hoenn"):
    start = s.index("RegionHub_EventScript_Attendant" + name + "::")
    end = s.index("RegionHub_EventScript_Return" + name + "::") if name != "Hoenn" else s.index("RegionHub_EventScript_SeedFirstVisitHoennMale::")
    flow = s[start:end]
    require("\tcallnative RegionHub_ScrEnterRegion" in flow, f"{name}: use real gate")
    require("\tcallnative RegionHub_ScrTargetHasFirstBadge" in flow, f"{name}: keep first-badge routing")
    require("\tcall RegionHub_EventScript_TryGiveHubPass" in flow, f"{name}: ensure return travel")
    prompt = s.split("RegionHub_Text_Travel" + name + ":", 1)[1].split("Travel to " + name.upper() + "?$", 1)[0]
    require("entire team travels with you" in prompt, f"{name}: text must match new behavior")
require("RegionHub_Text_TravelNoRoom:" not in s, "no obsolete PC-full refusal")
require("RegionHub_ScrTargetIsCurrent" not in s, "no obsolete same-region message branch")
require("retains the active party" in f, "keep feature documentation accurate")
for path in ("PalletTown_ProfessorOaksLab_Frlg", "NewBarkTown_Lab"):
    starter = (root / "data/maps" / path / "scripts.inc").read_text()
    require("MON_GIVEN_TO_PARTY" in starter and "MON_GIVEN_TO_PC" in starter,
            f"{path}: support choosing a starter with a full carried party")
    require("Common_EventScript_GetGiftMonPartySlot" in starter,
            f"{path}: nickname the newly appended mon, not slot zero")
    require("Common_EventScript_NameReceivedBoxMon" in starter,
            f"{path}: allow nickname for boxed starter")
    require("Common_EventScript_TransferredToPC" in starter,
            f"{path}: show honest PC transfer message")

print("World Transit party-continuity source contract: PASS")
