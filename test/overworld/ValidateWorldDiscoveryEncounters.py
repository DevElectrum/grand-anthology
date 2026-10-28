#!/usr/bin/env python3
"""Research retirement guard: normal encounters stay tied to real map habitats.

The retired Research mechanic must never substitute unrelated species into
ordinary overworld, grass, Surf or fishing encounters. This is a source-level
regression check, not an emulator capture-certification test.
"""
from pathlib import Path
import json

root = Path(__file__).resolve().parents[2]

def read(path):
    return (root / path).read_text()

def require(condition, reason):
    assert condition, reason

wild = read("src/wild_encounter.c")
header = read("include/wild_encounter.h")
menu = read("src/start_menu.c")
debug = read("src/debug.c")
debug_header = read("include/debug.h")
stats = read("include/constants/game_stat.h")
graphical = read("src/unbound_start_menu.c")

for retired in (
    "WorldChoosePostLeagueDiscoverySpecies",
    "WorldIsPostLeagueResearchSpecies",
    "GAME_STAT_WORLD_DISCOVERY_TARGET",
):
    for label, source in (
        ("encounters", wild), ("header", header), ("text menu", menu),
        ("debug UI", debug), ("debug header", debug_header),
        ("game stats", stats), ("graphical menu", graphical),
    ):
        require(retired not in source, f"Research code remains in {label}: {retired}")

for token in ("MENU_ACTION_DISCOVERY", "StartMenuDiscoveryCallback",
              "sText_MenuDiscovery", '_("RESEARCH")'):
    require(token not in menu, f"Classic Start still exposes {token}")
for token in ("Debug_ShowDiscoveryMenu", "DebugAction_Discovery_EditTarget",
              "DebugAction_Discovery_ClearTarget", "DebugAction_Discovery_HandleTarget",
              "sPlayerDiscoveryMenu_Main"):
    require(token not in debug and token not in debug_header, f"Research editor survives: {token}")

require("#define GAME_STAT_RESERVED_53                 53" in stats,
        "The retired stat index must remain allocated for saved games")
require("#define NUM_USED_GAME_STATS                   54" in stats,
        "Do not shrink the persisted game-stat array")
require("#define NUM_GAME_STATS                        64" in stats,
        "Do not move following save data")

# No post-League override: the route's natural/seeded species passes straight
# into the standard creation routine, and native fish can only be replaced by
# the authored aquatic habitat or optional seeded randomizer.
grass = wild.split("bool8 TryGenerateWildMon(", 1)[1].split(
    "static u16 GenerateFishingWildMon(", 1)[0]
fish = wild.split("static u16 GenerateFishingWildMon(", 1)[1].split(
    "bool8 SetUpMassOutbreakEncounter(", 1)[0]
require("CreateWildMon(wildSpecies, level);" in grass, "Grass/overworld creation must be direct")
require("WorldRandomizeWildSpecies(wildMonInfo->wildPokemon[wildMonIndex].species, wildMonIndex, area)" in grass,
        "Preserve the optional seeded wild Randomizer")
for term in ("AnthologyGetRareHabitatSpecies(area, wildMonInfo)",
             "AnthologyGetExtraHabitatSpecies(area, wildMonInfo, habitatSlot)",
             "AnthologyGetAquaticHabitatSpecies(area, wildMonInfo)",
             "if (useRare)", "else if (extra != SPECIES_NONE)", "else if (useAquatic)"):
    require(term in grass, f"Natural habitat lost: {term}")
require("enum Species wildMonSpecies = WorldRandomizeWildSpecies(" in fish,
        "Preserve native/randomized fishing species selection")
require("AnthologyGetAquaticHabitatSpecies(WILD_AREA_FISHING, wildMonInfo)" in fish,
        "Authored fishing habitat lost")
require("wildMonSpecies = aquatic;" in fish and "CreateWildMon(wildMonSpecies, level);" in fish,
        "Fishing must use native or authored aquatic species")
require("GAME_STAT_WILD_BATTLES" not in wild,
        "Wild battle count must not manipulate the species pool")
require("WorldIsFieldStableDiscoverySpecies(result)" in wild,
        "Keep field-stable form restrictions on seeded randomizer")
for token in ("WorldIsNaturalDiscoverySpecies(rare)",
              "WorldIsNaturalDiscoverySpecies(sAnthologyExtraHabitats[i].species)",
              "WorldIsNaturalDiscoverySpecies(species)"):
    require(token in wild, f"Natural habitat eligibility lost: {token}")
require("CreateWildMon(gSaveBlock1Ptr->outbreakPokemonSpecies, gSaveBlock1Ptr->outbreakPokemonLevel);" in wild,
        "Keep native mass outbreaks")

# The optimistic collection catalog predates this retirement and does not
# depend on Research as a source. It is not a claim of emulator reachability.
catalog = []
for line in read("data/catalog/world_acquisition.jsonl").splitlines():
    if line.strip():
        entry = json.loads(line)
        if entry.get("kind") == "pokemon":
            catalog.append(entry)
require(len(catalog) == 1025, "The National Pokédex source catalog changed")
for entry in catalog:
    require("research" not in json.dumps(entry.get("sources", [])).lower(),
            f"Research-only source for dex {entry['dex']}")
print("PASS: Research retired; native habitats, seeded Randomizer and save ABI retained")
