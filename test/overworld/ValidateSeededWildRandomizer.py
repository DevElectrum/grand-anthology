#!/usr/bin/env python3
"""P0 deterministic wild randomizer: ABI, migrations, guarded hooks and seed properties.

Source-contract guard for the actual production GBA integration, deliberately
NOT a pretend mGBA runtime success. All live encounter behavior still needs
milestone emulator playtests. Seed-mix mirror tests the pure 32-bit function.
"""
from pathlib import Path
import re

root = Path(__file__).resolve().parents[2]
def read(p): return (root / p).read_text()
def require(needle, data):
    assert needle in data, "Missing contract: " + needle

layout=read("include/global.h")
migrator=read("src/load_save.c")
fresh=read("src/new_game.c")
version=read("include/constants/global.h")
wild=read("src/wild_encounter.c")
menu=read("src/debug.c")
require("#define SAVE_FORMAT_VERSION 13", version)
for statement in ("u8 randomizerWild:1;", "u8 randomizerTrainers:1;", "u8 randomizerStatics:1;", "u8 randomizerGifts:1;", "u8 randomizerStarters:1;",
                  "/*0x96*/ u16 randomizerSeed;", "sizeof(struct SaveBlock2) == 0xF2C"):
    require(statement, layout)
for snippet in ("if (savedVersion < 12)", "gSaveBlock2Ptr->randomizerWild = FALSE;",
                "gSaveBlock2Ptr->randomizerStarters = FALSE;", "gSaveBlock2Ptr->randomizerSeed = 2026;"):
    require(snippet, migrator)
    if "savedVersion" not in snippet:
        require(snippet, fresh)
assert "regionBitsUnused" not in layout and "regionBitsUnused" not in migrator
assert "randomizerReserved" not in layout and "randomizerReserved" not in migrator
require("static enum Species WorldRandomizeWildSpecies(", wild)
require("!gSaveBlock2Ptr->randomizerWild", wild)
require("gMapHeader.battleType == MAP_BATTLE_SCENE_FRONTIER", wild)
require("WorldIsFieldStableDiscoverySpecies(result)", wild)
require("IsSpeciesEnabled(species)", wild)
for special in ("isMegaEvolution", "isPrimalReversion", "isUltraBurst",
                "isGigantamax", "isTeraForm", "isTotem"):
    require(special, wild)
require("attempts < NUM_SPECIES - 1", wild)
require("gSaveBlock2Ptr->randomizerSeed", wild)
require("(u16)gTasks[taskId].tInput", menu)
require("seed + delta > 65535", menu)
require("WorldRandomizeWildSpecies(wildMonInfo->wildPokemon[wildMonIndex].species, wildMonIndex, area)", wild)
require("WorldRandomizeWildSpecies(wildMonInfo->wildPokemon[wildMonIndex].species, wildMonIndex, WILD_AREA_FISHING)", wild)
# Scripted static/roamer/outbreak/Feebas entrypoint remains unmodified:
require("void CreateWildMon(enum Species species, u8 level)", wild)
require("CreateWildMon(gSaveBlock1Ptr->outbreakPokemonSpecies, gSaveBlock1Ptr->outbreakPokemonLevel);", wild)
require("CreateWildMon(gWildFeebas.species", wild) if "CreateWildMon(gWildFeebas.species" in wild else require("CreateWildMon(species, level);",wild)
for needle in ("sPlayerCheatMenu_Randomizer", "DebugAction_Randomizer_EnableWild",
               "DebugAction_Randomizer_DisableWild", "DebugAction_Randomizer_EditSeed",
               "Task.data[] is signed s16"):
    require(needle, menu)
# The GBA-native mapping uses 32-bit overflow on each multiplication.
MASK=(1<<32)-1
def mix(seed, original, slot, area, map_group, map_num, num_species=1500):
    h=0x9E3779B9 ^ seed
    h ^= (original * 0x85EBCA6B)&MASK
    h ^= ((slot + 1) * 0xC2B2AE35)&MASK
    h ^= (map_group<<24)|(map_num<<16)|(area<<8)
    h ^= h>>16
    h=(h*0x7FEB352D)&MASK
    h ^= h>>15
    h=(h*0x846CA68B)&MASK
    h ^= h>>16
    return 1+h%(num_species-1)

fixtures=[(2026,25,3,0,37,0),(2026,25,3,0,37,0),
          (2027,25,3,0,37,0),(2026,25,3,0,37,1)]
results=[mix(*x) for x in fixtures]
assert results[0] == results[1], "same seed and map must be deterministic"
assert results[0] != results[2], "distinct seeds unexpectedly collided in fixture"
assert results[0] != results[3], "distinct maps unexpectedly collided in fixture"
assert len({mix(2026,25,slot,0,37,0) for slot in range(12)}) >= 10
assert len({mix(seed,25,1,0,37,0) for seed in range(100)}) >= 75
assert all(1 <= mix(i,25,1,0,37,0) < 1500 for i in range(1000))
print("P0 wild randomizer save v12 / deterministic map+slot species / story guards / in-game seed UI: PASS")
