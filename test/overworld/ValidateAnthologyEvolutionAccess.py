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
     "{EVO_ITEM, ITEM_PEAT_BLOCK, SPECIES_URSALUNA, CONDITIONS({IF_TIME, TIME_NIGHT})}")
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
    species_headers = list(re.finditer(r"^\\s*\\[(SPECIES_[A-Z0-9_]+)\\]\\s*=", family_text, re.M))
    assert species_headers, family_file
    for index, match in enumerate(species_headers):
        body = family_text[match.end():species_headers[index + 1].start() if index + 1 < len(species_headers) else len(family_text)]
        item_edges = re.findall(r"\\{EVO_ITEM,\\s*(ITEM_[A-Z0-9_]+),\\s*(SPECIES_[A-Z0-9_]+)", body)
        evolution_items.update(item for item, _target in item_edges)
        for target in re.findall(r"\\{EVO_TRADE,\\s*[^,]+,\\s*(SPECIES_[A-Z0-9_]+)", body):
            trade_edges.append((match.group(1), target))
            assert any(item_target == target for _item, item_target in item_edges), (
                family_file.name, match.group(1), target, "no offline item evolution")

assert len(trade_edges) >= 30, f"Trade-edge audit unexpectedly shrank: {len(trade_edges)}"
for shop in shops:
    inventory = set(re.findall(r"\\.2byte\\s+(ITEM_[A-Z0-9_]+)", source(shop)))
    # The same item is named Upgrade / Up-Grade in different generations.
    if "ITEM_UP_GRADE" in inventory:
        inventory.add("ITEM_UPGRADE")
    missing = evolution_items - inventory
    assert not missing, (shop, "missing evolution items", sorted(missing))
    assert "ITEM_GIMMIGHOUL_COIN" in inventory, (shop, "Gholdengo coin supply absent")
print(f"P0 offline evolution: {len(trade_edges)} trade routes and {len(evolution_items)} item types stocked in all regions PASS")
