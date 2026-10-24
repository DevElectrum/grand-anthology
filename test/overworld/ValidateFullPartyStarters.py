#!/usr/bin/env python3
"""Pin full-party starter failure and PC routes for all 3 mapped regions."""
from pathlib import Path
import re
root=Path(__file__).resolve().parents[2]
def txt(path): return (root/path).read_text()
def body(s, label):
    m=re.search(r"^"+re.escape(label)+r"::\s*$",s,re.M)
    assert m, f"script missing: {label}"
    tail=s[m.end():]
    nxt=re.search(r"^[A-Za-z0-9_]+::\s*$",tail,re.M)
    return tail[:nxt.start()] if nxt else tail
oak=txt("data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc")
elm=txt("data/maps/NewBarkTown_Lab/scripts.inc")
birch=txt("data/maps/LittlerootTown_ProfessorBirchsLab/scripts.inc")
for source,start,prefix in ((oak,"PalletTown_ProfessorOaksLab_EventScript_ChoseStarter","PalletTown_ProfessorOaksLab_EventScript_"),
                             (elm,"NewBarkTown_Lab_EventScript_ChoseStarter","NewBarkTown_Lab_EventScript_")):
    b=body(source,start)
    assert "givemon PLAYER_STARTER_SPECIES, 5" in b
    assert f"MON_GIVEN_TO_PARTY, {prefix}StarterInParty" in b
    assert f"MON_GIVEN_TO_PC, {prefix}StarterInPC" in b
    assert b.index("goto Common_EventScript_NoMoreRoomForPokemon") < len(b)
    accepted=body(source,prefix+"StarterAccepted")
    assert "setflag FLAG_SYS_POKEMON_GET" in accepted
    assert f"call {prefix}StarterAccepted" in body(source,prefix+"StarterInPC")
    assert f"call {prefix}StarterAccepted" in body(source,prefix+"StarterInParty")
    # No progression event in the branch for BOTH party and all PC boxes full.
    assert "setflag" not in b and "removeobject" not in b
for species in ("Cyndaquil","Totodile","Chikorita"):
    base="LittlerootTown_ProfessorBirchsLab_EventScript_"
    b=body(birch,base+"Give"+species)
    assert f"givemon SPECIES_{species.upper()}, 5" in b
    assert f"MON_GIVEN_TO_PARTY, {base}Send{species}ToParty" in b
    assert f"MON_GIVEN_TO_PC, {base}Send{species}ToPC" in b
    assert "goto Common_EventScript_NoMoreRoomForPokemon" in b
    assert "setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 6" not in b
    pc=body(birch,base+"Send"+species+"ToPC")
    assert "call "+base+"ReceivedJohtoStarter" in pc
# Route / Give API is shared with the runtime Quick gift, not a debug function.
gift=txt("src/script_pokemon_util.c")
pokemon=txt("src/pokemon.c")
assert "return GiveScriptedMonToPlayer(&mon, PARTY_SIZE);" in gift
assert "return CopyMonToPC(mon);" in pokemon
print("P0 three-region full-party starter/PC overflow source contract: PASS")

# The BoxPokemon array starts at offset 4, not 1: u32 personality
# forces 3 bytes of alignment after PokemonStorage.currentBox.
# This matches the following boxNames offset 0x8344 (4+14*30*80).
storage=txt("include/pokemon_storage_system.h")
assert "/*0x0004*/ struct BoxPokemon boxes[TOTAL_BOXES_COUNT][IN_BOX_COUNT]" in storage
assert "/*0x8344*/ u8 boxNames" in storage
assert "TOTAL_BOXES_COUNT       14" in storage
assert "STATIC_ASSERT(offsetof(struct PokemonStorage, boxes) == 0x0004" in storage
assert "STATIC_ASSERT(offsetof(struct PokemonStorage, boxNames) == 0x8344" in storage
print("P0 shared PC box layout (aligned first slot offset 4): PASS")

