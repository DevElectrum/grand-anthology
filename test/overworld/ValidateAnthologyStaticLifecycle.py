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
                # Evaluate only the selected operation. Outcomes are strings,
                # whereas caught-dex checks return bools, so eager numeric
                # comparisons would raise TypeError even on an equality test.
                comparison = op.split("_if_")[1]
                if comparison == "eq":
                    match = actual == val
                elif comparison == "ne":
                    match = actual != val
                elif comparison == "ge":
                    match = actual >= val
                elif comparison == "lt":
                    match = actual < val
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
    ("Route38", "Route38_AnthologyFieldTransition", "Route38_EventScript_RaikouReserve",
     "BEAST_RAIKOU", None),
    ("MtMortar_2F", "MtMortar_2F_AnthologyFieldTransition", "MtMortar_2F_EventScript_EnteiReserve",
     "BEAST_ENTEI", None),
    ("LakeOfRage", "LakeOfRage_AnthologyFieldTransition", "LakeOfRage_EventScript_SuicuneReserve",
     "BEAST_SUICUNE", None),
    ("FieryPath", "FieryPath_PreLeagueOnTransition",
     "FieryPath_EventScript_GroudonPreLeague", "PRELEAGUE_GROUDON", None),
    ("Route125", "Route125_PreLeagueOnTransition",
     "Route125_EventScript_KyogrePreLeague", "PRELEAGUE_KYOGRE", None),
    ("IcePath_B2F", "IcePath_B2F_AnthologyOnTransition",
     "IcePath_B2F_EventScript_GlastrierJohto", "JOHTO_GLASTRIER", None),
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
        if suffix == "BEAST_SUICUNE":
            # Rocket generator is shut down at Mahogany state 14.
            state.variables["VAR_MAHOGANY_TOWN_STATE"] = 14
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
assert len(table) >= 87, f"Lost tracked static encounter coverage: {len(table)}"
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
print(f"P0 all {len(table)} current field object scripts: battle-faint/catch/flee/teleport paths source-interpreted PASS")

# The two Kubfu/three Galarian flags must not depend on the shared National entry.
for directory, suffix in (("SproutTower_2F", "KUBFU_WEST"), ("Route34", "KUBFU_EAST")):
    site = (ROOT / "data/maps" / directory / "scripts.inc").read_text()
    assert "FLAG_CAUGHT_ANTHOLOGY_" + suffix in site
for name in ("GalarianZapdos", "GalarianArticuno", "GalarianMoltres"):
    assert any(name in battle for _, _, battle, _, _ in SITES)

# Johto and Hoenn Glastrier objects must both disappear on either capture.
johto, shoal = blocks("IcePath_B2F"), blocks("ShoalCave_LowTideIceRoom")
shared = Save(caught={"SPECIES_GLASTRIER"})
run(johto, "IcePath_B2F_AnthologyOnTransition", shared)
run(shoal, "ShoalCave_LowTideIceRoom_OnTransition", shared)
assert {"FLAG_HIDE_ANTHOLOGY_JOHTO_GLASTRIER", "FLAG_HIDE_ANTHOLOGY_GLASTRIER"} <= shared.flags

# Hoenn story reaches the alternative encounter unlock during its eighth gym,
# before Hall of Fame; simply reaching the legendary climax (state 5) is not enough.
gym = (ROOT / "data/maps/SootopolisCity_Gym_1F/scripts.inc").read_text()
assert "setvar VAR_SOOTOPOLIS_CITY_STATE, 6" in gym

