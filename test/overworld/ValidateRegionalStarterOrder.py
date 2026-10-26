#!/usr/bin/env python3
"""Lock the player's original regional starter index and rival counter-choice.

A Kanto index describes Oak's chosen ball, NOT the rival's selected monster.
The champion/Route 22 scripts name branches after the rival species, so
a 0=Charmander *player* lookup silently misidentified Bulbasaur in credits,
Oak's champion dialogue and IsStarterInParty on authentic FRLG saves.
"""
from pathlib import Path
import re

root = Path(__file__).resolve().parents[2]
read = lambda rel: (root / rel).read_text(encoding="utf-8")
starter = read("src/starter_choose.c")
oak = read("data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc")
champion = read("data/maps/PokemonLeague_ChampionsRoom_Frlg/scripts.inc")
town = read("data/maps/Route22_Frlg/scripts.inc")
vars_frlg = read("include/constants/vars_frlg.h")
events = read("src/event_data.c")
credits = read("src/credits.c")

def tuple_for(region):
    match = re.search(
        r"\[" + re.escape(region) + r"\]\s*=\s*\{([^{}]+)\}", starter)
    assert match, f"missing {region} regional starter array"
    return tuple(re.findall(r"SPECIES_([A-Z0-9_]+)", match.group(1)))

assert tuple_for("REGION_HOENN") == (
    "TREECKO", "TORCHIC", "MUDKIP", "CHESPIN", "FENNEKIN", "FROAKIE",
    "SPRIGATITO", "FUECOCO", "QUAXLY")
assert tuple_for("REGION_JOHTO") == (
    "CHIKORITA", "CYNDAQUIL", "TOTODILE", "SNIVY", "TEPIG", "OSHAWOTT",
    "GROOKEY", "SCORBUNNY", "SOBBLE")
assert tuple_for("REGION_KANTO") == (
    "BULBASAUR", "SQUIRTLE", "CHARMANDER", "TURTWIG", "PIPLUP", "CHIMCHAR",
    "ROWLET", "POPPLIO", "LITTEN", "POIPOLE")

# Kanto selection now comes from the same multi-page menu as Birch and Elm.
oak = read("data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc")
champion = read("data/maps/PokemonLeague_ChampionsRoom_Frlg/scripts.inc")
town = read("data/maps/Route22_Frlg/scripts.inc")
assert "special AnthologyChooseKantoStarter" in oak
assert "copyvar PLAYER_STARTER_NUM, VAR_RESULT" in oak
assert "callnative Anthology_GetKantoChosenStarterSpecies" in oak
assert "copyvar VAR_STARTER_MON, PLAYER_STARTER_NUM" in oak
assert "SPECIES_RHYHORN" in oak and "LOCALID_CHARMANDER_BALL" in oak
assert "VAR_STARTER_MON_FRLG               VAR_KANTO_SLICE(0x0C)" in vars_frlg
assert "VAR_STARTER_MON_FRLG - REGION_VARS_START" in events
assert "GetStarterPokemon(VarGet(VAR_STARTER_MON))" in credits
assert "return sStarterMon[region][chosenStarterId]" in starter
for path in (
    "data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc",
    "data/maps/Route22_Frlg/scripts.inc",
    "data/maps/CeruleanCity_Frlg/scripts.inc",
    "data/maps/SSAnne_2F_Corridor_Frlg/scripts.inc",
    "data/maps/PokemonTower_2F_Frlg/scripts.inc",
    "data/maps/SilphCo_7F_Frlg/scripts.inc",
    "data/maps/PokemonLeague_ChampionsRoom_Frlg/scripts.inc",
):
    body = read(path)
    assert "callnative Anthology_GetKantoStarterStoryBranch" in body, path
    assert "call_if_eq VAR_STARTER_MON" not in body, path
    assert "goto_if_eq VAR_STARTER_MON" not in body, path
assert "GetKantoStarterBranch(VarGet(VAR_STARTER_MON))" in starter
assert "GetKantoStarterBranch(VarGet(VAR_TEMP_1))" in starter
assert "index == 9 ? 0 : index % STARTER_MON_COUNT" in starter
assert "sprite->invisible = (GetCurrentRegion() == REGION_KANTO" in starter
assert "GetCurrentRegion() == REGION_KANTO ? 4 : STARTER_GENERATION_PAGES" in starter
assert "def_special AnthologyChooseKantoStarter, waitstate=1" in read("data/specials.inc")
assert "void AnthologyChooseKantoStarter(void)" in read("src/battle_setup.c")
blue = read("src/battle_main.c")
for name in ("OAKS_LAB", "ROUTE22_EARLY", "CERULEAN", "SS_ANNE", "POKEMON_TOWER", "SILPH", "ROUTE22_LATE"):
    assert "TRAINER_RIVAL_" + name + "_CHARMANDER" in blue
