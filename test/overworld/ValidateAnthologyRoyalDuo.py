#!/usr/bin/env python3
"""Royal Duo source checks. Real battles, gift storage and persistence need emulator QA."""
from pathlib import Path
import json
import struct

root = Path(__file__).resolve().parents[2]
def source(path):
    return (root / path).read_text()

route = json.loads(source("data/maps/Route27/map.json"))
scripts = source("data/maps/Route27/scripts.inc")
flags = source("include/constants/flags.h")
battle = source("src/battle_setup.c")
rewards = source("src/script_pokemon_util.c")
cfg = source("include/config/wild_encounter.h")
specials = source("data/specials.inc")
layout = next(x for x in json.loads(source("data/layouts/layouts.json"))["layouts"]
              if x["id"] == route["layout"])
raw = (root / layout["blockdata_filepath"]).read_bytes()
assert len(raw) == layout["width"] * layout["height"] * 2

def walkable(x, y):
    if not (0 <= x < layout["width"] and 0 <= y < layout["height"]):
        return False
    tile = struct.unpack_from("<H", raw, 2 * (y * layout["width"] + x))[0]
    return ((tile >> 10) & 3) == 0

guardians = {}
for name in ("ZACIAN", "ZAMAZENTA"):
    found = [obj for obj in route["object_events"]
             if obj.get("local_id") == f"LOCALID_ANTHOLOGY_{name}"]
    assert len(found) == 1, (name, "expected exactly one Route 27 object")
    guardians[name] = found[0]
    assert found[0]["script"] == "Route27_EventScript_RoyalDuo" + name.title()
    assert found[0]["flag"] == "FLAG_HIDE_ANTHOLOGY_" + name
    assert walkable(found[0]["x"], found[0]["y"]), name
    assert not any(obj.get("local_id") == f"LOCALID_ANTHOLOGY_{name}"
                   for mapname in ("Route48", "RuinsOfAlph_Outside")
                   for obj in json.loads(source(f"data/maps/{mapname}/map.json"))["object_events"])
assert guardians["ZACIAN"]["y"] == guardians["ZAMAZENTA"]["y"]
assert abs(guardians["ZACIAN"]["x"] - guardians["ZAMAZENTA"]["x"]) == 1

occupied = route["object_events"] + route["warp_events"] + route["coord_events"] + route["bg_events"]
for obj in guardians.values():
    assert not any(other is not obj and other.get("x") == obj["x"] and other.get("y") == obj["y"]
                   for other in occupied)
# Both guardians stand in the same connected walkable region as Tohjo Falls's eastern exit.
start = (route["warp_events"][1]["x"], route["warp_events"][1]["y"])
todo = [start]
seen = set()
while todo:
    x, y = todo.pop()
    if (x, y) in seen or not walkable(x, y):
        continue
    seen.add((x, y))
    todo.extend(((x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)))
assert all((obj["x"], obj["y"]) in seen for obj in guardians.values())

assert "goto_if_unset FLAG_JOHTO_BADGE_8, Route27_RoyalDuoHide" in scripts
assert "goto_if_set FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED, Route27_RoyalDuoHide" in scripts
assert "special RoyalDuoCheckCapacity" in scripts
assert "special RoyalDuoPrepareBattle" in scripts
assert "special BattleSetup_StartRoyalDuoBattle" in scripts
assert "goto_if_ne VAR_RESULT, B_OUTCOME_WON, Route27_RoyalDuoRetry" in scripts
assert "special RoyalDuoAwardVictory" in scripts
assert "setflag FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED" in scripts
assert "removeobject LOCALID_ANTHOLOGY_ZACIAN" in scripts
assert "removeobject LOCALID_ANTHOLOGY_ZAMAZENTA" in scripts
assert "BATTLE_TYPE_DOUBLE | BATTLE_TYPE_LEGENDARY" in battle
assert "FlagClear(FLAG_ANTHOLOGY_ROYAL_DUO_NO_CATCH)" in battle
assert "FLAG_ANTHOLOGY_ROYAL_DUO_NO_CATCH" in cfg
assert all(f"def_special {sym}" in specials for sym in
           ("RoyalDuoCheckCapacity", "RoyalDuoPrepareBattle",
            "RoyalDuoAwardVictory", "BattleSetup_StartRoyalDuoBattle"))
assert all(f"void {fn}(void)" in rewards for fn in
           ("RoyalDuoCheckCapacity", "RoyalDuoPrepareBattle",
            "RoyalDuoAwardVictory"))
assert "sRoyalDuoRewards[2]" in rewards
assert "CopyMon(&sRoyalDuoRewards[i], mon, sizeof(struct Pokemon));" in rewards
assert "CopyMon(&mon, &sRoyalDuoRewards[i], sizeof(struct Pokemon));" in rewards
assert "HealPokemon(&mon);" in rewards
assert "GiveScriptedMonToPlayer(&mon, PARTY_SIZE)" in rewards
assert "freeSlots >= 2" in rewards
assert "SPECIES_ZACIAN_HERO, SPECIES_ZAMAZENTA_HERO" in rewards
assert "ITEM_RUSTED_SWORD, ITEM_RUSTED_SHIELD" in rewards
assert "SetMonMoveSlot(mon, MOVE_IRON_HEAD, 0)" in rewards
assert "FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED" in flags
assert "FLAG_ANTHOLOGY_ROYAL_DUO_NO_CATCH" in flags
# Item definitions and Crowned form-change tables are inherited; no extra loose distribution.
forms = source("src/data/pokemon/form_change_tables.h")
for item, mon in (("ITEM_RUSTED_SWORD", "SPECIES_ZACIAN_CROWNED"),
                  ("ITEM_RUSTED_SHIELD", "SPECIES_ZAMAZENTA_CROWNED")):
    assert mon in forms and item in forms
    for map_path in (root / "data/maps").glob("*/map.json"):
        for obj in json.loads(map_path.read_text()).get("object_events", []):
            assert obj.get("trainer_sight_or_berry_tree_id") != item, (item, map_path)
    for script in (root / "data/maps").glob("*/scripts.inc"):
        text = script.read_text()
        assert f"giveitem {item}" not in text and f"finditem {item}" not in text, (item, script)
print("PASS: pre-League two-sprite Royal Duo, stable rewards, Crowned items, and source access.")
