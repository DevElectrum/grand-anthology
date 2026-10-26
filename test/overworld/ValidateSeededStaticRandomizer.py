#!/usr/bin/env python3
"""Guard the production v13 seeded static-encounter feature without scene-by-scene tests.

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
need(battle, "WorldIsFieldStableDiscoverySpecies(result)")
need(battle, "randomizerVersion == 0")
need(battle, "ResolveRandomizedStaticRelic(enum Species species, enum Item scriptedItem)")
need(battle, "IsSpeciesEnabled(result)")
single=battle.split("void CreateScriptedWildMon(",1)[1].split("void CreateScriptedDoubleWildMon(",1)[0]
double=battle.split("void CreateScriptedDoubleWildMon(",1)[1].split("void ScriptSetMonMoveSlot(",1)[0]
need(single, "species = WorldRandomizeStaticSpecies(species, 0);")
need(double, "species1 = WorldRandomizeStaticSpecies(species1, 0);")
need(double, "species2 = WorldRandomizeStaticSpecies(species2, 0);")
need(single, "item = ResolveRandomizedStaticRelic(species, item);")
need(double, "item1 = ResolveRandomizedStaticRelic(species1, item1);")
need(double, "item2 = ResolveRandomizedStaticRelic(species2, item2);")
assert single.index("WorldRandomizeStaticSpecies") < single.index("GetMonPersonality")
assert double.index("WorldRandomizeStaticSpecies") < double.index("GetMonPersonality")
visible=src("src/event_object_movement.c")
need(visible, "WorldRandomizeStaticSpecies(original, slot)")
for gfx in ("SNORLAX", "SUDOWOODO", "KECLEON", "CELEBI"):
    need(visible, "case OBJ_EVENT_GFX_"+gfx+":")
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
# Encounter location flags must not be inferred from a different species catch.
for location, original, site_flag in (
    ("MtPyre_Exterior", "YVELTAL", "FLAG_CAUGHT_RANDOM_MTPYRE_EXTERIOR_YVELTAL"),
    ("CeruleanCave_2F_Frlg", "NECROZMA", "FLAG_CAUGHT_RANDOM_CERULEANCAVE_2F_NECROZMA"),
    ("SeafoamIslands_B2F_Frlg", "MANAPHY", "FLAG_CAUGHT_RANDOM_SEAFOAMISLANDS_B2F_MANAPHY"),
    ("SeafoamIslands_B2F_Frlg", "TAPU_FINI", "FLAG_CAUGHT_RANDOM_SEAFOAMISLANDS_B2F_TAPU_FINI"),
    ("Route102", "JIRACHI", "FLAG_CAUGHT_RANDOM_ROUTE102_JIRACHI"),
    ("Route105", "CHIEN_PAO", "FLAG_CAUGHT_RANDOM_ROUTE105_CHIEN_PAO"),
    ("Route125", "WALKING_WAKE", "FLAG_CAUGHT_RANDOM_ROUTE125_WALKING_WAKE"),
    ("Route124", "IRON_CROWN", "FLAG_CAUGHT_RANDOM_ROUTE124_IRON_CROWN"),
    ("Route43", "LANDORUS", "FLAG_CAUGHT_RANDOM_ROUTE43_LANDORUS"),
    ("Route41", "THUNDURUS", "FLAG_CAUGHT_RANDOM_ROUTE41_THUNDURUS"),
    ("Route44", "ETERNATUS", "FLAG_CAUGHT_RANDOM_ROUTE44_ETERNATUS"),
    ("Route44", "TORNADUS", "FLAG_CAUGHT_RANDOM_ROUTE44_TORNADUS"),
):
    map_script = src("data/maps/"+location+"/scripts.inc")
    need(map_script, "seteventmon SPECIES_"+original)
    need(map_script, "getcaughtmon SPECIES_"+original)
    need(map_script, "setflag "+site_flag)
    need(map_script, "callnative AnthologyStaticRandomizerIsOn")
    need(src("include/constants/flags.h"), "#define "+site_flag+" ")

for location, species, captured in (
    ("UnionCave_1F", "TERRAKION", "FLAG_CAUGHT_RANDOM_UNIONCAVE_1F_TERRAKION"),
    ("NationalPark_Normal", "VIRIZION", "FLAG_CAUGHT_RANDOM_NATIONALPARK_NORMAL_VIRIZION"),
    ("NationalPark_Normal", "VICTINI", "FLAG_CAUGHT_RANDOM_NATIONALPARK_NORMAL_VICTINI"),
    ("ViridianForest_Frlg", "SHAYMIN", "FLAG_CAUGHT_RANDOM_VIRIDIANFOREST_SHAYMIN"),
    ("Route39", "ENAMORUS", "FLAG_CAUGHT_RANDOM_ROUTE39_ENAMORUS"),
):
    site = src("data/maps/"+location+"/scripts.inc")
    need(site, "getcaughtmon SPECIES_"+species)
    need(site, "seteventmon SPECIES_"+species)
    need(site, "setflag "+captured)
    need(site, "callnative AnthologyStaticRandomizerIsOn")
    need(src("include/constants/flags.h"), "#define "+captured+" ")
for prerequisite in ("FLAG_CAUGHT_RANDOM_ROUTE44_TORNADUS", "FLAG_CAUGHT_RANDOM_ROUTE41_THUNDURUS", "FLAG_CAUGHT_RANDOM_ROUTE43_LANDORUS"):
    need(src("data/maps/Route39/scripts.inc"), "goto_if_unset "+prerequisite)
need(src("data/maps/NationalPark_Normal/scripts.inc"), "goto_if_unset FLAG_SYS_GAME_CLEAR, NationalPark_Normal_HideVictini")

print("P0 v13 seeded static singles/doubles, sprite parity, form safety, and relic ownership: PASS")

# Per-source capture guards for late October repair batch.
for location, original, site_flag in (
    ("DesertRuins", "REGIROCK", "FLAG_CAUGHT_RANDOM_DESERTRUINS_REGIROCK"),
    ("IslandCave", "REGICE", "FLAG_CAUGHT_RANDOM_ISLANDCAVE_REGICE"),
    ("AncientTomb", "REGISTEEL", "FLAG_CAUGHT_RANDOM_ANCIENTTOMB_REGISTEEL"),
    ("RockTunnel_B1F_Frlg", "REGIGIGAS", "FLAG_CAUGHT_RANDOM_ROCKTUNNEL_REGIGIGAS"),
    ("RockTunnel_B1F_Frlg", "UXIE", "FLAG_CAUGHT_RANDOM_ROCKTUNNEL_UXIE"),
    ("RockTunnel_B1F_Frlg", "MESPRIT", "FLAG_CAUGHT_RANDOM_ROCKTUNNEL_MESPRIT"),
    ("RockTunnel_B1F_Frlg", "AZELF", "FLAG_CAUGHT_RANDOM_ROCKTUNNEL_AZELF"),
    ("RockTunnel_B1F_Frlg", "HEATRAN", "FLAG_CAUGHT_RANDOM_ROCKTUNNEL_HEATRAN"),
    ("Route117", "OGERPON_TEAL", "FLAG_CAUGHT_RANDOM_ROUTE117_OGERPON"),
    ("Route112", "OKIDOGI", "FLAG_CAUGHT_RANDOM_ROUTE112_OKIDOGI"),
    ("Route113", "FEZANDIPITI", "FLAG_CAUGHT_RANDOM_ROUTE113_FEZANDIPITI"),
    ("Route114", "MUNKIDORI", "FLAG_CAUGHT_RANDOM_ROUTE114_MUNKIDORI"),
    ("Route117", "PECHARUNT", "FLAG_CAUGHT_RANDOM_ROUTE117_PECHARUNT"),
    ("PokemonTower_7F_Frlg", "DARKRAI", "FLAG_CAUGHT_RANDOM_POKEMONTOWER_DARKRAI"),
):
    script = src("data/maps/"+location+"/scripts.inc")
    need(script, "getcaughtmon SPECIES_"+original)
    need(script, "setflag "+site_flag)
    need(script, "callnative AnthologyStaticRandomizerIsOn")
    need(src("include/constants/flags.h"), "#define "+site_flag+" ")
for prereq in ("FLAG_CAUGHT_RANDOM_DESERTRUINS_REGIROCK",
               "FLAG_CAUGHT_RANDOM_ISLANDCAVE_REGICE",
               "FLAG_CAUGHT_RANDOM_ANCIENTTOMB_REGISTEEL"):
    need(src("data/maps/RockTunnel_B1F_Frlg/scripts.inc"), "goto_if_unset "+prereq)
for prereq in ("FLAG_CAUGHT_RANDOM_ROUTE117_OGERPON",
               "FLAG_CAUGHT_RANDOM_ROUTE112_OKIDOGI",
               "FLAG_CAUGHT_RANDOM_ROUTE113_FEZANDIPITI",
               "FLAG_CAUGHT_RANDOM_ROUTE114_MUNKIDORI"):
    need(src("data/maps/Route117/scripts.inc"), "goto_if_unset "+prereq)
need(src("data/maps/PokemonTower_7F_Frlg/scripts.inc"), "goto_if_unset FLAG_CAUGHT_RANDOM_MTMOON_CRESSELIA")


# Capture-source ledger continuity. This single audit closes the gap left by
# checking individual maps in isolation: all reserved site flags must be
# unique, stable, save-bank-safe and actually written from a battle script.
import re
flag_source = src("include/constants/flags.h")
source_defs = re.findall(
    r"^#define (FLAG_CAUGHT_RANDOM_[A-Z0-9_]+) \(FLAG_WORLD_MAP_BANK \+ 0x([0-9A-Fa-f]+)\)$",
    flag_source, re.M)
source_slots = {int(value, 16): name for name, value in source_defs}
next_free_match = re.search(
    r"^// Next free world-map flag: bank \+ 0x([0-9A-Fa-f]+)\.$",
    flag_source, re.M)
assert next_free_match, "Missing explicit seeded capture-site flag allocation boundary"
next_free_offset = int(next_free_match.group(1), 16)
assert len(source_defs) == len(source_slots), "Duplicate seeded source flag save offset"
assert set(source_slots) == set(range(0x12D, next_free_offset)), (
    "Noncontiguous seeded source flags or incorrect next-free marker",
    len(source_slots), hex(next_free_offset))
assert 0xD40 + max(source_slots) < 0x1030, "Source flags exceed saved FLAGS_COUNT"
all_world_map_flags = re.findall(
    r"^#define (FLAG_[A-Z0-9_]+) \(FLAG_WORLD_MAP_BANK \+ 0x([0-9A-Fa-f]+)\)$",
    flag_source, re.M)
for other, value in all_world_map_flags:
    offset = int(value, 16)
    if offset in source_slots:
        assert other == source_slots[offset], (
            "Source flag collides with unrelated save flag", other, source_slots[offset], hex(offset))
map_scripts = "\n".join(path.read_text() for path in (root / "data/maps").glob("*/scripts.inc"))
for flag in source_slots.values():
    assert "setflag " + flag in map_scripts, ("Caught-source flag never written in map scripts", flag)
print(f"P0 all {len(source_slots)} seeded source-caught flags: contiguous, noncolliding, inside save bank, written from game scripts PASS")
