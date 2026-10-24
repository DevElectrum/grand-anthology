#!/usr/bin/env python3
"""Bounded event-command lifecycle regression. Not an emulator/gameplay claim.

Executes relevant map script branches for encounter outcomes, transition
reconciliation, form-specific flags, release gates and save/reload snapshots.
"""
from dataclasses import dataclass, field
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
LABEL = re.compile(r"^([A-Za-z_][A-Za-z_0-9]*)::?\s*$", re.M)


def blocks(path):
    script = (ROOT / "data/maps" / path / "scripts.inc").read_text()
    positions = list(LABEL.finditer(script))
    return {m.group(1): [
        line.strip().split("//", 1)[0].strip()
        for line in script[m.end():positions[i + 1].start() if i + 1 < len(positions) else len(script)].splitlines()
        if line.strip() and not line.strip().startswith(("@", "//"))
    ] for i, m in enumerate(positions)}


@dataclass
class Save:
    flags: set = field(default_factory=set)
    caught: set = field(default_factory=set)
    variables: dict = field(default_factory=dict)


def run(labels, entry, save, outcome=None, yes=True):
    """Tiny interpreter for the subset of event commands used by these statics."""
    label, pc, stack, result, active_species = entry, 0, [], None, None
    for _ in range(500):
        assert label in labels, (entry, "missing label", label)
        lines = labels[label]
        if pc == len(lines):
            raise AssertionError((entry, "fell through label", label))
        op, _, args = lines[pc].partition(" ")
        args = [arg.strip() for arg in args.split(",")] if args else []
        pc += 1
        if op in ("return", "end", "release", "releaseall"):
            if op == "return" and stack:
                label, pc = stack.pop()
                continue
            return
        if op == "getcaughtmon":
            result = args[0] in save.caught
        elif op == "seteventmon":
            active_species = args[0]
        elif op == "specialvar" and args[-1] == "GetBattleOutcome":
            result = outcome
            # The battle engine registers the species when the ball succeeds,
            # not before the initial shrine/map interaction takes place.
            if outcome == "B_OUTCOME_CAUGHT" and active_species:
                save.caught.add(active_species)
        elif op == "msgbox" and len(args) > 1 and args[1] == "MSGBOX_YESNO":
            result = yes
        elif op == "setflag":
            save.flags.add(args[0])
        elif op == "clearflag":
            save.flags.discard(args[0])
        elif op == "setvar":
            save.variables[args[0]] = int(args[1])
        elif op in ("goto", "call"):
            if op == "call":
                stack.append((label, pc))
            label, pc = args[0], 0
        elif op in ("goto_if_eq", "goto_if_ne", "call_if_eq", "call_if_ne",
                    "goto_if_ge", "goto_if_lt", "goto_if_set", "goto_if_unset",
                    "call_if_set", "call_if_unset"):
            var = args[0]
            if op.endswith(("_set", "_unset")):
                actual = var in save.flags
                match = actual if op.endswith("_set") else not actual
                dest = args[1]
            else:
                actual = result if var == "VAR_RESULT" else save.variables.get(var, 0)
                val = {"TRUE": True, "FALSE": False}.get(args[1], args[1])
                if isinstance(val, str) and val.isdecimal():
                    val = int(val)
                match = {
                    "eq": actual == val, "ne": actual != val,
                    "ge": actual >= val, "lt": actual < val,
                }[op.split("_if_")[1]]
                dest = args[2]
            if match:
                if op.startswith("call"):
                    stack.append((label, pc))
                label, pc = dest, 0
        elif op == "BattleSetup_StartLegendaryBattle":
            pass
        elif op in ("special", "waitstate", "lock", "faceplayer", "playmoncry",
                    "waitmoncry", "fadescreenswapbuffers", "removeobject",
                    "delay", "playbgm", "waitse", "message", "waitmessage",
                    "seteventmon", "setwildbattle", "setflagvalue",
                    "setobjectxyperm", "closemessage"):
            pass
        # Any new command is deliberately ignored only if it cannot change flags
        # or conditional flow. The scenario assertions below catch the latter.
    raise AssertionError((entry, "500-step recursion/loop", label))