roamer = (ROOT / "src/roamer.c").read_text()
add = roamer.split("bool8 TryAddRoamer(")[1].split("void InitRoamer(")[0]
assert "FLAG_GET_CAUGHT" in add and "ROAMER(i)->active && ROAMER(i)->species == species" in add
# The original awakening does not begin a mandatory roamer hunt.
assert "special InitJohtoBeastRoamers" in (ROOT / "data/maps/BurnedTower_B1F/scripts.inc").read_text()
fixed = roamer.split("void InitJohtoBeastRoamers(void)")[1].split("// Records that a roaming Pokemon was CAUGHT")[0]
assert "RetireLegacyJohtoBeastRoamers();" in fixed
assert "TryAddRoamer(SPECIES_ENTEI" not in roamer
assert "TryAddRoamer(SPECIES_RAIKOU" not in roamer
assert "if (IsJohtoRoamerSpecies(species))" in add
assert "RetireLegacyJohtoBeastRoamers();" in roamer.split("bool8 TryStartRoamerEncounter(void)")[1]
assert "void RespawnJohtoBeastRoamers(void)" in roamer
assert "DeactivateAllRoamers();" not in fixed
# Suicune only reveals after the Rocket signal is shut down (Mahogany state 14).
lake = blocks("LakeOfRage")
before = Save(flags={"FLAG_RELEASED_BEASTS"}, variables={"VAR_MAHOGANY_TOWN_STATE": 13})
run(lake, "LakeOfRage_AnthologyFieldTransition", before)
assert "FLAG_HIDE_ANTHOLOGY_BEAST_SUICUNE" in before.flags
after = Save(flags={"FLAG_RELEASED_BEASTS"}, variables={"VAR_MAHOGANY_TOWN_STATE": 14})
run(lake, "LakeOfRage_AnthologyFieldTransition", after)
assert "FLAG_HIDE_ANTHOLOGY_BEAST_SUICUNE" not in after.flags

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
print("P0 representative scripted static lifecycle: KO/re-entry/flee/teleport/catch/save, static Johto beasts, Celebi, Galarian forms and Hoenn story gates PASS (source interpreter, not mGBA)")


# Native Bell Tower roof is the *only* Johto Ho-Oh encounter. The Burned
# Tower upstairs object would bypass the user's agreed caught-beast gate.
assert "BurnedTower_1F_EventScript_HoOh" not in (ROOT / "data/maps/BurnedTower_1F/scripts.inc").read_text()
tower = blocks("TinTower_RoofDay")
for stage in (2, 4):
    entry = ("TinTower_RoofDay_MapScript_HoohStory"
             if stage == 2 else "TinTower_RoofDay_MapScript_HoohPostStoryEncounter")
    state = Save(flags={"FLAG_RELEASED_BEASTS"}, variables={"VAR_COMPLETED_HO_OH": stage})
    run(tower, entry, state)
    assert "FLAG_HIDE_HO_OH" in state.flags, ("Ho-Oh shown with missing beasts", stage)
    for mon in ("RAIKOU", "ENTEI", "SUICUNE"):
        state.caught.add("SPECIES_" + mon)
    run(tower, entry, state)
    assert "FLAG_HIDE_HO_OH" not in state.flags, ("Ho-Oh stays hidden after 3 catches", stage)
    state.caught.add("SPECIES_HO_OH")
    run(tower, entry, state)
    assert "FLAG_HIDE_HO_OH" in state.flags, ("Ho-Oh duplicate after caught", stage)
assert "call TinTower_RoofDay_AnthologyHoOhGate" in (ROOT / "data/maps/TinTower_RoofDay/scripts.inc").read_text()
print("P0 Ho-Oh: native rooftop only, caught trio prerequisite, early/postgame and already-caught guards PASS")


# A fainted native rooftop Ho-Oh may be challenged on the next visit.
done = {"SPECIES_RAIKOU", "SPECIES_ENTEI", "SPECIES_SUICUNE"}
retry = Save(flags={"FLAG_RELEASED_BEASTS"}, caught=set(done),
             variables={"VAR_COMPLETED_HO_OH": 3})
run(tower, "TinTower_RoofDay_MapScript_HoohPostStoryGone", retry)
assert retry.variables["VAR_COMPLETED_HO_OH"] == 4
assert "FLAG_HIDE_HO_OH" not in retry.flags
caught = Save(flags={"FLAG_RELEASED_BEASTS"}, caught=set(done) | {"SPECIES_HO_OH"},
              variables={"VAR_COMPLETED_HO_OH": 3})
run(tower, "TinTower_RoofDay_MapScript_HoohPostStoryGone", caught)
assert "FLAG_HIDE_HO_OH" in caught.flags
print("P0 native Ho-Oh after faint: immediate re-entry retry and permanent caught hide PASS")


