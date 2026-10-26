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
    seeded: bool = False


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
        elif op == "callnative" and args[0] == "AnthologyStaticRandomizerIsOn":
            result = save.seeded
        elif op == "seteventmon":
            active_species = "SPECIES_SEEDED_REPLACEMENT" if save.seeded else args[0]
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
            # Native story scripts also assign symbolic species/local object ids.
            # Only numeric values participate in the supported variable gates.
            save.variables[args[0]] = int(args[1]) if args[1].isdecimal() else args[1]
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
        "JOHTO_GLASTRIER": "GLASTRIER",
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
assert len(table) >= 83, f"Lost tracked individual statics (the Royal Duo moved to its paired-battle validator): {len(table)}"
for directory, prefix, name, species, x, y, suffix, multiple in table:
    scripts = blocks(directory)
    battle = prefix + "_EventScript_" + name
    hide = "FLAG_HIDE_ANTHOLOGY_" + suffix
    if directory == "PokemonMansion_1F_Frlg" and name == "Mew":
        # Mew is four sequential fights, with three compulsory victories and a
        # catch-only fourth battle. One fixed outcome for all four rounds in
        # this tiny interpreter would incorrectly model a failed gauntlet.
        script = (ROOT / "data/maps" / directory / "scripts.inc").read_text()
        full = script.split("PokemonMansion_1F_EventScript_Mew::")[1].split("PokemonMansion_1F_Text_MewChallenge::")[0]
        assert full.count("special BattleSetup_StartMewGauntletBattle") == 4
        assert full.count("goto_if_ne VAR_RESULT, B_OUTCOME_WON") == 3
        assert "goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PokemonMansion_1F_AnthologyMewResolved" in full
        assert "goto PokemonMansion_1F_AnthologyMewRetreated" in full
        continue
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


# Hoenn's optional Ogerpon hunt: first capture unlocks each Loyal Three
# independently, and only all four caught-Dex bits unlock Pecharunt.
# A party or PC ownership check would wrongly block released Pokémon.
_hunt = (
    ("Route117", "Ogerpon", "OGERPON_TEAL", "OGERPON", set()),
    ("Route112", "Okidogi", "OKIDOGI", "OKIDOGI", {"SPECIES_OGERPON_TEAL"}),
    ("Route113", "Fezandipiti", "FEZANDIPITI", "FEZANDIPITI", {"SPECIES_OGERPON_TEAL"}),
    ("Route114", "Munkidori", "MUNKIDORI", "MUNKIDORI", {"SPECIES_OGERPON_TEAL"}),
    ("Route117", "Pecharunt", "PECHARUNT", "PECHARUNT",
     {"SPECIES_OGERPON_TEAL", "SPECIES_OKIDOGI",
      "SPECIES_FEZANDIPITI", "SPECIES_MUNKIDORI"}),
)
for _map, _name, _species, _suffix, _prereqs in _hunt:
    _labels = blocks(_map)
    _entry = _map + "_AnthologyHuntTransition"
    _battle = _map + "_EventScript_" + _name
    _hide = "FLAG_HIDE_ANTHOLOGY_" + _suffix
    for _one_missing in _prereqs:
        _state = Save(caught=set(_prereqs) - {_one_missing})
        run(_labels, _entry, _state)
        assert _hide in _state.flags, (_name, "appeared without", _one_missing)
    _state = Save(caught=set(_prereqs))
    run(_labels, _entry, _state)
    assert _hide not in _state.flags, (_name, "gated despite prerequisites")
    run(_labels, _battle, _state, "B_OUTCOME_WON")
    assert _hide in _state.flags, (_name, "faint didn't hide")
    run(_labels, _entry, _state)
    assert _hide not in _state.flags, (_name, "faint didn't retry")
    run(_labels, _battle, _state, "B_OUTCOME_RAN")
    assert _hide not in _state.flags, (_name, "flee incorrectly hid")
    run(_labels, _battle, _state, "B_OUTCOME_CAUGHT")
    assert _hide in _state.flags, (_name, "capture didn't hide")
    assert "SPECIES_" + _species in _state.caught, (_name, "no caught registration")
    _reloaded = Save(set(_state.flags), set(_state.caught), dict(_state.variables))
    run(_labels, _entry, _reloaded)
    assert _hide in _reloaded.flags, (_name, "reappeared after save/revisit")
