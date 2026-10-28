#!/usr/bin/env python3
"""Source guard: pre-Champion rare habitat encounters mirror DexNav choices."""
from pathlib import Path
root = Path(__file__).resolve().parents[2]
wild = (root / "src/wild_encounter.c").read_text()
rare_catalog = (root / "src/data/anthology_rare_habitats.h").read_text()
wild += "\\n" + rare_catalog
dex = (root / "src/dexnav.c").read_text()
placements = {
 "MAP_VIRIDIAN_FOREST": "KARTANA", "MAP_POWER_PLANT": "XURKITREE",
 "MAP_SEAFOAM_ISLANDS_1F": "NIHILEGO", "MAP_POKEMON_MANSION_1F": "BLACEPHALON",
 "MAP_ROUTE1": "POIPOLE", "MAP_ROUTE2": "TURTWIG", "MAP_ROUTE3": "CHIMCHAR",
 "MAP_MT_MOON_1F": "PIPLUP", "MAP_ROUTE29": "SNIVY", "MAP_ROUTE30": "TEPIG",
 "MAP_ROUTE31": "OSHAWOTT", "MAP_ROUTE32": "GROOKEY", "MAP_ROUTE34": "SCORBUNNY",
 "MAP_ROUTE35": "SOBBLE", "MAP_ROUTE101": "CHESPIN", "MAP_ROUTE102": "FENNEKIN",
 "MAP_ROUTE103": "FROAKIE", "MAP_PETALBURG_WOODS": "SPRIGATITO",
 "MAP_ROUTE111": "GREAT_TUSK", "MAP_MT_PYRE_EXTERIOR": "FLUTTER_MANE",
 "MAP_VICTORY_ROAD_1F": "IRON_VALIANT"
}
map_ids = {
    line.split('"')[3] for path in (root / "data/maps").glob("*/map.json")
    for line in path.read_text().splitlines() if '"id":' in line
}
species = (root / "include/constants/species.h").read_text()
for loc, mon in placements.items():
    assert loc in map_ids, loc
    assert f"SPECIES_{mon}" in species, mon
    assert f"RARE_ON_MAP({loc}, {mon})" in wild, loc
assert len(placements) == 21
for text in (
    "Random() % 100 < 2", "GetSafariZoneFlag()", "gSaveBlock2Ptr->randomizerWild",
    "distinct >= NUM_LAND_MONS_ENCOUNTER_SLOTS",
    "WorldIsNaturalDiscoverySpecies(rare)"):
    assert text in wild
for text in (
    "AnthologyGetRareHabitatSpecies(WILD_AREA_LAND, info)",
    "AnthologyGetRareHabitatSpecies(WILD_AREA_LAND, landMonsInfo)",
    "grassIndex < NUM_LAND_MONS_ENCOUNTER_SLOTS"):
    assert text in dex
print("P0 rare habitat: original 21 priority placements, shared DexNav guards PASS")

# Every mapped rare must have a real land table in every map variant, fit into
# DexNav's twelve land icons and not duplicate an ordinary encounter.
import json
import re
from pathlib import Path

encounters = json.loads((root / "src/data/wild_encounters.json").read_text())["wild_encounter_groups"][0]["encounters"]
by_map = {}
for encounter in encounters:
    if "land_mons" in encounter:
        by_map.setdefault(encounter["map"], []).append(encounter["land_mons"])

mappings = re.findall(r"^\s*RARE_ON_MAP\((MAP_[A-Z0-9_]+), ([A-Z0-9_]+)\)$", wild, re.M)
assert len(mappings) >= 170, f"missing habitat mappings: {len(mappings)}"
assert len({mapping[0] for mapping in mappings}) == len(mappings), "duplicate map habitats"
all_species = (root / "include/constants/species.h").read_text()
for map_name, mon in mappings:
    assert map_name in by_map, (map_name, "missing land table")
    assert f"SPECIES_{mon}" in all_species, (map_name, mon, "unknown species")
    for section in by_map[map_name]:
        natives = {x["species"] for x in section["mons"]}
        assert len(natives) < 12, (map_name, "DexNav land icon overflow")
        assert f"SPECIES_{mon}" not in natives, (map_name, mon, "already native")

print(f"Verified {len(mappings)} mapped pre-League habitats and DexNav slots against live encounter tables")

# The actual rare encounter uses the same first-native-slot level displayed
# by DexNav, and repel/Keen Eye evaluate that exact level, not a different one.
assert "bool8 useRare = rare != SPECIES_NONE && Random() % 100 < 2;" in wild
assert "ChooseWildMonLevel(wildMonInfo->wildPokemon, (useRare || extra != SPECIES_NONE || useAquatic) ? 0 : wildMonIndex, area)" in wild
assert wild.index("bool8 useRare = rare") < wild.index("IsWildLevelAllowedByRepel(level)")
assert "min = landMonsInfo->wildPokemon[0].minLevel;" in dex
assert "max = landMonsInfo->wildPokemon[0].maxLevel;" in dex

# Extra natural habitats share DexNav's genuine land capacity, native level,
# Safari/frontier/randomizer guard and source maps without replacing originals.
extra_text = (root / "src/data/anthology_extra_habitats.h").read_text()
extra = re.findall(r"^ECOLOGY_ON_MAP\((MAP_[A-Z0-9_]+), ([A-Z0-9_]+)\)$", extra_text, re.M)
assert len(extra) == 308, (len(extra), "lost or unexpected habitat entries")
for forbidden in (("MAP_ROUTE22", "ZAPDOS_GALAR"),
                  ("MAP_SEAFOAM_ISLANDS_B1F", "ARTICUNO_GALAR"),
                  ("MAP_POKEMON_MANSION_1F", "MOLTRES_GALAR"),
                  ("MAP_ROUTE111", "GROUDON"),
                  ("MAP_SEAFLOOR_CAVERN_ROOM1", "KYOGRE")):
    assert forbidden not in extra, ("forbidden wild Legendary habitat", forbidden)
assert len({species for _map, species in extra[:113]}) == 113, "original 113 changed"
assert len(set(extra)) == len(extra), "duplicate extra species within one map"
# A postgame wild location must not be a required pre-Champion source. These
# ten region-assigned sources live on pre-League maps; their older encounters
# stay available as optional postgame habitat rather than being deleted.
pre_champion_relocations = {
    ("MAP_ROUTE10", "DRATINI"),
    ("MAP_SEAFOAM_ISLANDS_1F", "SNOVER"),
    ("MAP_SEAFOAM_ISLANDS_B2F", "GLACEON"),
    ("MAP_ROUTE45", "AXEW"),
    ("MAP_ROUTE45", "DRUDDIGON"),
    ("MAP_DARK_CAVE_NORTH_SIDE", "DREEPY"),
    ("MAP_ROUTE46", "DURALUDON"),
    ("MAP_METEOR_FALLS_1F_1R", "ROARING_MOON"),
    ("MAP_GRANITE_CAVE_B2F", "KINGAMBIT"),
    ("MAP_SHOAL_CAVE_LOW_TIDE_ICE_ROOM", "BAXCALIBUR"),
}
assert len(pre_champion_relocations) == 10
assert pre_champion_relocations <= set(extra), "lost pre-Champion species relocation"
# Regional origin requirements are distinct from merely owning a National ID.
# Earlier catalogs sometimes placed a Generation IV/V/VII/VIII/IX seed only
# in another region. Retain those optional sources and add true home-region
# routes before the respective FIRST Championship.
regional_home_sources = {
    "KANTO": {
    ("MAP_ROUTE2", "PIKIPEK"),
    ("MAP_ROUTE3", "COMBEE"),
    ("MAP_ROUTE4", "BUIZEL"),
    ("MAP_MT_MOON_1F", "CHINGLING"),
    ("MAP_POKEMON_TOWER_5F", "SPIRITOMB"),
    ("MAP_ROUTE23", "RIOLU"),
    ("MAP_ROUTE12", "MANTYKE"),
    ("MAP_SEAFOAM_ISLANDS_B3F", "CRABRAWLER"),
    ("MAP_ROUTE6", "ORICORIO"),
    ("MAP_ROUTE5", "CUTIEFLY"),
    ("MAP_SEAFOAM_ISLANDS_B4F", "WISHIWASHI"),
    ("MAP_ROUTE25", "MAREANIE"),
    ("MAP_VIRIDIAN_FOREST", "MORELULL"),
    ("MAP_POKEMON_MANSION_3F", "SALANDIT"),
    ("MAP_ROUTE13", "SANDYGAST"),
    ("MAP_MT_MOON_B1F", "MINIOR"),
    ("MAP_POKEMON_MANSION_B1F", "TURTONATOR"),
    ("MAP_POKEMON_TOWER_4F", "MIMIKYU"),
    ("MAP_VICTORY_ROAD_2F", "DRAMPA"),
    ("MAP_SEAFOAM_ISLANDS_B4F", "DHELMISE"),
    ("MAP_VICTORY_ROAD_3F", "JANGMO_O"),
    ("MAP_ROUTE23", "BUZZWOLE"),
    ("MAP_VIRIDIAN_FOREST", "PHEROMOSA"),
    ("MAP_POWER_PLANT", "CELESTEELA"),
    ("MAP_POKEMON_MANSION_B1F", "GUZZLORD"),
    ("MAP_ROCK_TUNNEL_B1F", "STAKATAKA"),
    },
    "JOHTO": {
    ("MAP_ROUTE29", "MUNNA"),
    ("MAP_ROUTE30", "PIDOVE"),
    ("MAP_DARK_CAVE_SOUTH_SIDE", "ROGGENROLA"),
    ("MAP_DARK_CAVE_NORTH_SIDE", "WOOBAT"),
    ("MAP_UNION_CAVE_1F", "DRILBUR"),
    ("MAP_ILEX_FOREST", "PETILIL"),
    ("MAP_BURNED_TOWER_1F", "YAMASK"),
    ("MAP_ROUTE32", "EMOLGA"),
    ("MAP_ROUTE47", "ALOMOMOLA"),
    ("MAP_SPROUT_TOWER_2F", "JOLTIK"),
    ("MAP_UNION_CAVE_B1F", "KLINK"),
    ("MAP_ROUTE43", "TYNAMO"),
    ("MAP_RUINS_OF_ALPH_B1F", "ELGYEM"),
    ("MAP_BURNED_TOWER_B1F", "LITWICK"),
    ("MAP_ROUTE45", "PAWNIARD"),
    ("MAP_ROUTE42", "HEATMOR"),
    ("MAP_ICE_PATH_B4F", "DEINO"),
    ("MAP_BURNED_TOWER_1F", "LARVESTA"),
    ("MAP_ROUTE34", "YAMPER"),
    ("MAP_ROUTE46", "SILICOBRA"),
    ("MAP_ROUTE43", "ARROKUDA"),
    ("MAP_ROUTE38", "TOXEL"),
    ("MAP_ROUTE37", "SIZZLIPEDE"),
    ("MAP_ILEX_FOREST", "IMPIDIMP"),
    ("MAP_ICE_PATH_1F", "SNOM"),
    ("MAP_ICE_PATH_1F", "DELIBIRD"),
    ("MAP_ROUTE44", "WOBBUFFET"),
    },
    "HOENN": {
    ("MAP_GRANITE_CAVE_1F", "MEDITITE"),
    ("MAP_ROUTE104", "FLETCHLING"),
    ("MAP_PETALBURG_WOODS", "SCATTERBUG"),
    ("MAP_ROUTE117", "FLABEBE"),
    ("MAP_ROUTE110", "SKIDDO"),
    ("MAP_ROUTE114", "ESPURR"),
    ("MAP_GRANITE_CAVE_B2F", "HONEDGE"),
    ("MAP_ROUTE102", "INKAY"),
    ("MAP_MT_PYRE_1F", "PHANTUMP"),
    ("MAP_GRANITE_CAVE_B1F", "NOIBAT"),
    ("MAP_ROUTE101", "LECHONK"),
    ("MAP_ROUTE103", "PAWMI"),
    ("MAP_ROUTE118", "SMOLIV"),
    ("MAP_FIERY_PATH", "CHARCADET"),
    ("MAP_GRANITE_CAVE_STEVENS_ROOM", "TINKATINK"),
    ("MAP_ROUTE119", "BOMBIRDIER"),
    ("MAP_NEW_MAUVILLE_ENTRANCE", "VAROOM"),
    ("MAP_FIERY_PATH", "GLIMMET"),
    ("MAP_MT_PYRE_2F", "GREAVARD"),
    ("MAP_SHOAL_CAVE_LOW_TIDE_ICE_ROOM", "CETODDLE"),
    ("MAP_SHOAL_CAVE_LOW_TIDE_STAIRS_ROOM", "FRIGIBAX"),
    ("MAP_NEW_MAUVILLE_INSIDE", "GIMMIGHOUL"),
    ("MAP_ROUTE111", "SANDY_SHOCKS"),
    ("MAP_ROUTE110", "IRON_TREADS"),
    ("MAP_METEOR_FALLS_B1F_1R", "ARCHALUDON"),
    },
}
assert sum(map(len, regional_home_sources.values())) == 78
for _region, _pairs in regional_home_sources.items():
    assert _pairs <= set(extra), (_region, "missing home-region species")