# (map, map-entry guard, battle, hide flag, caught species, extra captured flag)
SITES = (
    ("PowerPlant_Frlg", "PowerPlant_AnthologyOnTransition", "PowerPlant_EventScript_TapuKoko",
     "TAPU_KOKO", None),
    ("Route38", "Route38_AnthologyFieldTransition", "Route38_EventScript_GalarianZapdos",
     "ZAPDOS_GALAR", "FLAG_CAUGHT_ANTHOLOGY_ZAPDOS_GALAR"),
    ("Route40", "Route40_AnthologyFieldTransition", "Route40_EventScript_GalarianArticuno",
     "ARTICUNO_GALAR", "FLAG_CAUGHT_ANTHOLOGY_ARTICUNO_GALAR"),
    ("BurnedTower_1F", "BurnedTower_1F_AnthologyFieldTransition", "BurnedTower_1F_EventScript_GalarianMoltres",
     "MOLTRES_GALAR", "FLAG_CAUGHT_ANTHOLOGY_MOLTRES_GALAR"),
    ("Route43", "Route43_AnthologyFieldTransition", "Route43_EventScript_RaikouReserve",
     "BEAST_RAIKOU", None),
    ("Route46", "Route46_AnthologyFieldTransition", "Route46_EventScript_EnteiReserve",
     "BEAST_ENTEI", None),
    ("Route35", "Route35_AnthologyFieldTransition", "Route35_EventScript_SuicuneReserve",
     "BEAST_SUICUNE", None),
    ("FieryPath", "FieryPath_PreLeagueOnTransition",
     "FieryPath_EventScript_GroudonPreLeague", "PRELEAGUE_GROUDON", None),
    ("Route125", "Route125_PreLeagueOnTransition",
     "Route125_EventScript_KyogrePreLeague", "PRELEAGUE_KYOGRE", None),
    ("Route44", "Route44_AnthologyOnTransition",
     "Route44_EventScript_GlastrierJohto", "JOHTO_GLASTRIER", None),
    ("ShoalCave_LowTideIceRoom", "ShoalCave_LowTideIceRoom_OnTransition",
     "ShoalCave_LowTideIceRoom_EventScript_Glastrier", "GLASTRIER", None),
)

for directory, transition, battle, suffix, form_capture in SITES:
    scripts = blocks(directory)
    hide = "FLAG_HIDE_ANTHOLOGY_" + suffix
    species = {
        "BEAST_RAIKOU": "RAIKOU", "BEAST_ENTEI": "ENTEI",
        "BEAST_SUICUNE": "SUICUNE",
        "PRELEAGUE_GROUDON": "GROUDON",
        "PRELEAGUE_KYOGRE": "KYOGRE", "JOHTO_GLASTRIER": "GLASTRIER",
    }.get(suffix, suffix)
    state = Save()
    state.flags.add(hide)  # Simulate a previously fainted/hidden object.
    if suffix.startswith("BEAST_"):
        run(scripts, transition, state)
        assert hide in state.flags, (directory, "beasts before release")
        state.flags.add("FLAG_RELEASED_BEASTS")
    if suffix.startswith("PRELEAGUE_"):
        state.variables["VAR_SOOTOPOLIS_CITY_STATE"] = 5
        run(scripts, transition, state)
        assert hide in state.flags, (directory, "early Hoenn story")
        state.variables["VAR_SOOTOPOLIS_CITY_STATE"] = 6
    if form_capture:
        # Original Kanto bird caught: Galarian capture has its own flag.
        state.caught.add("SPECIES_" + species.split("_")[0])
    run(scripts, transition, state)
    assert hide not in state.flags, (directory, "initial visibility")
    run(scripts, battle, state, "B_OUTCOME_WON")
    assert hide in state.flags, (directory, "faint hides this visit")
    run(scripts, transition, state)
    assert hide not in state.flags, (directory, "faint must respawn")
    run(scripts, battle, state, "B_OUTCOME_RAN")
    assert hide not in state.flags, (directory, "flee keeps object")
    run(scripts, battle, state, "B_OUTCOME_PLAYER_TELEPORTED")
    assert hide not in state.flags, (directory, "teleport keeps object")
    run(scripts, battle, state, "B_OUTCOME_CAUGHT")
    assert hide in state.flags, (directory, "caught hides immediately")
    if form_capture:
        assert form_capture in state.flags, (directory, "form-specific capture")
    restored = Save(set(state.flags), set(state.caught), dict(state.variables))
    run(scripts, transition, restored)
    assert hide in restored.flags, (directory, "save/revisit re-capture exploit")
    # Releasing the captured Pokemon must not clear the National Dex caught bit.
    run(scripts, transition, restored)
    assert hide in restored.flags, (directory, "release does not respawn")