print("P0 Ogerpon + Loyal Three + Pecharunt: independent gates, retry, capture and save/revisit source interpretation PASS")

for area, species, hide in (
    ("TerraCave_End", "GROUDON", "FLAG_HIDE_TERRA_CAVE_GROUDON"),
    ("MarineCave_End", "KYOGRE", "FLAG_HIDE_MARINE_CAVE_KYOGRE"),
):
    entry = area + "_OnTransition"
    scripts = blocks(area)
    player = Save()
    run(scripts, entry, player)
    assert hide in player.flags, (area, "Rayquaza not yet caught")
    player.caught.add("SPECIES_RAYQUAZA")
    run(scripts, entry, player)
    assert hide not in player.flags, (area, "caught Rayquaza unlocks native cavern")
    player.caught.add("SPECIES_" + species)
    run(scripts, entry, player)
    assert hide in player.flags, (area, "caught legendary must not respawn")
print("P0 Hoenn weather trio: prerequisite and permanent catch persistence PASS")

location_gates = (
    ("PetalburgWoods", "PetalburgWoods_AnthologyOnTransition",
     "FLAG_HIDE_ANTHOLOGY_ZYGARDE_10", (), None, "FLAG_CAUGHT_ANTHOLOGY_ZYGARDE_10"),
    ("MeteorFalls_1F_1R", "MeteorFalls_1F_1R_AnthologyOnTransition",
     "FLAG_HIDE_ANTHOLOGY_ZYGARDE", ("FLAG_BADGE06_GET",), None, "FLAG_CAUGHT_ANTHOLOGY_ZYGARDE"),
    ("Route128_AncientGrotto", "Route128_AncientGrotto_AnthologyOnTransition",
     "FLAG_HIDE_ANTHOLOGY_KORAIDON", ("FLAG_BADGE07_GET", "FLAG_DEFEATED_MAGMA_SPACE_CENTER"), None, "SPECIES_KORAIDON"),
    ("NewMauville_GeneratorAnnex", "NewMauville_GeneratorAnnex_AnthologyOnTransition",
     "FLAG_HIDE_ANTHOLOGY_MIRAIDON", ("FLAG_BADGE07_GET", "FLAG_DEFEATED_MAGMA_SPACE_CENTER"), 2, "SPECIES_MIRAIDON"),
    ("MossdeepCity_SpaceCenter_1F", "MossdeepCity_SpaceCenter_1F_AnthologyOnTransition",
     "FLAG_HIDE_ANTHOLOGY_LATIOS", ("FLAG_DEFEATED_MAGMA_SPACE_CENTER",), None, "SPECIES_LATIOS"),
)
for area, transition, hide, prereqs, generator, claimed in location_gates:
    scripts = blocks(area)
    state = Save(flags={hide})
    if prereqs or generator is not None:
        run(scripts, transition, state)
        assert hide in state.flags, (area, "early gate")
    state.flags.update(prereqs)
    if generator is not None:
        state.variables["VAR_NEW_MAUVILLE_STATE"] = generator
    run(scripts, transition, state)
    assert hide not in state.flags, (area, "unlocked")
    if claimed.startswith("SPECIES_"):
        state.caught.add(claimed)
    else:
        state.flags.add(claimed)
    run(scripts, transition, state)
    assert hide in state.flags, (area, "caught/form flag must hide permanently")
print("P0 Hoenn location/form gates: both Zygarde forms, cover legendaries and Latios PASS")