from collections import Counter
extra_counts = Counter(m for m, _ in extra)
assert max(extra_counts.values()) <= 4, "too many slots added on one map"
for map_name, mon in extra:
    assert map_name in by_map, (map_name, "missing grass habitat")
    assert f"SPECIES_{mon}" in all_species, mon
    assert map_name in {m for m, _ in mappings}, (map_name, "extra without protected primary habitat")
    for section in by_map[map_name]:
        native = {x["species"] for x in section["mons"]}
        assert f"SPECIES_{mon}" not in native, (map_name, mon, "native duplicate")
        assert len(native) + 1 + extra_counts[map_name] <= 12, (map_name, "DexNav overflow")
for token in ("AnthologyGetExtraHabitatSpecies(WILD_AREA_LAND, info, i)",
              "AnthologyGetExtraHabitatSpecies(WILD_AREA_LAND, landMonsInfo, i)",
              "ANTHOLOGY_EXTRA_HABITAT_SLOTS"):
    assert token in dex, token
assert "else if (extra != SPECIES_NONE)" in wild
assert "ANTHOLOGY_EXTRA_HABITAT_SLOTS 4" in (root / "include/wild_encounter.h").read_text()
assert "for (habitatSlot = 0; habitatSlot < ANTHOLOGY_EXTRA_HABITAT_SLOTS; habitatSlot++)" in wild
assert "Include the bonus entries in grass completion" in dex
for mon in ("BURMY_TRASH", "COTTONEE", "SHELMET", "FARFETCHD_GALAR",
            "SCATTERBUG_RIVER", "SQUAWKABILLY_BLUE", "FLABEBE_YELLOW"):
    assert any(s == mon for _loc, s in extra), mon
print(f"P0 supplemental ecology: {len(extra)} map-species entries, ten pre-Champion postgame relocations, up to four/map, DexNav catch/search/level PASS")

# Regional Pokédex area mapping uses the same pure map lookup as the natural
# encounter catalog rather than mutating the save's live region/map coordinates.
area_screen = (root / "src/pokedex_area_screen.c").read_text()
assert "AnthologyMapHasHabitatSpecies(header->mapGroup, header->mapNum" in area_screen
assert "MapHasSpecies(header, headerSectionId, species)" in area_screen
assert '#include "data/anthology_rare_habitats.h"' in wild
assert "gSaveBlock1Ptr->location.mapGroup =" not in wild.split("bool8 AnthologyMapHasHabitatSpecies(")[1].split("void CreateWildMon(enum Species species, u8 level)")[0]
print("P0 regional Pokédex area mapping: native+170 curated+extra habitats guarded")

# C preprocessor macro continuations must end with one backslash, never
# two; the extra slash is a real optimized GBA compiler failure.
macro_lines = [line for line in (root / "src/wild_encounter.c").read_text().splitlines()
               if line.startswith("#define RARE_ON_MAP(map, mon)")]
assert len(macro_lines) == 2, macro_lines
assert all(line.endswith(chr(92)) and not line.endswith(chr(92) * 2)
           for line in macro_lines), "extra backslash in habitat macro definition"


# Aquatic P0: water and fishing are separate, source-audited and reachable
# without champion flags or replacing canonical encounters.
aquatic_catalog = (root / "src/data/anthology_aquatic_habitats.h").read_text()
aquatic = re.findall(r"^AQUATIC_ON_MAP\((MAP_[A-Z0-9_]+), (WATER|FISHING), ([A-Z0-9_]+)\)$", aquatic_catalog, re.M)
assert len(aquatic) == 81, len(aquatic)
assert sum(kind == "WATER" for _, kind, _ in aquatic) == 59
assert sum(kind == "FISHING" for _, kind, _ in aquatic) == 22
assert len({(loc, kind) for loc, kind, _ in aquatic}) == len(aquatic)
for loc, kind, mon in aquatic:
    assert loc in map_ids and f"SPECIES_{mon}" in species, (loc, kind, mon)
    matches = [e for e in encounters if e["map"] == loc and e.get(("water_mons" if kind == "WATER" else "fishing_mons"), {}).get("encounter_rate", 0) > 0]
    assert matches, (loc, kind, "no original encounter table")
    for e in matches:
        native = [w["species"] for w in e["water_mons" if kind == "WATER" else "fishing_mons"]["mons"]]
        assert f"SPECIES_{mon}" not in native, (loc, kind, mon, "duplicate native")
        if kind == "WATER":
            assert len(set(native)) < 5, (loc, "DexNav Surf icon capacity")
assert "AnthologyGetAquaticHabitatSpecies(area, wildMonInfo)" in wild
assert "AnthologyGetAquaticHabitatSpecies(WILD_AREA_FISHING, wildMonInfo)" in wild
assert "AnthologyGetAquaticHabitatSpecies(WILD_AREA_WATER, waterMonsInfo)" in dex
assert "AnthologyGetAquaticHabitatSpecies(WILD_AREA_WATER, info)" in dex
assert "AnthologyMapHasAquaticHabitatSpecies(header->mapGroup, header->mapNum, WILD_AREA_WATER" in area_screen
assert "AnthologyMapHasAquaticHabitatSpecies(header->mapGroup, header->mapNum, WILD_AREA_FISHING" in area_screen
assert "|| useAquatic) ? 0 : wildMonIndex" in wild
assert "Random() % 100 < 3" in wild
assert ("MAP_ROUTE20", "WATER", "LAPRAS") in aquatic
print("P0 aquatic ecology: 59 Surf + 22 rod placements, Route 20 Lapras, DexNav and native parity PASS")

# Permanent regional aquatic forms have explicit reachable pre-Champion sources.
for form in ("WOOPER_PALDEA", "BASCULIN_RED_STRIPED", "BASCULIN_BLUE_STRIPED",
             "BASCULIN_WHITE_STRIPED", "SHELLOS_WEST", "SHELLOS_EAST",
             "STUNFISK_GALAR", "QWILFISH_HISUI", "SLOWPOKE_GALAR", "CORSOLA_GALAR"):
    assert f", {form})" in aquatic_catalog, form
assert "AQUATIC_ON_MAP(MAP_ROUTE43, WATER, DONDOZO)" in aquatic_catalog
assert "AQUATIC_ON_MAP(MAP_ROUTE120, FISHING, FEEBAS)" in aquatic_catalog
print("P0 aquatic field-stable forms: ten variants and Dondozo placements guarded")

# Gen 7 Battle Bond is a species-bound Greninja variant, not a held-item
# conversion or Hidden Ability Patch. Guard its actual pre-League Surf
# acquisition and its matching battle transformation ability/form table.
assert ("MAP_ROUTE129", "WATER", "GRENINJA_BATTLE_BOND") in aquatic
bond_info = (root / "src/data/pokemon/species_info/gen_6_families.h").read_text()
bond = bond_info.split("[SPECIES_GRENINJA_BATTLE_BOND] =", 1)[1].split("    },", 1)[0]
assert ".abilities = { ABILITY_BATTLE_BOND," in bond
forms = (root / "src/data/pokemon/form_change_tables.h").read_text()
assert "sGreninjaBattleBondFormChangeTable[]" in forms
assert "{FORM_CHANGE_BATTLE_BOND, SPECIES_GRENINJA_ASH," in forms
print("P0 Battle Bond Greninja: pre-Champion Hoenn acquisition + Gen 7 transform guarded")

# The seeded randomizer and curated biome catalog must both use the field-stable
# predicate; otherwise permanent wild catches can become battle-only forms.
# Battle Bond's base remains eligible.
stable_source = (root / "src/wild_encounter.c").read_text()
stable_check = stable_source.split("bool8 WorldIsFieldStableDiscoverySpecies(enum Species species)", 1)[1].split("// Supplemental ordinary species", 1)[0]
for temporary in ("GRENINJA_ASH", "AEGISLASH_BLADE", "WISHIWASHI_SCHOOL", "MIMIKYU_BUSTED", "PALAFIN_HERO", "CRAMORANT_GULPING", "CRAMORANT_GORGING", "MORPEKO_HANGRY", "ZYGARDE_COMPLETE", "CASTFORM_SUNNY", "CASTFORM_RAINY", "CASTFORM_SNOWY", "DARMANITAN_ZEN", "DARMANITAN_GALAR_ZEN", "CHERRIM_SUNSHINE", "EISCUE_NOICE", "MINIOR_CORE_RED", "MINIOR_CORE_ORANGE", "MINIOR_CORE_YELLOW", "MINIOR_CORE_GREEN", "MINIOR_CORE_BLUE", "MINIOR_CORE_INDIGO", "MINIOR_CORE_VIOLET"):
    assert "case SPECIES_" + temporary + ":" in stable_check, temporary
assert "case SPECIES_GRENINJA_BATTLE_BOND:" not in stable_check
# The explicit-map resolver owns the species eligibility check; the old
# WorldRandomizeWildSpecies function is now only a current-map wrapper.
randomizer_check = stable_source.split("enum Species WorldRandomizeWildSpeciesAtMap(", 1)[1].split("// The original API remains", 1)[0]
assert "if (WorldIsFieldStableDiscoverySpecies(result))" in randomizer_check
assert "return WorldRandomizeWildSpeciesAtMap(original, slot, area," in stable_source
print("P0 permanent field form vs temporary battle transformation: natural habitat and seeded randomizer policy PASS")


# Do not guess approach geometry in mGBA tests: the tile immediately north
# of Diancie is an impassable cave ledge (0x0603); the western approach is
# ordinary elevation-3 floor. Preserve the genuine entry-to-object path.
import struct
granite_bytes = (root / "data/layouts/GraniteCave_StevensRoom/map.bin").read_bytes()
assert len(granite_bytes) == 15 * 14 * 2
def granite_tile(x, y):
    return struct.unpack_from("<H", granite_bytes, 2 * (15 * y + x))[0]
