Route125_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route125_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, Route125_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route125_OnFrame
	.byte 0

Route125_OnTransition:
	call Route125_PreLeagueOnTransition
	call Route125_AnthologyOnTransition
	call_if_set FLAG_SYS_WEATHER_CTRL, Common_EventScript_SetAbnormalWeather
	call_if_eq VAR_SHOULD_END_ABNORMAL_WEATHER, 1, AbnormalWeather_EventScript_HideMapNamePopup
	call_if_eq VAR_ABNORMAL_WEATHER_LOCATION, ABNORMAL_WEATHER_ROUTE_125_WEST, AbnormalWeather_StartKyogreWeather
	call_if_eq VAR_ABNORMAL_WEATHER_LOCATION, ABNORMAL_WEATHER_ROUTE_125_EAST, AbnormalWeather_StartKyogreWeather
	end

Route125_OnLoad:
	call_if_eq VAR_ABNORMAL_WEATHER_LOCATION, ABNORMAL_WEATHER_ROUTE_125_WEST, AbnormalWeather_EventScript_PlaceTilesRoute125West
	call_if_eq VAR_ABNORMAL_WEATHER_LOCATION, ABNORMAL_WEATHER_ROUTE_125_EAST, AbnormalWeather_EventScript_PlaceTilesRoute125East
	end

Route125_OnFrame:
	map_script_2 VAR_SHOULD_END_ABNORMAL_WEATHER, 1, AbnormalWeather_EventScript_EndEventAndCleanup_1
	.2byte 0

Route125_EventScript_Nolen::
	trainerbattle_single TRAINER_NOLEN, Route125_Text_NolenIntro, Route125_Text_NolenDefeat
	msgbox Route125_Text_NolenPostBattle, MSGBOX_AUTOCLOSE
	end

Route125_EventScript_Stan::
	trainerbattle_single TRAINER_STAN, Route125_Text_StanIntro, Route125_Text_StanDefeat
	msgbox Route125_Text_StanPostBattle, MSGBOX_AUTOCLOSE
	end

Route125_EventScript_Tanya::
	trainerbattle_single TRAINER_TANYA, Route125_Text_TanyaIntro, Route125_Text_TanyaDefeat
	msgbox Route125_Text_TanyaPostBattle, MSGBOX_AUTOCLOSE
	end

Route125_EventScript_Sharon::
	trainerbattle_single TRAINER_SHARON, Route125_Text_SharonIntro, Route125_Text_SharonDefeat
	msgbox Route125_Text_SharonPostBattle, MSGBOX_AUTOCLOSE
	end

Route125_EventScript_Ernest::
	trainerbattle_single TRAINER_ERNEST_1, Route125_Text_ErnestIntro, Route125_Text_ErnestDefeat, Route125_EventScript_RegisterErnest
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route125_EventScript_RematchErnest
	msgbox Route125_Text_ErnestPostBattle, MSGBOX_DEFAULT
	release
	end

Route125_EventScript_RegisterErnest::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox Route125_Text_ErnestRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_ERNEST_1
	release
	end

Route125_EventScript_RematchErnest::
	trainerbattle_rematch TRAINER_ERNEST_1, Route125_Text_ErnestRematchIntro, Route125_Text_ErnestRematchDefeat
	msgbox Route125_Text_ErnestRematchPostBattle, MSGBOX_AUTOCLOSE
	end

Route125_EventScript_Kim::
	trainerbattle_double TRAINER_KIM_AND_IRIS, Route125_Text_KimIntro, Route125_Text_KimDefeat, Route125_Text_KimNotEnoughMons
	msgbox Route125_Text_KimPostBattle, MSGBOX_AUTOCLOSE
	end

Route125_EventScript_Iris::
	trainerbattle_double TRAINER_KIM_AND_IRIS, Route125_Text_IrisIntro, Route125_Text_IrisDefeat, Route125_Text_IrisNotEnoughMons
	msgbox Route125_Text_IrisPostBattle, MSGBOX_AUTOCLOSE
	end

Route125_EventScript_Presley::
	trainerbattle_single TRAINER_PRESLEY, Route125_Text_PresleyIntro, Route125_Text_PresleyDefeat
	msgbox Route125_Text_PresleyPostBattle, MSGBOX_AUTOCLOSE
	end

Route125_EventScript_Auron::
	trainerbattle_single TRAINER_AURON, Route125_Text_AuronIntro, Route125_Text_AuronDefeat
	msgbox Route125_Text_AuronPostBattle, MSGBOX_AUTOCLOSE
	end

// Place this unique legendary-Paradox encounter on a Surf-accessible
// shoreline instead of gated/legacy-diving cave maps.
Route125_AnthologyOnTransition::
	getcaughtmon SPECIES_WALKING_WAKE
	call_if_eq VAR_RESULT, TRUE, Route125_AnthologyHideWalkingWake
	call_if_ne VAR_RESULT, TRUE, Route125_AnthologyShowWalkingWake
	return

Route125_AnthologyHideWalkingWake::
	setflag FLAG_HIDE_ANTHOLOGY_WALKING_WAKE
	return

Route125_AnthologyShowWalkingWake::
	clearflag FLAG_HIDE_ANTHOLOGY_WALKING_WAKE
	return

Route125_EventScript_WalkingWake::
	lock
	faceplayer
	playmoncry SPECIES_WALKING_WAKE, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_WALKING_WAKE, 50
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route125_AnthologyWalkingWakeRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route125_AnthologyWalkingWakeRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route125_AnthologyWalkingWakeResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route125_AnthologyWalkingWakeResolved
	release
	end

Route125_AnthologyWalkingWakeResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_WALKING_WAKE
	setflag FLAG_HIDE_ANTHOLOGY_WALKING_WAKE
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

Route125_AnthologyWalkingWakeRetreated::
	release
	end

// Alternate before-first-Champion capture source. Native legendary story,
 // puzzle and optional post-League site remain in their original maps.
Route125_PreLeagueOnTransition::
	goto_if_lt VAR_SOOTOPOLIS_CITY_STATE, 6, Route125_PreLeagueKYOGREHide
	getcaughtmon SPECIES_KYOGRE
	goto_if_eq VAR_RESULT, TRUE, Route125_PreLeagueKYOGREHide
	goto Route125_PreLeagueKYOGREShow

Route125_PreLeagueKYOGREHide::
	setflag FLAG_HIDE_ANTHOLOGY_PRELEAGUE_KYOGRE
	return

Route125_PreLeagueKYOGREShow::
	clearflag FLAG_HIDE_ANTHOLOGY_PRELEAGUE_KYOGRE
	return

Route125_EventScript_KyogrePreLeague::
	lock
	faceplayer
	playmoncry SPECIES_KYOGRE, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_KYOGRE, 50
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route125_PreLeagueKYOGRERetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route125_PreLeagueKYOGRERetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route125_PreLeagueKYOGREResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route125_PreLeagueKYOGREResolved
	release
	end

Route125_PreLeagueKYOGREResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_PRELEAGUE_KYOGRE
	setflag FLAG_HIDE_ANTHOLOGY_PRELEAGUE_KYOGRE
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

Route125_PreLeagueKYOGRERetreated::
	release
	end
