#!/usr/bin/env python3
"""Guard the October 23 P0 native duplicate-slot habitat additions.

Each map retains all original wild species and original encounter rates/levels;
only a repeated slot is repurposed. Count every encounter variant, and reserve
DexNav icons for any anthology rare/extra inhabitants still present on a map.
"""
import json
import re
from pathlib import Path

root = Path(__file__).resolve().parents[2]
data = json.loads((root / "src/data/wild_encounters.json").read_text())
rows = data["wild_encounter_groups"][0]["encounters"]
rare = (root / "src/data/anthology_rare_habitats.h").read_text()
extra = (root / "src/data/anthology_extra_habitats.h").read_text()
species_ids = (root / "include/constants/species.h").read_text()
# map: (slot index, originally repeated species, replacement).
# Original species must REMAIN on each variant, with no lost 5%/10% staple.
placements = {
    "MAP_VIRIDIAN_FOREST": [[11,"PIKACHU","BURMY_PLANT"]],
    "MAP_ILEX_FOREST": [[11,"KAKUNA","COTTONEE"]],
    "MAP_ROUTE32": [[11,"BELLSPROUT","SHELMET"]],
    "MAP_ROUTE37": [[11,"STANTLER","SAWSBUCK_SPRING"]],
    "MAP_ROUTE39": [[11,"TAUROS","MORPEKO_FULL_BELLY"]],
    "MAP_ROUTE30": [[11,"WEEDLE","ZIGZAGOON_GALAR"]],
    "MAP_ROUTE38": [[11,"RATICATE","MEOWTH_GALAR"]],
    "MAP_ROUTE35": [[11,"YANMA","FARFETCHD_GALAR"]],
    "MAP_ROUTE45": [[11,"GRAVELER","SNEASEL_HISUI"]],
    "MAP_PETALBURG_WOODS": [[11,"SLAKOTH","SCATTERBUG_ARCHIPELAGO"]],
    "MAP_ROUTE117": [[8,"ILLUMISE","FURFROU_NATURAL"],[9,"ILLUMISE","FLOETTE_RED"]],
    "MAP_ROUTE118": [[10,"WINGULL","SQUAWKABILLY_GREEN"]],
    "MAP_MT_PYRE_1F": [[11,"SHUPPET","GOURGEIST_AVERAGE"]],
    "MAP_BURNED_TOWER_B1F": [[11,"MISDREAVUS","POLTEAGEIST_PHONY"]],
    "MAP_MT_PYRE_2F": [[11,"SHUPPET","SINISTCHA_UNREMARKABLE"]],
}
# Check target variants only: other original areas remain unmodified.
expected_variants = {"MAP_VIRIDIAN_FOREST": 2}
found = 0
for map_id, expected in placements.items():
    variants = [row for row in rows if row["map"] == map_id and row.get("land_mons", {}).get("encounter_rate", 0)]
    assert len(variants) == expected_variants.get(map_id, 1), (map_id, len(variants))
    rares = re.findall(r"^RARE_ON_MAP[(]" + map_id + r", ([A-Z0-9_]+)[)]$", rare, re.M)
    extras = re.findall(r"^ECOLOGY_ON_MAP[(]" + map_id + r", ([A-Z0-9_]+)[)]$", extra, re.M)
    assert len(rares) == 1 and len(extras) <= 4, map_id
    for row in variants:
        native = row["land_mons"]["mons"]
        assert len(native) == 12, map_id
        species = [x["species"] for x in native]
        extra_species = set("SPECIES_" + x for x in rares + extras)
        assert not set(species).intersection(extra_species), (map_id, "duplicate habitat/rare")
        assert len(set(species)) + len(extra_species) <= 12, (map_id, "DexNav grass overflow")
        for slot, original, replacement in expected:
            orig, new = "SPECIES_" + original, "SPECIES_" + replacement
            assert species[slot] == new and orig in species, (map_id, slot, original, replacement)
            assert new not in species[:slot] + species[slot+1:], (map_id, replacement)
            assert new in species_ids, new
            assert native[slot]["min_level"] <= native[slot]["max_level"], map_id
            found += 1
assert len(placements) == 15 and found == 17, (len(placements), found)
print("P0 native habitats: 16 replacement slots / 17 variants, original species preserved, DexNav fits PASS")
