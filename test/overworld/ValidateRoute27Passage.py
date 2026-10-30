#!/usr/bin/env python3
"""Route 27's narrow League approach must remain open past ambient Pokemon."""

import json
import struct
from collections import deque
from pathlib import Path

root = Path(__file__).resolve().parents[2]
route = json.loads((root / "data/maps/Route27/map.json").read_text())
layouts = json.loads((root / "data/layouts/layouts.json").read_text())["layouts"]
layout = next(entry for entry in layouts if entry["id"] == route["layout"])
raw = (root / layout["blockdata_filepath"]).read_bytes()
width = layout["width"]
height = layout["height"]
assert len(raw) == width * height * 2

def walkable(x, y):
    if not (0 <= x < width and 0 <= y < height):
        return False
    value = struct.unpack_from("<H", raw, 2 * (y * width + x))[0]
    collision = (value >> 10) & 3
    elevation = (value >> 12) & 15
    return collision == 0 and elevation == 3

expected = {"ARBOK": (114, 15), "DODRIO": (117, 15)}
for species, position in expected.items():
    name = f"OBJ_EVENT_GFX_SPECIES({species})"
    candidates = [obj for obj in route["object_events"]
                  if obj["graphics_id"] == name and
                  (obj["x"], obj["y"]) == position]
    assert len(candidates) == 1, f"{species} is missing from its Route 27 side pocket"
    obj = candidates[0]
    assert obj["script"] == "NULL" and obj["flag"] == "0"
    assert obj["movement_type"] == "MOVEMENT_TYPE_FACE_DOWN"
    assert obj["movement_range_x"] == 0 and obj["movement_range_y"] == 0
    assert walkable(*position), f"{species} is on impassable terrain"

blocked = {(obj["x"], obj["y"]) for obj in route["object_events"]}
stairs = {(x, y) for x in (109, 110) for y in (11, 12, 13, 14)}
assert not (blocked & stairs), "An object blocks Route 27's narrow staircase"

start = (109, 15)
goal = (120, 11)
queue = deque([start])
seen = set()
while queue:
    x, y = queue.popleft()
    if (x, y) in seen or (x, y) in blocked or not walkable(x, y):
        continue
    seen.add((x, y))
    queue.extend(((x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)))
assert goal in seen, "Route 27 staircase no longer connects its lower and upper passages"

assert any(obj.get("local_id") == "LOCALID_ANTHOLOGY_ZACIAN"
           for obj in route["object_events"])
assert any(obj.get("local_id") == "LOCALID_ANTHOLOGY_ZAMAZENTA"
           for obj in route["object_events"])

print("PASS: Route 27 Arbok/Dodrio remain on side grass and the League path is open")
