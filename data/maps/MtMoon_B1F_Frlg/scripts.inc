MtMoon_B1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMoon_B1F_OnTransition
	.byte 0

MtMoon_B1F_OnTransition::
	call MtMoon_B1F_AnthologyOnTransition
	end

// The original cave event set was empty. These statics coexist with the native
// wild habitat and normal stairs, items, Strength puzzles, and Articuno events.
MtMoon_B1F_AnthologyOnTransition::
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, MtMoon_B1F_AnthologyRandomOnTransition
	getcaughtmon SPECIES_CRESSELIA
	call_if_eq VAR_RESULT, TRUE, MtMoon_B1F_AnthologyHideCresselia
	call_if_ne VAR_RESULT, TRUE, MtMoon_B1F_AnthologyShowCresselia
	getcaughtmon SPECIES_DIALGA
	call_if_eq VAR_RESULT, TRUE, MtMoon_B1F_AnthologyHideDialga
	call_if_ne VAR_RESULT, TRUE, MtMoon_B1F_AnthologyShowDialga
	getcaughtmon SPECIES_PALKIA
	call_if_eq VAR_RESULT, TRUE, MtMoon_B1F_AnthologyHidePalkia
	call_if_ne VAR_RESULT, TRUE, MtMoon_B1F_AnthologyShowPalkia
	return

// In randomized games each encounter's own completion bit, not the species
// caught anywhere else, controls visibility and Giratina's prerequisites.
MtMoon_B1F_AnthologyRandomOnTransition::
	call_if_set FLAG_CAUGHT_RANDOM_MTMOON_CRESSELIA, MtMoon_B1F_AnthologyHideCresselia
	call_if_unset FLAG_CAUGHT_RANDOM_MTMOON_CRESSELIA, MtMoon_B1F_AnthologyShowCresselia
	call_if_set FLAG_CAUGHT_RANDOM_MTMOON_DIALGA, MtMoon_B1F_AnthologyHideDialga
	call_if_unset FLAG_CAUGHT_RANDOM_MTMOON_DIALGA, MtMoon_B1F_AnthologyShowDialga
	call_if_set FLAG_CAUGHT_RANDOM_MTMOON_PALKIA, MtMoon_B1F_AnthologyHidePalkia
	call_if_unset FLAG_CAUGHT_RANDOM_MTMOON_PALKIA, MtMoon_B1F_AnthologyShowPalkia
	return

MtMoon_B1F_AnthologyHideCresselia::
	setflag FLAG_HIDE_ANTHOLOGY_CRESSELIA
	return

MtMoon_B1F_AnthologyShowCresselia::
	clearflag FLAG_HIDE_ANTHOLOGY_CRESSELIA
	return

// Caught: permanently hidden. Won without catch: reappears on re-entry.
// Run/teleport: remains available at the current site.
MtMoon_B1F_EventScript_Cresselia::
	lock
	faceplayer
	playmoncry SPECIES_CRESSELIA, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_CRESSELIA, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtMoon_B1F_AnthologyCresseliaRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtMoon_B1F_AnthologyCresseliaRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtMoon_B1F_AnthologyCresseliaCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtMoon_B1F_AnthologyCresseliaResolved
	release
	end

MtMoon_B1F_AnthologyCresseliaCaught::
	setflag FLAG_CAUGHT_RANDOM_MTMOON_CRESSELIA
	goto MtMoon_B1F_AnthologyCresseliaResolved

MtMoon_B1F_AnthologyCresseliaResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_CRESSELIA
	setflag FLAG_HIDE_ANTHOLOGY_CRESSELIA
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

MtMoon_B1F_AnthologyCresseliaRetreated::
	release
	end

MtMoon_B1F_AnthologyHideDialga::
	setflag FLAG_HIDE_ANTHOLOGY_DIALGA
	return

MtMoon_B1F_AnthologyShowDialga::
	clearflag FLAG_HIDE_ANTHOLOGY_DIALGA
	return

// Caught: permanently hidden. Won without catch: reappears on re-entry.
// Run/teleport: remains available at the current site.
MtMoon_B1F_EventScript_Dialga::
	lock
	faceplayer
	playmoncry SPECIES_DIALGA, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_DIALGA, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtMoon_B1F_AnthologyDialgaRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtMoon_B1F_AnthologyDialgaRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtMoon_B1F_AnthologyDialgaCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtMoon_B1F_AnthologyDialgaResolved
	release
	end

MtMoon_B1F_AnthologyDialgaCaught::
	setflag FLAG_CAUGHT_RANDOM_MTMOON_DIALGA
	goto MtMoon_B1F_AnthologyDialgaResolved

MtMoon_B1F_AnthologyDialgaResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_DIALGA
	setflag FLAG_HIDE_ANTHOLOGY_DIALGA
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

MtMoon_B1F_AnthologyDialgaRetreated::
	release
	end

MtMoon_B1F_AnthologyHidePalkia::
	setflag FLAG_HIDE_ANTHOLOGY_PALKIA
	return

MtMoon_B1F_AnthologyShowPalkia::
	clearflag FLAG_HIDE_ANTHOLOGY_PALKIA
	return

// Caught: permanently hidden. Won without catch: reappears on re-entry.
// Run/teleport: remains available at the current site.
MtMoon_B1F_EventScript_Palkia::
	lock
	faceplayer
	playmoncry SPECIES_PALKIA, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_PALKIA, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtMoon_B1F_AnthologyPalkiaRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtMoon_B1F_AnthologyPalkiaRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtMoon_B1F_AnthologyPalkiaCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtMoon_B1F_AnthologyPalkiaResolved
	release
	end

MtMoon_B1F_AnthologyPalkiaCaught::
	setflag FLAG_CAUGHT_RANDOM_MTMOON_PALKIA
	goto MtMoon_B1F_AnthologyPalkiaResolved

MtMoon_B1F_AnthologyPalkiaResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_PALKIA
	setflag FLAG_HIDE_ANTHOLOGY_PALKIA
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

MtMoon_B1F_AnthologyPalkiaRetreated::
	release
	end

