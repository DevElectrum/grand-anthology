#!/usr/bin/env python3
"""P0 seeded *optional* scripted gift encounters, independent from starters/statics.

Source contract + deterministic hash fixtures, not an mGBA gameplay assertion.
"""
from pathlib import Path

root=Path(__file__).resolve().parents[2]
def src(p): return (root/p).read_text()
def need(s,t): assert t in s,"Gift randomizer contract missing: "+t

layout=src("include/global.h")
migrate=src("src/load_save.c")
fresh=src("src/new_game.c")
gift=src("src/script_pokemon_util.c")
ui=src("src/debug.c")
event=src("asm/macros/event.inc")
need(layout,"u8 randomizerGifts:1;")
need(layout,"u8 randomizerStarters:1;")
need(layout,"/*0x96*/ u16 randomizerSeed;")
need(layout,"sizeof(struct SaveBlock2) == 0xF2C")
need(migrate,"if (savedVersion < 12)")
for block in (migrate, fresh):
    need(block,"gSaveBlock2Ptr->randomizerGifts = FALSE;")
need(src("include/constants/global.h"),"#define SAVE_FORMAT_VERSION 13")
need(src("test/overworld/SavePatch.py"),"sv.sb2[bits_off] &= 0x07")
need(gift,"static enum Species WorldRandomizeOptionalGiftSpecies(enum Species original)")
need(gift,"!gSaveBlock2Ptr->randomizerGifts")
need(gift,"gSaveBlock2Ptr->randomizerSeed")
for species in ("SPECIES_EEVEE","SPECIES_LAPRAS","SPECIES_CASTFORM","SPECIES_PORYGON","SPECIES_SHUCKLE"):
    need(gift,"case "+species+":")
for blocked in ("isMegaEvolution","isPrimalReversion","isUltraBurst","isGigantamax","isTeraForm","isTotem"):
    need(gift,blocked)
need(gift,"attempts < NUM_SPECIES - 1")
need(gift,"IsSpeciesEnabled(result)")
need(event,".macro givemon species:req")
need(event,".byte PARTY_SIZE @ assign to first empty slot")
# One genuine script path only; generic ScriptGiveMon is used by the Cheats
# menu and original Elm/Oak starter compatibility functions.
give=gift.split("void ScrCmd_createmon(",1)[1].split("#undef PARSE_FLAG",1)[0]
need(give,"if (side == B_SIDE_PLAYER && slot == PARTY_SIZE && item == ITEM_NONE)")
need(give,"species = WorldRandomizeOptionalGiftSpecies(species);")
need(give,"bool32 randomizedGift = species != giftOriginal;")
need(give,"moves[i] = MOVE_DEFAULT;")
need(give,"abilityNum = NUM_ABILITY_PERSONALITY;")
need(give,"gender = MON_GENDER_RANDOM;")
assert give.index("WorldRandomizeOptionalGiftSpecies") < give.index("// Perfect IV calculation")
assert give.index("if (randomizedGift)") < give.index("enum ShinyMode shinyMode")
main=gift.split("u32 ScriptGiveMon(enum Species species,",1)[1].split("static enum Species WorldRandomizeOptionalGiftSpecies(",1)[0]
assert "WorldRandomizeOptionalGiftSpecies" not in main
for t in ("Enable Gifts","Disable Gifts","DebugAction_Randomizer_EnableGifts","DebugAction_Randomizer_DisableGifts",
          "gSaveBlock2Ptr->randomizerGifts = TRUE","gSaveBlock2Ptr->randomizerGifts = FALSE",
          "i == 10"):
    need(ui,t)
# Pure hash mirror for repeatable seed/form/map behavior.
mask=0xFFFFFFFF
def mix(seed,original,group,num,count=1800):
    h=(0x9E3779B9^seed)&mask
    h^=(original*0x85EBCA6B)&mask
    h^=((group<<24)|(num<<16)|0x4700)&mask
    h^=h>>16
    h=(h*0x7FEB352D)&mask
    h^=h>>15
    h=(h*0x846CA68B)&mask
    h^=h>>16
    return 1+h%(count-1)

assert mix(2026,133,75,7)==mix(2026,133,75,7)
assert mix(2027,133,75,7)!=mix(2026,133,75,7)
assert mix(2026,133,75,8)!=mix(2026,133,75,7)
assert len({mix(i,133,75,7) for i in range(100)})>=75
print("P0 seeded optional gifts (five safe source species), old-save migration, separate player toggle: PASS")
