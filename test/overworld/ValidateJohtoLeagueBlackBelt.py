#!/usr/bin/env python3
"""Source regression for Johto's pre-League and rematch doorway."""

import json
from pathlib import Path

root = Path(__file__).resolve().parents[2]

def read(path):
    return (root / path).read_text()

def script_section(source, start, end):
    assert source.count(start) == 1, f"Missing or duplicate {start}"
    return source.split(start, 1)[1].split(end, 1)[0]

center = read("data/maps/JohtoIndigoPlateau_PokemonCenter/scripts.inc")
route = read("data/maps/Route27/scripts.inc")
hall = read("data/maps/JohtoPokemonLeague_HallOfFame/scripts.inc")
data = json.loads(read("data/maps/JohtoIndigoPlateau_PokemonCenter/map.json"))

belt = [obj for obj in data["object_events"]
        if obj["graphics_id"] == "OBJ_EVENT_GFX_BLACK_BELT"
        and (obj["x"], obj["y"]) == (32, 3)]
assert len(belt) == 1
assert belt[0]["flag"] == "FLAG_HIDE_LEAGUE_BLACKBELT"
assert belt[0]["movement_type"] == "MOVEMENT_TYPE_NONE"
assert any((warp["x"], warp["y"]) == (32, 2)
           and warp["dest_map"] == "MAP_JOHTO_POKEMON_LEAGUE_WILLS_ROOM"
           for warp in data["warp_events"])

first_steps = script_section(route, "Route27_EventScript_FirstSteps::",
                             "Route27_EventScript_FirstStepsWalkTop::")
assert "setflag FLAG_HIDE_LEAGUE_BLACKBELT" in first_steps
assert first_steps.index("setflag FLAG_HIDE_LEAGUE_BLACKBELT") < first_steps.index(
    "setvar VAR_ROUTE27_STATE, 1")

transition = script_section(center, "JohtoIndigoPlateau_Pokecenter_OnTransition::",
                            "JohtoIndigoPlateau_EventScript_Cooltrainer::")
assert "setrespawn HEAL_LOCATION_JOHTO_INDIGO_PLATEAU" in transition
assert "call_if_ge VAR_ROUTE27_STATE, 1, JohtoIndigoPlateau_EventScript_HideLeagueBlackBelt" in transition
assert "JohtoIndigoPlateau_EventScript_HideLeagueBlackBelt::\n\tsetflag FLAG_HIDE_LEAGUE_BLACKBELT\n\treturn" in transition
assert "setflag FLAG_HIDE_LEAGUE_BLACKBELT" not in transition.split(
    "JohtoIndigoPlateau_EventScript_HideLeagueBlackBelt::", 1)[0]

assert "clearflag FLAG_HIDE_LEAGUE_BLACKBELT" not in hall
assert "setflag FLAG_JOHTO_CHAMPION" in hall
assert "setvar VAR_DIFFICULTY, DIFFICULTY_HARD" in hall

print("PASS: Route 27 hides the League blocker; room entry repairs old saves; Hall of Fame preserves rematch access")
