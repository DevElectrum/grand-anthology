PokemonMansion_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, PokemonMansion_1F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonMansion_1F_OnTransition
	.byte 0

PokemonMansion_1F_OnLoad::
	call_if_set FLAG_POKEMON_MANSION_SWITCH_STATE, PokemonMansion_EventScript_PressSwitch_1F
	end

PokemonMansion_1F_OnTransition::
	call PokemonMansion_1F_AnthologyOnTransition
	setworldmapflag FLAG_WORLD_MAP_POKEMON_MANSION_1F
	end

PokemonMansion_1F_EventScript_Statue::
	lockall
	setvar VAR_0x8004, 0
	call PokemonMansion_EventScript_SecretSwitch
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	releaseall
	end

PokemonMansion_1F_EventScript_Ted::
	trainerbattle_single TRAINER_SCIENTIST_TED, PokemonMansion_1F_Text_TedIntro, PokemonMansion_1F_Text_TedDefeat
	msgbox PokemonMansion_1F_Text_TedPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonMansion_1F_EventScript_Johnson::
	trainerbattle_single TRAINER_YOUNGSTER_JOHNSON, PokemonMansion_1F_Text_JohnsonIntro, PokemonMansion_1F_Text_JohnsonDefeat
	msgbox PokemonMansion_1F_Text_JohnsonPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonMansion_1F_Text_TedIntro::
	.string "Who are you?\n"
	.string "There shouldn't be anyone here.$"

PokemonMansion_1F_Text_TedDefeat::
	.string "Ouch!$"

PokemonMansion_1F_Text_TedPostBattle::
	.string "A key? I don't know what you're\n"
	.string "talking about.$"

PokemonMansion_1F_Text_JohnsonIntro::
	.string "W-w-waah! You startled me!\n"
	.string "I thought you were a ghost.$"

PokemonMansion_1F_Text_JohnsonDefeat::
	.string "Tch!\n"
	.string "I can't get any wins.$"

PokemonMansion_1F_Text_JohnsonPostBattle::
	.string "I was exploring here by myself,\n"
	.string "but I feel spooked.\p"
	.string "I ought to go soon.$"

// Independent pre-League encounters; preserve original local puzzle/story.
PokemonMansion_1F_AnthologyOnTransition::
	getcaughtmon SPECIES_MEW
	call_if_eq VAR_RESULT, TRUE, PokemonMansion_1F_AnthologyHideMew
	call_if_ne VAR_RESULT, TRUE, PokemonMansion_1F_AnthologyShowMew
	getcaughtmon SPECIES_TYPE_NULL
	call_if_eq VAR_RESULT, TRUE, PokemonMansion_1F_AnthologyHideTypeNull
	call_if_ne VAR_RESULT, TRUE, PokemonMansion_1F_AnthologyShowTypeNull
	return

PokemonMansion_1F_AnthologyHideMew::
	setflag FLAG_HIDE_ANTHOLOGY_MEW
	return

PokemonMansion_1F_AnthologyShowMew::
	clearflag FLAG_HIDE_ANTHOLOGY_MEW
	return

PokemonMansion_1F_EventScript_Mew::
	lock
	faceplayer
	playmoncry SPECIES_MEW, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_MEW, 35
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PokemonMansion_1F_AnthologyMewRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PokemonMansion_1F_AnthologyMewRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PokemonMansion_1F_AnthologyMewResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PokemonMansion_1F_AnthologyMewResolved
	release
	end

PokemonMansion_1F_AnthologyMewResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_MEW
	setflag FLAG_HIDE_ANTHOLOGY_MEW
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

PokemonMansion_1F_AnthologyMewRetreated::
	release
	end

PokemonMansion_1F_AnthologyHideTypeNull::
	setflag FLAG_HIDE_ANTHOLOGY_TYPE_NULL
	return

PokemonMansion_1F_AnthologyShowTypeNull::
	clearflag FLAG_HIDE_ANTHOLOGY_TYPE_NULL
	return

PokemonMansion_1F_EventScript_TypeNull::
	lock
	faceplayer
	playmoncry SPECIES_TYPE_NULL, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_TYPE_NULL, 35
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PokemonMansion_1F_AnthologyTypeNullRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PokemonMansion_1F_AnthologyTypeNullRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PokemonMansion_1F_AnthologyTypeNullResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PokemonMansion_1F_AnthologyTypeNullResolved
	release
	end

PokemonMansion_1F_AnthologyTypeNullResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_TYPE_NULL
	setflag FLAG_HIDE_ANTHOLOGY_TYPE_NULL
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

PokemonMansion_1F_AnthologyTypeNullRetreated::
	release
	end