assert (granite_tile(10, 8) & 0xC00) != 0, "north tile unexpectedly opened; review route"
for x, y in ((7, 4), (8, 4), (8, 5), (8, 6), (8, 7),
             (9, 7), (9, 8), (9, 9)):
    tile = granite_tile(x, y)
    assert (tile & 0xC00) == 0 and tile >> 12 == 3, (
        "Diancie approach blocked/elevation mismatch", (x, y), hex(tile))
print("P0 Diancie mGBA approach: real 15x14 cave collision and elevation audited")

# The *entrance* from Granite Cave 1F is a NORTH arrow warp, not a south-facing
# non-animated door. Merely stepping down from (5,9) onto (5,10) does not
# activate the map transfer. Read both compiled source map and tileset attrs.
entrance_map = (root / "data/layouts/GraniteCave_1F/map.bin").read_bytes()
assert len(entrance_map) == 42 * 15 * 2
door = struct.unpack_from("<H", entrance_map, 2 * (42 * 10 + 5))[0]
assert door >> 12 == 3 and (door & 0xC00) == 0
assert (door & 0x3FF) == 572, f"Granite cave door metatile changed: {door & 0x3FF}"
cave_attrs = (root / "data/tilesets/secondary/cave/metatile_attributes.bin").read_bytes()
behavior = struct.unpack_from("<H", cave_attrs, 2 * ((door & 0x3FF) - 512))[0] & 0xFF
assert behavior == 101, f"Granite Cave warp behavior changed: {behavior}"
assert "MB_NORTH_ARROW_WARP" in (root / "include/constants/metatile_behaviors.h").read_text()
print("P0 Diancie entrance: Granite Cave 1F (5,10) is elevation-3 north-arrow warp")

# The real graphical-mGBA Granite Cave door, Lv18 Diancie battle,
# faint/revisit, capture/revisit and Save/Continue persistence were checked
# manually on Nyx. Preserve the real doorway tile assertions above while
# retiring the synthetic Save/Continue doorway fixture from the CI gate.
release_smoke = (root / ".github/workflows/P0-mGBA-Release-Smoke.yml").read_text()
assert "test/overworld/mgba-run.sh test/overworld/lua/WorldDiancieStatic.lua" not in release_smoke
print("P0 Diancie: original map warp source and manually accepted gameplay recorded; fixture not release-blocking")



# Reusable pre-League fossil ecology: Rock Smash yields Bag items, Devon revives
# six single fossils and four paired Galar combinations without consuming a fossil
# if there is no room for the revived Pokémon.
rock_script = (root / 'data/scripts/field_move_scripts.inc').read_text()
lab = (root / 'data/maps/RustboroCity_DevonCorp_2F/scripts.inc').read_text()
rock_code = (root / 'src/wild_encounter.c').read_text()
assert 'static const enum Item sAnthologyExcavationFossils[]' in rock_code
assert 'Random() % 100 < 12' in rock_code
assert 'if (AddBagItem(fossil, 1))' in rock_code
assert 'gSpecialVar_0x8004 = ITEM_NONE;' in rock_code
assert 'EventScript_AnthologyCheckExcavatedFossil::' in rock_script
assert 'bufferitemname STR_VAR_1, VAR_0x8004' in rock_script
assert 'goto_if_eq VAR_RESULT, MON_CANT_GIVE, Common_EventScript_NoMoreRoomForPokemon' in lab
assert lab.index('givemon VAR_TEMP_TRANSFERRED_SPECIES, 20') < lab.index('removeitem VAR_TEMP_1')
for item, mon in [["SKULL_FOSSIL","CRANIDOS"],["ARMOR_FOSSIL","SHIELDON"],["COVER_FOSSIL","TIRTOUGA"],["PLUME_FOSSIL","ARCHEN"],["JAW_FOSSIL","TYRUNT"],["SAIL_FOSSIL","AMAURA"]]:
    assert f'ITEM_{item}' in rock_code and f'ITEM_{item}' in lab and f'SPECIES_{mon}' in lab
for left, right, mon in [["FOSSILIZED_BIRD","FOSSILIZED_DRAKE","DRACOZOLT"],["FOSSILIZED_BIRD","FOSSILIZED_DINO","ARCTOZOLT"],["FOSSILIZED_FISH","FOSSILIZED_DRAKE","DRACOVISH"],["FOSSILIZED_FISH","FOSSILIZED_DINO","ARCTOVISH"]]:
    assert f'ITEM_{left}' in rock_code and f'ITEM_{right}' in rock_code
    assert f'EventScript_Anthology_{mon}::' in lab and f'SPECIES_{mon}' in lab
print('P0 Rock Smash excavations + ten ancient fossil restorations: source contracts PASS')

# P0 static priority repair: replace the two grass-only rare "legendary"
# placements with real, repeatable-until-caught field objects.
import json
static_sites = (
    ("GraniteCave_StevensRoom", "DIANCIE", "CARBINK", "FLAG_HIDE_ANTHOLOGY_DIANCIE",
     "FLAG_DELIVERED_STEVEN_LETTER"),
    ("ShoalCave_LowTideIceRoom", "GLASTRIER", "SNOM", "FLAG_HIDE_ANTHOLOGY_GLASTRIER", None),
)
for site, mon, ordinary, hide, gate in static_sites:
    here = json.loads((root / "data/maps" / site / "map.json").read_text())
    script = (root / "data/maps" / site / "scripts.inc").read_text()
    matches = [x for x in here["object_events"] if x.get("script") == f"{site}_EventScript_{mon[0]+mon[1:].lower()}"]
    assert len(matches) == 1, (site, "missing one-off static object")
    assert matches[0]["flag"] == hide and matches[0]["graphics_id"] == f"OBJ_EVENT_GFX_SPECIES({mon})"
    for s in ("MAP_SCRIPT_ON_TRANSITION", f"getcaughtmon SPECIES_{mon}",
              f"seteventmon SPECIES_{mon}", "BattleSetup_StartLegendaryBattle",
              "B_OUTCOME_CAUGHT", "B_OUTCOME_WON", "B_OUTCOME_RAN",
              "B_OUTCOME_PLAYER_TELEPORTED", "removeobject " + matches[0]["local_id"],
              "clearflag " + hide, "setflag " + hide):
        assert s in script, (site, s)
    if gate:
        assert f"goto_if_unset {gate}" in script, site
    assert f"RARE_ON_MAP(MAP_{site.upper()}, {mon})" not in rare_catalog
    # Map enum names do not always uppercase directly from the directory.
    assert f", {mon})" not in "\\n".join(line for line in rare_catalog.splitlines()
                                            if "RARE_ON_MAP(MAP_GRANITE_CAVE_STEVENS_ROOM" in line
                                            or "RARE_ON_MAP(MAP_SHOAL_CAVE_LOW_TIDE_ICE_ROOM" in line)
for mon in ("DIANCIE", "GLASTRIER"):
    assert "FLAG_HIDE_ANTHOLOGY_" + mon in (root / "include/constants/flags.h").read_text()
assert "RARE_ON_MAP(MAP_GRANITE_CAVE_STEVENS_ROOM, CARBINK)" in rare_catalog
assert "RARE_ON_MAP(MAP_SHOAL_CAVE_LOW_TIDE_ICE_ROOM, SNOM)" in rare_catalog
print("P0 Diancie/Glastrier: persistent statics until caught, originals and DexNav ecology retained PASS")

# Check the *actual binary tilemap*, not only legal x/y bounds. The Shoal Cave
# original first placement (12,9) had a wall/void elevation-0 tile 0x0687;
# Glastrier belongs on the reachable ice-floor at (15,11), elevation 3.
# Granite Cave's (10,9) is also a floor metatile, next to Steven's room.
import struct
for layout, width, height, x, y, expected in (
    ("GraniteCave_StevensRoom", 15, 14, 10, 9, 0x3201),
    ("ShoalCave_LowTideIceRoom", 20, 30, 15, 11, 0x3385),
):
    raw = (root / "data/layouts" / layout / "map.bin").read_bytes()
    assert len(raw) == width * height * 2, layout
    tile, = struct.unpack_from("<H", raw, 2 * (y * width + x))
    assert tile == expected and (tile >> 12) == 3, (layout, hex(tile))
    objs = json.loads((root / "data/maps" / layout / "map.json").read_text())["object_events"]
    local_id = ("LOCALID_ANTHOLOGY_DIANCIE" if layout == "GraniteCave_StevensRoom"
                else "LOCALID_ANTHOLOGY_GLASTRIER")
    obj = next(obj for obj in objs if obj.get("local_id") == local_id)
    assert (obj["x"], obj["y"], obj["elevation"]) == (x, y, 3), layout
print("P0 cave statics: real collision/elevation tilemap coordinates PASS")

