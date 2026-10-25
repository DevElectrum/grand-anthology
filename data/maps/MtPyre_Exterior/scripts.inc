MtPyre_Exterior_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtPyre_Exterior_OnTransition
	.byte 0

MtPyre_Exterior_OnTransition:
	call MtPyre_Exterior_EventScript_CheckEnterFromSummit
	call MtPyre_Exterior_AnthologyOnTransition
	end

MtPyre_Exterior_EventScript_CheckEnterFromSummit::
	getplayerxy VAR_TEMP_0, VAR_TEMP_1
	goto_if_lt VAR_TEMP_1, 12, MtPyre_Exterior_EventScript_EnterFromSummit
	return

MtPyre_Exterior_EventScript_EnterFromSummit::
	setweather WEATHER_FOG_HORIZONTAL
	return

MtPyre_Exterior_EventScript_FogTrigger::
	setweather WEATHER_FOG_HORIZONTAL
	doweather
	end

MtPyre_Exterior_EventScript_SunTrigger::
	setweather WEATHER_SUNNY
	doweather
	end

// All four statics are on one stable exterior layout. Fog/sun boundaries and
// canonical summit script continue to run without changing their logic.
MtPyre_Exterior_AnthologyOnTransition::
	getcaughtmon SPECIES_YVELTAL
	call_if_eq VAR_RESULT, TRUE, MtPyre_Exterior_AnthologyHideYveltal
	call_if_ne VAR_RESULT, TRUE, MtPyre_Exterior_AnthologyShowYveltal
	getcaughtmon SPECIES_WO_CHIEN
	call_if_eq VAR_RESULT, TRUE, MtPyre_Exterior_AnthologyHideWoChien
	call_if_ne VAR_RESULT, TRUE, MtPyre_Exterior_AnthologyShowWoChien
	return

MtPyre_Exterior_AnthologyHideYveltal::
	setflag FLAG_HIDE_ANTHOLOGY_YVELTAL
	return

MtPyre_Exterior_AnthologyShowYveltal::
	clearflag FLAG_HIDE_ANTHOLOGY_YVELTAL
	return

MtPyre_Exterior_EventScript_Yveltal::
	lock
	faceplayer
	playmoncry SPECIES_YVELTAL, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_YVELTAL, 50
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtPyre_Exterior_AnthologyYveltalRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtPyre_Exterior_AnthologyYveltalRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtPyre_Exterior_AnthologyYveltalResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtPyre_Exterior_AnthologyYveltalResolved
	release
	end

MtPyre_Exterior_AnthologyYveltalResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_YVELTAL
	setflag FLAG_HIDE_ANTHOLOGY_YVELTAL
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

MtPyre_Exterior_AnthologyYveltalRetreated::
	release
	end

MtPyre_Exterior_AnthologyHideWoChien::
	setflag FLAG_HIDE_ANTHOLOGY_WO_CHIEN
	return

MtPyre_Exterior_AnthologyShowWoChien::
	clearflag FLAG_HIDE_ANTHOLOGY_WO_CHIEN
	return

MtPyre_Exterior_EventScript_WoChien::
	lock
	faceplayer
	playmoncry SPECIES_WO_CHIEN, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_WO_CHIEN, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtPyre_Exterior_AnthologyWoChienRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtPyre_Exterior_AnthologyWoChienRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtPyre_Exterior_AnthologyWoChienResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtPyre_Exterior_AnthologyWoChienResolved
	release
	end

MtPyre_Exterior_AnthologyWoChienResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_WO_CHIEN
	setflag FLAG_HIDE_ANTHOLOGY_WO_CHIEN
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

MtPyre_Exterior_AnthologyWoChienRetreated::
	release
	end

