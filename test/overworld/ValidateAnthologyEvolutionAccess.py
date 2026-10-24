#!/usr/bin/env python3
"""Source contracts for accessible P0 evolution, shops and 1025-Dex; not runtime proof."""
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
def source(p): return (ROOT / p).read_text(encoding="utf-8")
def need(p, *bits):
    data = source(p)
    for b in bits:
        assert b in data, f"{p}: missing {b}"
shops = (
    "data/maps/CeladonCity_DepartmentStore_4F_Frlg/scripts.inc",
    "data/maps/GoldenrodCity_DepartmentStore_4F/scripts.inc",
    "data/maps/LilycoveCity_DepartmentStore_2F/scripts.inc",
)
for p in shops:
    need(p, "pokemart ", "ITEM_ABILITY_CAPSULE", "ITEM_ABILITY_PATCH", "ITEM_LEADERS_CREST")
need(shops[1], "ITEM_DUSK_STONE")
need(shops[2], "ITEM_GIMMIGHOUL_COIN")
need("src/data/items.h", ".price = 20000,", ".price = 400,")
g9 = "src/data/pokemon/species_info/gen_9_families.h"
need(g9, "{EVO_LEVEL, 38, SPECIES_PALAFIN_ZERO}", "{EVO_LEVEL, 32, SPECIES_PAWMOT}",
     "{EVO_LEVEL, 30, SPECIES_RABSCA}", "{EVO_LEVEL, 30, SPECIES_BRAMBLEGHAST}")
assert source(g9).count("IF_BAG_ITEM_COUNT, ITEM_GIMMIGHOUL_COIN, 30") == 2
need("src/data/pokemon/species_info/gen_7_families.h",
     "{EVO_LEVEL, 40, SPECIES_MELMETAL, CONDITIONS({IF_HOLD_ITEM, ITEM_METAL_COAT})}")
need("src/data/pokemon/species_info/gen_5_families.h",
     "{EVO_ITEM, ITEM_DUSK_STONE, SPECIES_RUNERIGUS}",
     "{EVO_LEVEL, 52, SPECIES_KINGAMBIT, CONDITIONS({IF_HOLD_ITEM, ITEM_LEADERS_CREST})}",
     "IF_KNOWS_MOVE, MOVE_WAVE_CRASH")
need("src/data/pokemon/species_info/gen_2_families.h",
     "{EVO_ITEM, ITEM_PEAT_BLOCK, SPECIES_URSALUNA}")
need("src/data/pokemon/species_info/gen_1_families.h", "{EVO_LEVEL, 30, SPECIES_SIRFETCHD}")
need("include/config/battle.h", "#define B_BATTLE_BOND               GEN_8")
pokedex = source("src/pokedex.c").split("bool16 HasAllMons(void)")[1].split("static void ResetOtherVideoRegisters")[0]
assert "isMythical" not in pokedex
assert "required[dn >> 3] |= 1 << (dn & 7)" in pokedex
assert "GetSetPokedexFlag(dn, FLAG_GET_CAUGHT)" in pokedex
print("P0 anthology evolution access and national completion source guards: PASS")

# The zero-price important/key-item Kubfu scrolls previously listed in marts
# were not reliable purchases. Both are ordinary purchasable evolution items.
for item in ("SCROLL_OF_DARKNESS", "SCROLL_OF_WATERS"):
    block = source("src/data/items.h").split(f"[ITEM_{item}] =", 1)[1].split("\n    },", 1)[0]
    assert ".price = 6000," in block
    assert ".pocket = POCKET_ITEMS," in block
    assert ".importance = 1," not in block
    assert "ItemUseOutOfBattle_EvolutionStone" in block
for item in ["FIRE_STONE","WATER_STONE","THUNDER_STONE","LEAF_STONE","MOON_STONE","SUN_STONE","SHINY_STONE","DAWN_STONE","ICE_STONE","METAL_COAT","KINGS_ROCK","DRAGON_SCALE","UP_GRADE","DUBIOUS_DISC","PRISM_SCALE","DEEP_SEA_TOOTH","DEEP_SEA_SCALE","ELECTIRIZER","MAGMARIZER","PROTECTOR","REAPER_CLOTH","RAZOR_CLAW","RAZOR_FANG","LINKING_CORD","OVAL_STONE","TART_APPLE","SWEET_APPLE","SYRUPY_APPLE","GALARICA_CUFF","GALARICA_WREATH","PEAT_BLOCK","BLACK_AUGURITE","METAL_ALLOY","SCROLL_OF_DARKNESS","SCROLL_OF_WATERS","STRAWBERRY_SWEET","LOVE_SWEET","BERRY_SWEET","CLOVER_SWEET","FLOWER_SWEET","STAR_SWEET","RIBBON_SWEET","CRACKED_POT","CHIPPED_POT","UNREMARKABLE_TEACUP","MASTERPIECE_TEACUP"]:
    assert f".2byte ITEM_{item}" in source(shops[1]), item