assert "SPECIES_RHYHORN" in blue and "SPECIES_RHYDON" in blue
assert "VarGet(VAR_STARTER_MON) != 9" in blue
assert "TRAINER_CHAMPION_FIRST_CHARMANDER" not in blue.split("static bool32 IsEarlyBlueCharmanderTrainer")[1].split("static void ApplyPoipoleBlueRhyhorn")[0]

# Hoenn story branches must normalize the full 0..8 starter index to the
# original 0..2 elemental story branches; do not force nine new rivals.
for path in (
    "data/maps/Route103/scripts.inc",
    "data/maps/Route110/scripts.inc",
    "data/maps/Route119/scripts.inc",
    "data/maps/Route104/scripts.inc",
    "data/maps/RustboroCity/scripts.inc",
    "data/maps/LilycoveCity/scripts.inc",
    "data/maps/MauvilleCity_GameCorner/scripts.inc",
):
    text = read(path)
    assert "switch VAR_STARTER_MON" not in text, path
    assert "callnative Anthology_GetHoennStarterStoryBranch\n\tswitch VAR_RESULT" in text, path

assert "gSpecialVar_Result = VarGet(VAR_STARTER_MON) % STARTER_MON_COUNT;" in starter
assert "tStarterPage * STARTER_MON_COUNT" in starter
assert "JOY_NEW(L_BUTTON)" in starter and "JOY_NEW(R_BUTTON)" in starter
assert "gSpecialVar_Result = gTasks[taskId].tStarterPage * STARTER_MON_COUNT" in starter
random = read("src/world_starter_randomizer.c")
hoenn_slots = random.split("static const enum Species sHoennStarterSlots[] =", 1)[1].split("};", 1)[0]
for pokemon in ("CHESPIN","FENNEKIN","FROAKIE","SPRIGATITO","FUECOCO","QUAXLY"):
    assert "SPECIES_" + pokemon in hoenn_slots

# Elm reuses the selector without triggering Birch's Zigzagoon battle.
elm = read("data/maps/NewBarkTown_Lab/scripts.inc")
native = read("src/battle_setup.c")
specials = read("data/specials.inc")
assert "def_special AnthologyChooseJohtoStarter, waitstate=1" in specials
assert "void AnthologyChooseJohtoStarter(void)" in native
# The live Elm/Chikorita emulator regression requires page-one cursor zero;
# leave the classic Hoenn center cursor behavior intact.
assert "gTasks[taskId].tStarterSelection = GetCurrentRegion() == REGION_HOENN ? 1 : 0;" in starter
assert "special AnthologyChooseJohtoStarter" in elm
assert "callnative Anthology_GetJohtoChosenStarterSpecies" in elm
assert "copyvar VAR_STARTER_MON, VAR_RESULT" in elm
assert "givemon PLAYER_STARTER_SPECIES, 5" in elm
assert "gSpecialVar_Result = GetStarterPokemon(VarGet(VAR_STARTER_MON));" in starter
for path in ("data/maps/NewBarkTown_Lab/scripts.inc", "data/maps/AzaleaTown/scripts.inc",
             "data/maps/BurnedTower_1F/scripts.inc", "data/maps/CherrygroveCity/scripts.inc",
             "data/maps/JohtoIndigoPlateau_PokemonCenter/scripts.inc",
             "data/maps/GoldenrodCity_UndergroundSwitches/scripts.inc"):
    text = read(path)
    assert "callnative Anthology_GetJohtoStarterStoryBranch" in text, path
    assert "call_if_eq VAR_STARTER_MON" not in text, path
    assert "goto_if_eq VAR_STARTER_MON" not in text, path
assert "sPokeballCoords[STARTER_MON_COUNT][2] =" in starter
assert "sStarterLabelCoords[STARTER_MON_COUNT][2] =" in starter
assert "GetCurrentRegion() == REGION_JOHTO" in starter
print("P0 Johto nine starters and Silver continuity: source guard PASS")
print("P0 regional starter indices, Hoenn paging/story branches, and Oak canon: PASS")
