#!/usr/bin/env python3
"""Regression: Route 44's Ice Path approach must remain open."""

import json
from pathlib import Path

root = Path(__file__).resolve().parents[2]
route44 = json.loads((root / "data/maps/Route44/map.json").read_text())
ice_path = json.loads((root / "data/maps/IcePath_1F/map.json").read_text())

warps = [warp for warp in route44["warp_events"]
         if warp["dest_map"] == "MAP_ICE_PATH_1F"]
assert len(warps) == 1, "Expected one Route 44 entrance to Ice Path"

entrance = warps[0]
assert (entrance["x"], entrance["y"]) == (68, 12)
assert str(entrance["dest_warp_id"]) == "0"

return_warp = ice_path["warp_events"][0]
assert return_warp["dest_map"] == "MAP_ROUTE44"
assert str(return_warp["dest_warp_id"]) == "0"

occupied = {(obj["x"], obj["y"]) for obj in route44["object_events"]}
assert (68, 13) not in occupied, "Ice Path approach (68, 13) is blocked"
assert (68, 12) not in occupied, "Ice Path warp (68, 12) is blocked"

print("PASS: Route 44's Ice Path entrance has an open approach")