birth = blocks("BirthIsland_Exterior")
harbor = (ROOT / "data/maps/LilycoveCity_Harbor/scripts.inc").read_text()
assert "getcaughtmon SPECIES_DEOXYS_NORMAL" in (ROOT / "data/maps/BirthIsland_Exterior/scripts.inc").read_text()
assert "BirthIsland_Exterior_EventScript_Triangle" in birth
assert "goto_if_eq VAR_RESULT, TRUE, LilycoveCity_Harbor_EventScript_SailorBirthIsland" in harbor
assert "getcaughtmon SPECIES_RAYQUAZA" in harbor
assert "warp MAP_BIRTH_ISLAND_HARBOR, 8, 4" in harbor
assert "setvar VAR_FERRY_DEPARTURE, FERRY_DEPART_LILYCOVE" in harbor
print("P0 Deoxys: native triangle plus ticket-free Rayquaza ferry source contract PASS")


# Test the actual event-command branches with seeded catches. The current
# original-species Pokédex may contain a species caught at another location,
# but only this exact source site's successful capture may set its completion.
# A KO must disappear for the visit and respawn on re-entry; run/teleport
# never consume the encounter. This is source interpreter evidence, not mGBA.
SEED_SITES = (
    # map, transition, battle, original species, hide flag, captured-site bit, flags, vars
    ("PokemonMansion_1F_Frlg", "PokemonMansion_1F_AnthologyOnTransition",
     "PokemonMansion_1F_RandomizedMewStatic", "MEW", "FLAG_HIDE_ANTHOLOGY_MEW",
     "FLAG_CAUGHT_RANDOM_MANSION_MEW", set(), {}),
    ("PokemonMansion_1F_Frlg", "PokemonMansion_1F_AnthologyOnTransition",
     "PokemonMansion_1F_EventScript_TypeNull", "TYPE_NULL", "FLAG_HIDE_ANTHOLOGY_TYPE_NULL",
     "FLAG_CAUGHT_RANDOM_MANSION_TYPE_NULL", set(), {}),
    ("IlexForest", "IlexForest_AnthologyOnTransition",
     "IlexForest_EventScript_Zarude", "ZARUDE", "FLAG_HIDE_ANTHOLOGY_ZARUDE",
     "FLAG_CAUGHT_RANDOM_ILEX_ZARUDE", set(), {}),
    ("Route38", "Route38_FieldSyncRaikouReserve",
     "Route38_EventScript_RaikouReserve", "RAIKOU", "FLAG_HIDE_ANTHOLOGY_BEAST_RAIKOU",
     "FLAG_CAUGHT_RANDOM_ROUTE38_RAIKOU", {"FLAG_RELEASED_BEASTS"}, {}),
    ("MtMortar_2F", "MtMortar_2F_FieldSyncEnteiReserve",
     "MtMortar_2F_EventScript_EnteiReserve", "ENTEI", "FLAG_HIDE_ANTHOLOGY_BEAST_ENTEI",
     "FLAG_CAUGHT_RANDOM_MTMORTAR_2F_ENTEI", {"FLAG_RELEASED_BEASTS"}, {}),
    ("LakeOfRage", "LakeOfRage_FieldSyncSuicuneReserve",
     "LakeOfRage_EventScript_SuicuneReserve", "SUICUNE", "FLAG_HIDE_ANTHOLOGY_BEAST_SUICUNE",
     "FLAG_CAUGHT_RANDOM_LAKEOFRAGE_SUICUNE", {"FLAG_RELEASED_BEASTS"}, {"VAR_MAHOGANY_TOWN_STATE": 14}),
    ("Route40", "Route40_AnthologyOnTransition",
     "Route40_EventScript_Lugia", "LUGIA", "FLAG_HIDE_ANTHOLOGY_LUGIA",
     "FLAG_CAUGHT_RANDOM_ROUTE40_LUGIA", set(), {}),
    ("ShoalCave_LowTideIceRoom", "ShoalCave_LowTideIceRoom_OnTransition",
     "ShoalCave_LowTideIceRoom_EventScript_Glastrier", "GLASTRIER",
     "FLAG_HIDE_ANTHOLOGY_GLASTRIER", "FLAG_CAUGHT_RANDOM_SHOALCAVE_GLASTRIER", set(), {}),
    ("Route118", "Route118_SecondBatchOnTransition",
     "Route118_EventScript_IronBoulder", "IRON_BOULDER", "FLAG_HIDE_ANTHOLOGY_IRON_BOULDER",
     "FLAG_CAUGHT_RANDOM_ROUTE118_IRON_BOULDER", set(), {}),
    ("Route119", "Route119_AnthologyOnTransition",
     "Route119_EventScript_RagingBolt", "RAGING_BOLT", "FLAG_HIDE_ANTHOLOGY_RAGING_BOLT",
     "FLAG_CAUGHT_RANDOM_ROUTE119_RAGING_BOLT", set(), {}),
    ("Route119", "Route119_XerneasTransition",
     "Route119_EventScript_Xerneas", "XERNEAS", "FLAG_HIDE_ANTHOLOGY_XERNEAS",
     "FLAG_CAUGHT_RANDOM_ROUTE119_XERNEAS", set(), {}),
    ("Route120", "Route120_SecondBatchOnTransition",
     "Route120_EventScript_IronLeaves", "IRON_LEAVES", "FLAG_HIDE_ANTHOLOGY_IRON_LEAVES",
     "FLAG_CAUGHT_RANDOM_ROUTE120_IRON_LEAVES", set(), {}),
    ("Route120", "Route120_SecondBatchOnTransition",
     "Route120_EventScript_Latias", "LATIAS", "FLAG_HIDE_ANTHOLOGY_LATIAS",
     "FLAG_CAUGHT_RANDOM_ROUTE120_LATIAS", set(), {}),
    ("Route120", "Route120_WoChienTransition",
     "Route120_EventScript_WoChien", "WO_CHIEN", "FLAG_HIDE_ANTHOLOGY_WO_CHIEN",
     "FLAG_CAUGHT_RANDOM_ROUTE120_WO_CHIEN", set(), {}),
    ("Route128_AncientGrotto", "Route128_AncientGrotto_AnthologyOnTransition",
     "Route128_AncientGrotto_EventScript_Koraidon", "KORAIDON",
     "FLAG_HIDE_ANTHOLOGY_KORAIDON", "FLAG_CAUGHT_RANDOM_ANCIENTGROTTO_KORAIDON",
     {"FLAG_BADGE07_GET", "FLAG_DEFEATED_MAGMA_SPACE_CENTER"}, {}),
    ("CeladonCity_Frlg", "CeladonCity_AnthologyOnTransition",
     "CeladonCity_EventScript_TapuLele", "TAPU_LELE", "FLAG_HIDE_ANTHOLOGY_TAPU_LELE",
     "FLAG_CAUGHT_RANDOM_CELADON_TAPU_LELE", set(), {}),
    ("Route25_Frlg", "Route25_Frlg_OnTransition",
     "Route25_EventScript_Suicune", "SUICUNE", "FLAG_HIDE_ROUTE25_SUICUNE",
     "FLAG_CAUGHT_RANDOM_ROUTE25_SUICUNE", {"FLAG_JOHTO_CHAMPION"}, {"VAR_SUICUNE_ENCOUNTERS": 3}),
)
constants = (ROOT / "include/constants/flags.h").read_text()
for directory, transition, battle, original, hidden, bit, gates, variables in SEED_SITES:
    script = blocks(directory)
    assert "#define " + bit in constants
    assert bit not in gates
    assert "setflag " + bit in (ROOT / "data/maps" / directory / "scripts.inc").read_text()
    for outcome in ("B_OUTCOME_WON", "B_OUTCOME_RAN", "B_OUTCOME_PLAYER_TELEPORTED", "B_OUTCOME_CAUGHT"):
        # Caught entry elsewhere must not suppress this source location.
        state = Save(flags=set(gates), caught={"SPECIES_" + original},
                     variables=dict(variables), seeded=True)
        run(script, transition, state)
        assert hidden not in state.flags, (directory, original, "original species from elsewhere hid site")
        run(script, battle, state, outcome)
        assert (bit in state.flags) == (outcome == "B_OUTCOME_CAUGHT"), (
            directory, original, outcome, "incorrect source completion")
        # Mansion Mew's noncapture result retreats without consuming or hiding.
        expected_hidden = (outcome == "B_OUTCOME_CAUGHT"
                           if original == "MEW" else outcome in ("B_OUTCOME_WON", "B_OUTCOME_CAUGHT"))
        assert (hidden in state.flags) == expected_hidden, (
            directory, original, outcome, "wrong same-visit hide")
        saved = Save(flags=set(state.flags), caught=set(state.caught),
                     variables=dict(state.variables), seeded=True)
        run(script, transition, saved)
        assert (hidden in saved.flags) == (outcome == "B_OUTCOME_CAUGHT"), (
            directory, original, outcome, "wrong saved/revisited visibility")
        if outcome == "B_OUTCOME_CAUGHT":
            assert "SPECIES_SEEDED_REPLACEMENT" in saved.caught, (directory, "original used despite seed")
