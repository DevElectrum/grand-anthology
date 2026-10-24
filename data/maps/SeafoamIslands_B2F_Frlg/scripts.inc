SeafoamIslands_B2F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SeafoamIslands_B2F_OnTransition
	.byte 0

SeafoamIslands_B2F_OnTransition::
	call SeafoamIslands_B2F_AnthologyOnTransition
	end

// The original cave event set was empty. These statics coexist with the native
// wild habitat and normal stairs, items, Strength puzzles, and Articuno events.
SeafoamIslands_B2F_AnthologyOnTransition::
	getcaughtmon SPECIES_MANAPHY
	call_if_eq VAR_RESULT, TRUE, SeafoamIslands_B2F_AnthologyHideManaphy
	call_if_ne VAR_RESULT, TRUE, SeafoamIslands_B2F_AnthologyShowManaphy
	getcaughtmon SPECIES_TAPU_FINI
	call_if_eq VAR_RESULT, TRUE, SeafoamIslands_B2F_AnthologyHideTapuFini
	call_if_ne VAR_RESULT, TRUE, SeafoamIslands_B2F_AnthologyShowTapuFini
	return

SeafoamIslands_B2F_AnthologyHideManaphy::
	setflag FLAG_HIDE_ANTHOLOGY_MANAPHY
	return

SeafoamIslands_B2F_AnthologyShowManaphy::
	clearflag FLAG_HIDE_ANTHOLOGY_MANAPHY
	return

// Caught: permanently hidden. Won without catch: reappears on re-entry.
// Run/teleport: remains available at the current site.
SeafoamIslands_B2F_EventScript_Manaphy::
	lock
	faceplayer
	playmoncry SPECIES_MANAPHY, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_MANAPHY, 35
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, SeafoamIslands_B2F_AnthologyManaphyRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, SeafoamIslands_B2F_AnthologyManaphyRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, SeafoamIslands_B2F_AnthologyManaphyResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, SeafoamIslands_B2F_AnthologyManaphyResolved
	release
	end

SeafoamIslands_B2F_AnthologyManaphyResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_MANAPHY
	setflag FLAG_HIDE_ANTHOLOGY_MANAPHY
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

SeafoamIslands_B2F_AnthologyManaphyRetreated::
	release
	end

SeafoamIslands_B2F_AnthologyHideTapuFini::
	setflag FLAG_HIDE_ANTHOLOGY_TAPU_FINI
	return

SeafoamIslands_B2F_AnthologyShowTapuFini::
	clearflag FLAG_HIDE_ANTHOLOGY_TAPU_FINI
	return

// Caught: permanently hidden. Won without catch: reappears on re-entry.
// Run/teleport: remains available at the current site.
SeafoamIslands_B2F_EventScript_TapuFini::
	lock
	faceplayer
	playmoncry SPECIES_TAPU_FINI, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_TAPU_FINI, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, SeafoamIslands_B2F_AnthologyTapuFiniRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, SeafoamIslands_B2F_AnthologyTapuFiniRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, SeafoamIslands_B2F_AnthologyTapuFiniResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, SeafoamIslands_B2F_AnthologyTapuFiniResolved
	release
	end

SeafoamIslands_B2F_AnthologyTapuFiniResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_TAPU_FINI
	setflag FLAG_HIDE_ANTHOLOGY_TAPU_FINI
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

SeafoamIslands_B2F_AnthologyTapuFiniRetreated::
	release
	end
