#!/usr/bin/env python3
"""Exact three-region National Dex partition and award source contracts.

Source-only validation, not an emulator pass or proof of 1,025 catchable species.
"""
from pathlib import Path
root = Path(__file__).resolve().parents[2]
def read(path): return (root / path).read_text()
def require(path, words):
    data = read(path)
    for word in words:
        assert word in data, f"{path}: missing {word!r}"

gen = [(1, 151), (152, 251), (252, 386), (387, 493), (494, 649),
       (650, 721), (722, 809), (810, 905), (906, 1025)]
groups = {"KANTO": (0, 3, 6), "JOHTO": (1, 4, 7), "HOENN": (2, 5, 8)}
sizes = {"KANTO": 346, "JOHTO": 352, "HOENN": 327}
assigned = []
for region, gens in groups.items():
    members = [n for index in gens for n in range(gen[index][0], gen[index][1] + 1)]
    assert len(members) == sizes[region], (region, len(members))
    assert members == sorted(members)
    assigned.extend(members)
assert len(assigned) == 1025 and sorted(assigned) == list(range(1, 1026))

require("include/pokedex.h", (
    "AnthologyRegionalToNationalOrder", "AnthologyNationalToRegionalOrder",
    "GetAnthologyRegionalPokedexCount", "HasAllAnthologyRegionalMons"))
require("src/pokedex.c", (
    "case REGION_KANTO: return 346", "case REGION_JOHTO: return 352",
    "case REGION_HOENN: return 327",
    "GetSetPokedexFlag(number, FLAG_GET_CAUGHT)",
    "GetAnthologyRegionalPokedexCount(GetActiveRegion(), caseID)",
    "AnthologyNationalToRegionalOrder"))
require("src/pokedex_plus_hgss.c", (
    "temp_dexNum = AnthologyRegionalToNationalOrder(region, i + 1);",
    "IsInAnthologyRegionalDex(region, temp_dexNum)",
    "AnthologyNationalToRegionalOrder(AnthologyGetDisplayRegion(), dexNum);"))
require("include/constants/flags.h", (
    "FLAG_ANTHOLOGY_OAK_MEDAL", "FLAG_ANTHOLOGY_ELM_MEDAL",
    "FLAG_ANTHOLOGY_BIRCH_MEDAL", "FLAG_ANTHOLOGY_GRAND_RESEARCHER"))
require("src/region_switch.c", (
    "HasAllAnthologyRegionalMons(REGION_KANTO), 3, ITEM_RARE_CANDY, 10",
    "HasAllAnthologyRegionalMons(REGION_JOHTO), 3, ITEM_ABILITY_PATCH, 5, ITEM_PP_MAX, 10",
    "HasAllAnthologyRegionalMons(REGION_HOENN), 3, ITEM_RARE_CANDY, 25, ITEM_PP_MAX, 10",
    "HasAllMons(), 10, ITEM_NONE, 0, ITEM_NONE, 0",
    "RemoveBagItem(item1, count1)", "FlagSet(flag);"))
for region, path, symbol in (
    ("OAK", "data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc", "Anthology_ScrClaimOakMedal"),
    ("ELM", "data/maps/NewBarkTown_Lab/scripts.inc", "Anthology_ScrClaimElmMedal"),
    ("BIRCH", "data/maps/LittlerootTown_ProfessorBirchsLab/scripts.inc", "Anthology_ScrClaimBirchMedal"),
):
    require(path, (f"callnative {symbol}", "ResearchMedalBagFull", "ResearchMedalAward"))
require("data/maps/RegionHub/scripts.inc", (
    "callnative RegionHub_ScrClaimGrandResearcher",
    "FLAG_ANTHOLOGY_GRAND_RESEARCHER", "RegionHub_Items_Premium",
    "ITEM_MASTER_BALL", "ITEM_RARE_CANDY", "ITEM_PP_MAX",
    "FLAG_DEX_REWARD_1", "FLAG_DEX_REWARD_2",
    "RegionHub_Text_GrandResearcherCeremony"))
require("src/data/items.h", ('.name = ITEM_NAME("Master Ball"),\n        .price = 50000',))
require("src/trainer_card.c", ("FlagGet(FLAG_ANTHOLOGY_GRAND_RESEARCHER)", "return 4;"))
print("P0 anthology regional-dex 346/352/327 partition + medals + national reward guards PASS")