print("P0 Goldenrod pre-League complete evolutionary item supplies and purchasable Kubfu scrolls PASS")


# P0 offline-acquisition contract: a trade-specific evolution with no EVO_ITEM
# alternative strands its National species when link features are disabled.
# Parse every enabled family, not just a hand-picked Karrablast/Shelmet example.
import re
family_files = sorted((ROOT / "src/data/pokemon/species_info").glob("gen_*_families.h"))
assert len(family_files) == 9, "Incomplete generation family registry"
trade_edges = []
evolution_items = set()
for family_file in family_files:
    family_text = family_file.read_text(encoding="utf-8")
    species_headers = list(re.finditer(r"^\s*\[(SPECIES_[A-Z0-9_]+)\]\s*=", family_text, re.M))
    assert species_headers, family_file
    for index, match in enumerate(species_headers):
        body = family_text[match.end():species_headers[index + 1].start() if index + 1 < len(species_headers) else len(family_text)]
        item_edges = re.findall(r"\{EVO_ITEM,\s*(ITEM_[A-Z0-9_]+),\s*(SPECIES_[A-Z0-9_]+)", body)
        evolution_items.update(item for item, _target in item_edges)
        for target in re.findall(r"\{EVO_TRADE,\s*[^,]+,\s*(SPECIES_[A-Z0-9_]+)", body):
            trade_edges.append((match.group(1), target))
            assert any(item_target == target for _item, item_target in item_edges), (
                family_file.name, match.group(1), target, "no offline item evolution")

assert len(trade_edges) >= 30, f"Trade-edge audit unexpectedly shrank: {len(trade_edges)}"
for shop in shops:
    inventory = set(re.findall(r"\.2byte\s+(ITEM_[A-Z0-9_]+)", source(shop)))
    # The same item is named Upgrade / Up-Grade in different generations.
    if "ITEM_UP_GRADE" in inventory:
        inventory.add("ITEM_UPGRADE")
    missing = evolution_items - inventory
    assert not missing, (shop, "missing evolution items", sorted(missing))
    assert "ITEM_GIMMIGHOUL_COIN" in inventory, (shop, "Gholdengo coin supply absent")
print(f"P0 offline evolution: {len(trade_edges)} trade routes and {len(evolution_items)} item types stocked in all regions PASS")


# A move-based evolution is not practically reachable if its pre-evolution
# cannot learn the requirement. Audit every such edge against the active
# Gen 9 level-up set, rather than checking specific names or TM availability.
learnset_config = source("include/config/pokemon.h")
assert re.search(r"^#define\s+P_LVL_UP_LEARNSETS\s+GEN_LATEST\b", learnset_config, re.M)
level_data = source("src/data/pokemon/level_up_learnsets/gen_9.h")
level_sets = dict(re.findall(
    r"static const struct LevelUpMove\s+(s[A-Za-z0-9_]+LevelUpLearnset)\[\]\s*=\s*\{(.*?)LEVEL_UP_END",
    level_data, re.S))
assert len(level_sets) >= 100, "Gen 9 level-up learnset parse failed"
required_moves = []
for family_file in family_files:
    family_text = family_file.read_text(encoding="utf-8")
    species_headers = list(re.finditer(r"^\s*\[(SPECIES_[A-Z0-9_]+)\]\s*=", family_text, re.M))
    for index, match in enumerate(species_headers):
        body = family_text[match.end():species_headers[index + 1].start() if index + 1 < len(species_headers) else len(family_text)]
        moves = set(re.findall(r"\{IF_KNOWS_MOVE,\s*(MOVE_[A-Z0-9_]+)", body))
        if not moves:
            continue
        selected = re.search(r"\.levelUpLearnset\s*=\s*(s[A-Za-z0-9_]+LevelUpLearnset)", body)
        assert selected and selected.group(1) in level_sets, (match.group(1), "missing level-up learnset")
        learned = set(re.findall(r"LEVEL_UP_MOVE\(\s*\d+\s*,\s*(MOVE_[A-Z0-9_]+)\s*\)", level_sets[selected.group(1)]))
        for move in moves:
            required_moves.append((match.group(1), move))
            assert move in learned, (match.group(1), move, "evolution requirement not learned naturally")
assert len(required_moves) >= 13, f"Move-gated evolution audit shrank: {len(required_moves)}"
print(f"P0 move-based evolution: {len(required_moves)} offline naturally learnable requirements PASS")


