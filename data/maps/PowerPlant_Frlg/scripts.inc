PowerPlant_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, PowerPlant_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, PowerPlant_OnTransition
	.byte 0

PowerPlant_OnResume::
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, PowerPlant_EventScript_TryRemoveStaticMon
	end

PowerPlant_EventScript_TryRemoveStaticMon::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

PowerPlant_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_POWER_PLANT
	getcaughtmon SPECIES_ZAPDOS
	call_if_eq VAR_RESULT, TRUE, PowerPlant_NativeZAPDOSCaught
	call_if_ne VAR_RESULT, TRUE, PowerPlant_NativeZAPDOSUncaught
	call_if_unset FLAG_FOUGHT_POWER_PLANT_ELECTRODE_1, PowerPlant_EventScript_ShowElectrode1
	call_if_unset FLAG_FOUGHT_POWER_PLANT_ELECTRODE_2, PowerPlant_EventScript_ShowElectrode2
	call PowerPlant_AnthologyOnTransition
	end

PowerPlant_EventScript_ShowZapdos::
	clearflag FLAG_HIDE_ZAPDOS
	return

PowerPlant_EventScript_ShowElectrode1::
	clearflag FLAG_HIDE_POWER_PLANT_ELECTRODE_1
	return

PowerPlant_EventScript_ShowElectrode2::
	clearflag FLAG_HIDE_POWER_PLANT_ELECTRODE_2
	return

PowerPlant_EventScript_Zapdos::
	lock
	faceplayer
	setwildbattle SPECIES_ZAPDOS, 50
	waitse
	playmoncry SPECIES_ZAPDOS, CRY_MODE_ENCOUNTER
	message Text_Gyaoo
	waitmessage
	waitmoncry
	delay 10
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PowerPlant_EventScript_DefeatedZapdos
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PowerPlant_EventScript_RanFromZapdos
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PowerPlant_EventScript_RanFromZapdos
	setflag FLAG_FOUGHT_ZAPDOS
	release
	end

PowerPlant_EventScript_DefeatedZapdos::
	setflag FLAG_FOUGHT_ZAPDOS
	goto EventScript_RemoveStaticMon
	end

PowerPlant_EventScript_RanFromZapdos::
	setvar VAR_0x8004, SPECIES_ZAPDOS
	goto EventScript_MonFlewAway
	end

PowerPlant_EventScript_Electrode1::
	lock
	faceplayer
	setwildbattle SPECIES_ELECTRODE, 34
	waitse
	playmoncry SPECIES_ELECTRODE, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PowerPlant_EventScript_FoughtElectrode1
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PowerPlant_EventScript_FoughtElectrode1
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PowerPlant_EventScript_FoughtElectrode1
	setflag FLAG_FOUGHT_POWER_PLANT_ELECTRODE_1
	release
	end

PowerPlant_EventScript_FoughtElectrode1::
	setflag FLAG_FOUGHT_POWER_PLANT_ELECTRODE_1
	goto EventScript_RemoveStaticMon
	end

PowerPlant_EventScript_Electrode2::
	lock
	faceplayer
	setwildbattle SPECIES_ELECTRODE, 34
	waitse
	playmoncry SPECIES_ELECTRODE, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PowerPlant_EventScript_FoughtElectrode2
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PowerPlant_EventScript_FoughtElectrode2
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PowerPlant_EventScript_FoughtElectrode2
	setflag FLAG_FOUGHT_POWER_PLANT_ELECTRODE_2
	release
	end

PowerPlant_EventScript_FoughtElectrode2::
	setflag FLAG_FOUGHT_POWER_PLANT_ELECTRODE_2
	goto EventScript_RemoveStaticMon
	end


