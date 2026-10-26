CeruleanCave_B1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, CeruleanCave_B1F_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCave_B1F_OnTransition
	.byte 0

CeruleanCave_B1F_OnResume::
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, CeruleanCave_B1F_EventScript_TryRemoveMewtwo
	end

CeruleanCave_B1F_EventScript_TryRemoveMewtwo::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

CeruleanCave_B1F_OnTransition::
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, CeruleanCave_B1F_OnSeededTransition
	getcaughtmon SPECIES_MEW
	goto_if_ne VAR_RESULT, TRUE, CeruleanCave_B1F_NativeMEWTWOBlocked
	getcaughtmon SPECIES_MEWTWO
	call_if_eq VAR_RESULT, TRUE, CeruleanCave_B1F_NativeMEWTWOCaught
	call_if_ne VAR_RESULT, TRUE, CeruleanCave_B1F_NativeMEWTWOUncaught
	end

CeruleanCave_B1F_OnSeededTransition::
	goto_if_set FLAG_CAUGHT_RANDOM_MANSION_MEW, CeruleanCave_B1F_SeededMewPrerequisiteMet
	goto_if_set FLAG_CAUGHT_RANDOM_FARAWAY_MEW, CeruleanCave_B1F_SeededMewPrerequisiteMet
	setflag FLAG_HIDE_MEWTWO
	end

CeruleanCave_B1F_SeededMewPrerequisiteMet::
	call_if_set FLAG_CAUGHT_RANDOM_CERULEAN_MEWTWO, CeruleanCave_B1F_NativeMEWTWOCaught
	call_if_unset FLAG_CAUGHT_RANDOM_CERULEAN_MEWTWO, CeruleanCave_B1F_NativeMEWTWOUncaught
	end

// The original cave remains locked behind the Kanto postgame/Celio quest.
// Within it, Mewtwo is discoverable only after Mew is registered caught.
CeruleanCave_B1F_NativeMEWTWOBlocked::
	setflag FLAG_HIDE_MEWTWO
	return

CeruleanCave_B1F_EventScript_ShowMewtwo::
	clearflag FLAG_HIDE_MEWTWO
	return

CeruleanCave_B1F_EventScript_Mewtwo::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_MEWTWO, CRY_MODE_ENCOUNTER
	message CeruleanCave_B1F_Text_Mew
	waitmessage
	waitmoncry
	delay 20
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setwildbattle SPECIES_MEWTWO, 70
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, CeruleanCave_B1F_EventScript_DefeatedMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, CeruleanCave_B1F_EventScript_RanFromMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, CeruleanCave_B1F_EventScript_RanFromMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, CeruleanCave_B1F_EventScript_CaughtMewtwo
	setflag FLAG_FOUGHT_MEWTWO
	release
	end

CeruleanCave_B1F_EventScript_CaughtMewtwo::
	callnative AnthologyStaticRandomizerIsOn
	call_if_eq VAR_RESULT, TRUE, CeruleanCave_B1F_MarkSeededMewtwo
	setflag FLAG_FOUGHT_MEWTWO
	release
	end

CeruleanCave_B1F_MarkSeededMewtwo::
	setflag FLAG_CAUGHT_RANDOM_CERULEAN_MEWTWO
	return

CeruleanCave_B1F_EventScript_DefeatedMewtwo::
	setflag FLAG_FOUGHT_MEWTWO
	goto EventScript_RemoveStaticMon
	end

CeruleanCave_B1F_EventScript_RanFromMewtwo::
	setvar VAR_0x8004, SPECIES_MEWTWO
	goto EventScript_MonFlewAway
	end

CeruleanCave_B1F_Text_Mew::
	.string "Mew!$"

// Reconcile old defeated flags with the persistent caught species dex bit.
// This also repairs a pre-upgrade save where the species was KO'd or fled.
// All native room puzzles and battle scripts are unchanged.
CeruleanCave_B1F_NativeMEWTWOCaught::
	setflag FLAG_FOUGHT_MEWTWO
	setflag FLAG_HIDE_MEWTWO
	return

CeruleanCave_B1F_NativeMEWTWOUncaught::
	clearflag FLAG_FOUGHT_MEWTWO
	clearflag FLAG_HIDE_MEWTWO
	return
