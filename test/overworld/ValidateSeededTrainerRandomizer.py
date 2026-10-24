#!/usr/bin/env python3
"""P0 trainer randomizer integration contract: deterministic seeded teams, story safety.

This is a source-level build gate, not a claim of completed emulator coverage.
"""
from pathlib import Path

base=Path(__file__).resolve().parents[2]
def source(p): return (base/p).read_text()
def has(s, needle):
    assert needle in s, "Missing P0 trainer randomizer contract: " + needle

battle=source("src/battle_main.c")
save=source("include/global.h")
migration=source("src/load_save.c")
new=source("src/new_game.c")
ui=source("src/debug.c")
version=source("include/constants/global.h")
has(version, "#define SAVE_FORMAT_VERSION 12")
patcher=source("test/overworld/SavePatch.py")
has(patcher,"SAVE_FORMAT_VERSION = 12")
has(patcher,"sv.sb2[bits_off] &= 0x07")
has(patcher,"struct.pack_into('<H', sv.sb2, seed_off, 2026)")
has(save, "u8 randomizerTrainers:1;")
has(save, "u8 randomizerStatics:1;")
has(save, "u8 randomizerWild:1;")
has(save, "u8 randomizerGifts:1;")
has(save, "u8 randomizerStarters:1;")
has(save, "/*0x96*/ u16 randomizerSeed")
has(save, "sizeof(struct SaveBlock2) == 0xF2C")
has(migration, "if (savedVersion < 12)")
for x in (migration,new):
    has(x,"gSaveBlock2Ptr->randomizerTrainers = FALSE;")
    has(x,"gSaveBlock2Ptr->randomizerWild = FALSE;")
    has(x,"gSaveBlock2Ptr->randomizerSeed = 2026;")
has(battle,"static enum Species WorldRandomizeTrainerSpecies")
has(battle,"gSaveBlock2Ptr->randomizerTrainers")
has(battle,"gSaveBlock2Ptr->randomizerSeed")
has(battle,"party != gParties[B_TRAINER_PLAYER]")
for flag in ("BATTLE_TYPE_FRONTIER","BATTLE_TYPE_LINK","BATTLE_TYPE_RECORDED",
             "BATTLE_TYPE_TRAINER_HILL","BATTLE_TYPE_EREADER_TRAINER",
             "BATTLE_TYPE_SECRET_BASE","BATTLE_TYPE_CATCH_TUTORIAL",
             "BATTLE_TYPE_FIRST_BATTLE","BATTLE_TYPE_INGAME_PARTNER"):
    has(battle, flag)
has(battle,"trainerIdentity = trainerIdentity * 33 + trainer->trainerName[n]")
has(battle,"species = WorldRandomizeTrainerSpecies(partyData[i].species,")
has(battle,"trainerIdentity, i);")
has(battle,"whose addresses drift when ROM data is rearranged")
has(battle,"Pin the seeded species to the *fixed* canonical team slot")
has(battle,"CreateMon(&party[i], species, partyData[monIndex].lvl, personalityValue, otId);")
has(battle,"if (randomized)\n                GiveMonInitialMoveset(&party[i]);")
has(battle,"if (!randomized && partyData[monIndex].nickname != NULL)")
has(battle,"if (randomized)\n            {\n                // Original trainer's forced ability")
for bad in ("isMegaEvolution","isPrimalReversion","isUltraBurst","isGigantamax","isTeraForm","isTotem"):
    has(battle, bad)
for control in ("Enable Trainers","Disable Trainers","DebugAction_Randomizer_EnableTrainers",
                "DebugAction_Randomizer_DisableTrainers","randomizerTrainers = TRUE",
                "randomizerTrainers = FALSE"):
    has(ui,control)
has(ui,"items == sPlayerCheatMenu_Randomizer")
has(ui,"STR_CONV_MODE_LEADING_ZEROS, 5);")

mask=(1<<32)-1
def mix(seed, species, slot, identity, species_count=1600):
    h=(0x9E3779B9^seed)&mask
    h ^= (species * 0x85EBCA6B)&mask
    h ^= ((slot + 1) * 0xC2B2AE35)&mask
    h ^= identity&mask
    h ^= h>>16
    h=(h*0x7FEB352D)&mask
    h ^= h>>15
    h=(h*0x846CA68B)&mask
    h ^= h>>16
    return 1 + h % (species_count-1)

assert mix(2026,25,0,0x34) == mix(2026,25,0,0x34)
assert mix(2027,25,0,0x34) != mix(2026,25,0,0x34)
assert mix(2026,25,1,0x34) != mix(2026,25,0,0x34)
assert len({mix(seed,25,0,0x34) for seed in range(100)}) >= 75
assert all(1<=mix(seed,25,0,0x34)<1600 for seed in range(1000))
print("P0 seeded NPC trainers / v12 save / player toggles / preserved battle guards: PASS")