# The shared World Transit shop must sell ordinary equipment for stable
# alternate forms across all three regions. Key items and temporary battle
# transformations have separate event/gimmick requirements.
forms = source("src/data/pokemon/form_change_tables.h")
plate_items = set(re.findall(
    r"\{FORM_CHANGE_ITEM_HOLD,\s*SPECIES_ARCEUS_\w+,\s*(ITEM_[A-Z0-9_]+_PLATE)", forms))
memory_items = set(re.findall(
    r"\{FORM_CHANGE_ITEM_HOLD,\s*SPECIES_SILVALLY_\w+,\s*(ITEM_[A-Z0-9_]+_MEMORY)", forms))
nectar_items = set(re.findall(
    r"\{FORM_CHANGE_ITEM_USE,\s*SPECIES_ORICORIO_\w+,\s*(ITEM_[A-Z0-9_]+_NECTAR)", forms))
assert (len(plate_items), len(memory_items), len(nectar_items)) == (17, 17, 4)
hub_shop = source("data/maps/RegionHub/scripts.inc").split("RegionHub_Items::", 1)[1].split("pokemartlistend", 1)[0]
hub_stock = set(re.findall(r"\.2byte\s+(ITEM_[A-Z0-9_]+)", hub_shop))
form_requirements = plate_items | memory_items | nectar_items
assert form_requirements <= hub_stock, sorted(form_requirements - hub_stock)
form_item_definitions = source("src/data/items.h")
for item in form_requirements:
    item_entry = form_item_definitions.split(f"[{item}] =", 1)[1].split("\n    },", 1)[0]
    assert ".pocket = POCKET_ITEMS" in item_entry, item
    assert re.search(r"\.price\s*=\s*(?:1000|300),", item_entry), item
print(f"P0 permanent form supplies: {len(form_requirements)} buyable hub item paths PASS")


# Every inherited evolution requiring Alola, Galar, or Hisui was dead in
# this Kanto/Johto/Hoenn-only game: GetCurrentRegion() never selects those
# three values. Keep the original regional behavior for compatibility and
# add a distinct, purchasable evolution-stone path for each alternate form.
# Ordinary base-form methods must remain untouched (Thunder/Leaf stones,
# level-up branches, etc.). These are SOURCE checks, not emulator proof.
regional_stone_overrides = {
    "RAICHU_ALOLA": "SHINY_STONE",
    "EXEGGUTOR_ALOLA": "SUN_STONE",
    "MAROWAK_ALOLA": "DUSK_STONE",
    "WEEZING_GALAR": "SHINY_STONE",
    "MR_MIME_GALAR": "ICE_STONE",
    "TYPHLOSION_HISUI": "DUSK_STONE",
    "SAMUROTT_HISUI": "DUSK_STONE",
    "LILLIGANT_HISUI": "DAWN_STONE",
    "BRAVIARY_HISUI": "DAWN_STONE",
    "SLIGGOO_HISUI": "SHINY_STONE",
    "AVALUGG_HISUI": "ICE_STONE",
    "DECIDUEYE_HISUI": "DAWN_STONE",
}
found_regional = set()
for family_file in family_files:
    family_text = family_file.read_text(encoding="utf-8")
    headers = list(re.finditer(r"^\s*\[(SPECIES_[A-Z0-9_]+)\]\s*=", family_text, re.M))
    for index, match in enumerate(headers):
        body = family_text[match.end():headers[index + 1].start() if index + 1 < len(headers) else len(family_text)]
        gated = re.findall(
            r"\{EVO_(?:LEVEL|ITEM)[^\n]*SPECIES_([A-Z0-9_]+),\s*CONDITIONS\([^\n]*\{IF_REGION,\s*REGION_(?:ALOLA|GALAR|HISUI)\}", body)
        for target in gated:
            assert target in regional_stone_overrides, (match.group(1), target, "unhandled inaccessible regional evolution")
            stone = regional_stone_overrides[target]
            assert re.search(r"\{EVO_ITEM,\s*ITEM_" + stone + r",\s*SPECIES_" + target + r"\s*\}", body), (match.group(1), target, stone)
            found_regional.add(target)
assert found_regional == set(regional_stone_overrides), sorted(set(regional_stone_overrides) - found_regional)
for shop in shops:
    available = set(re.findall(r"\.2byte\s+ITEM_([A-Z0-9_]+)", source(shop)))
    assert set(regional_stone_overrides.values()) <= available, (shop, "missing regional-form evolution stones")
for stone in set(regional_stone_overrides.values()):
    item_data = source("src/data/items.h").split(f"[ITEM_{stone}] =", 1)[1].split("\n    },", 1)[0]
    assert "ItemUseOutOfBattle_EvolutionStone" in item_data and ".pocket = POCKET_ITEMS" in item_data, stone
