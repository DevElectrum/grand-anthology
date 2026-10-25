TinTower_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, TinTower_2F_OnTransition
	.byte 0

TinTower_2F_OnTransition::
	setescapewarp MAP_BELLCHIME_TRAIL, 35, 42
	call TinTower_2F_AnthologyOnTransition
	end

TinTower_2F_AnthologyOnTransition::
	getcaughtmon SPECIES_RESHIRAM
	goto_if_eq VAR_RESULT, TRUE, TinTower_2F_AnthologyHideReshiram
	clearflag FLAG_HIDE_ANTHOLOGY_RESHIRAM
	return

TinTower_2F_AnthologyHideReshiram::
	setflag FLAG_HIDE_ANTHOLOGY_RESHIRAM
	return

TinTower_2F_EventScript_Reshiram::
	lock
	faceplayer
	playmoncry SPECIES_RESHIRAM, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_RESHIRAM, 44
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, TinTower_2F_AnthologyReshiramRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, TinTower_2F_AnthologyReshiramRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, TinTower_2F_AnthologyReshiramResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, TinTower_2F_AnthologyReshiramResolved
	release
	end

TinTower_2F_AnthologyReshiramResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_RESHIRAM
	setflag FLAG_HIDE_ANTHOLOGY_RESHIRAM
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

TinTower_2F_AnthologyReshiramRetreated::
	release
	end
