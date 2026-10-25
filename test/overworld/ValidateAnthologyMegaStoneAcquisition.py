#!/usr/bin/env python3
"""Source-level Mega Stone acquisition coverage. Does not claim map reachability or mGBA acceptance."""
from pathlib import Path
from collections import defaultdict
import json
import re

ROOT = Path(__file__).resolve().parents[2]
ITEMS = (ROOT / "include/constants/items.h").read_text()
BATTLE = (ROOT / "src/battle_net.c").read_text()
FLAGS = (ROOT / "include/constants/flags.h").read_text()
MAPS = ROOT / "data/maps"

roster = {}
for name, number in re.findall(r"\bITEM_([A-Z_0-9]+)\s*=\s*(\d+)", ITEMS):
    number = int(number)
    if 292 <= number <= 338 or 829 <= number <= 873:
        roster[f"ITEM_{name}"] = number

assert len(roster) == 92, ("defined Mega Stones", len(roster))
assert set(roster.values()) == set(range(292, 339)) | set(range(829, 874))

def table(name):
    match = re.search(r"static const [^\n]*\b" + name + r"\[\] =\s*\{([\s\S]*?)\n\};", BATTLE)
    assert match, ("missing table", name)
    return match.group(1)

def stones_in(data):
    return {item for item in re.findall(r"\bITEM_[A-Z_0-9]+\b", data) if item in roster}

vendor = stones_in(table("sVendorStones"))
rematches = stones_in(table("sBattleNetRewards"))
starters = stones_in(table("sStarterStones"))
assert len(vendor) == 21, ("vendor", len(vendor))
assert len(rematches) == 30, ("signature rematches", len(rematches))
assert len(starters) == 13 and starters <= vendor, ("free starter choices", len(starters))
assert not vendor & rematches, "Original rematch rewards must not be removed or reused in the vendor"

layout_data = json.loads((ROOT / "data/layouts/layouts.json").read_text())["layouts"]
layouts = {layout["id"]: layout for layout in layout_data}
world = defaultdict(list)
used_flags = set()
specials = {
    "ITEM_ZYGARDITE": "FLAG_CAUGHT_ANTHOLOGY_ZYGARDE_COMPLETE",
    "ITEM_HEATRANITE": "getcaughtmon SPECIES_HEATRAN",
    "ITEM_DARKRANITE": "getcaughtmon SPECIES_DARKRAI",
    "ITEM_ZERAORITE": "getcaughtmon SPECIES_ZERAORA",
    "ITEM_MAGEARNITE": "FLAG_GOT_LAPRAS_FROM_SILPH",
}

for map_path in MAPS.glob("*/map.json"):
    info = json.loads(map_path.read_text())
    for obj in info.get("object_events", []):
        item = obj.get("trainer_sight_or_berry_tree_id")
        if item not in roster or not str(obj.get("flag", "")).startswith("FLAG_ANTHOLOGY_MEGA_STONE_"):
            continue
        assert obj["graphics_id"] == "OBJ_EVENT_GFX_ITEM_BALL", (map_path, item, "not a visible item ball")
        flag = obj["flag"]
        assert flag not in used_flags, ("duplicate flag", flag)
        used_flags.add(flag)
        assert flag in FLAGS, (map_path, flag, "undefined persistent flag")
        x, y = obj["x"], obj["y"]
        layout = layouts[info["layout"]]
        assert 0 <= x < layout["width"] and 0 <= y < layout["height"], (map_path, item, "outside layout")
        for other in info["object_events"] + info.get("warp_events", []) + info.get("coord_events", []) + info.get("bg_events", []):
            if other is obj:
                continue
            if other.get("x") == x and other.get("y") == y:
                raise AssertionError((map_path, item, "overlapping event", x, y))
        script_name = obj["script"]
        if item in specials:
            assert script_name == "Anthology_MegaStone_" + item[5:], (map_path, item, script_name)
            script = (map_path.parent / "scripts.inc").read_text()
            assert script_name + "::" in script, (map_path, item, "missing script")
            assert specials[item] in script, (map_path, item, "missing unlock")
            assert f"finditem {item}" in script, (map_path, item, "missing persistent pickup")
        else:
            assert script_name == "Common_EventScript_FindItem", (map_path, item, script_name)
        world[item].append(f"{map_path.parent.name}@({x},{y})")

assert len(world) == 41, ("world pickup count", len(world))
assert len(used_flags) == 41, ("unique world flags", len(used_flags))
assert all(len(locations) == 1 for locations in world.values()), "Unexpected duplicate new world sources"
for i in range(41):
    assert f"FLAG_ANTHOLOGY_MEGA_STONE_{i:02}" in used_flags, ("missing world flag", i)

coverage = set(vendor) | set(rematches) | set(world)
missing = sorted(set(roster) - coverage)
assert not missing, ("Mega Stones without real acquisition sources", missing)
assert len(coverage) == 92, ("total covered", len(coverage))

assert all(item in world for item in specials), ("missing thematic rewards", specials)
assert len({item for item in vendor if roster[item] >= 829}) == 11
assert len({item for item in world if roster[item] >= 829}) == 34
assert len({item for item in world if roster[item] < 829}) == 7
print("PASS: 92/92 Mega Stones have source-level real-game acquisition coverage.")
print("       30 rematch, 21 vendor, 41 distinct world (29 normal Z-A, 5 special Z-A, 7 older).")
print("       13 free starter choices are additional, never exclusive.")
print("       NOTE: this checks scripts, ids, flags, map bounds and event collisions,")
print("             NOT walkable geometry or actual emulator/save-persistence acceptance.")