// Restore uncaught statics on entry; let the original map scripts run.
PowerPlant_AnthologyOnTransition::
	getcaughtmon SPECIES_TAPU_KOKO
	call_if_eq VAR_RESULT, TRUE, PowerPlant_AnthologyHideTapuKoko
	call_if_ne VAR_RESULT, TRUE, PowerPlant_AnthologyShowTapuKoko
	getcaughtmon SPECIES_MAGEARNA
	call_if_eq VAR_RESULT, TRUE, PowerPlant_AnthologyHideMagearna
	call_if_ne VAR_RESULT, TRUE, PowerPlant_AnthologyShowMagearna
	getcaughtmon SPECIES_ZERAORA
	call_if_eq VAR_RESULT, TRUE, PowerPlant_AnthologyHideZeraora
	call_if_ne VAR_RESULT, TRUE, PowerPlant_AnthologyShowZeraora
	return

PowerPlant_AnthologyHideTapuKoko::
	setflag FLAG_HIDE_ANTHOLOGY_TAPU_KOKO
	return

PowerPlant_AnthologyShowTapuKoko::
	clearflag FLAG_HIDE_ANTHOLOGY_TAPU_KOKO
	return

// Caught: permanently gone. Fainted: returns on re-entry. Run: stays here.
PowerPlant_EventScript_TapuKoko::
	lock
	faceplayer
	playmoncry SPECIES_TAPU_KOKO, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_TAPU_KOKO, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PowerPlant_AnthologyTapuKokoRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PowerPlant_AnthologyTapuKokoRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PowerPlant_AnthologyTapuKokoResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PowerPlant_AnthologyTapuKokoResolved
	release
	end

PowerPlant_AnthologyTapuKokoResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_TAPU_KOKO
	setflag FLAG_HIDE_ANTHOLOGY_TAPU_KOKO
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

PowerPlant_AnthologyTapuKokoRetreated::
	release
	end

PowerPlant_AnthologyHideMagearna::
	setflag FLAG_HIDE_ANTHOLOGY_MAGEARNA
	return

PowerPlant_AnthologyShowMagearna::
	clearflag FLAG_HIDE_ANTHOLOGY_MAGEARNA
	return

// Caught: permanently gone. Fainted: returns on re-entry. Run: stays here.
PowerPlant_EventScript_Magearna::
	lock
	faceplayer
	playmoncry SPECIES_MAGEARNA, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_MAGEARNA, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PowerPlant_AnthologyMagearnaRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PowerPlant_AnthologyMagearnaRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PowerPlant_AnthologyMagearnaResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PowerPlant_AnthologyMagearnaResolved
	release
	end

PowerPlant_AnthologyMagearnaResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_MAGEARNA
	setflag FLAG_HIDE_ANTHOLOGY_MAGEARNA
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

PowerPlant_AnthologyMagearnaRetreated::
	release
	end

PowerPlant_AnthologyHideZeraora::
	setflag FLAG_HIDE_ANTHOLOGY_ZERAORA
	return

PowerPlant_AnthologyShowZeraora::
	clearflag FLAG_HIDE_ANTHOLOGY_ZERAORA
	return

// Same acquisition semantics as the other Power Plant statics.
// Defeat: hide until re-entry. Run: remain on map. Catch: permanently hide.
PowerPlant_EventScript_Zeraora::
	lock
	faceplayer
	playmoncry SPECIES_ZERAORA, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_ZERAORA, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PowerPlant_AnthologyZeraoraRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PowerPlant_AnthologyZeraoraRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PowerPlant_AnthologyZeraoraResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PowerPlant_AnthologyZeraoraResolved
	release
	end

PowerPlant_AnthologyZeraoraResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_ZERAORA
	setflag FLAG_HIDE_ANTHOLOGY_ZERAORA
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

PowerPlant_AnthologyZeraoraRetreated::
	release
	end

// Reconcile old defeated flags with the persistent caught species dex bit.
// This also repairs a pre-upgrade save where the species was KO'd or fled.
// All native room puzzles and battle scripts are unchanged.
PowerPlant_NativeZAPDOSCaught::
	setflag FLAG_FOUGHT_ZAPDOS
	setflag FLAG_HIDE_ZAPDOS
	return

PowerPlant_NativeZAPDOSUncaught::
	clearflag FLAG_FOUGHT_ZAPDOS
	clearflag FLAG_HIDE_ZAPDOS
	return
