#!/usr/bin/env python3
"""Source contracts for a second, permanently Stellar Terapagos. Not an emulator pass."""
from pathlib import Path
import json
import re
import struct

ROOT = Path(__file__).resolve().parents[2]
def read(path):
    return (ROOT / path).read_text()

forms = read("src/data/pokemon/form_change_tables.h")
species = read("src/data/pokemon/species_info/gen_9_families.h")
battle = read("src/battle_util.c")
city = read("data/maps/SootopolisCity/scripts.inc")
cave_script = read("data/maps/CaveOfOrigin_B1F/scripts.inc")
normal = read("data/maps/Route114/scripts.inc")
flags = read("include/constants/flags.h")
cave = json.loads(read("data/maps/CaveOfOrigin_B1F/map.json"))
layouts = {x["id"]: x for x in json.loads(read("data/layouts/layouts.json"))["layouts"]}

stellar_form = re.search(r"static const struct FormChange sTerapagosPermanentStellarFormChangeTable\[\] =\s*\{(.*?)\};", forms, re.S)
assert stellar_form and "FORM_CHANGE_TERMINATOR" in stellar_form[1]
assert not re.search(r"FORM_CHANGE_(?:FAINT|END_BATTLE|BATTLE_TERASTALLIZATION)", stellar_form[1])
stellar_info = species.split("[SPECIES_TERAPAGOS_STELLAR] =", 1)[1].split("#endif //P_FAMILY_TERAPAGOS", 1)[0]
normal_info = species.split("[SPECIES_TERAPAGOS_NORMAL] =", 1)[1].split("[SPECIES_TERAPAGOS_TERASTAL] =", 1)[0]
assert ".formChangeTable = sTerapagosPermanentStellarFormChangeTable" in stellar_info
assert ".formChangeTable = sTerapagosFormChangeTable" in normal_info
assert ".baseHP        = 160" in stellar_info and ".baseSpAttack  = 130" in stellar_info
assert ".abilities = { ABILITY_TERAFORM_ZERO" in stellar_info
assert "case ABILITY_TERAFORM_ZERO:" in battle.split("case ABILITYEFFECT_ON_SWITCHIN:", 1)[1].split("case ABILITY_TRACE:", 1)[0]
assert "SPECIES_TERAPAGOS_STELLAR" in battle.split("case ABILITYEFFECT_ON_SWITCHIN:", 1)[1].split("case ABILITY_TRACE:", 1)[0]

assert "call_if_set FLAG_HOENN_CHAMPION, SootopolisCity_EventScript_ReopenCaveForChampion" in city
assert "setobjectxyperm LOCALID_SOOTOPOLIS_EXPERT, 30, 18" in city
assert "goto_if_set FLAG_HOENN_CHAMPION, SootopolisCity_EventScript_ExpertChampion" in city
assert "goto_if_unset FLAG_HOENN_CHAMPION, CaveOfOrigin_B1F_HideStellar" in cave_script
assert "goto_if_set FLAG_CAUGHT_ANTHOLOGY_TERAPAGOS_STELLAR, CaveOfOrigin_B1F_HideStellar" in cave_script
assert "seteventmon SPECIES_TERAPAGOS_STELLAR, 75" in cave_script
assert "goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, CaveOfOrigin_B1F_StellarCaught" in cave_script
assert "setflag FLAG_CAUGHT_ANTHOLOGY_TERAPAGOS_STELLAR" in cave_script
assert "FLAG_CAUGHT_ANTHOLOGY_TERAPAGOS_NORMAL" in normal
assert "getcaughtmon SPECIES_TERAPAGOS_NORMAL" not in normal
assert "seteventmon SPECIES_TERAPAGOS_NORMAL, 31" in normal
assert "FLAG_CAUGHT_ANTHOLOGY_TERAPAGOS_NORMAL" in flags
assert "FLAG_CAUGHT_ANTHOLOGY_TERAPAGOS_STELLAR" in flags

objects = [x for x in cave["object_events"] if x["script"] == "CaveOfOrigin_B1F_EventScript_StellarTerapagos"]
assert len(objects) == 1
event = objects[0]
assert event["flag"] == "FLAG_HIDE_ANTHOLOGY_TERAPAGOS_STELLAR"
assert event["graphics_id"] == "OBJ_EVENT_GFX_SPECIES(TERAPAGOS_TERASTAL)"
assert (event["x"], event["y"], event["elevation"]) == (9, 14, 3)
layout = layouts[cave["layout"]]
w, h = layout["width"], layout["height"]
blocks = (ROOT / layout["blockdata_filepath"]).read_bytes()
assert len(blocks) == 2 * w * h
def walk(x, y):
    if not (0 <= x < w and 0 <= y < h):
        return False
    v = struct.unpack_from("<H", blocks, 2 * (y * w + x))[0]
    return (v >> 12) == 3 and ((v >> 10) & 3) == 0

start = (cave["warp_events"][0]["x"], cave["warp_events"][0]["y"])
assert walk(*start) and walk(event["x"], event["y"])
seen, todo = set(), [start]
while todo:
    x, y = todo.pop()
    if (x, y) in seen or not walk(x, y):
        continue
    seen.add((x, y))
    todo.extend([(x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)])
assert (event["x"], event["y"]) in seen, "Stellar Terapagos must be reachable from cave entrance"
print("PASS: permanent Stellar Terapagos source, independence, story access and walkable cave path.")
