Route5_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route5_OnTransition
	.byte 0

Route5_OnTransition::
	call Route5_BigBatchOnTransition
	end

Route5_EventScript_UndergroundPathSign::
	msgbox Route5_Text_UndergroundPathSign, MSGBOX_SIGN
	end

Route5_Text_UndergroundPathSign::
	.string "UNDERGROUND PATH\n"
	.string "CERULEAN CITY - VERMILION CITY$"

// Multi-location acquisition batch: preserve canonical map scripts.
Route5_BigBatchOnTransition::
	getcaughtmon SPECIES_SHAYMIN
	call_if_eq VAR_RESULT, TRUE, Route5_BigBatchHideShaymin
	call_if_ne VAR_RESULT, TRUE, Route5_BigBatchShowShaymin
	getcaughtmon SPECIES_TAPU_BULU
	call_if_eq VAR_RESULT, TRUE, Route5_BigBatchHideTapuBulu
	call_if_ne VAR_RESULT, TRUE, Route5_BigBatchShowTapuBulu
	return

Route5_BigBatchHideShaymin::
	setflag FLAG_HIDE_ANTHOLOGY_SHAYMIN
	return

Route5_BigBatchShowShaymin::
	clearflag FLAG_HIDE_ANTHOLOGY_SHAYMIN
	return

Route5_EventScript_Shaymin::
	lock
	faceplayer
	playmoncry SPECIES_SHAYMIN, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_SHAYMIN, 35
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route5_BigBatchShayminRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route5_BigBatchShayminRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route5_BigBatchShayminResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route5_BigBatchShayminResolved
	release
	end

Route5_BigBatchShayminResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_SHAYMIN
	setflag FLAG_HIDE_ANTHOLOGY_SHAYMIN
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

Route5_BigBatchShayminRetreated::
	release
	end

Route5_BigBatchHideTapuBulu::
	setflag FLAG_HIDE_ANTHOLOGY_TAPU_BULU
	return

Route5_BigBatchShowTapuBulu::
	clearflag FLAG_HIDE_ANTHOLOGY_TAPU_BULU
	return

Route5_EventScript_TapuBulu::
	lock
	faceplayer
	playmoncry SPECIES_TAPU_BULU, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_TAPU_BULU, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route5_BigBatchTapuBuluRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route5_BigBatchTapuBuluRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route5_BigBatchTapuBuluResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route5_BigBatchTapuBuluResolved
	release
	end

Route5_BigBatchTapuBuluResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_TAPU_BULU
	setflag FLAG_HIDE_ANTHOLOGY_TAPU_BULU
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

Route5_BigBatchTapuBuluRetreated::
	release
	end
