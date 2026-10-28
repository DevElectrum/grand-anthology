#!/usr/bin/env python3
"""Source-level safety contracts for SwSh PC bulk release (not emulator QA)."""
from pathlib import Path

root = Path(__file__).resolve().parents[2]
source = (root / "src/swsh_storage_system.c").read_text()
texts = (root / "src/data/swsh_storage_system.h").read_text()
storage = (root / "include/pokemon_storage_system.h").read_text()


def function_body(signature):
    start = source.index(signature + "\n{")
    opening = source.index("{", start)
    depth = 0
    for i in range(opening, len(source)):
        if source[i] == "{":
            depth += 1
        elif source[i] == "}":
            depth -= 1
            if depth == 0:
                return source[opening:i + 1]
    raise AssertionError(f"Unclosed function: {signature}")


eligibility = function_body("static bool8 BulkReleaseEligible(u8 position)")
prepare = function_body("static void BulkReleasePrepare(void)")
release = function_body("static void Task_BulkRelease(u8 taskId)")
finish = function_body("static void BulkReleaseFinish(void)")
options = function_body("static void AddBoxOptionsMenu(void)")
single = function_body("static void Task_ReleaseMon(u8 taskId)")

for token in (
    "MON_DATA_IS_EGG", "MON_DATA_MARKINGS", "1 << 3",
    "isRestrictedLegendary", "isSubLegendary", "isMythical",
    "NationalPokedexNumToSpecies", "ItemIsMail",
    "GetRestrictedReleaseMoves", "MON_DATA_KNOWN_MOVES",
):
    assert token in eligibility, f"Missing bulk protection: {token}"

for unprotected in ("MON_DATA_IS_SHINY", ".isUltraBeast", ".isParadox"):
    assert unprotected not in eligibility, f"Unexpected bulk protection: {unprotected}"

assert "CountPartyNonEggMons()" in prepare
assert "CountStorageNonEggMons()" in prepare
assert "remaining - 3" in prepare
assert "sBulkReleaseMask &= ~bit" in prepare

assert "ShowYesNoWindow(1)" in release, "No must be the default"
assert release.index("ShowYesNoWindow(1)") < release.index("ZeroBoxMonAt(")
assert "if (item != ITEM_NONE && !AddBagItem(item, 1))" in release
assert "continue;" in release
assert release.index("AddBagItem(item, 1)") < release.index("ZeroBoxMonAt(")
assert "DestroyBoxMonIconAtPosition(i)" in release
assert "sBulkClearBox" in release and "sBulkReleaseMask" in release
assert "BulkReleaseUpdateIcons()" in finish
assert "SetPokeStorageTask(Task_PokeStorageMain)" in finish
assert "MENU_BULK_RELEASE" in options and "MENU_CLEAR_BOX" in options
assert "OPTION_MOVE_MONS" in options and "!sIsMonBeingMoved" in options
assert "MSG_RELEASE_POKE" in single, "Individual release must stay available"

for label in ("MENU_BULK_RELEASE", "MENU_CLEAR_BOX", "MSG_BULK_CONFIRM", "MSG_BULK_DONE"):
    assert f"[{label}]" in texts

assert "#define TOTAL_BOXES_COUNT       14" in storage
assert "STATIC_ASSERT(offsetof(struct PokemonStorage, boxes) == 0x0004" in storage

print("PASS: PC bulk release source policy, item ordering and save layout")
