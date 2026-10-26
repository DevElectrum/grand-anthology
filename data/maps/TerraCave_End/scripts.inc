TerraCave_End_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, TerraCave_End_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, TerraCave_End_OnTransition
	.byte 0

TerraCave_End_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, TerraCave_End_EventScript_TryRemoveGroudon
	end

TerraCave_End_EventScript_TryRemoveGroudon::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_TERRA_CAVE_GROUDON
	return

TerraCave_End_OnTransition:
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, TerraCave_End_SeededOnTransition
	getcaughtmon SPECIES_GROUDON
	goto_if_eq VAR_RESULT, TRUE, TerraCave_End_NativeGROUDONCaught
	getcaughtmon SPECIES_RAYQUAZA
	call_if_eq VAR_RESULT, TRUE, TerraCave_End_NativeGROUDONUncaught
	call_if_ne VAR_RESULT, TRUE, TerraCave_End_NativeGROUDONLocked
	end

TerraCave_End_SeededOnTransition::
	goto_if_set FLAG_CAUGHT_RANDOM_TERRACAVE_GROUDON, TerraCave_End_NativeGROUDONCaught
	goto_if_set FLAG_CAUGHT_RANDOM_SKYPILLAR_RAYQUAZA, TerraCave_End_SeededRayquazaPrerequisiteMet
	goto_if_set FLAG_CAUGHT_RANDOM_EMBEDDED_RAYQUAZA, TerraCave_End_SeededRayquazaPrerequisiteMet
	call TerraCave_End_NativeGROUDONLocked
	end

TerraCave_End_SeededRayquazaPrerequisiteMet::
	call TerraCave_End_NativeGROUDONUncaught
	end

TerraCave_End_EventScript_ShowGroudon::
	clearflag FLAG_HIDE_TERRA_CAVE_GROUDON
	setvar VAR_TEMP_1, 1
	return

TerraCave_End_EventScript_Groudon::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	applymovement LOCALID_TERRA_CAVE_GROUDON, TerraCave_End_Movement_GroudonApproach
	waitmovement 0
	waitse
	playmoncry SPECIES_GROUDON, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_TERRA_CAVE_GROUDON
	setwildbattle SPECIES_GROUDON, 70
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	setvar VAR_TEMP_1, 0
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, TerraCave_End_EventScript_DefeatedGroudon
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, TerraCave_End_EventScript_RanFromGroudon
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, TerraCave_End_EventScript_RanFromGroudon
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, TerraCave_End_SeededCaught
	setvar VAR_SHOULD_END_ABNORMAL_WEATHER, 1
	setflag FLAG_DEFEATED_GROUDON
	releaseall
	end

TerraCave_End_SeededCaught::
	callnative AnthologyStaticRandomizerIsOn
	call_if_eq VAR_RESULT, TRUE, TerraCave_End_MarkSeededCaught
	setvar VAR_SHOULD_END_ABNORMAL_WEATHER, 1
	setflag FLAG_DEFEATED_GROUDON
	releaseall
	end

TerraCave_End_MarkSeededCaught::
	setflag FLAG_CAUGHT_RANDOM_TERRACAVE_GROUDON
	return

TerraCave_End_EventScript_DefeatedGroudon::
	setvar VAR_SHOULD_END_ABNORMAL_WEATHER, 1
	setflag FLAG_DEFEATED_GROUDON
	goto Common_EventScript_RemoveStaticPokemon
	end

TerraCave_End_EventScript_RanFromGroudon::
	setvar VAR_0x8004, SPECIES_GROUDON
	goto Common_EventScript_LegendaryFlewAway
	end

TerraCave_End_Movement_GroudonApproach:
	init_affine_anim
	walk_down_start_affine
	delay_16
	delay_16
	walk_down_affine
	delay_16
	delay_16
	walk_down_affine
	step_end

// Reconcile old defeated flags with the persistent caught species dex bit.
// This also repairs a pre-upgrade save where the species was KO'd or fled.
// All native room puzzles and battle scripts are unchanged.
TerraCave_End_NativeGROUDONCaught::
	setflag FLAG_DEFEATED_GROUDON
	setflag FLAG_HIDE_TERRA_CAVE_GROUDON
	return

TerraCave_End_NativeGROUDONUncaught::
	clearflag FLAG_DEFEATED_GROUDON
	clearflag FLAG_HIDE_TERRA_CAVE_GROUDON
	setvar VAR_TEMP_1, 1
	return

TerraCave_End_NativeGROUDONLocked::
	setflag FLAG_HIDE_TERRA_CAVE_GROUDON
	return
