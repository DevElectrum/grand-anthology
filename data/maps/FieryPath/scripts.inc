FieryPath_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FieryPath_OnTransition
	.byte 0

FieryPath_OnTransition:
	call FieryPath_PreLeagueOnTransition
	call_if_unset FLAG_LANDMARK_FIERY_PATH, FieryPath_EventScript_MoveScottToFallarbor
	setflag FLAG_LANDMARK_FIERY_PATH
	call FieryPath_AnthologyOnTransition
	end

FieryPath_EventScript_MoveScottToFallarbor::
	setflag FLAG_HIDE_VERDANTURF_TOWN_SCOTT
	clearflag FLAG_HIDE_FALLARBOR_TOWN_BATTLE_TENT_SCOTT
	return

// Cluster-first optional statics. The original cave boulders, passage,
// weather, and Team Magma stories remain controlled by their native scripts.
FieryPath_AnthologyOnTransition::
	getcaughtmon SPECIES_VOLCANION
	call_if_eq VAR_RESULT, TRUE, FieryPath_AnthologyHideVolcanion
	call_if_ne VAR_RESULT, TRUE, FieryPath_AnthologyShowVolcanion
	getcaughtmon SPECIES_CHI_YU
	call_if_eq VAR_RESULT, TRUE, FieryPath_AnthologyHideChiYu
	call_if_ne VAR_RESULT, TRUE, FieryPath_AnthologyShowChiYu
	return

FieryPath_AnthologyHideVolcanion::
	setflag FLAG_HIDE_ANTHOLOGY_VOLCANION
	return

FieryPath_AnthologyShowVolcanion::
	clearflag FLAG_HIDE_ANTHOLOGY_VOLCANION
	return

FieryPath_EventScript_Volcanion::
	lock
	faceplayer
	playmoncry SPECIES_VOLCANION, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_VOLCANION, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, FieryPath_AnthologyVolcanionRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, FieryPath_AnthologyVolcanionRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, FieryPath_AnthologyVolcanionResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, FieryPath_AnthologyVolcanionResolved
	release
	end

FieryPath_AnthologyVolcanionResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_VOLCANION
	setflag FLAG_HIDE_ANTHOLOGY_VOLCANION
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

FieryPath_AnthologyVolcanionRetreated::
	release
	end

FieryPath_AnthologyHideChiYu::
	setflag FLAG_HIDE_ANTHOLOGY_CHI_YU
	return

FieryPath_AnthologyShowChiYu::
	clearflag FLAG_HIDE_ANTHOLOGY_CHI_YU
	return

FieryPath_EventScript_ChiYu::
	lock
	faceplayer
	playmoncry SPECIES_CHI_YU, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_CHI_YU, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, FieryPath_AnthologyChiYuRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, FieryPath_AnthologyChiYuRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, FieryPath_AnthologyChiYuResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, FieryPath_AnthologyChiYuResolved
	release
	end

FieryPath_AnthologyChiYuResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_CHI_YU
	setflag FLAG_HIDE_ANTHOLOGY_CHI_YU
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

FieryPath_AnthologyChiYuRetreated::
	release
	end

// Alternate before-first-Champion capture source. Native legendary story,
 // puzzle and optional post-League site remain in their original maps.
FieryPath_PreLeagueOnTransition::
	goto_if_lt VAR_SOOTOPOLIS_CITY_STATE, 6, FieryPath_PreLeagueGROUDONHide
	getcaughtmon SPECIES_GROUDON
	goto_if_eq VAR_RESULT, TRUE, FieryPath_PreLeagueGROUDONHide
	goto FieryPath_PreLeagueGROUDONShow

FieryPath_PreLeagueGROUDONHide::
	setflag FLAG_HIDE_ANTHOLOGY_PRELEAGUE_GROUDON
	return

FieryPath_PreLeagueGROUDONShow::
	clearflag FLAG_HIDE_ANTHOLOGY_PRELEAGUE_GROUDON
	return

FieryPath_EventScript_GroudonPreLeague::
	lock
	faceplayer
	playmoncry SPECIES_GROUDON, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_GROUDON, 50
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, FieryPath_PreLeagueGROUDONRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, FieryPath_PreLeagueGROUDONRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, FieryPath_PreLeagueGROUDONResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, FieryPath_PreLeagueGROUDONResolved
	release
	end

FieryPath_PreLeagueGROUDONResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_PRELEAGUE_GROUDON
	setflag FLAG_HIDE_ANTHOLOGY_PRELEAGUE_GROUDON
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

FieryPath_PreLeagueGROUDONRetreated::
	release
	end