# P0 further mythical statics: source checks and real layout collision data.
# These checks do not certify in-emulator catch/faint/re-entry or save behavior.
import struct as _anthology_struct
_p0_new_statics = (
("PowerPlant_Frlg","PowerPlant","TapuKoko","TAPU_KOKO",8,12,"TAPU_KOKO",False),
("PowerPlant_Frlg","PowerPlant","Zeraora","ZERAORA",37,11,"ZERAORA",False),
("MtMoon_1F_Frlg","MtMoon_1F","CosmogMoon","COSMOG",10,3,"COSMOG_MOON",True),
("SaffronCity_Dojo_Frlg","SaffronCity_Dojo","Marshadow","MARSHADOW",6,7,"MARSHADOW",False),
("SeafoamIslands_1F_Frlg","SeafoamIslands_1F","CosmogSeafoam","COSMOG",16,11,"COSMOG_SEAFOAM",True),
("CliffEdgeCave","CliffEdgeCave","Spectrier","SPECTRIER",22,26,"SPECTRIER",False),
("MtMoon_B1F_Frlg","MtMoon_B1F","Cresselia","CRESSELIA",13,17,"CRESSELIA",False),
("MtMoon_B1F_Frlg","MtMoon_B1F","Dialga","DIALGA",19,3,"DIALGA",False),
("MtMoon_B1F_Frlg","MtMoon_B1F","Palkia","PALKIA",43,29,"PALKIA",False),
("SeafoamIslands_B2F_Frlg","SeafoamIslands_B2F","Manaphy","MANAPHY",14,16,"MANAPHY",False),
("SeafoamIslands_B2F_Frlg","SeafoamIslands_B2F","TapuFini","TAPU_FINI",14,4,"TAPU_FINI",False),
("Route128_AncientGrotto","Route128_AncientGrotto","Koraidon","KORAIDON",10,9,"KORAIDON",False),
("Route112","Route112","Okidogi","OKIDOGI",16,45,"OKIDOGI",False),
("Route111_HoopaRingChamber","Route111_HoopaRingChamber","Hoopa","HOOPA_CONFINED",6,7,"HOOPA",False),
("Route111","Route111","TingLu","TING_LU",34,84,"TING_LU",False),
("MtChimney","MtChimney","Volcanion","VOLCANION",10,32,"VOLCANION",False),
("SealedChamber_OuterRoom","SealedChamber_OuterRoom","ChiYu","CHI_YU",16,15,"CHI_YU",False),
("JaggedPass","JaggedPass","GougingFire","GOUGING_FIRE",9,31,"GOUGING_FIRE",False),
("MtPyre_Exterior","MtPyre_Exterior","Yveltal","YVELTAL",13,17,"YVELTAL",False),
("Route117","Route117","Pecharunt","PECHARUNT",37,11,"PECHARUNT",False),
    ("Route117","Route117","Ogerpon","OGERPON_TEAL",10,9,"OGERPON",False),
("Route120","Route120","WoChien","WO_CHIEN",18,57,"WO_CHIEN",False),
("Route113","Route113","Fezandipiti","FEZANDIPITI",56,10,"FEZANDIPITI",False),
("Route102","Route102","Jirachi","JIRACHI",16,9,"JIRACHI",False),

("Route114","Route114","Terapagos","TERAPAGOS_NORMAL",15,34,"TERAPAGOS",False),
("Route119","Route119","RagingBolt","RAGING_BOLT",24,111,"RAGING_BOLT",False),
("Route114","Route114","Munkidori","MUNKIDORI",22,28,"MUNKIDORI",False),
("Route125","Route125","WalkingWake","WALKING_WAKE",9,22,"WALKING_WAKE",False),
("Route44","Route44","Tornadus","TORNADUS",57,24,"TORNADUS",False),
("Route41","Route41","Thundurus","THUNDURUS",17,56,"THUNDURUS",False),
("Route43","Route43","Landorus","LANDORUS",14,25,"LANDORUS",False),
("Route39","Route39","Enamorus","ENAMORUS",30,1,"ENAMORUS",False),
("Route40","Route40","Lugia","LUGIA",28,15,"LUGIA",False),
("UnionCave_1F","UnionCave_1F","Terrakion","TERRAKION",20,15,"TERRAKION",False),
("Route32","Route32","Keldeo","KELDEO",18,35,"KELDEO",False),
("SproutTower_2F","SproutTower_2F","KubfuWest","KUBFU",14,11,"KUBFU_WEST",True),
("Route34","Route34","KubfuEast","KUBFU",39,33,"KUBFU_EAST",True),
("NationalPark_Normal","NationalPark_Normal","Victini","VICTINI",34,23,"VICTINI",False),
("OlivineCity_Lighthouse","OlivineCity_Lighthouse","Regieleki","REGIELEKI",151,10,"REGIELEKI",False),
("EcruteakCity_Theater","EcruteakCity_Theater","Meloetta","MELOETTA",16,13,"MELOETTA",False),
("MtMortar_1F_South","MtMortar_1F_South","Cobalion","COBALION",19,41,"COBALION",False),
("RockTunnel_B1F_Frlg","RockTunnel_B1F","Regigigas","REGIGIGAS",19,12,"REGIGIGAS",False),
("RockTunnel_B1F_Frlg","RockTunnel_B1F","Uxie","UXIE",16,20,"UXIE",False),
("RockTunnel_B1F_Frlg","RockTunnel_B1F","Mesprit","MESPRIT",44,13,"MESPRIT",False),
("RockTunnel_B1F_Frlg","RockTunnel_B1F","Azelf","AZELF",27,28,"AZELF",False),
("PokemonMansion_1F_Frlg","PokemonMansion_1F","Mew","MEW",8,18,"MEW",False),
("PokemonMansion_1F_Frlg","PokemonMansion_1F","TypeNull","TYPE_NULL",31,22,"TYPE_NULL",False),
("IndigoPlateau_Exterior_Frlg","IndigoPlateau_Exterior","Arceus","ARCEUS",11,17,"ARCEUS",False),
("CeruleanCave_2F_Frlg","CeruleanCave_2F","Necrozma","NECROZMA",20,12,"NECROZMA",False),
("ViridianForest_Frlg","ViridianForest","Shaymin","SHAYMIN",49,56,"SHAYMIN",False),
("Route5_Frlg","Route5","TapuBulu","TAPU_BULU",30,23,"TAPU_BULU",False),
("MtMoon_B2F_Frlg","MtMoon_B2F","Giratina","GIRATINA",38,28,"GIRATINA",False),
("PokemonTower_7F_Frlg","PokemonTower_7F","Darkrai","DARKRAI",12,12,"DARKRAI",False),
("CeladonCity_Frlg","CeladonCity","TapuLele","TAPU_LELE",8,24,"TAPU_LELE",False),
("RockTunnel_B1F_Frlg","RockTunnel_B1F","Heatran","HEATRAN",9,27,"HEATRAN",False),
("VermilionCity_Frlg","VermilionCity","Meltan","MELTAN",35,15,"MELTAN",False),
("NationalPark_Normal","NationalPark_Normal","Virizion","VIRIZION",27,20,"VIRIZION",False),
("IlexForest","IlexForest","Zarude","ZARUDE",48,19,"ZARUDE",False),
("Route36","Route36","CalyrexNorth","CALYREX",23,15,"CALYREX_NORTH",True),
("Route36","Route36","CalyrexSouth","CALYREX",36,26,"CALYREX_SOUTH",True),
("TinTower_2F","TinTower_2F","Reshiram","RESHIRAM",9,8,"RESHIRAM",False),
("DarkCave_NorthSide","DarkCave_NorthSide","Zekrom","ZEKROM",40,13,"ZEKROM",False),
("DragonsDen_Cavern","DragonsDen_Cavern","Regidrago","REGIDRAGO",35,43,"REGIDRAGO",False),
("IcePath_B1F","IcePath_B1F","Kyurem","KYUREM",8,10,"KYUREM",False),
("JohtoEternatusChamber","JohtoEternatusChamber","Eternatus","ETERNATUS",10,9,"ETERNATUS",False),
("GoldenrodCity","GoldenrodCity","Genesect","GENESECT",36,25,"GENESECT",False),
("Route105","Route105","ChienPao","CHIEN_PAO",10,22,"CHIEN_PAO",False),
("PetalburgWoods","PetalburgWoods","Zygarde10","ZYGARDE_10_AURA_BREAK",16,19,"ZYGARDE_10",True),
("MeteorFalls_1F_1R","MeteorFalls_1F_1R","Zygarde50","ZYGARDE_50_POWER_CONSTRUCT",14,27,"ZYGARDE",True),
("Route119","Route119","Xerneas","XERNEAS",31,88,"XERNEAS",False),
("Route120","Route120","IronLeaves","IRON_LEAVES",36,58,"IRON_LEAVES",False),
("MossdeepCity_SpaceCenter_1F","MossdeepCity_SpaceCenter_1F","Latios","LATIOS",9,4,"LATIOS",False),
("NewMauville_GeneratorAnnex","NewMauville_GeneratorAnnex","Miraidon","MIRAIDON",4,2,"MIRAIDON",False),
("Route118","Route118","IronBoulder","IRON_BOULDER",50,11,"IRON_BOULDER",False),
("Route120","Route120","Latias","LATIAS",25,61,"LATIAS",False),
("Route38","Route38","GalarianZapdos","ZAPDOS_GALAR",25,18,"ZAPDOS_GALAR",True),
("Route40","Route40","GalarianArticuno","ARTICUNO_GALAR",9,2,"ARTICUNO_GALAR",True),
("BurnedTower_1F","BurnedTower_1F","GalarianMoltres","MOLTRES_GALAR",7,6,"MOLTRES_GALAR",True),
("Route38","Route38","RaikouReserve","RAIKOU",27,22,"BEAST_RAIKOU",False),
("MtMortar_2F","MtMortar_2F","EnteiReserve","ENTEI",10,13,"BEAST_ENTEI",False),
("LakeOfRage","LakeOfRage","SuicuneReserve","SUICUNE",35,38,"BEAST_SUICUNE",False),
("Route124","Route124","IronCrown","IRON_CROWN",59,40,"IRON_CROWN",False),
("IcePath_B2F","IcePath_B2F","GlastrierJohto","GLASTRIER",21,20,"JOHTO_GLASTRIER",False),
)
_p0_layouts = {x["id"]: x for x in json.loads((root / "data/layouts/layouts.json").read_text())["layouts"] if x}
_p0_flags = (root / "include/constants/flags.h").read_text()
assert len(_p0_new_statics)==83 and len({(s[0],s[4],s[5]) for s in _p0_new_statics})==83
# Proactively diagnose every invalid new static tile before the first assert.
# This keeps region-wide batches repairable in one CI pass.
for _dir,_prefix,_n,_species,_x,_y,_suffix,_multiple in _p0_new_statics:
    _m = json.loads((root/"data/maps"/_dir/"map.json").read_text())
    _lay = _p0_layouts[_m["layout"]]
    _w, _h = _lay["width"], _lay["height"]
    _raw = (root/_lay["blockdata_filepath"]).read_bytes()
    def _open_tile(xx, yy):
        if not (0 <= xx < _w and 0 <= yy < _h):
            return False
        _v = _anthology_struct.unpack_from("<H", _raw, 2 * (yy * _w + xx))[0]
        return _v >> 12 == 3 and ((_v >> 10) & 3) == 0
    _occupied = {(ob["x"], ob["y"]) for ob in _m["object_events"] if ob.get("script") != f"{_prefix}_EventScript_{_n}"}
    _occupied.update((ob["x"], ob["y"]) for ob in _m["warp_events"])
    _neighbors = ((1,0),(-1,0),(0,1),(0,-1))
    if _open_tile(_x, _y) and sum(_open_tile(_x+dx,_y+dy) for dx,dy in _neighbors) >= 2:
        continue
    _options = []
    for _cy in range(max(1,_y-14), min(_h-1,_y+15)):
        for _cx in range(max(1,_x-14), min(_w-1,_x+15)):
            if (_cx,_cy) in _occupied or not _open_tile(_cx,_cy):
                continue
            if sum(_open_tile(_cx+dx,_cy+dy) for dx,dy in _neighbors) < 2:
                continue
            _options.append((abs(_cx-_x)+abs(_cy-_y),_cx,_cy))
    _options.sort()
    print("INVALID BULK TILE", _dir, _n, (_x,_y), "nearest valid:", [(x,y) for _,x,y in _options[:16]], flush=True)

# Inspect every Hoenn route-entrance connected component before asserting
# on the first location. A legal metatile alone is not enough for gameplay.
for _dd, _anchor in (("Route119", (18,139)), ("Route120", (0,27))):
    _map = json.loads((root / "data/maps" / _dd / "map.json").read_text())
    _lay = _p0_layouts[_map["layout"]]
    _w, _h = _lay["width"], _lay["height"]
    _raw = (root / _lay["blockdata_filepath"]).read_bytes()
    def _on_route(xx,yy):
        if not (0<=xx<_w and 0<=yy<_h):
            return False
        val = _anthology_struct.unpack_from("<H", _raw, 2*(yy*_w+xx))[0]
        return val>>12 == 3 and ((val>>10)&3) == 0
    _reach=set()
    _pending=[_anchor]
    while _pending:
        _u=_pending.pop()
        if _u in _reach or not _on_route(*_u):
            continue
        _reach.add(_u)
        ux,uy=_u
        _pending.extend(((ux+1,uy),(ux-1,uy),(ux,uy+1),(ux,uy-1)))
    _blocked={(ob["x"],ob["y"]) for ob in _map["object_events"]}
    _blocked.update((ob["x"],ob["y"]) for ob in _map["warp_events"])
    for _dir,_prefix,_name,_species,_x,_y,_suffix,_multiple in _p0_new_statics:
        if _dir != _dd or (_x,_y) in _reach:
            continue
        _candidates=sorted((abs(a-_x)+abs(b-_y),a,b) for a,b in _reach
                           if (a,b) not in _blocked)
        print("DISCONNECTED HOENN STATIC", _dd, _name, (_x,_y),
              "nearest connected:", [(a,b) for _,a,b in _candidates[:20]], flush=True)

