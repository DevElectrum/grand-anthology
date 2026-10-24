#!/usr/bin/env python3
"""Source guard: production-facing Cheats entry uses a whitelist, not raw debug."""
from pathlib import Path
root=Path(__file__).resolve().parents[2]
start=(root/"src/start_menu.c").read_text()
debug=(root/"src/debug.c").read_text()
header=(root/"include/debug.h").read_text()
assert 'AddStartMenuAction(MENU_ACTION_CHEATS);' in start.split('static void BuildNormalStartMenu(void)\n{',1)[1].split('static void BuildDebugStartMenu',1)[0]
assert "Debug_ShowPlayerCheatMenu();" in start
assert "void Debug_ShowPlayerCheatMenu(void);" in header
assert 'static const struct DebugMenuOption sPlayerCheatMenu_Main[] =' in debug
m=debug.split('static const struct DebugMenuOption sPlayerCheatMenu_Main[] =',1)[1].split('};',1)[0]
for required in ("DebugAction_Give_PokemonSimple","DebugAction_Give_PokemonComplex","DebugAction_Give_Item","DebugAction_Give_MaxMoney","DebugAction_Party_HealParty","sPlayerCheatMenu_Pokemon"):
    assert required in m,required
for forbidden in ("DebugAction_FlagsVars","DebugAction_PCBag_Clear","DebugAction_Util_Warp","DebugAction_Util_CheatStart","DebugAction_Party_ClearParty"):
    assert forbidden not in m,forbidden
# Both production creation paths must remain selectable, not merely declared.
assert 'COMPOUND_STRING("Quick Pokémon"),   DebugAction_Give_PokemonSimple' in m
assert 'COMPOUND_STRING("Custom Pokémon"),  DebugAction_Give_PokemonComplex' in m
# Quick creation must call the game-owned gift API (with normal party/box routing).
simple=debug.split('static void DebugAction_Give_Pokemon_SelectLevel(u8 taskId)\n{',1)[1].split('static void Debug_Display_Nature(',1)[0]
assert 'ScriptGiveMon(sDebugMonData->species, gTasks[taskId].tInput, ITEM_NONE);' in simple
assert 'Debug_ShowMenu(DebugTask_HandleMenuInput_General, sPlayerCheatMenu_Main);' in debug
print("P0 player-facing cheat whitelist: PASS")

# Production uses graphical Start, not BuildNormalStartMenu's text list.
# Exercise the real 12-slot save ABI: this is an alias to the old dev icon,
# not a new appended icon which would reshape SaveBlock3.region.usmSaved.
cfg = (root/"include/config/start_menu.h").read_text()
icons = (root/"include/constants/unbound_start_menu.h").read_text()
usm = (root/"src/unbound_start_menu.c").read_text()
assert "#define PW_GRAPHICAL_START_MENU     TRUE" in cfg
assert "USM_ICO_CHEATS = USM_ICO_DEBUG" in icons
assert "USM_ICO_CHEATS,  USM_ICO_POKEDEX" in usm
assert '[USM_ICO_CHEATS]  = {' in usm
assert '.label = COMPOUND_STRING("Cheats")' in usm
assert '.callback = UsmMenuCB_Cheats' in usm
assert "Usm_DeferCallback(Debug_ShowPlayerCheatMenu);" in usm
assert "Usm_DeferCallback(Debug_ShowMainMenu);" not in usm
assert "case USM_ICO_CHEATS:" in usm
print("P0 graphical Start Cheats icon and stable save ABI: PASS")