need("src/pokemon.c", "case EVO_MODE_ITEM_USE:", "case EVO_ITEM:", "evolutions[i].param == evolutionItem")
print(f"P0 region-inaccessible evolution repair: {len(found_regional)} purchasable alternate-form stone paths PASS")

# Cubone also offers a disabled/legacy Totem branch after the new stone method.
# Missing the separator compiles no C despite the source-level acquisition passing.
cubone = source("src/data/pokemon/species_info/gen_1_families.h")
assert re.search(
    r"\{EVO_ITEM, ITEM_DUSK_STONE, SPECIES_MAROWAK_ALOLA\},\s*\{EVO_NONE, 0, SPECIES_MAROWAK_ALOLA_TOTEM\}",
    cubone), "Cubone's alternate evolution entries must be comma-delimited"


# P0 practical clock/weather-independent acquisition alternatives.
# The original day/night/weather/friendship/spin routes remain available.
# These purchasable, ordinary evolution-stone options are intentionally optional.
time_weather_alternatives = {
    "RATTATA_ALOLA": (1, {"MOON_STONE": "RATICATE_ALOLA"}, "IF_TIME, TIME_NIGHT"),
    "SNEASEL_HISUI": (2, {"DAWN_STONE": "SNEASLER"}, "IF_NOT_TIME, TIME_NIGHT"),
    "LINOONE_GALAR": (3, {"DUSK_STONE": "OBSTAGOON"}, "IF_TIME, TIME_NIGHT"),
    "AMAURA": (6, {"ICE_STONE": "AURORUS"}, "IF_TIME, TIME_NIGHT"),
    "SLIGGOO": (6, {"SHINY_STONE": "GOODRA"}, "IF_WEATHER, WEATHER_RAIN"),
    "SLIGGOO_HISUI": (6, {"SHINY_STONE": "GOODRA_HISUI"}, "IF_WEATHER, WEATHER_FOG"),
    "ROCKRUFF": (7, {"SUN_STONE": "LYCANROC_MIDDAY", "MOON_STONE": "LYCANROC_MIDNIGHT"}, "IF_TIME, TIME_NIGHT"),
    "ROCKRUFF_OWN_TEMPO": (7, {"DUSK_STONE": "LYCANROC_DUSK"}, "IF_TIME, TIME_EVENING"),
    "COSMOEM": (7, {"SUN_STONE": "SOLGALEO", "MOON_STONE": "LUNALA"}, "IF_TIME, TIME_NIGHT"),
    "SNOM": (8, {"ICE_STONE": "FROSMOTH"}, "IF_MIN_FRIENDSHIP, FRIENDSHIP_EVO_THRESHOLD"),
    "GREAVARD": (9, {"DUSK_STONE": "HOUNDSTONE"}, "IF_TIME, TIME_NIGHT"),
    "MILCERY": (8, {"SHINY_STONE": "ALCREMIE_STRAWBERRY_VANILLA_CREAM"}, "EVO_SPIN"),
}
definitions = source("src/data/items.h")
fallback_count = 0
for parent, (generation, variants, original_condition) in time_weather_alternatives.items():
    family = source(f"src/data/pokemon/species_info/gen_{generation}_families.h")
    assert f"[SPECIES_{parent}] =" in family, parent
    body = family.split(f"[SPECIES_{parent}] =", 1)[1].split("\n    [SPECIES_", 1)[0]
    assert original_condition in body, (parent, "lost original method")
    for item, target in variants.items():
        assert f"{{EVO_ITEM, ITEM_{item}, SPECIES_{target}}}" in body, (parent, item, target)
        item_body = definitions.split(f"[ITEM_{item}] =", 1)[1].split("\n    },", 1)[0]
        assert ".fieldUseFunc = ItemUseOutOfBattle_EvolutionStone" in item_body, item
        assert ".pocket = POCKET_ITEMS" in item_body, item
        for mart in shops:
            assert f".2byte ITEM_{item}" in source(mart), (mart, item)
        fallback_count += 1
assert fallback_count == 14, fallback_count
ursaring = source("src/data/pokemon/species_info/gen_2_families.h").split("[SPECIES_URSARING] =", 1)[1].split("\n    [SPECIES_", 1)[0]
assert "{EVO_ITEM, ITEM_PEAT_BLOCK, SPECIES_URSALUNA}" in ursaring
assert "ITEM_PEAT_BLOCK, SPECIES_URSALUNA, CONDITIONS({IF_TIME" not in ursaring
print(f"P0 offline clock/weather evolution: {fallback_count} stone routes plus all-hours Ursaluna PASS")