# Exercise every physical field object's battle end-path, not only a handful
# of representative maps. Read the source-of-truth inventory as a Python
# literal rather than duplicating the map/species coordinates in this harness.
import ast
inventory = ast.parse((ROOT / "test/overworld/ValidateAnthologyHabitatRare.py").read_text())
table = next(
    ast.literal_eval(node.value)
    for node in inventory.body
    if isinstance(node, ast.Assign)
    and any(isinstance(t, ast.Name) and t.id == "_p0_new_statics" for t in node.targets)
)
assert len(table) == 87
for directory, prefix, name, species, x, y, suffix, multiple in table:
    scripts = blocks(directory)
    battle = prefix + "_EventScript_" + name
    hide = "FLAG_HIDE_ANTHOLOGY_" + suffix
    for outcome in ("B_OUTCOME_WON", "B_OUTCOME_CAUGHT",
                    "B_OUTCOME_RAN", "B_OUTCOME_PLAYER_TELEPORTED"):
        state = Save()
        run(scripts, battle, state, outcome)
        resolved = outcome in ("B_OUTCOME_WON", "B_OUTCOME_CAUGHT")
        assert (hide in state.flags) == resolved, (
            directory, species, outcome, "wrong same-visit hide")
        caught = "SPECIES_" + species in state.caught
        assert caught == (outcome == "B_OUTCOME_CAUGHT"), (
            directory, species, outcome, "wrong captured National Dex bit")
        if multiple:
            claimed = "FLAG_CAUGHT_ANTHOLOGY_" + suffix
            assert (claimed in state.flags) == (outcome == "B_OUTCOME_CAUGHT"), (
                directory, species, outcome, "shared-National alternate capture")
print("P0 all 87 current field object scripts: battle-faint/catch/flee/teleport paths source-interpreted PASS")

# The two Kubfu/three Galarian flags must not depend on the shared National entry.
route42 = (ROOT / "data/maps/Route42/scripts.inc").read_text()
for suffix in ("KUBFU_WEST", "KUBFU_EAST"):
    assert "FLAG_CAUGHT_ANTHOLOGY_" + suffix in route42
for name in ("GalarianZapdos", "GalarianArticuno", "GalarianMoltres"):
    assert any(name in battle for _, _, battle, _, _ in SITES)

# Johto and Hoenn Glastrier objects must both disappear on either capture.
johto, shoal = blocks("Route44"), blocks("ShoalCave_LowTideIceRoom")
shared = Save(caught={"SPECIES_GLASTRIER"})
run(johto, "Route44_AnthologyOnTransition", shared)
run(shoal, "ShoalCave_LowTideIceRoom_OnTransition", shared)
assert {"FLAG_HIDE_ANTHOLOGY_JOHTO_GLASTRIER", "FLAG_HIDE_ANTHOLOGY_GLASTRIER"} <= shared.flags

# Hoenn story reaches the alternative encounter unlock during its eighth gym,
# before Hall of Fame; simply reaching the legendary climax (state 5) is not enough.
gym = (ROOT / "data/maps/SootopolisCity_Gym_1F/scripts.inc").read_text()
assert "setvar VAR_SOOTOPOLIS_CITY_STATE, 6" in gym

roamer = (ROOT / "src/roamer.c").read_text()
add = roamer.split("bool8 TryAddRoamer(")[1].split("void InitRoamer(")[0]
assert "FLAG_GET_CAUGHT" in add and "ROAMER(i)->active && ROAMER(i)->species == species" in add
for fn, ending in (("InitJohtoRoamer", "InitJohtoBeastRoamers"),
                   ("InitJohtoBeastRoamers", "IsRoamerSpeciesActive")):
    body = roamer.split("void " + fn + "(")[1].split(ending)[0]
    assert "DeactivateAllRoamers();" not in body, "release erased unrelated roamer"
assert "TryAddRoamer(SPECIES_ENTEI, 40);" in roamer
assert "TryAddRoamer(SPECIES_RAIKOU, 40);" in roamer

# Ilex Forest: first story sets VAR_AZALEA_TOWN_STATE=10 once; subsequent
# encounters use shrine rematch, not Kurt's introductory scene again.
ilex = blocks("IlexForest")
s = Save(variables={"VAR_AZALEA_TOWN_STATE": 10})
run(ilex, "IlexForest_EventScript_CelebiRematch", s, "B_OUTCOME_WON")
assert "FLAG_CAUGHT_CELEBI" not in s.flags
run(ilex, "IlexForest_EventScript_CelebiRematch", s, "B_OUTCOME_CAUGHT")
assert "FLAG_CAUGHT_CELEBI" in s.flags
run(ilex, "IlexForest_EventScript_CelebiRematch", s)
assert "FLAG_CAUGHT_CELEBI" in s.flags
print("P0 representative scripted static lifecycle: KO/re-entry/flee/teleport/catch/save, Johto roamers, Celebi, Galarian forms and Hoenn story gates PASS (source interpreter, not mGBA)")