print(f"P0 seeded source-site lifecycle: {len(SEED_SITES)} sites x 4 outcomes, re-entry/save and unrelated species ownership PASS")

# Two Mew source sites feed one Mewtwo prerequisite. No unrelated Mew Dex
# ownership can bypass the source-lifecycle gates.
mewtwo = blocks("CeruleanCave_B1F_Frlg")
for completion in (set(), {"FLAG_CAUGHT_RANDOM_MANSION_MEW"},
                   {"FLAG_CAUGHT_RANDOM_FARAWAY_MEW"}):
    state = Save(flags=set(completion), caught={"SPECIES_MEW"}, seeded=True)
    run(mewtwo, "CeruleanCave_B1F_OnTransition", state)
    assert ("FLAG_HIDE_MEWTWO" in state.flags) == (not completion)
    if completion:
        run(mewtwo, "CeruleanCave_B1F_EventScript_Mewtwo", state, "B_OUTCOME_CAUGHT")
        assert "FLAG_CAUGHT_RANDOM_CERULEAN_MEWTWO" in state.flags
        run(mewtwo, "CeruleanCave_B1F_OnTransition", state)
        assert "FLAG_HIDE_MEWTWO" in state.flags
print("P0 seeded Mew/Mewtwo source prerequisite and captured Mewtwo persistence PASS")

