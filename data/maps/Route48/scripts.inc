Route48_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route48_OnTransition
	.byte 0

Route48_OnTransition::
	call Route48_AnthologyOnTransition
	end

// Independent special encounter flags; keep native events authoritative.
Route48_AnthologyOnTransition::
	getcaughtmon SPECIES_ZACIAN
	call_if_eq VAR_RESULT, TRUE, Route48_AnthologyHideZacian
	call_if_ne VAR_RESULT, TRUE, Route48_AnthologyShowZacian
	return

Route48_AnthologyHideZacian::
	setflag FLAG_HIDE_ANTHOLOGY_ZACIAN
	return

Route48_AnthologyShowZacian::
	clearflag FLAG_HIDE_ANTHOLOGY_ZACIAN
	return

Route48_EventScript_Zacian::
	lock
	faceplayer
	playmoncry SPECIES_ZACIAN, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_ZACIAN, 50
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route48_AnthologyZacianRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route48_AnthologyZacianRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route48_AnthologyZacianResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route48_AnthologyZacianResolved
	release
	end

Route48_AnthologyZacianResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_ZACIAN
	setflag FLAG_HIDE_ANTHOLOGY_ZACIAN
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

Route48_AnthologyZacianRetreated::
	release
	end
