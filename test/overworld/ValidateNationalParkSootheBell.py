#!/usr/bin/env python3
"""National Park: Soothe Bell must stay reachable despite ambient Pokémon patrols.

The September 2026 upstream placements put Nidoran F and Weedle on consecutive
one-tile chokepoints after Cut. This checks the actual map collision/elevation
and conservatively reserves every tile of BOTH species' horizontal patrols.
"""

import json
import struct
from collections import deque
from pathlib import Path

root = Path(__file__).resolve().parents[2]
park = json.loads((root / "data/maps/NationalPark_Normal/map.json").read_text())
layouts = json.loads((root / "data/layouts/layouts.json").read_text())["layouts"]
layout = next(item for item in layouts if item["id"] == park["layout"])
width, height = layout["width"], layout["height"]
raw = (root / layout["blockdata_filepath"]).read_bytes()
assert len(raw) == width * height * 2
assert (width, height) == (48, 56)

def walkable(x, y):
    if not (0 <= x < width and 0 <= y < height):
        return False
    value = struct.unpack_from("<H", raw, 2 * (y * width + x))[0]
    return (value >> 10) & 3 == 0 and value >> 12 == 3

def find_event(graphics, position):
    matches = [obj for obj in park["object_events"]
               if obj["graphics_id"] == graphics
               and (obj["x"], obj["y"]) == position]
    assert len(matches) == 1, f"Missing or duplicate {graphics} at {position}"
    return matches[0]

item = find_event("OBJ_EVENT_GFX_POKE_BALL", (39, 15))
assert item["script"] == "NationalPark_Normal_EventScript_Item_Soothe_Bell"
assert item["flag"] == "FLAG_ITEM_NATIONAL_PARK_SOOTHE_BELL"
cut_tree = find_event("OBJ_EVENT_GFX_CUTTABLE_TREE", (26, 3))
assert cut_tree["script"] == "EventScript_CutTree"

nidoran = find_event("OBJ_EVENT_GFX_SPECIES(NIDORAN_F)", (28, 10))
weedle = find_event("OBJ_EVENT_GFX_SPECIES(WEEDLE)", (29, 12))
assert nidoran["movement_type"] == "MOVEMENT_TYPE_WALK_RIGHT_AND_LEFT"
assert weedle["movement_type"] == "MOVEMENT_TYPE_WALK_LEFT_AND_RIGHT"

ambient = (nidoran, weedle)
patrol = set()
for obj in ambient:
    assert obj["script"] == "NULL" and str(obj["flag"]) == "0"
    assert obj["movement_range_x"] == 2 and obj["movement_range_y"] == 0
    for x in range(obj["x"] - 2, obj["x"] + 3):
        pos = (x, obj["y"])
        assert walkable(*pos), f"Ambient Pokémon patrol crosses a wall: {pos}"
        patrol.add(pos)

# Conservatively treat other map objects as occupied, including seasonal
# Pokémon and both independent scripted legendary encounters. Cut is cleared.
# Lamp/light sprites are visual overlays, not movement obstructions.
other_objects = {
    (obj["x"], obj["y"])
    for obj in park["object_events"]
    if obj not in ambient
    and obj is not cut_tree
    and obj["graphics_id"] != "OBJ_EVENT_GFX_LIGHT_SPRITE"
}
assert not patrol & other_objects, "Ambient patrol overlaps another object"
assert (26, 3) not in patrol, "Cut tree approach has been blocked again"
assert (39, 14) not in patrol, "Soothe Bell pickup approach blocked again"

def reachable(start, goal, occupied):
    queue = deque([start])
    seen = set()
    while queue:
        x, y = queue.popleft()
        pos = (x, y)
        if pos in seen or pos in occupied or not walkable(x, y):
            continue
        if pos == goal:
            return True
        seen.add(pos)
        queue.extend(((x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)))
    return False

# This makes the regression discriminating: the actual PRE-FIX placement
# fails to reach the pickup even if every other park event is unchanged.
assert not reachable((12, 48), (39, 14), other_objects | {(32, 4), (33, 5)})
assert not reachable((12, 48), (39, 14), other_objects | {(33, 5)})

worst_case = other_objects | patrol
assert reachable((12, 48), (26, 4), worst_case), "Southern visitor cannot approach Cut"
assert reachable((26, 4), (39, 14), worst_case), "Soothe Bell sealed beyond cleared Cut"
assert reachable((12, 48), (39, 14), worst_case), "Soothe Bell sealed from south gate"
assert reachable((40, 19), (39, 14), worst_case), "Soothe Bell sealed from east gate"

print("PASS: National Park's Soothe Bell is accessible after Cut throughout both full patrol ranges")