# Johto's two Celebi sources are independent in seeded mode. The second
# source retains the Champion/Azalea prerequisites while unrelated captures
# of original Celebi cannot consume either source.
ilex = blocks("IlexForest")
shrine = Save(flags={"FLAG_JOHTO_CHAMPION"}, caught={"SPECIES_CELEBI"},
              variables={"VAR_AZALEA_TOWN_STATE": 10}, seeded=True)
run(ilex, "IlexForest_EventScript_CelebiRematch", shrine, "B_OUTCOME_CAUGHT")
assert "FLAG_CAUGHT_RANDOM_ILEX_SHRINE_CELEBI" in shrine.flags
assert "FLAG_CAUGHT_RANDOM_ILEX_SECOND_CELEBI" not in shrine.flags
run(ilex, "IlexForest_EventScript_SecondCelebi", shrine, "B_OUTCOME_CAUGHT")
assert "FLAG_CAUGHT_RANDOM_ILEX_SECOND_CELEBI" in shrine.flags
run(ilex, "IlexForest_EventScript_SyncSecondCelebi", shrine)
assert "FLAG_HIDE_ILEX_FOREST_SECOND_CELEBI" in shrine.flags
print("P0 seeded two-site Celebi capture independence PASS")

# Ho-Oh may only appear after catching all three beast SOURCE locations, not
# merely after owning the original Raikou/Entei/Suicune dex entries.
roof = blocks("TinTower_RoofDay")
beast_sites = {"FLAG_CAUGHT_RANDOM_ROUTE38_RAIKOU",
               "FLAG_CAUGHT_RANDOM_MTMORTAR_2F_ENTEI",
               "FLAG_CAUGHT_RANDOM_LAKEOFRAGE_SUICUNE"}
for absent in beast_sites:
    state = Save(flags={"FLAG_RELEASED_BEASTS"} | (beast_sites - {absent}),
                 caught={"SPECIES_RAIKOU", "SPECIES_ENTEI", "SPECIES_SUICUNE"},
                 seeded=True)
    run(roof, "TinTower_RoofDay_AnthologyHoOhGate", state)
    assert "FLAG_HIDE_HO_OH" in state.flags