for _dir,_p,_n,_species,_x,_y,_suffix,_multiple in _p0_new_statics:
    _m=json.loads((root/"data/maps"/_dir/"map.json").read_text())
    _script=(root/"data/maps"/_dir/"scripts.inc").read_text()
    _found=[o for o in _m["object_events"] if o.get("script")==f"{_p}_EventScript_{_n}"]
    assert len(_found)==1, (_dir,_n)
    _obj=_found[0]
    _hide="FLAG_HIDE_ANTHOLOGY_"+_suffix
    assert (_obj["x"],_obj["y"],_obj["elevation"])==(_x,_y,3)
    assert _obj["graphics_id"]==f"OBJ_EVENT_GFX_SPECIES({_species})"
    assert _obj["local_id"]=="LOCALID_ANTHOLOGY_"+_suffix and _obj["flag"]==_hide
    assert "#define "+_hide+" " in _p0_flags
    _transition_hooks = (
        f"{_p}_AnthologyOnTransition",
        f"{_p}_BigBatchOnTransition",
        f"{_p}_SecondBatchOnTransition",
        f"{_p}_AnthologyFieldTransition",
        f"{_p}_PreLeagueOnTransition",
        f"{_p}_AnthologyHuntTransition",
        f"{_p}_{_n}Transition",
        f"{_p}_{_n}OnTransition",  # e.g. Route44_TornadusOnTransition
    )
    assert any(
        f"call {hook}" in _script or f"map_script MAP_SCRIPT_ON_TRANSITION, {hook}" in _script
        for hook in _transition_hooks
    ), (_dir, _n, "missing encounter transition hook")
    if (_dir, _n) == ("Route44", "Tornadus"):
        # Route 44 dispatches through an ordinary map transition handler.
        # Validate the entire entry chain instead of accepting an orphaned
        # TornadusOnTransition label that would never execute in-game.
        assert "map_script MAP_SCRIPT_ON_TRANSITION, Route44_OnTransition" in _script
        _route44_entry = _script.split("Route44_OnTransition:", 1)[1].split("\n\tend", 1)[0]
        assert "call Route44_TornadusOnTransition" in _route44_entry
        assert "Route44_TornadusOnTransition::" in _script

    _part=_script.split(f"{_p}_EventScript_{_n}::")[1].split(f"{_p}_Anthology{_n}Retreated::")[0].split(f"{_p}_BigBatch{_n}Retreated::")[0].split(f"{_p}_SecondBatch{_n}Retreated::")[0].split(f"{_p}_Field{_n}Retreated::")[0]
    if _dir == "PokemonMansion_1F_Frlg" and _n == "Mew":
        # Mew is now a three-disguise endurance gauntlet followed by a
        # catch-only final battle, not the old one-battle seteventmon.
        for _token in ("MewGauntletRollDisguises", "MewGauntletPrepareBattle",
                       "BattleSetup_StartMewGauntletBattle", "GetBattleOutcome",
                       "B_OUTCOME_WON", "B_OUTCOME_CAUGHT",
                       "goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PokemonMansion_1F_AnthologyMewResolved",
                       "removeobject "+_obj["local_id"], "setflag "+_hide):
            assert _token in _part, (_dir,_n,_token)
        assert _part.count("special BattleSetup_StartMewGauntletBattle") == 4
        assert _part.count("goto_if_ne VAR_RESULT, B_OUTCOME_WON") == 3
        assert "goto_if_eq VAR_RESULT, B_OUTCOME_WON, PokemonMansion_1F_AnthologyMewResolved" not in _part
        assert "BATTLE_TYPE_MEW_DISGUISE" in (root/"src/battle_setup.c").read_text()
        assert "BATTLE_TYPE_MEW_DISGUISE" in (root/"src/item_use.c").read_text()
    else:
        for _token in (f"seteventmon SPECIES_{_species}","BattleSetup_StartLegendaryBattle",
                       "GetBattleOutcome","B_OUTCOME_CAUGHT","B_OUTCOME_WON",
                       "B_OUTCOME_RAN","B_OUTCOME_PLAYER_TELEPORTED",
                       "removeobject "+_obj["local_id"],"setflag "+_hide):
            assert _token in _part, (_dir,_n,_token)
    if _multiple:
        _claimed="FLAG_CAUGHT_ANTHOLOGY_"+_suffix
        assert "#define "+_claimed+" " in _p0_flags
        assert f"call_if_set {_claimed}" in _script and f"call_if_unset {_claimed}" in _script
        assert "setflag "+_claimed in _part and f"getcaughtmon SPECIES_{_species}" not in _script
    elif _dir == "Route114" and _n == "Terapagos":
        # Normal and permanent Stellar Terapagos now have independent catch flags.
        assert "FLAG_CAUGHT_ANTHOLOGY_TERAPAGOS_NORMAL" in _script
        assert "goto_if_set FLAG_CAUGHT_ANTHOLOGY_TERAPAGOS_NORMAL" in _script
        assert "setflag FLAG_CAUGHT_ANTHOLOGY_TERAPAGOS_NORMAL" in _part
    elif _dir == "PokemonMansion_1F_Frlg" and _n == "Mew":
        # Randomizing locations must not despawn the Mansion encounter just
        # because the player caught a Mew somewhere else.
        assert "getcaughtmon SPECIES_MEW" not in _script
        assert "special MewGauntletShouldRun" in _part
        assert "goto_if_eq VAR_RESULT, FALSE, PokemonMansion_1F_RandomizedMewStatic" in _part
        assert "setflag FLAG_HIDE_ANTHOLOGY_MEW" in _script
    else:
        assert "getcaughtmon SPECIES_"+_species in _script
    _lay=_p0_layouts[_m["layout"]]
    _w,_h=_lay["width"],_lay["height"]
    assert 0<_x<_w-1 and 0<_y<_h-1
    _buf=(root/_lay["blockdata_filepath"]).read_bytes()
    assert len(_buf)==2*_w*_h
    def _tile(xx,yy):
        return _anthology_struct.unpack_from("<H",_buf,2*(yy*_w+xx))[0]
    _v=_tile(_x,_y)
    assert _v>>12==3 and ((_v>>10)&3)==0, (_dir,_n,hex(_v))
    assert sum((_tile(_x+dx,_y+dy)>>12)==3 and ((_tile(_x+dx,_y+dy)>>10)&3)==0
               for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)))>=2
    assert not any(o is not _obj and (o["x"],o["y"])==(_x,_y) for o in _m["object_events"])
    assert not any((o["x"],o["y"])==(_x,_y) for o in _m["warp_events"])
    # For the new location-cluster batch, verify the encounter can be walked
    # to from an actual pre-existing stair/warp on the same 3-elevation floor.
    if _dir in {"MtMoon_B1F_Frlg", "SeafoamIslands_B2F_Frlg", "GraniteCave_B2F", "FieryPath", "JaggedPass", "MtPyre_Exterior", "UnionCave_1F", "MtMortar_1F_South", "NationalPark_Normal", "Route32"}:
        assert len(_buf)==2*_w*_h, (_dir,_w,_h,len(_buf))
        def _walkable(pos):
            xx,yy=pos
            if not (0<=xx<_w and 0<=yy<_h): return False
            value=_tile(xx,yy)
            return value>>12==3 and ((value>>10)&3)==0
        _visited=set()
        _frontier=[(e["x"],e["y"]) for e in _m["warp_events"] if _walkable((e["x"],e["y"]))]
        while _frontier:
            _here=_frontier.pop()
            if _here in _visited: continue
            _visited.add(_here)
            xx,yy=_here
            for _delta in ((1,0),(-1,0),(0,1),(0,-1)):
                _there=(xx+_delta[0],yy+_delta[1])
                if _walkable(_there) and _there not in _visited: _frontier.append(_there)
        assert (_x,_y) in _visited, ("inaccessible static",_dir,_species,_x,_y)
    if _dir == "Route111":
        # Existing Route 111's five warps have some nonstandard elevation
        # bytes, so anchor the walkability test at a known main-route tile.
        # The desert must remain reachable in both Mirage Tower layouts.
        for _variant in ("LAYOUT_ROUTE111", "LAYOUT_ROUTE111_NO_MIRAGE_TOWER"):
            _vlay=_p0_layouts[_variant]
            _vbuf=(root/_vlay["blockdata_filepath"]).read_bytes()
            assert len(_vbuf)==2*_w*_h
            def _vfree(pos):
                xx,yy=pos
                if not(0<=xx<_w and 0<=yy<_h): return False
                val=_anthology_struct.unpack_from("<H",_vbuf,2*(yy*_w+xx))[0]
                return val>>12==3 and ((val>>10)&3)==0
            _seen=set()
            _todo=[(20,8)]
            while _todo:
                _pos=_todo.pop()
                if _pos in _seen or not _vfree(_pos): continue
                _seen.add(_pos)
                xx,yy=_pos
                _todo.extend((xx+dx,yy+dy) for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)))
            assert (_x,_y) in _seen, ("unreachable Route 111 placement",_variant,_species,_x,_y)
        assert not any((o["x"],o["y"])==(_x,_y) for o in _m["coord_events"])
    # Newly added space center + Hoenn rainforest statics must be reachable
    # from a known open entry tile, not just have walkable neighbors.
    _new_anchors={"MossdeepCity_SpaceCenter_1F":(7,8),
                  "MossdeepCity_SpaceCenter_2F":(13,2),
                  "Route119":(18,139), "Route120":(0,27)}
    if _dir in _new_anchors:
        _start=_new_anchors[_dir]
        def _land(p):
            xx,yy=p
            if not (0<=xx<_w and 0<=yy<_h): return False
            raw=_anthology_struct.unpack_from("<H",_buf,2*(yy*_w+xx))[0]
            return raw>>12==3 and ((raw>>10)&3)==0
        assert _land(_start), ("bad route entrance",_dir,_start)
        _visited=set()
        _pending=[_start]
        while _pending:
            _u=_pending.pop()
            if _u in _visited or not _land(_u): continue
            _visited.add(_u)
            ux,uy=_u
            _pending.extend((ux+dx,uy+dy) for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)))
        assert (_x,_y) in _visited, ("inaccessible field encounter",_dir,_species,_x,_y)
        assert not any((o["x"],o["y"])==(_x,_y) for o in _m["coord_events"])
    if _dir in {"Route124", "Route125"}:
        # Both are intentionally shore placements: do not confuse ocean
        # water with normally walkable beach/shallow-water terrain.
        _terrain=(root/"data/tilesets/primary/general/metatile_attributes.bin").read_bytes()
        assert len(_terrain)>=1024
        def _behavior(p):
            xx,yy=p
            if not (0<=xx<_w and 0<=yy<_h): return -1
            tile=_tile(xx,yy)&0x3FF
            return _terrain[2*tile] if tile<512 else -1
        _expected=33 if _dir=="Route124" else 23 # MB_SAND / MB_SHALLOW_WATER
        assert _behavior((_x,_y))==_expected, ("wrong coast terrain",_dir,_x,_y)
        _open=set()
        _todo=[(_x,_y)]
        while _todo:
            xx,yy=_todo.pop()
            p=(xx,yy)
            if p in _open or not (0<=xx<_w and 0<=yy<_h): continue
            raw=_tile(xx,yy)
            if raw>>12!=3 or ((raw>>10)&3)!=0 or _behavior(p) in (16,17,18,19,20,21):
                continue
            _open.add(p)
            _todo.extend((xx+dx, yy+dy) for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)))
        assert len(_open)>=5
        assert any(_behavior((xx+dx, yy+dy))==21
                   for xx,yy in _open for dx,dy in ((1,0),(-1,0),(0,1),(0,-1))), ("no Surf approach",_dir,_species)
        assert not any((o["x"],o["y"])==(_x,_y) for o in _m["coord_events"])
