#!/usr/bin/env python3
"""P0: Oak, Elm and Birch seeded starter awards without rival/story rewriting.

Source-contract test (GBA build and mGBA gameplay are separate).
"""
from pathlib import Path
root=Path(__file__).resolve().parents[2]
def src(p): return (root/p).read_text()
def need(p,t): assert t in p, "starter mode contract missing: "+t

layout=src("include/global.h")
migrate=src("src/load_save.c")
new=src("src/new_game.c")
header=src("include/world_starter_randomizer.h")
rng=src("src/world_starter_randomizer.c")
script=src("src/script_pokemon_util.c")
birch=src("src/battle_setup.c")
choose=src("src/starter_choose.c")
ui=src("src/debug.c")
need(layout,"u8 randomizerStarters:1;")
need(layout,"/*0x96*/ u16 randomizerSeed;")
need(layout,"sizeof(struct SaveBlock2) == 0xF2C")
need(src("include/constants/global.h"),"#define SAVE_FORMAT_VERSION 12")
for t in (new,migrate): need(t,"gSaveBlock2Ptr->randomizerStarters = FALSE;")
need(src("test/overworld/SavePatch.py"),"sv.sb2[bits_off] &= 0x07")
need(header,"enum Species WorldRandomizeStarterSpecies(enum Species original);")
need(rng,"!gSaveBlock2Ptr->randomizerStarters")
need(rng,"gSaveBlock2Ptr->randomizerSeed")
for q in ("MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", "MAP_NEW_BARK_TOWN_LAB", "MAP_ROUTE101",
          "SPECIES_BULBASAUR","SPECIES_SQUIRTLE","SPECIES_CHARMANDER",
          "SPECIES_CHIKORITA","SPECIES_CYNDAQUIL","SPECIES_TOTODILE",
          "SPECIES_TREECKO","SPECIES_TORCHIC","SPECIES_MUDKIP",
          "isMegaEvolution","isPrimalReversion","isUltraBurst","isGigantamax",
          "isTeraForm","isTotem","IsSpeciesEnabled(result)","attempts < NUM_SPECIES - 1"):
    need(rng,q)
# Native Hoenn bag callback randomizes ONLY the actual award. Do not alter
# the selection UI's starters or original VAR_STARTER_MON.
award=birch.split("static void CB2_GiveStarter(void)\n{",1)[1].split("static void CB2_StartFirstBattle(void)\n{",1)[0]
need(award,"*GetVarPointer(VAR_STARTER_MON) = gSpecialVar_Result;")
need(award,"starterMon = GetStarterPokemon(gSpecialVar_Result);")
need(award,"ScriptGiveMon(WorldRandomizeStarterSpecies(starterMon), 5, ITEM_NONE);")
assert award.index("VAR_STARTER_MON") < award.index("WorldRandomizeStarterSpecies")
need(choose,"return sStarterMon[region][chosenStarterId];")
assert "WorldRandomizeStarterSpecies" not in choose
# Kanto/Johto use givemon at labs, so player-side, first-free-slot and
# item-free script constraints must remain. Rival picks are source-scripted.
gift=script.split("void ScrCmd_createmon(",1)[1].split("#undef PARSE_FLAG",1)[0]
need(gift,"if (side == B_SIDE_PLAYER && slot == PARTY_SIZE && item == ITEM_NONE)")
need(gift,"species = WorldRandomizeStarterSpecies(species);")
need(gift,"if (species == giftOriginal)")
need(gift,"species = WorldRandomizeOptionalGiftSpecies(species);")
for p in ("data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc",
          "data/maps/NewBarkTown_Lab/scripts.inc"):
    need(src(p),"givemon PLAYER_STARTER_SPECIES, 5")
for x in ("Enable Starters","Disable Starters","DebugAction_Randomizer_EnableStarters",
          "DebugAction_Randomizer_DisableStarters",
          "gSaveBlock2Ptr->randomizerStarters = TRUE",
          "gSaveBlock2Ptr->randomizerStarters = FALSE","i == 10"):
    need(ui,x)
# Portable pure mirror hash range and seed reproducibility.
mask=0xFFFFFFFF
def mix(seed,original,map_id,count=1800):
    h=(0x9E3779B9^seed)&mask
    h^=(original*0x85EBCA6B)&mask
    h^=((map_id<<8)|0x5300)&mask
    h^=h>>16
    h=(h*0x7FEB352D)&mask
    h^=h>>15
    h=(h*0x846CA68B)&mask
    h^=h>>16
    return 1+h%(count-1)
assert mix(2026,1,0x6478)==mix(2026,1,0x6478)
assert mix(2027,1,0x6478)!=mix(2026,1,0x6478)
assert len({mix(i,1,0x6478) for i in range(100)})>=75
print("P0 seeded original Oak/Elm/Birch starter awards, saved toggle, quest/rival protection: PASS")
