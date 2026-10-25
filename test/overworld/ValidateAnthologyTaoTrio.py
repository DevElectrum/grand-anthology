#!/usr/bin/env python3
"""Source-level checks only: map geometry, encounters, gates and Splicers."""
from pathlib import Path
import json
import struct

ROOT = Path(__file__).resolve().parents[2]
MAPS = ROOT / "data/maps"
LAYOUTS = {entry["id"]: entry for entry in json.loads((ROOT / "data/layouts/layouts.json").read_text())["layouts"]}
SITES = (
    ("TinTower_2F", "Reshiram", "RESHIRAM", (9, 8), (9, 4)),
    ("DarkCave_NorthSide", "Zekrom", "ZEKROM", (40, 13), (45, 9)),
    ("IcePath_B1F", "Kyurem", "KYUREM", (8, 10), (9, 20)),
)

for directory, name, species, coord, entrance in SITES:
    map_data = json.loads((MAPS / directory / "map.json").read_text())
    script = (MAPS / directory / "scripts.inc").read_text()
    objs = [e for e in map_data["object_events"] if e["script"] == f"{directory}_EventScript_{name}"]
    assert len(objs) == 1, (directory, "duplicate or missing object")
    o = objs[0]
    assert (o["x"], o["y"], o["elevation"]) == (*coord, 3)
    assert o["flag"] == f"FLAG_HIDE_ANTHOLOGY_{species}"
    assert o["graphics_id"] == f"OBJ_EVENT_GFX_SPECIES({species})"
    assert f"getcaughtmon SPECIES_{species}" in script
    assert f"seteventmon SPECIES_{species}" in script
    assert f"call {directory}_AnthologyOnTransition" in script
    layout = LAYOUTS[map_data["layout"]]
    w, h = layout["width"], layout["height"]
    tiles = (ROOT / layout["blockdata_filepath"]).read_bytes()
    assert len(tiles) == w * h * 2
    def walk(x, y):
        if not (0 <= x < w and 0 <= y < h):
            return False
        value = struct.unpack_from("<H", tiles, 2 * (y * w + x))[0]
        return (value >> 12) == 3 and ((value >> 10) & 3) == 0
    assert walk(*coord), (directory, "blocked tile")
    x, y = coord
    assert sum(walk(x + dx, y + dy) for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))) >= 2
    assert any((e["x"], e["y"]) == entrance for e in map_data["warp_events"])
    seen, todo = set(), [entrance]
    while todo:
        xy = todo.pop()
        if xy in seen or not walk(*xy):
            continue
        seen.add(xy)
        x, y = xy
        todo += [(x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)]
    assert coord in seen, (directory, "unreachable from natural entry warp")
    assert f"goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, {directory}_Anthology{name}" in script
    assert f"goto_if_eq VAR_RESULT, B_OUTCOME_WON, {directory}_Anthology{name}Resolved" in script
    assert "B_OUTCOME_RAN" in script and "B_OUTCOME_PLAYER_TELEPORTED" in script

for old, species in (("Route45", "RESHIRAM"), ("Route45", "ZEKROM"), ("Route44", "KYUREM")):
    j = json.loads((MAPS / old / "map.json").read_text())
    src = (MAPS / old / "scripts.inc").read_text()
    assert not any(e["graphics_id"] == f"OBJ_EVENT_GFX_SPECIES({species})" for e in j["object_events"])
    assert f"getcaughtmon SPECIES_{species}" not in src
    assert f"seteventmon SPECIES_{species}" not in src

ice = (MAPS / "IcePath_B1F/scripts.inc").read_text()
gate = ice.split("IcePath_B1F_AnthologyOnTransition::")[1].split("IcePath_B1F_AnthologyHideKyurem::")[0]
assert gate.index("getcaughtmon SPECIES_KYUREM") < gate.index("getcaughtmon SPECIES_RESHIRAM") < gate.index("getcaughtmon SPECIES_ZEKROM") < gate.index("clearflag FLAG_HIDE_ANTHOLOGY_KYUREM")
assert gate.count("goto_if_ne VAR_RESULT, TRUE, IcePath_B1F_AnthologyHideKyurem") == 2
caught = ice.split("IcePath_B1F_AnthologyKyuremCaught::")[1].split("IcePath_B1F_AnthologyKyuremRetreated::")[0]
assert "giveitem ITEM_DNA_SPLICERS" in caught and "checkitem ITEM_DNA_SPLICERS" in caught
assert "giveitem ITEM_DNA_SPLICERS" not in ice.split("IcePath_B1F_AnthologyKyuremResolved::")[1].split("IcePath_B1F_AnthologyKyuremCaught::")[0]
plinth = json.loads((MAPS / "IcePath_B1F/map.json").read_text())["bg_events"]
assert sum(e["script"] == "IcePath_B1F_EventScript_DNASplicersPlinth" for e in plinth) == 1
assert "getcaughtmon SPECIES_KYUREM" in ice.split("IcePath_B1F_EventScript_DNASplicersPlinth::")[1]
roof = (MAPS / "TinTower_RoofDay/scripts.inc").read_text()
sage = (MAPS / "EcruteakCity_SageOffice1/scripts.inc").read_text()
tower = (MAPS / "TinTower_2F/scripts.inc").read_text()
assert "checkitem ITEM_CLEAR_BELL" in sage
assert all(f"getcaughtmon SPECIES_{m}" in roof for m in ("RAIKOU", "ENTEI", "SUICUNE"))
assert all(f"SPECIES_{m}" not in tower for m in ("RAIKOU", "ENTEI", "SUICUNE"))
items = (ROOT / "src/data/items.h").read_text()
fusions = (ROOT / "src/data/pokemon/form_change_tables.h").read_text()
assert "[ITEM_DNA_SPLICERS]" in items and ".fieldUseFunc = ItemUseOutOfBattle_Fusion" in items
assert "SPECIES_KYUREM_WHITE" in fusions and "SPECIES_KYUREM_BLACK" in fusions
print("PASS: Tao Trio source gates, paths, captures and DNA Splicers")