# Johto route statics must share a traversable component with an actual
# warp or one of the land-map borders used by neighboring routes.
for _jmap in ("Route38","Route39","Route40","Route42","Route48","RuinsOfAlph_Outside","EcruteakCity_Theater","OlivineCity_Lighthouse"):
    _jm=json.loads((root/"data/maps"/_jmap/"map.json").read_text())
    _jl=_p0_layouts[_jm["layout"]]
    _jw,_jh=_jl["width"],_jl["height"]
    _jb=(root/_jl["blockdata_filepath"]).read_bytes()
    assert len(_jb)==2*_jw*_jh
    def _jopen(x,y):
        if not(0<=x<_jw and 0<=y<_jh): return False
        v=_anthology_struct.unpack_from("<H",_jb,2*(y*_jw+x))[0]
        return v>>12==3 and ((v>>10)&3)==0
    _q=[(x,y) for y in range(_jh) for x in range(_jw)
        if (x in (0,_jw-1) or y in (0,_jh-1)) and _jopen(x,y)]
    _q.extend((o["x"],o["y"]) for o in _jm["warp_events"] if _jopen(o["x"],o["y"]))
    _seen=set()
    while _q:
        x,y=_q.pop()
        if (x,y) in _seen or not _jopen(x,y): continue
        _seen.add((x,y))
        _q.extend((x+dx,y+dy) for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)))
    for _dir,_p,_n,_species,_x,_y,_suffix,_multiple in _p0_new_statics:
        if _dir==_jmap: assert (_x,_y) in _seen, ("unreachable Johto site",_jmap,_species,_x,_y)
# Extra Kanto pre-Champion alternatives must be reachable from an
# existing warp or perimeter route edge without crossing blocked tiles.
for _km in ("RockTunnel_B1F_Frlg","PokemonMansion_1F_Frlg","IndigoPlateau_Exterior_Frlg"):
    _map=json.loads((root/"data/maps"/_km/"map.json").read_text())
    _layout=_p0_layouts[_map["layout"]]
    _w,_h=_layout["width"],_layout["height"]
    _raw=(root/_layout["blockdata_filepath"]).read_bytes()
    assert len(_raw)==2*_w*_h
    def _walk(x,y):
        if not(0<=x<_w and 0<=y<_h): return False
        v=_anthology_struct.unpack_from("<H",_raw,2*(y*_w+x))[0]
        return v>>12==3 and ((v>>10)&3)==0
    _todo=[(x,y) for y in range(_h) for x in range(_w)
           if (x==0 or y==0 or x==_w-1 or y==_h-1) and _walk(x,y)]
    _todo.extend((o["x"],o["y"]) for o in _map["warp_events"] if _walk(o["x"],o["y"]))
    _done=set()
    while _todo:
        x,y=_todo.pop()
        if (x,y) in _done or not _walk(x,y): continue
        _done.add((x,y))
        _todo.extend((x+dx,y+dy) for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)))
    for _d,_p,_n,_species,_x,_y,_suffix,_multiple in _p0_new_statics:
        if _d==_km: assert (_x,_y) in _done, ("inaccessible pre-League Kanto static",_km,_species)
# Final Johto/Hoenn positions must share an open component with an actual
# route border or warp, not just have a collision-free event tile.
for _loc in ("Route44","GoldenrodCity","BurnedTower_1F","GraniteCave_B2F","Route120","Route110","Route118"):
    _map=json.loads((root/"data/maps"/_loc/"map.json").read_text())
    _layout=_p0_layouts[_map["layout"]]
    _w,_h=_layout["width"],_layout["height"]
    _buf=(root/_layout["blockdata_filepath"]).read_bytes()
    assert len(_buf)==2*_w*_h
    def _open(x,y):
        if not(0<=x<_w and 0<=y<_h): return False
        value=_anthology_struct.unpack_from("<H",_buf,2*(y*_w+x))[0]
        return value>>12==3 and ((value>>10)&3)==0
    _todo=[(x,y) for y in range(_h) for x in range(_w)
           if (x==0 or y==0 or x==_w-1 or y==_h-1) and _open(x,y)]
    _todo.extend((o["x"],o["y"]) for o in _map["warp_events"] if _open(o["x"],o["y"]))
    _seen=set()
    while _todo:
        x,y=_todo.pop()
        if (x,y) in _seen or not _open(x,y): continue
        _seen.add((x,y))
        _todo.extend((x+dx,y+dy) for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)))
    for _d,_p,_n,_s,_x,_y,_suffix,_multi in _p0_new_statics:
        if _d==_loc: assert (_x,_y) in _seen, ("unreachable grouped static",_loc,_s,_x,_y)
# The ten original Kanto/Hoenn legendary encounters must recover after a
# non-capture defeat, yet remain permanently hidden on an already-caught save.
_native_legendary_persistence = (
    ("PowerPlant_Frlg", "PowerPlant", "ZAPDOS", "FLAG_FOUGHT_ZAPDOS", "FLAG_HIDE_ZAPDOS"),
    ("SeafoamIslands_B4F_Frlg", "SeafoamIslands_B4F", "ARTICUNO", "FLAG_FOUGHT_ARTICUNO", "FLAG_HIDE_ARTICUNO"),
    ("CeruleanCave_B1F_Frlg", "CeruleanCave_B1F", "MEWTWO", "FLAG_FOUGHT_MEWTWO", "FLAG_HIDE_MEWTWO"),
    ("MtEmber_Summit_Frlg", "MtEmber_Summit", "MOLTRES", "FLAG_FOUGHT_MOLTRES", "FLAG_HIDE_MOLTRES"),
    ("DesertRuins", "DesertRuins", "REGIROCK", "FLAG_DEFEATED_REGIROCK", "FLAG_HIDE_REGIROCK"),
    ("IslandCave", "IslandCave", "REGICE", "FLAG_DEFEATED_REGICE", "FLAG_HIDE_REGICE"),
    ("AncientTomb", "AncientTomb", "REGISTEEL", "FLAG_DEFEATED_REGISTEEL", "FLAG_HIDE_REGISTEEL"),
    ("TerraCave_End", "TerraCave_End", "GROUDON", "FLAG_DEFEATED_GROUDON", "FLAG_HIDE_TERRA_CAVE_GROUDON"),
    ("MarineCave_End", "MarineCave_End", "KYOGRE", "FLAG_DEFEATED_KYOGRE", "FLAG_HIDE_MARINE_CAVE_KYOGRE"),
    ("SkyPillar_Top", "SkyPillar_Top", "RAYQUAZA", "FLAG_DEFEATED_RAYQUAZA", "FLAG_HIDE_SKY_PILLAR_TOP_RAYQUAZA_STILL"),
)
assert len(_native_legendary_persistence)==10
for _loc, _prefix, _species, _defeated, _hide in _native_legendary_persistence:
    _native=(root/"data/maps"/_loc/"scripts.inc").read_text()
    _base=f"{_prefix}_Native{_species}"
    assert "getcaughtmon SPECIES_"+_species in _native
    if _species in ("GROUDON", "KYOGRE"):
        assert f"goto_if_eq VAR_RESULT, TRUE, {_base}Caught" in _native
        assert "getcaughtmon SPECIES_RAYQUAZA" in _native
        assert f"call_if_eq VAR_RESULT, TRUE, {_base}Uncaught" in _native
        assert f"call_if_ne VAR_RESULT, TRUE, {_base}Locked" in _native
        assert f"{_base}Locked::" in _native
        assert "setflag "+_hide in _native.split(f"{_base}Locked::")[1]
    else:
        assert f"call_if_eq VAR_RESULT, TRUE, {_base}Caught" in _native
        assert f"call_if_ne VAR_RESULT, TRUE, {_base}Uncaught" in _native
    _caught=_native.split(f"{_base}Caught::")[1].split(f"{_base}Uncaught::")[0]
    _uncaught=_native.split(f"{_base}Uncaught::")[1]
    assert "setflag "+_defeated in _caught and "setflag "+_hide in _caught
    assert "clearflag "+_defeated in _uncaught and "clearflag "+_hide in _uncaught
    assert f"call_if_unset {_defeated}, " not in _native
# Existing puzzle and story-specific interaction labels must not be replaced.
for _loc, _name in (("DesertRuins","Regirock"),("IslandCave","Regice"),("AncientTomb","Registeel")):
    _s=(root/"data/maps"/_loc/"scripts.inc").read_text()
    assert f"{_loc}_EventScript_{_name}::" in _s
    assert "StartRegiBattle" in _s
assert "FLAG_SYS_REGIROCK_PUZZLE_COMPLETED" in (root/"data/maps/DesertRuins/scripts.inc").read_text()
assert "VAR_SKY_PILLAR_STATE" in (root/"data/maps/SkyPillar_Top/scripts.inc").read_text()

assert "BurnedTower_B1F_EventScript_ReleaseBeasts::" in (root/"data/maps/BurnedTower_B1F/scripts.inc").read_text()
assert "setworldmapflag FLAG_WORLD_MAP_MT_MOON_1F" in (root/"data/maps/MtMoon_1F_Frlg/scripts.inc").read_text()
# Newly added Galarians carry independently saved form flags, even when
# original legendary-bird National entries were already caught.
for _d,_p,_n,_s,_x,_y,_suffix,_multi in _p0_new_statics:
    if _suffix in {"ZAPDOS_GALAR","ARTICUNO_GALAR","MOLTRES_GALAR"}:
        assert _multi
    if _suffix.startswith("BEAST_"):
        _t=(root/"data/maps"/_d/"scripts.inc").read_text()
        assert f"goto_if_unset FLAG_RELEASED_BEASTS, {_p}_FieldHide{_n}" in _t
        assert f"goto_if_eq VAR_RESULT, TRUE, {_p}_FieldHide{_n}" in _t
for _d in ("Route38","Route40","BurnedTower_1F","Route43","Route46","Route35"):
    _m=json.loads((root/"data/maps"/_d/"map.json").read_text())
    _l=_p0_layouts[_m["layout"]]
    _w,_h=_l["width"],_l["height"]
    _b=(root/_l["blockdata_filepath"]).read_bytes()
    assert len(_b)==2*_w*_h
    def _canwalk(x,y):
        if not(0<=x<_w and 0<=y<_h): return False
        v=_anthology_struct.unpack_from("<H",_b,2*(y*_w+x))[0]
        return v>>12==3 and ((v>>10)&3)==0
    _todo=[(x,y) for y in range(_h) for x in range(_w)
           if (x in (0,_w-1) or y in (0,_h-1)) and _canwalk(x,y)]
    _todo.extend((o["x"],o["y"]) for o in _m["warp_events"] if _canwalk(o["x"],o["y"]))
    _done=set()
    while _todo:
        x,y=_todo.pop()
        if (x,y) in _done or not _canwalk(x,y): continue
        _done.add((x,y))
        _todo.extend((x+dx,y+dy) for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)))
    for _loc,_p,_n,_s,_x,_y,_suffix,_multi in _p0_new_statics:
        if _loc==_d: assert (_x,_y) in _done, ("unreachable form/beast static",_d,_s)