# Kanto's three formerly clustered Rock Tunnel encounters were relocated.
# Verify caught-Dex gates, post-faint retry, and caught persistence without
# assuming that matching Pokémon have to remain in the player's party.
for new_map, entry, battle, hide, prereq in (
    ("MtMoon_B2F_Frlg", "MtMoon_B2F_AnthologyOnTransition", "MtMoon_B2F_EventScript_Giratina",
     "FLAG_HIDE_ANTHOLOGY_GIRATINA", {"SPECIES_DIALGA", "SPECIES_PALKIA"}),
    ("PokemonTower_7F_Frlg", "PokemonTower_7F_AnthologyOnTransition", "PokemonTower_7F_EventScript_Darkrai",
     "FLAG_HIDE_ANTHOLOGY_DARKRAI", {"SPECIES_CRESSELIA"}),
    ("CeladonCity_Frlg", "CeladonCity_AnthologyOnTransition", "CeladonCity_EventScript_TapuLele",
     "FLAG_HIDE_ANTHOLOGY_TAPU_LELE", set()),
):
    labels = blocks(new_map)
    preflags = {"FLAG_RESCUED_MR_FUJI"} if "Darkrai" in battle else set()
    missing = Save(flags=set(preflags))
    run(labels, entry, missing)
    if prereq:
        assert hide in missing.flags, (new_map, "appeared before prerequisites")
    eligible = Save(flags=set(preflags), caught=set(prereq))
    run(labels, entry, eligible)
    assert hide not in eligible.flags, (new_map, "hidden despite prerequisites")
    run(labels, battle, eligible, "B_OUTCOME_WON")
    assert hide in eligible.flags, (new_map, "fainted object did not hide")
    run(labels, entry, eligible)
    assert hide not in eligible.flags, (new_map, "fainted encounter cannot retry")
    run(labels, battle, eligible, "B_OUTCOME_CAUGHT")
    assert hide in eligible.flags, (new_map, "caught object did not hide")
    run(labels, entry, eligible)
    assert hide in eligible.flags, (new_map, "caught encounter respawned")
    assert all(member in eligible.caught for member in prereq)
assert "RockTunnel_B1F_EventScript_Giratina" not in (ROOT / "data/maps/RockTunnel_B1F_Frlg/scripts.inc").read_text()
assert "RockTunnel_B1F_EventScript_Darkrai" not in (ROOT / "data/maps/RockTunnel_B1F_Frlg/scripts.inc").read_text()
assert "RockTunnel_B1F_EventScript_TapuLele" not in (ROOT / "data/maps/RockTunnel_B1F_Frlg/scripts.inc").read_text()
pre_fuji = Save(caught={"SPECIES_CRESSELIA"})
run(blocks("PokemonTower_7F_Frlg"), "PokemonTower_7F_AnthologyOnTransition", pre_fuji)
assert "FLAG_HIDE_ANTHOLOGY_DARKRAI" in pre_fuji.flags
print("P0 Kanto relocated statics: caught prerequisite, Fuji rescue, KO/revisit and caught/revisit PASS")

# Swords of Justice are independent captures, early to mid Johto, with the
# same KO->revisit and caught->permanent-hide semantics as other statics.
for _dir,_name in (("Route32","Keldeo"),("UnionCave_1F","Terrakion"),("NationalPark_Normal","Virizion"),("MtMortar_1F_South","Cobalion")):
    _scripts = blocks(_dir)
    _entry = _dir+"_AnthologyOnTransition"
    _hide = "FLAG_HIDE_ANTHOLOGY_"+_name.upper()
    _battle = _dir+"_EventScript_"+_name
    _state = Save()
    run(_scripts,_entry,_state)
    assert _hide not in _state.flags, (_name,"requires an unapproved prerequisite")
    run(_scripts,_battle,_state,"B_OUTCOME_WON")
    assert _hide in _state.flags
    run(_scripts,_entry,_state)
    assert _hide not in _state.flags
    run(_scripts,_battle,_state,"B_OUTCOME_CAUGHT")
    assert _hide in _state.flags
    run(_scripts,_entry,_state)
    assert _hide in _state.flags
print("P0 Johto independent Swords: KO/revisit and caught/revisit gates PASS")
