#!/usr/bin/env python3
"""Post-League natural discovery access: source guards and cycle mathematics.

This is a source/algorithm contract, not an in-emulator catchability or form
legality certification. It does not claim battle-exclusive transformations
can be caught as normal permanent Pokémon.
"""
from pathlib import Path
import re

root = Path(__file__).resolve().parents[2]
def read(p): return (root / p).read_text()
def need(text, part): assert part in text, "missing discovery contract: " + part

wild = read("src/wild_encounter.c")
flags = read("include/constants/flags.h")
species = read("include/constants/species.h")
need(flags, "FLAG_SYS_GAME_CLEAR")
need(wild, "static enum Species WorldChoosePostLeagueDiscoverySpecies(enum Species original)")
for guard in ("!FlagGet(FLAG_SYS_GAME_CLEAR)", "gSaveBlock2Ptr->randomizerWild",
              "GetSafariZoneFlag()", "InBattlePike()", "PYRAMID_LOCATION_NONE",
              "MAP_BATTLE_SCENE_FRONTIER", "IsSpeciesEnabled(species)"):
    need(wild, guard)
for excluded in ("isMegaEvolution", "isPrimalReversion", "isUltraBurst",
                 "isGigantamax", "isTeraForm", "isTotem"):
    need(wild, "!gSpeciesInfo[species]." + excluded)
for hook in (
    "WorldRandomizeWildSpecies(wildMonInfo->wildPokemon[wildMonIndex].species, wildMonIndex, area)",
    "WorldChoosePostLeagueDiscoverySpecies(wildSpecies)",
    "WorldChoosePostLeagueDiscoverySpecies(WorldRandomizeWildSpecies(wildMonInfo->wildPokemon[wildMonIndex].species, wildMonIndex, WILD_AREA_FISHING))",
):
    need(wild, hook)
# Keep scripted static/roamer/Feebas and outbreaks outside the new pipeline.
need(wild, "CreateWildMon(gSaveBlock1Ptr->outbreakPokemonSpecies, gSaveBlock1Ptr->outbreakPokemonLevel);")
need(wild, "CreateWildMon(species, level);")
need(wild, "CreateWildMon(gSaveBlock1Ptr->outbreakPokemonSpecies")
need(wild, "battles = GetGameStat(GAME_STAT_WILD_BATTLES);")
need(wild, "if (battles % 3 != 2)")
need(wild, "result = 1 + (battles / 3) % (NUM_SPECIES - 1);")
need(wild, "attempts < NUM_SPECIES - 1")
need(wild, "GetGameStat(GAME_STAT_WORLD_DISCOVERY_TARGET)")
need(wild, "WorldIsPostLeagueResearchSpecies(result)")
# Wild randomization deliberately keeps the broader field-stable pool.
randomizer = wild.split("enum Species WorldRandomizeWildSpeciesAtMap(", 1)[1].split("// The original API remains", 1)[0]
need(randomizer, "WorldIsFieldStableDiscoverySpecies(result)")
# Excluded Legendaries cannot be introduced by post-League Research/rotation.
natural = wild.split("bool8 WorldIsNaturalDiscoverySpecies")[1].split("// Supplemental ordinary species")[0]
for guard in ("isRestrictedLegendary", "isSubLegendary", "isMythical",
              "isUltraBeast", "isParadox", "SPECIES_ZARUDE",
              "SPECIES_MELTAN", "SPECIES_PHIONE"):
    need(natural, guard)
postgame = wild.split("static enum Species WorldChoosePostLeagueDiscoverySpecies")[1].split("void CreateWildMon")[0]
assert "WorldIsFieldStableDiscoverySpecies(result)" not in postgame
assert postgame.count("WorldIsPostLeagueResearchSpecies(result)") == 2
research_guard = wild.split("bool8 WorldIsPostLeagueResearchSpecies")[1].split("// Supplemental ordinary species")[0]
for excluded in ("isRestrictedLegendary", "isSubLegendary", "isMythical"):
    need(research_guard, excluded)
need(wild, "WorldIsNaturalDiscoverySpecies(rare)")
need(wild, "WorldIsNaturalDiscoverySpecies(sAnthologyExtraHabitats[i].species)")
need(wild, "WorldIsNaturalDiscoverySpecies(species)")
stats = read("include/constants/game_stat.h")
need(stats, "#define GAME_STAT_WORLD_DISCOVERY_TARGET      53")
need(stats, "#define NUM_GAME_STATS                        64")
need(stats, "#define NUM_USED_GAME_STATS                   54")
menu = read("src/start_menu.c")
need(menu, "MENU_ACTION_DISCOVERY")
need(menu, "FlagGet(FLAG_SYS_GAME_CLEAR)")
need(menu, "Debug_ShowDiscoveryMenu();")
ui = read("src/debug.c")
for guard in ("DebugAction_Discovery_EditTarget", "DebugAction_Discovery_HandleTarget",
              "DebugAction_Discovery_ClearTarget",
              "SetGameStat(GAME_STAT_WORLD_DISCOVERY_TARGET, target)",
              "WorldIsPostLeagueResearchSpecies(target)"):
    need(ui, guard)

# All assigned raw indices must appear in exactly one regular, uninterrupted
# cycle of (NUM_SPECIES-1) postgame discovery encounters.
last = re.search(r"SPECIES_GLIMMORA_MEGA = (\d+)", species)
assert last, "species enum layout changed"
num_species = int(last.group(1)) + 1 # CUSTOM_START aliases this ID; CUSTOM_END/Egg is next
eligible_slots = num_species - 1
encounter_ids = [1 + ((battle // 3) % eligible_slots)
                 for battle in range(2, 3 * eligible_slots, 3)]
assert len(encounter_ids) == eligible_slots
assert set(encounter_ids) == set(range(1, num_species))
# No discovery effect in first two ordinary fights of each 3-battle group.
assert [b for b in range(12) if b % 3 == 2] == [2, 5, 8, 11]
print("P0 scoped natural habitats, nonlegendary Research and randomizer-exempt rotation PASS")
