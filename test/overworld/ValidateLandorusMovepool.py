#!/usr/bin/env python3
"""Guard Landorus's special Flying moves and the reusable Nasty Plot TM."""
import json
import re
from pathlib import Path

root = Path(__file__).resolve().parents[2]

def read(path):
    return (root / path).read_text()

# Test the learnset actually compiled by src/pokemon.c under the active config.
config = read("include/config/pokemon.h")
pokemon = read("src/pokemon.c")
assert "#define P_LVL_UP_LEARNSETS          GEN_LATEST" in config
assert "#if P_LVL_UP_LEARNSETS >= GEN_9" in pokemon
assert '#include "data/pokemon/level_up_learnsets/gen_9.h"' in pokemon
learnsets = read("src/data/pokemon/level_up_learnsets/gen_9.h")
landorus = learnsets.split("static const struct LevelUpMove sLandorusLevelUpLearnset[] = {", 1)[1].split("LEVEL_UP_END", 1)[0]
assert landorus.count("LEVEL_UP_MOVE(") <= 20, "Move Relearner scans MAX_LEVEL_UP_MOVES=20 entries"
# The screenshot's original moves confirm this is the active Gen 9 table.
for move in ("SMACK_DOWN", "SAND_TOMB", "LEER", "BLOCK", "BULLDOZE"):
    assert f"MOVE_{move}" in landorus, f"Wrong Landorus learnset: {move}"
for level, move in ((37, "AIR_SLASH"), (43, "NASTY_PLOT"), (55, "HURRICANE")):
    assert f"LEVEL_UP_MOVE({level:2d}, MOVE_{move})" in landorus, (level, move)
species = read("src/data/pokemon/species_info/gen_5_families.h")
for form in ("INCARNATE", "THERIAN"):
    section = species.split(f"[SPECIES_LANDORUS_{form}] =", 1)[1].split("    },", 1)[0]
    assert ".levelUpLearnset = sLandorusLevelUpLearnset," in section, form
    assert ".teachableLearnset = sLandorusTeachableLearnset," in section, form

tms = read("include/constants/tms_hms.h").split("#define FOREACH_TM(F)", 1)[1].split("#define FOREACH_HM(F)", 1)[0]
tms_in_order = re.findall(r"F\((\w+)\)", tms)
assert len(tms_in_order) == 51, len(tms_in_order)
assert tms_in_order[-1] == "NASTY_PLOT", "TM51 must be Nasty Plot"
assert "ITEM_TM51 = 632" in read("include/constants/items.h"), "Preserve TM51 save item ID"
item = read("src/data/items.h").split("[ITEM_TM_NASTY_PLOT] =", 1)[1].split("    },", 1)[0]
for token in ('ITEM_NAME("TM51")', ".pocket = POCKET_TM_HM", ".fieldUseFunc = ItemUseOutOfBattle_TMHM", ".importance = I_REUSABLE_TMS"):
    assert token in item, token

custom = json.loads(read("src/data/pokemon/special_movesets.json"))
assert "MOVE_NASTY_PLOT" in custom["customTmLearnsets"]["LANDORUS"]
generator = read("tools/learnset_helpers/make_teachables.py")
assert 'special_movesets.get("customTmLearnsets", {}).get(species_upper, [])' in generator

celadon = read("data/maps/CeladonCity_DepartmentStore_2F_Frlg/scripts.inc")
lilycove = read("data/maps/LilycoveCity_DepartmentStore_4F/scripts.inc")
goldenrod = json.loads(read("data/maps/GoldenrodCity_DepartmentStore_5F/map.json"))
assert ".2byte ITEM_TM_NASTY_PLOT" in celadon.split("CeladonCity_DepartmentStore_2F_TMs::", 1)[1].split("ITEM_NONE", 1)[0]
assert ".2byte ITEM_TM_NASTY_PLOT" in lilycove.split("LilycoveCity_DepartmentStore_4F_Pokemart_AllTMs:", 1)[1].split("pokemartlistend", 1)[0]
assert any(event.get("script") == "LilycoveCity_DepartmentStore_4F_EventScript_ClerkRight" for event in goldenrod["object_events"]), "Goldenrod TM clerk must use shared Lilycove stock"
print("PASS: Landorus learns Air Slash, Nasty Plot, Hurricane; TM51 works at all three department stores")
