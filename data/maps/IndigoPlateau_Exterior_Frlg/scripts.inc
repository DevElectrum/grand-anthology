IndigoPlateau_Exterior_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, IndigoPlateau_Exterior_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, IndigoPlateau_Exterior_OnFrame
	.byte 0

IndigoPlateau_Exterior_OnTransition::
	call IndigoPlateau_Exterior_AnthologyOnTransition
	setworldmapflag FLAG_WORLD_MAP_INDIGO_PLATEAU_EXTERIOR
	call_if_eq VAR_MAP_SCENE_INDIGO_PLATEAU_EXTERIOR, 1, IndigoPlateau_Exterior_EventScript_PlayCreditsMusic
	end

IndigoPlateau_Exterior_EventScript_PlayCreditsMusic::
	playbgm MUS_CREDITS, 0
	return

IndigoPlateau_Exterior_OnFrame::
	map_script_2 VAR_MAP_SCENE_INDIGO_PLATEAU_EXTERIOR, 1, IndigoPlateau_Exterior_EventScript_Credits
	.2byte 0


@ After beating the champion, you are warped to the main
@ entrance of Indigo Plateau. This script starts running
@ immediately after your walking animation.
@ The script is timed to the music.
IndigoPlateau_Exterior_EventScript_Credits::
	lockall
	delay 105

	@ Rival exits building. Spawn a dummy object to lock the camera.
	opendoor 11, 6
	waitdooranim
	special SpawnCameraObject
	addobject LOCALID_CREDITS_RIVAL
	applymovement LOCALID_CREDITS_RIVAL, IndigoPlateau_Exterior_Movement_RivalExitBuilding
	applymovement LOCALID_PLAYER, IndigoPlateau_Exterior_Movement_PushPlayerOutOfWay
	waitmovement 0
	closedoor 11, 6
	waitdooranim

	@ Rival runs off
	applymovement LOCALID_PLAYER, IndigoPlateau_Exterior_Movement_PlayerWatchRivalLeave
	applymovement LOCALID_CREDITS_RIVAL, IndigoPlateau_Exterior_Movement_RivalLeave
	waitmovement 0
	delay 100

	@ Oak exits building
	opendoor 11, 6
	waitdooranim
	addobject LOCALID_CREDITS_PROF_OAK
	applymovement LOCALID_CREDITS_PROF_OAK, IndigoPlateau_Exterior_Movement_OakExitBuilding
	waitmovement 0
	closedoor 11, 6
	waitdooranim
	delay 160

	@ Oak looks at player, then walks off
	applymovement LOCALID_PLAYER, IndigoPlateau_Exterior_Movement_PlayerWatchOakLeave
	applymovement LOCALID_CREDITS_PROF_OAK, IndigoPlateau_Exterior_Movement_OakLeave
	waitmovement 0
	delay 100

	@ Player begin to leave
	applymovement LOCALID_PLAYER, IndigoPlateau_Exterior_Movement_PlayerBeginLeave
	waitmovement 0
	delay 155
	applymovement LOCALID_PLAYER, IndigoPlateau_Exterior_Movement_PlayerFaceLeague
	waitmovement 0
	delay 90

	@ Player runs off
	applymovement LOCALID_PLAYER, IndigoPlateau_Exterior_Movement_PlayerTurnAround
	waitmovement 0
	applymovement LOCALID_PLAYER, IndigoPlateau_Exterior_Movement_PlayerLeave
	waitmovement 0

	@ Reset the map state and roll the credits
	special RemoveCameraObject
	setvar VAR_MAP_SCENE_INDIGO_PLATEAU_EXTERIOR, 0
	removeobject LOCALID_CREDITS_RIVAL
	removeobject LOCALID_CREDITS_PROF_OAK
	special CB2_StartCreditsSequence
	waitstate
	releaseall
	end

IndigoPlateau_Exterior_Movement_PlayerLeave::
	player_run_down
	player_run_down
	player_run_down
	player_run_down
	player_run_down
	player_run_down
	step_end

@ Unused
IndigoPlateau_Exterior_Movement_PlayerExitBuilding::
	walk_down
	walk_down
	step_end

IndigoPlateau_Exterior_Movement_PlayerWatchRivalLeave::
	delay_16
	delay_4
	walk_in_place_faster_down
	step_end

IndigoPlateau_Exterior_Movement_PlayerWatchOakLeave::
	delay_8
	walk_in_place_faster_right
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_down
	step_end

