#!/usr/bin/env python3
"""Johto Eternatus Victory Road chamber source contract; emulator acceptance remains separate."""
from pathlib import Path
import json
import struct

root = Path(__file__).resolve().parents[2]

def read(path):
    return (root / path).read_text()

def events(map_name):
    return json.loads(read("data/maps/" + map_name + "/map.json"))

def layout(map_info):
    for value in json.loads(read("data/layouts/layouts.json"))["layouts"]:
        if value["id"] == map_info["layout"]:
            return value
    raise AssertionError("missing layout " + map_info["layout"])

def walkable(layout_info, x, y):
    width = layout_info["width"]
    height = layout_info["height"]
    if not (0 <= x < width and 0 <= y < height):
        return False
    raw = (root / layout_info["blockdata_filepath"]).read_bytes()
    assert len(raw) == width * height * 2
    tile = struct.unpack_from("<H", raw, 2 * (y * width + x))[0]
    return ((tile >> 10) & 3) == 0

def connected(layout_info, start):
    seen = set()
    todo = [start]
    while todo:
        x, y = todo.pop()
        if (x, y) in seen or not walkable(layout_info, x, y):
            continue
        seen.add((x, y))
        todo.extend(((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)))
    return seen

parent = events("JohtoVictoryRoad_1F")
chamber = events("JohtoEternatusChamber")
old = events("Route44")
parent_script = read("data/maps/JohtoVictoryRoad_1F/scripts.inc")
chamber_script = read("data/maps/JohtoEternatusChamber/scripts.inc")
old_script = read("data/maps/Route44/scripts.inc")
flags = read("include/constants/flags.h")
groups = json.loads(read("data/maps/map_groups.json"))["gMapGroup_JohtoLeague"]
assembly = read("data/event_scripts.s")

assert groups[-1] == "JohtoEternatusChamber", "Append new map to preserve all prior map indices"
assert "data/maps/JohtoEternatusChamber/scripts.inc" in assembly
assert all(obj.get("local_id") != "LOCALID_ANTHOLOGY_ETERNATUS" for obj in old["object_events"])
assert "Route44_EventScript_Eternatus" not in old_script
assert "Route44_TornadusOnTransition" in old_script
assert len(parent["warp_events"]) == 5
assert parent["warp_events"][1]["dest_map"] == "MAP_JOHTO_INDIGO_PLATEAU"
assert (parent["warp_events"][1]["x"], parent["warp_events"][1]["y"]) == (39, 5)

entrances = [obj for obj in parent["object_events"]
             if obj.get("local_id") == "LOCALID_ANTHOLOGY_ETERNATUS_SEAL"]
assert len(entrances) == 1
entrance = entrances[0]
assert (entrance["x"], entrance["y"]) == (35, 4)
assert entrance["script"] == "JohtoVictoryRoad_1F_EventScript_EternatusSeal"
assert "goto_if_unset FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED, JohtoVictoryRoad_1F_EternatusStillSealed" in parent_script
assert "warp MAP_JOHTO_ETERNATUS_CHAMBER, 7, 6" in parent_script
assert "FLAG_SYS_GAME_CLEAR" not in parent_script[parent_script.index("JohtoVictoryRoad_1F_EventScript_EternatusSeal::"):]

parent_tiles = layout(parent)
reachable = connected(parent_tiles, (39, 5))
assert (35, 4) in reachable and (35, 5) in reachable, "League-side entrance must be accessible"
assert not any(e.get("x") == 35 and e.get("y") == 4
               for e in parent["object_events"] + parent["warp_events"]
               + parent["bg_events"] + parent["coord_events"] if e is not entrance)

assert chamber["id"] == "MAP_JOHTO_ETERNATUS_CHAMBER"
assert chamber["region"] == "REGION_JOHTO"
assert chamber["layout"] == "LAYOUT_GRANITE_CAVE_STEVENS_ROOM"
assert len(chamber["warp_events"]) == 1
assert chamber["warp_events"][0]["dest_map"] == "MAP_JOHTO_VICTORY_ROAD_1F"
assert chamber["warp_events"][0]["dest_warp_id"] == "1"
mons = [obj for obj in chamber["object_events"]
        if obj.get("local_id") == "LOCALID_ANTHOLOGY_ETERNATUS"]
assert len(mons) == 1
assert (mons[0]["x"], mons[0]["y"]) == (10, 9)
assert mons[0]["graphics_id"] == "OBJ_EVENT_GFX_SPECIES(ETERNATUS)"
assert mons[0]["flag"] == "FLAG_HIDE_ANTHOLOGY_ETERNATUS"
room_tiles = layout(chamber)
room_reachable = connected(room_tiles, (7, 6))
assert (7, 3) in room_reachable and (10, 9) in room_reachable
assert "goto_if_unset FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED, JohtoEternatusChamber_Hide" in chamber_script
assert "goto_if_unset FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED, JohtoEternatusChamber_NotReady" in chamber_script
assert "seteventmon SPECIES_ETERNATUS, 55" in chamber_script
assert "getcaughtmon SPECIES_ETERNATUS" in chamber_script
assert "callnative AnthologyStaticRandomizerIsOn" in chamber_script
assert "FLAG_CAUGHT_RANDOM_ROUTE44_ETERNATUS" in chamber_script
assert "setflag FLAG_CAUGHT_RANDOM_ROUTE44_ETERNATUS" in chamber_script
assert "goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT" in chamber_script
assert "goto_if_eq VAR_RESULT, B_OUTCOME_WON" in chamber_script
assert "goto_if_eq VAR_RESULT, B_OUTCOME_RAN" in chamber_script
assert "goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED" in chamber_script
assert "#define FLAG_CAUGHT_RANDOM_ROUTE44_ETERNATUS " in flags
assert "#define FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED " in flags
print("PASS: Royal Duo-gated, walkable Eternatus side chamber; original Route 44 retired.")
print("NOTE: source contract only; real emulator capture/return/save still pending.")