state = Save(flags={"FLAG_RELEASED_BEASTS"} | beast_sites, seeded=True)
run(roof, "TinTower_RoofDay_AnthologyHoOhGate", state)
assert "FLAG_HIDE_HO_OH" not in state.flags
run(roof, "TinTower_RoofDay_EventScript_CaughtHooh", state)
assert "FLAG_CAUGHT_RANDOM_TINTOWER_HO_OH" in state.flags
run(roof, "TinTower_RoofDay_AnthologyHoOhGate", state)
assert "FLAG_HIDE_HO_OH" in state.flags
print("P0 seeded Ho-Oh beast-source prerequisite, capture and repeat prevention PASS")


# Native weather trio: neither a Rayquaza from elsewhere nor an original
# Groudon/Kyogre in the Dex can unlock/consume the seeded local source.
for area, native, species, bit, hidden in (
    ("SkyPillar_Top", "Rayquaza", "RAYQUAZA",
     "FLAG_CAUGHT_RANDOM_SKYPILLAR_RAYQUAZA", "FLAG_HIDE_SKY_PILLAR_TOP_RAYQUAZA_STILL"),
    ("TerraCave_End", "Groudon", "GROUDON",
     "FLAG_CAUGHT_RANDOM_TERRACAVE_GROUDON", "FLAG_HIDE_TERRA_CAVE_GROUDON"),
    ("MarineCave_End", "Kyogre", "KYOGRE",
     "FLAG_CAUGHT_RANDOM_MARINECAVE_KYOGRE", "FLAG_HIDE_MARINE_CAVE_KYOGRE"),
):
    script = blocks(area)
    state = Save(caught={"SPECIES_RAYQUAZA", "SPECIES_" + species},
                 seeded=True)
    if area != "SkyPillar_Top":
        run(script, area + "_OnTransition", state)
        assert hidden in state.flags, (area, "unrelated Rayquaza capture bypassed cave gate")
        state.flags.add("FLAG_CAUGHT_RANDOM_SKYPILLAR_RAYQUAZA")
        run(script, area + "_OnTransition", state)
        assert hidden not in state.flags, (area, "source Rayquaza failed to open native cave")
    else:
        state.variables["VAR_SKY_PILLAR_STATE"] = 2
        run(script, "SkyPillar_Top_EventScript_TryShowRayquaza", state)
        assert hidden not in state.flags, (area, "unrelated Rayquaza captured elsewhere")
    run(script, area + "_EventScript_" + native, state, "B_OUTCOME_CAUGHT")
    assert bit in state.flags, (area, "native captured source bit absent")
    check = (area + "_OnTransition" if area != "SkyPillar_Top"
             else "SkyPillar_Top_EventScript_TryShowRayquaza")
    run(script, check, state)
    assert hidden in state.flags, (area, "caught native source reappeared")
print("P0 seeded native Rayquaza, Groudon and Kyogre prereqs and captures PASS")

tower = blocks("EmbeddedTower")
for native, source_flag, hidden in (
    ("Rayquaza", "FLAG_CAUGHT_RANDOM_EMBEDDED_RAYQUAZA", "FLAG_HIDE_RAYQUAZA"),
    ("Groudon", "FLAG_CAUGHT_RANDOM_EMBEDDED_GROUDON", "FLAG_HIDE_GROUDON"),
    ("Kyogre", "FLAG_CAUGHT_RANDOM_EMBEDDED_KYOGRE", "FLAG_HIDE_KYOGRE"),
):
    state = Save(caught={"SPECIES_" + native.upper()}, seeded=True)
    run(tower, "EmbeddedTower_OnTransition", state)
    assert source_flag not in state.flags, ("embedded premature completion", native)
    run(tower, "EmbeddedTower_EventScript_" + native, state, "B_OUTCOME_CAUGHT")
    assert source_flag in state.flags, ("embedded missing source bit", native)
    saved = Save(set(state.flags), set(state.caught), dict(state.variables), True)
    run(tower, "EmbeddedTower_OnTransition", saved)
    assert hidden in saved.flags, ("embedded captured source respawned", native)
print("P0 seeded three independent Embedded Tower captures PASS")