IndigoPlateau_Exterior_Movement_PlayerBeginLeave::
	walk_right
	delay_16
	walk_down
	walk_down
	delay_16
	delay_16
	step_end

IndigoPlateau_Exterior_Movement_PlayerTurnAround::
	walk_in_place_faster_down
	step_end

IndigoPlateau_Exterior_Movement_PushPlayerOutOfWay::
	face_right
	lock_facing_direction
	walk_left
	unlock_facing_direction
	step_end

IndigoPlateau_Exterior_Movement_PlayerFaceLeague::
	walk_in_place_faster_up
	step_end

IndigoPlateau_Exterior_Movement_RivalLeave::
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	step_end

IndigoPlateau_Exterior_Movement_RivalExitBuilding::
	walk_fast_down
	step_end

IndigoPlateau_Exterior_Movement_OakLeave::
	walk_in_place_faster_left
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_down
	delay_16
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

IndigoPlateau_Exterior_Movement_OakExitBuilding::
	walk_down
	step_end

// The post-Champion creation-trio hunt must be completed before Arceus.
IndigoPlateau_Exterior_AnthologyOnTransition::
	goto_if_unset FLAG_KANTO_CHAMPION, IndigoPlateau_Exterior_AnthologyHideArceus
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, IndigoPlateau_Exterior_SeededArceusGate
	getcaughtmon SPECIES_DIALGA
	goto_if_ne VAR_RESULT, TRUE, IndigoPlateau_Exterior_AnthologyHideArceus
	getcaughtmon SPECIES_PALKIA
	goto_if_ne VAR_RESULT, TRUE, IndigoPlateau_Exterior_AnthologyHideArceus
	getcaughtmon SPECIES_GIRATINA
	goto_if_ne VAR_RESULT, TRUE, IndigoPlateau_Exterior_AnthologyHideArceus
	getcaughtmon SPECIES_ARCEUS
	call_if_eq VAR_RESULT, TRUE, IndigoPlateau_Exterior_AnthologyHideArceus
	call_if_ne VAR_RESULT, TRUE, IndigoPlateau_Exterior_AnthologyShowArceus
	return

IndigoPlateau_Exterior_SeededArceusGate::
	goto_if_unset FLAG_CAUGHT_RANDOM_MTMOON_DIALGA, IndigoPlateau_Exterior_AnthologyHideArceus
	goto_if_unset FLAG_CAUGHT_RANDOM_MTMOON_PALKIA, IndigoPlateau_Exterior_AnthologyHideArceus
	goto_if_unset FLAG_CAUGHT_RANDOM_MTMOON_GIRATINA, IndigoPlateau_Exterior_AnthologyHideArceus
	call_if_set FLAG_CAUGHT_RANDOM_INDIGO_ARCEUS, IndigoPlateau_Exterior_AnthologyHideArceus
	call_if_unset FLAG_CAUGHT_RANDOM_INDIGO_ARCEUS, IndigoPlateau_Exterior_AnthologyShowArceus
	return

IndigoPlateau_Exterior_AnthologyHideArceus::
	setflag FLAG_HIDE_ANTHOLOGY_ARCEUS
	return

IndigoPlateau_Exterior_AnthologyShowArceus::
	clearflag FLAG_HIDE_ANTHOLOGY_ARCEUS
	return

IndigoPlateau_Exterior_EventScript_Arceus::
	lock
	faceplayer
	playmoncry SPECIES_ARCEUS, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_ARCEUS, 50
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, IndigoPlateau_Exterior_AnthologyArceusRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, IndigoPlateau_Exterior_AnthologyArceusRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, IndigoPlateau_Exterior_SeededArceusCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, IndigoPlateau_Exterior_AnthologyArceusResolved
	release
	end

IndigoPlateau_Exterior_SeededArceusCaught::
	callnative AnthologyStaticRandomizerIsOn
	call_if_eq VAR_RESULT, TRUE, IndigoPlateau_Exterior_MarkSeededArceus
	goto IndigoPlateau_Exterior_AnthologyArceusResolved

IndigoPlateau_Exterior_MarkSeededArceus::
	setflag FLAG_CAUGHT_RANDOM_INDIGO_ARCEUS
	return

IndigoPlateau_Exterior_AnthologyArceusResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_ARCEUS
	setflag FLAG_HIDE_ANTHOLOGY_ARCEUS
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

IndigoPlateau_Exterior_AnthologyArceusRetreated::
	release
	end