# Hoenn's Birch rescue gives the starter in a callback rather than through
# a normal script command. Guard the *entire* scene before its flags change,
# and avoid trapping a full-PC player inside Route 101's rescue boundary.
route=txt("data/maps/Route101/scripts.inc")
preflight=body(route,"Route101_EventScript_BirchsBag")
assert "callnative Route101_ScrCanReceiveStarter" in preflight
assert "goto_if_eq VAR_RESULT, FALSE, Route101_EventScript_NoStarterSpace" in preflight
assert preflight.index("callnative Route101_ScrCanReceiveStarter") < preflight.index("setflag FLAG_SYS_POKEMON_GET")
assert preflight.index("callnative Route101_ScrCanReceiveStarter") < preflight.index("setflag FLAG_RESCUED_BIRCH")
blocked=body(route,"Route101_EventScript_NoStarterSpace")
assert "warp MAP_LITTLEROOT_TOWN, 10, 4" in blocked
for forbidden in ("FLAG_SYS_POKEMON_GET","FLAG_RESCUED_BIRCH","VAR_BIRCH_LAB_STATE"):
    assert forbidden not in blocked
cfunc=txt("src/region_switch.c")
assert "void Route101_ScrCanReceiveStarter(struct ScriptContext *ctx)" in cfunc
assert "gSpecialVar_Result = !IsPlayerPartyAndPokemonStorageFull();" in cfunc
assert "void Route101_ScrCanReceiveStarter(struct ScriptContext *ctx);" in txt("include/regions.h")
print("P0 Hoenn Birch full-party/full-PC refusal preserves starter and quest state: PASS")

# Assert that the starter choices are physically reachable as actual object
# events. These mappings are easy to break when demaking maps/merging scripts
# even when every starter function still compiles and source-checks correctly.
import json
def mapdata(name):
    return json.loads(txt(f"data/maps/{name}/map.json"))
def starter_objects(name):
    return {(o["x"],o["y"]):o["script"] for o in mapdata(name)["object_events"]
            if "Ball" in o["script"] or o["script"]=="Route101_EventScript_BirchsBag"}
assert {k:starter_objects("NewBarkTown_Lab")[k] for k in ((8,4),(9,4),(10,4))} == {
    (8,4):"NewBarkTown_Lab_EventScript_ChikoritaBall",
    (9,4):"NewBarkTown_Lab_EventScript_CyndaquilBall",
    (10,4):"NewBarkTown_Lab_EventScript_TotodileBall",
}
assert {k:starter_objects("PalletTown_ProfessorOaksLab_Frlg")[k] for k in ((8,4),(9,4),(10,4))} == {
    (8,4):"PalletTown_ProfessorOaksLab_EventScript_BulbasaurBall",
    (9,4):"PalletTown_ProfessorOaksLab_EventScript_SquirtleBall",
    (10,4):"PalletTown_ProfessorOaksLab_EventScript_CharmanderBall",
}
assert starter_objects("Route101")[(7,14)]=="Route101_EventScript_BirchsBag"
# The World Transit Johto destination lands on the house's special second
# upstairs warp, with a walkable route through 1F, town and Elm's lab.
hub=txt("data/maps/RegionHub/scripts.inc")
assert "warp MAP_NEW_BARK_TOWN_PLAYERS_HOUSE_2F, 1" in body(hub,"RegionHub_EventScript_AttendantJohtoBoard")
def warp(name,xy,dest):
    assert any((z["x"],z["y"])==xy and z["dest_map"]==dest
               for z in mapdata(name)["warp_events"]), (name,xy,dest)
warp("NewBarkTown_PlayersHouse_2F",(1,6),"MAP_NEW_BARK_TOWN_PLAYERS_HOUSE_1F")
warp("NewBarkTown_PlayersHouse_1F",(9,8),"MAP_NEW_BARK_TOWN")
warp("NewBarkTown",(10,9),"MAP_NEW_BARK_TOWN_LAB")
print("P0 in-world Kanto/Johto/Hoenn starter objects and Johto route wiring: PASS")