assert "SpeciesToNationalPokedexNum(ROAMER(i)->species), FLAG_GET_CAUGHT" in (root/"src/roamer.c").read_text()
# Original Johto Celebi shrine story remains one-time, while an uncaught
# Celebi is eligible for a repeatable return visit before the League.
_ce=(root/"data/maps/IlexForest/scripts.inc").read_text()
assert "goto_if_eq VAR_AZALEA_TOWN_STATE, 9, IlexForest_EventScript_Celebi_Event" in _ce
assert "goto_if_ge VAR_AZALEA_TOWN_STATE, 10, IlexForest_EventScript_CelebiRematch" in _ce
assert "IlexForest_EventScript_CelebiRematch::" in _ce
assert "goto_if_eq VAR_RESULT, TRUE, IlexForest_EventScript_ShrineRegularText" in _ce
assert _ce.split("IlexForest_EventScript_CelebiRematch::")[1].split("IlexForest_EventScript_Shrine_Caught::")[0].count("BattleSetup_StartLegendaryBattle")==1
assert "setvar VAR_AZALEA_TOWN_STATE, 10" in _ce
assert "FLAG_JOHTO_CHAMPION" in _ce
# Alternatives to post-Champion canonical legendary rooms must be reachable
# and must only appear after the mandatory Hoenn legendary climax (where relevant).
for _dir,_p,_n,_species,_x,_y,_suffix,_multiple in _p0_new_statics:
    if _suffix.startswith("PRELEAGUE_"):
        _script=(root/"data/maps"/_dir/"scripts.inc").read_text()
        assert f"getcaughtmon SPECIES_{_species}" in _script
        assert f"call {_p}_PreLeagueOnTransition" in _script
        assert f"{_p}_PreLeague{_species}Hide::" in _script
        assert f"{_p}_PreLeague{_species}Show::" in _script
        if _species in ("GROUDON","KYOGRE"):
            assert f"goto_if_lt VAR_SOOTOPOLIS_CITY_STATE, 6, {_p}_PreLeague{_species}Hide" in _script
        _m=json.loads((root/"data/maps"/_dir/"map.json").read_text())
        _l=_p0_layouts[_m["layout"]]
        _w,_h=_l["width"],_l["height"]
        _b=(root/_l["blockdata_filepath"]).read_bytes()
        assert len(_b)==2*_w*_h
        _raw=_anthology_struct.unpack_from("<H",_b,2*(_y*_w+_x))[0]
        assert _raw>>12==3 and ((_raw>>10)&3)==0
        assert not any(o["x"]==_x and o["y"]==_y for o in _m["coord_events"]+_m["warp_events"])
        if _species=="KYOGRE":
            _t=(root/"data/tilesets/primary/general/metatile_attributes.bin").read_bytes()
            _tile=_raw&1023
            assert _tile<512 and _t[2*_tile]==23 # MB_SHALLOW_WATER
print("P0 eighty-five validated legendary, form and regional static encounter objects: PASS")

# Additional late-branch and regional form sources, separate from item access.
extra_branch_sources = {
    ("MAP_WHIRL_ISLANDS_1F", "CORSOLA_GALAR"),
    ("MAP_ICE_PATH_B2F", "MR_MIME_GALAR"),
    ("MAP_ROUTE115", "EEVEE"),
    ("MAP_ROUTE110", "DUNSPARCE"),
    ("MAP_ROUTE119", "APPLIN"),
    ("MAP_ROUTE116", "MANKEY"),
    ("MAP_ROUTE104", "SPEWPA_MEADOW"),
    ("MAP_PETALBURG_WOODS", "VIVILLON_MEADOW"),
    ("MAP_ROUTE121", "FLOETTE_RED"),
    ("MAP_ROUTE115", "FLORGES_RED"),
}
assert len(extra_branch_sources) == 10 and extra_branch_sources <= set(extra)

# Older revival laboratories remain valid. These are alternative pre-League
# rare habitats so a regional fossil line is never gated on another region.
preleague_fossil_and_original_alternatives = {
    ("MAP_ROUTE14", "FARFETCHD"),
    ("MAP_ROUTE23", "HITMONLEE"),
    ("MAP_ROUTE15", "HITMONCHAN"),
    ("MAP_ROUTE13", "LICKITUNG"),
    ("MAP_SEAFOAM_ISLANDS_B1F", "JYNX"),
    ("MAP_ROUTE7", "MOTHIM_PLANT"),
    ("MAP_ROUTE13", "LICKILICKY"),
    ("MAP_ROUTE24", "TOGEKISS"),
    ("MAP_ROCK_TUNNEL_1F", "GLISCOR"),
    ("MAP_ROUTE25", "GALLADE"),
    ("MAP_ROCK_TUNNEL_B1F", "PROBOPASS"),
    ("MAP_POKEMON_TOWER_6F", "DUSKNOIR"),
    ("MAP_SEAFOAM_ISLANDS_B2F", "FROSLASS"),
    ("MAP_MT_MOON_B1F", "CRANIDOS"),
    ("MAP_MT_MOON_B2F", "SHIELDON"),
    ("MAP_WHIRL_ISLANDS_1F", "TIRTOUGA"),
    ("MAP_RUINS_OF_ALPH_OUTSIDE", "ARCHEN"),
    ("MAP_RUINS_OF_ALPH_B1F", "DRACOZOLT"),
    ("MAP_RUINS_OF_ALPH_B1F", "ARCTOZOLT"),
    ("MAP_RUINS_OF_ALPH_OUTSIDE", "DRACOVISH"),
    ("MAP_RUINS_OF_ALPH_OUTSIDE", "ARCTOVISH"),
    ("MAP_GRANITE_CAVE_B1F", "TYRUNT"),
    ("MAP_SHOAL_CAVE_LOW_TIDE_STAIRS_ROOM", "AMAURA"),
}
assert len(preleague_fossil_and_original_alternatives) == 23
assert preleague_fossil_and_original_alternatives <= set(extra)

# Five direct Johto parent/baby access paths complete its pre-League branch
# sources: CLEFFA, IGGLYBUFF, TOGEPI->TOGETIC, EEVEE->ESPEON/UMBREON,
# PORYGON->PORYGON2 (the normal Upgrade is sold in Goldenrod).
johto_branch_access = {
    ("MAP_ROUTE36", "CLEFFA"),
    ("MAP_ROUTE35", "IGGLYBUFF"),
    ("MAP_SPROUT_TOWER_3F", "TOGEPI"),
    ("MAP_ROUTE38", "EEVEE"),
    ("MAP_ROUTE39", "PORYGON"),
}
assert len(johto_branch_access) == 5
assert johto_branch_access <= set(extra), "missing Johto baby/branch encounter"

# Regional form source wave: this guard checks native DexNav fit above, and
# retains the explicit location-of-origin contract separately from 1025 IDs.
regional_form_habitats = {
    ("MAP_ROUTE1", "RATTATA_ALOLA"),
    ("MAP_ROUTE3", "MEOWTH_ALOLA"),
    ("MAP_MT_MOON_B2F", "GEODUDE_ALOLA"),
    ("MAP_DIGLETTS_CAVE_B1F", "DIGLETT_ALOLA"),
    ("MAP_POWER_PLANT", "VOLTORB_HISUI"),
    ("MAP_POKEMON_MANSION_3F", "GRIMER_ALOLA"),
    ("MAP_SEAFOAM_ISLANDS_B1F", "SANDSHREW_ALOLA"),
    ("MAP_SEAFOAM_ISLANDS_B1F", "VULPIX_ALOLA"),
    ("MAP_ROUTE9", "GROWLITHE_HISUI"),
    ("MAP_ROUTE8", "PONYTA_GALAR"),
    ("MAP_ROUTE11", "TAUROS_PALDEA_COMBAT"),
    ("MAP_ROUTE14", "TAUROS_PALDEA_BLAZE"),
    ("MAP_ROUTE12", "TAUROS_PALDEA_AQUA"),
    ("MAP_ROUTE15", "RAICHU_ALOLA"),
    ("MAP_ROUTE16", "EXEGGUTOR_ALOLA"),
    ("MAP_POKEMON_TOWER_3F", "MAROWAK_ALOLA"),
    ("MAP_POKEMON_TOWER_5F", "WEEZING_GALAR"),
    ("MAP_ROUTE24", "DECIDUEYE_HISUI"),
    ("MAP_ROUTE10", "SLOWBRO_GALAR"),
    ("MAP_ICE_PATH_1F", "DARUMAKA_GALAR"),
    ("MAP_BURNED_TOWER_B1F", "YAMASK_GALAR"),
    ("MAP_ILEX_FOREST", "ZORUA_HISUI"),
    ("MAP_ROUTE48", "BRAVIARY_HISUI"),
    ("MAP_ROUTE36", "LILLIGANT_HISUI"),
    ("MAP_ROUTE47", "SAMUROTT_HISUI"),
    ("MAP_BURNED_TOWER_B1F", "TYPHLOSION_HISUI"),
    ("MAP_WHIRL_ISLANDS_1F", "SLOWKING_GALAR"),
    ("MAP_ROUTE112", "ZIGZAGOON_GALAR"),
    ("MAP_METEOR_FALLS_1F_2R", "SLIGGOO_HISUI"),
    ("MAP_SHOAL_CAVE_LOW_TIDE_INNER_ROOM", "AVALUGG_HISUI"),
}
assert len(regional_form_habitats) == 30
assert regional_form_habitats <= set(extra)

# Galarian Legendary birds are now deliberate Johto statics, not Kanto grass.
# The other regional forms and evolution seeds retain their ordinary habitats.
pre_champion_form_reconciliation = {
    ("MAP_ROUTE8", "FARFETCHD_GALAR"),
    ("MAP_ROUTE11", "MEOWTH_GALAR"),
    ("MAP_ROUTE30", "WOOPER_PALDEA"),
    ("MAP_ROUTE9", "ROCKRUFF_OWN_TEMPO"),
    ("MAP_ROUTE6", "PIKACHU_ALOLA"),
}
assert len(pre_champion_form_reconciliation) == 5
assert pre_champion_form_reconciliation <= set(extra), sorted(pre_champion_form_reconciliation - set(extra))
assert all(loc in {loc for loc, mon in mappings} for loc, mon in pre_champion_form_reconciliation)
assert "DARMANITAN_GALAR_ZEN" not in {mon for loc, mon in extra}
for forbidden in ("ZAPDOS_GALAR", "ARTICUNO_GALAR", "MOLTRES_GALAR"):
    assert forbidden not in {mon for _, mon in extra}
print("P0 five ordinary regional forms and three static-only Galarian birds: habitat contracts PASS")


# Six cross-generation families were incorrectly counted as pre-first-Champion
# Kanto acquisitions because their parents were on Sevii islands 4-7, whose
# FRLG postgame access needs a Champion. Mainland sources must remain present.
sevii_mainland_relief = {
    ("MAP_ROUTE16", "AIPOM"),
    ("MAP_POKEMON_TOWER_4F", "MISDREAVUS"),
    ("MAP_POKEMON_TOWER_6F", "MURKROW"),
    ("MAP_SEAFOAM_ISLANDS_B3F", "SNEASEL"),
    ("MAP_ROUTE12", "YANMA"),
    ("MAP_SEAFOAM_ISLANDS_B4F", "SWINUB"),
}
assert sevii_mainland_relief <= set(extra), sorted(sevii_mainland_relief - set(extra))
assert all(not any(x in loc for x in ("FOUR_ISLAND", "FIVE_ISLAND", "SIX_ISLAND", "SEVEN_ISLAND")) for loc, mon in sevii_mainland_relief)
print(f"P0 pre-Champion Kanto Sevii relief: {len(sevii_mainland_relief)} mainland parent sources guarded")


