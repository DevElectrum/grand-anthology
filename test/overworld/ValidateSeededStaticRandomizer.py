#!/usr/bin/env python3
"""Guard the production P0 seeded static-encounter feature without scene-by-scene tests.

This checks v12 save layout, legacy migration, source-level story protection,
ordinary scripted single/double battle integration and deterministic seed
mixing. A real GBA build and mGBA runtime smoke are still separately required.
"""
from pathlib import Path

root=Path(__file__).resolve().parents[2]
def src(p): return (root/p).read_text()
def need(haystack, needle):
    assert needle in haystack, "Static randomizer contract missing: "+needle

layout=src("include/global.h")
migrate=src("src/load_save.c")
fresh=src("src/new_game.c")
battle=src("src/script_pokemon_util.c")
ui=src("src/debug.c")
offline=src("test/overworld/SavePatch.py")
need(layout, "u8 randomizerStatics:1;")
need(layout, "u8 randomizerGifts:1;")
need(layout, "u8 randomizerStarters:1;")
need(layout, "/*0x96*/ u16 randomizerSeed")
need(layout, "sizeof(struct SaveBlock2) == 0xF2C")
need(src("include/constants/global.h"), "#define SAVE_FORMAT_VERSION 13")
need(migrate, "if (savedVersion < 12)")
migration=migrate.split("if (savedVersion < 12)",1)[1].split("gSaveBlock2Ptr->saveVersion = SAVE_FORMAT_VERSION",1)[0]
for name in ("randomizerWild", "randomizerTrainers", "randomizerStatics"):
    need(migration, "gSaveBlock2Ptr->"+name+" = FALSE;")
    need(fresh, "gSaveBlock2Ptr->"+name+" = FALSE;")
need(migration, "gSaveBlock2Ptr->randomizerSeed = 2026;")
need(offline, "sv.sb2[bits_off] &= 0x07")
need(offline, "struct.pack_into('<H', sv.sb2, seed_off, 2026)")
need(battle, "enum Species WorldRandomizeStaticSpecies(enum Species original, u32 slot)")
need(battle, "!gSaveBlock2Ptr->randomizerStatics")
need(battle, "gSaveBlock2Ptr->randomizerSeed")
for special in ("SPECIES_SNORLAX", "SPECIES_SUDOWOODO", "SPECIES_KECLEON",
                "isRestrictedLegendary", "isSubLegendary", "isMythical",
                "isUltraBeast", "isParadox"):
    need(battle,special)
for excluded in ("isMegaEvolution", "isPrimalReversion", "isUltraBurst",
                 "isGigantamax", "isTeraForm", "isTotem"):
    need(battle,excluded)
need(battle, "attempts < NUM_SPECIES - 1")
need(battle, "IsSpeciesEnabled(result)")
single=battle.split("void CreateScriptedWildMon(",1)[1].split("void CreateScriptedDoubleWildMon(",1)[0]
double=battle.split("void CreateScriptedDoubleWildMon(",1)[1].split("void ScriptSetMonMoveSlot(",1)[0]
need(single, "species = WorldRandomizeStaticSpecies(species, 0);")
need(double, "species1 = WorldRandomizeStaticSpecies(species1, 0);")
need(double, "species2 = WorldRandomizeStaticSpecies(species2, 0);")
assert single.index("WorldRandomizeStaticSpecies") < single.index("GetMonPersonality")
assert double.index("WorldRandomizeStaticSpecies") < double.index("GetMonPersonality")
for action in ("Enable Statics", "Disable Statics",
               "DebugAction_Randomizer_EnableStatics", "DebugAction_Randomizer_DisableStatics",
               "gSaveBlock2Ptr->randomizerStatics = TRUE",
               "gSaveBlock2Ptr->randomizerStatics = FALSE"):
    need(ui,action)
need(ui, "i == 10") # seed below all five independent mode pairs
mask=(1<<32)-1
def mix(seed,original,slot,group,num,nspecies=1600):
    h=(0x9E3779B9^seed)&mask
    h^=(original*0x85EBCA6B)&mask
    h^=((slot+1)*0xC2B2AE35)&mask
    h^=((group<<24)|(num<<16)|0x5300)&mask
    h^=h>>16
    h=(h*0x7FEB352D)&mask
    h^=h>>15
    h=(h*0x846CA68B)&mask
    h^=h>>16
    return 1+h%(nspecies-1)

assert mix(2026,25,0,37,10)==mix(2026,25,0,37,10)
assert mix(2027,25,0,37,10)!=mix(2026,25,0,37,10)
assert mix(2026,25,1,37,10)!=mix(2026,25,0,37,10)
assert len({mix(i,25,0,37,10) for i in range(100)}) >= 75
assert all(1<=mix(i,25,0,37,10)<1600 for i in range(1000))
print("P0 seeded static singles/doubles, v12 save, player menu, quest guards: PASS")