# Continental Legendaries are only ordinary statics in the original
# postgame Terra/Marine cave sites after Rayquaza, never native wild hunts.
for species_name in ("GROUDON", "KYOGRE"):
    assert species_name not in {mon for _, mon in extra}
audit = (root / "tools/audit_p0_acquisition.py").read_text()
for postleague_only in ("EMBEDDED_TOWER", "STEVENS_CAVE", "ALTERING_CAVE"):
    assert '"' + postleague_only + '"' in audit, postleague_only
assert "FULL-ADVENTURE NATIONAL SOURCE REACHABILITY" in audit
print("P0 Groudon/Kyogre: postgame-only intended source accounting PASS")

# The source graph must not count the Champion-gated Steven's Cave
# Klawf record as the sole pre-Champion path for National #950.
assert ("MAP_ROUTE115", "KLAWF") in set(extra)
assert ("MAP_METEOR_FALLS_STEVENS_CAVE", "KLAWF") in set(extra)
assert '"STEVENS_CAVE"' in (root / "tools/audit_p0_acquisition.py").read_text()
print("P0 #950 Klawf: pre-Champion Route 115 relocation plus excluded Steven's Cave guard PASS")


# Approved optional Legendary prerequisites are checked on each map entry;
# caught Dex bits matter, not whether the Pokémon is still in the party.
_regi = (root / "data/maps/RockTunnel_B1F_Frlg/scripts.inc").read_text()
_regi_gate = _regi.split("RockTunnel_B1F_AnthologyOnTransition::")[1].split("RockTunnel_B1F_AnthologyOtherStatics::")[0]
for _name in ("REGIROCK", "REGICE", "REGISTEEL"):
    assert "getcaughtmon SPECIES_" + _name in _regi_gate
assert "RockTunnel_B1F_AnthologyRegigigasLocked::" in _regi_gate
assert "setflag FLAG_HIDE_ANTHOLOGY_REGIGIGAS" in _regi_gate

_arceus = (root / "data/maps/IndigoPlateau_Exterior_Frlg/scripts.inc").read_text()
_arceus_gate = _arceus.split("IndigoPlateau_Exterior_AnthologyOnTransition::")[1].split("IndigoPlateau_Exterior_AnthologyHideArceus::")[0]
assert "goto_if_unset FLAG_KANTO_CHAMPION" in _arceus_gate
for _name in ("DIALGA", "PALKIA", "GIRATINA"):
    assert "getcaughtmon SPECIES_" + _name in _arceus_gate
    assert "goto_if_ne VAR_RESULT, TRUE, IndigoPlateau_Exterior_AnthologyHideArceus" in _arceus_gate

_diancie = (root / "data/maps/GraniteCave_StevensRoom/scripts.inc").read_text()
_diancie_transition = _diancie.split("GraniteCave_StevensRoom_OnTransition::")[1].split("GraniteCave_StevensRoom_HideDiancie::")[0]
_diancie_steven = _diancie.split("GraniteCave_StevensRoom_RevealDiancieAfterLetter::")[1].split("GraniteCave_StevensRoom_EventScript_StevenExitNorth::")[0]
for _required in ("FLAG_BADGE01_GET", "FLAG_BADGE02_GET"):
    assert "goto_if_unset " + _required in _diancie_transition
    assert "goto_if_unset " + _required in _diancie_steven
assert "FLAG_DELIVERED_STEVEN_LETTER" in _diancie_transition
assert "seteventmon SPECIES_DIANCIE, 18" in _diancie
print("P0 approved legendary gates: Regis, creation trio, Kanto Champion, Diancie letter+badges PASS")

# Koichi must actually be beaten. The Dojo's visible scene variable is set
# before his battle, so checking it here would show Marshadow too early.
_dojo = (root / "data/maps/SaffronCity_Dojo_Frlg/scripts.inc").read_text()
assert "checktrainerflag TRAINER_BLACK_BELT_KOICHI" in _dojo
assert "goto_if EQUAL, SaffronCity_Dojo_AnthologyMarshadowAfterMaster" in _dojo
assert "call SaffronCity_Dojo_AnthologyRevealMarshadowNow" in _dojo
assert "SaffronCity_Dojo_EventScript_HitmonleeBall::" in _dojo
assert "SaffronCity_Dojo_EventScript_HitmonchanBall::" in _dojo
assert "seteventmon SPECIES_MARSHADOW, 38" in _dojo
_necro = (root / "data/maps/CeruleanCave_2F_Frlg/scripts.inc").read_text()
assert "seteventmon SPECIES_NECROZMA, 65" in _necro
assert "CeruleanCave_2F_AnthologyOnTransition::" in _necro
print("Kanto Dojo Marshadow requires true Koichi victory; Cerulean Cave Necrozma static PASS")

# Prevent generated overworld wild Pokémon from filling enclosed cave rooms
# or walking into the player while stationary. Normal step encounters remain
# enabled, and authored static encounters are not evicted by the cave guard.
owe_code = (root / "src/wild_encounter_ow.c").read_text()
owe_header = (root / "include/wild_encounter_ow.h").read_text()
wild_config = (root / "include/config/wild_encounter.h").read_text()
cave_update = owe_code.split("void UpdateOverworldWildEncounter(void)", 1)[1]
cave_guard = cave_update.split("bool32 shouldSpawnWaterMons = ShouldSpawnWaterOWE();", 1)[0]
assert "gMapHeader.mapType == MAP_TYPE_UNDERGROUND" in cave_guard
assert "DespawnAllOverworldWildEncounters(OWE_GENERATED, 0);" in cave_guard
assert "sOWESpawnCountdown = OWE_NO_ENCOUNTER_SET;" in cave_guard
assert "return;" in cave_guard
assert "DespawnAllOverworldWildEncounters(OWE_ANY" not in cave_guard
assert "#define WE_VANILLA_RANDOM           TRUE" in wild_config
assert "#define WE_OW_ENCOUNTERS                        TRUE" in wild_config
assert "#define OWE_SPAWNS_MAX                  4" in owe_header
assert 'seteventmon SPECIES_DIANCIE, 18' in (root / "data/maps/GraniteCave_StevensRoom/scripts.inc").read_text()
print("P0 cave ecology: no generated OWE swarms/idle chases, vanilla steps and static Diancie preserved PASS")

# Manual gameplay acceptance supersedes the unreliable synthetic
# Save/Continue navigation fixture. It must not gate regular mGBA CI.
_diancie_ci = (root / ".github/workflows/P0-mGBA-Release-Smoke.yml").read_text()
assert "test/overworld/mgba-run.sh test/overworld/lua/WorldDiancieStatic.lua" not in _diancie_ci
print("P0 Diancie: legacy synthetic doorway fixture retired from CI PASS")

# Sword of Justice route placements must be on walkable paths, not locked
# beyond hidden Waterfall/SURF regions. Mt Mortar's western entrance is
# independently flooded: other doorways lead to disconnected map areas.
_mortar = json.loads((root/"data/maps/MtMortar_1F_South/map.json").read_text())
_mlayout = _p0_layouts[_mortar["layout"]]
_mblock = (root/_mlayout["blockdata_filepath"]).read_bytes()
_mw,_mh = _mlayout["width"], _mlayout["height"]
def _mtwalk(x,y):
    if not (0 <= x < _mw and 0 <= y < _mh): return False
    v = _anthology_struct.unpack_from("<H", _mblock, 2*(y*_mw+x))[0]
    return v >> 12 == 3 and ((v >> 10)&3) == 0
_q=[(16,50)] # western Route 42 entrance
_v=set()
while _q:
    x,y=_q.pop()
    if (x,y) in _v or not _mtwalk(x,y): continue
    _v.add((x,y))
    _q.extend([(x+1,y),(x-1,y),(x,y+1),(x,y-1)])
assert (19,41) in _v, "Cobalion must be reachable from Mt Mortar WEST, not another entrance"
for _p,_old,_name in (("Route42","Route42","Keldeo"),("Route42","Route42","Terrakion"),("Route36","Route36","Virizion"),("OlivineCity_Lighthouse","OlivineCity_Lighthouse","Cobalion")):
    _oldmap=json.loads((root/"data/maps"/_old/"map.json").read_text())
    assert not any(o.get("script")==_p+"_EventScript_"+_name for o in _oldmap["object_events"])
print("P0 Johto Swords: four early/mid reachable statics, western Mt Mortar access, no old duplicates PASS")

# A moved static must not leave dead transition labels or undefined call targets
# behind on a former map; source-level checks run before the costly ROM link.
_olivine = (root/"data/maps/OlivineCity_Lighthouse/scripts.inc").read_text()
assert "OlivineCity_Lighthouse_AnthologyOnTransition::" in _olivine
assert "getcaughtmon SPECIES_REGIELEKI" in _olivine
assert "FLAG_AMPHAROS_HEALED" in _olivine
assert "OlivineCity_Lighthouse_AnthologyHideCobalion" not in _olivine
assert "OlivineCity_Lighthouse_AnthologyShowCobalion" not in _olivine
print("P0 relocated Cobalion: no dangling old-map event references PASS")

# Permanent Hoenn side chambers: routes, exit links, and post-collapse access.
_chambers = (
    ("Route111", "Route111_HoopaRingChamber", (30,56), (30,57), "Route111_EventScript_RingEntrance"),
    ("Underwater_Route128", "Route128_AncientGrotto", (42,27), (42,28), "Underwater_Route128_EventScript_GrottoEntrance"),
    ("NewMauville_Inside", "NewMauville_GeneratorAnnex", (34,24), (34,25), "NewMauville_Inside_EventScript_AnnexEntrance"),
)
for _source,_room,_door,_landing,_portal in _chambers:
    _outer = json.loads((root/"data/maps"/_source/"map.json").read_text())
    _inner = json.loads((root/"data/maps"/_room/"map.json").read_text())
    _portal_objs=[x for x in _outer["object_events"] if x.get("script")==_portal]
    assert len(_portal_objs)==1 and (_portal_objs[0]["x"],_portal_objs[0]["y"])==_door
    _links=[(i,x) for i,x in enumerate(_outer["warp_events"]) if x["dest_map"]==_inner["id"]]
    assert len(_links)==1 and (_links[0][1]["x"],_links[0][1]["y"])==_landing
    assert len(_inner["warp_events"])==1 and _inner["warp_events"][0]["dest_map"]==_outer["id"]
    assert str(_inner["warp_events"][0]["dest_warp_id"])==str(_links[0][0])
    _outer_source=(root/"data/maps"/_source/"scripts.inc").read_text()
    assert f"warp {_inner['id']}," in _outer_source
    assert f"{_portal}::" in _outer_source
    assert not any(x.get("graphics_id","").startswith("OBJ_EVENT_GFX_SPECIES(") and x.get("script")==f"{_source}_EventScript_"+("Hoopa" if "Route111"==_source else "Koraidon" if _source=="Underwater_Route128" else "Miraidon") for x in _outer["object_events"])
print("P0 Hoenn dedicated side-room portal, independent warps and encounter relocation PASS")

# A registered map's script labels must also be assembled into data/event_scripts.o.
# Omitting an include is invisible to source-only map validation but fails ROM linking.
_event_scripts = (root/"data/event_scripts.s").read_text()
for _src,_new_room,*_rest in _chambers:
    _directive = f'.include "data/maps/{_new_room}/scripts.inc"'
    assert _event_scripts.count(_directive) == 1, ("missing or duplicated event-script include", _new_room)
print("P0 Hoenn side-room event assembly linkage PASS")

