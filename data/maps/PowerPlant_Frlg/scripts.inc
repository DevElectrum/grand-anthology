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
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, PowerPlant_SeededZapdosVisibility
	getcaughtmon SPECIES_ZAPDOS
	call_if_eq VAR_RESULT, TRUE, PowerPlant_NativeZAPDOSCaught
	call_if_ne VAR_RESULT, TRUE, PowerPlant_NativeZAPDOSUncaught
	goto PowerPlant_AfterSeededZapdosVisibility

PowerPlant_AfterSeededZapdosVisibility::
	call_if_unset FLAG_FOUGHT_POWER_PLANT_ELECTRODE_1, PowerPlant_EventScript_ShowElectrode1
	call_if_unset FLAG_FOUGHT_POWER_PLANT_ELECTRODE_2, PowerPlant_EventScript_ShowElectrode2
	call PowerPlant_AnthologyOnTransition
	end

PowerPlant_SeededZapdosVisibility::
	call_if_set FLAG_CAUGHT_RANDOM_POWERPLANT_ZAPDOS, PowerPlant_NativeZAPDOSCaught
	call_if_unset FLAG_CAUGHT_RANDOM_POWERPLANT_ZAPDOS, PowerPlant_NativeZAPDOSUncaught
	goto PowerPlant_AfterSeededZapdosVisibility

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
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PowerPlant_SeededZapdosCaught
	setflag FLAG_FOUGHT_ZAPDOS
	release
	end

PowerPlant_SeededZapdosCaught::
	callnative AnthologyStaticRandomizerIsOn
	call_if_eq VAR_RESULT, TRUE, PowerPlant_SeededZapdosMark
	setflag FLAG_FOUGHT_ZAPDOS
	release
	end

PowerPlant_SeededZapdosMark::
	setflag FLAG_CAUGHT_RANDOM_POWERPLANT_ZAPDOS
	return

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
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, PowerPlant_AnthologyRandomOnTransition
	getcaughtmon SPECIES_TAPU_KOKO
	call_if_eq VAR_RESULT, TRUE, PowerPlant_AnthologyHideTapuKoko
	call_if_ne VAR_RESULT, TRUE, PowerPlant_AnthologyShowTapuKoko
	getcaughtmon SPECIES_ZERAORA
	call_if_eq VAR_RESULT, TRUE, PowerPlant_AnthologyHideZeraora
	call_if_ne VAR_RESULT, TRUE, PowerPlant_AnthologyShowZeraora
	return

PowerPlant_AnthologyRandomOnTransition::
	call_if_set FLAG_CAUGHT_RANDOM_POWERPLANT_TAPU_KOKO, PowerPlant_AnthologyHideTapuKoko
	call_if_unset FLAG_CAUGHT_RANDOM_POWERPLANT_TAPU_KOKO, PowerPlant_AnthologyShowTapuKoko
	call_if_set FLAG_CAUGHT_RANDOM_POWERPLANT_ZERAORA, PowerPlant_AnthologyHideZeraora
	call_if_unset FLAG_CAUGHT_RANDOM_POWERPLANT_ZERAORA, PowerPlant_AnthologyShowZeraora
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
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PowerPlant_AnthologyTapuKokoResolvedRandomCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PowerPlant_AnthologyTapuKokoResolved
	release
	end

PowerPlant_AnthologyTapuKokoResolvedRandomCaught::
	setflag FLAG_CAUGHT_RANDOM_POWERPLANT_TAPU_KOKO
	goto PowerPlant_AnthologyTapuKokoResolved

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
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PowerPlant_AnthologyZeraoraResolvedRandomCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PowerPlant_AnthologyZeraoraResolved
	release
	end

PowerPlant_AnthologyZeraoraResolvedRandomCaught::
	setflag FLAG_CAUGHT_RANDOM_POWERPLANT_ZERAORA
	goto PowerPlant_AnthologyZeraoraResolved

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

Anthology_MegaStone_ZERAORITE::
    callnative AnthologyStaticRandomizerIsOn
    goto_if_eq VAR_RESULT, TRUE, Anthology_MegaStone_ZERAORITE_Randomized
    getcaughtmon SPECIES_ZERAORA
    goto_if_ne VAR_RESULT, TRUE, Anthology_MegaStone_ZERAORITE_Locked
    goto Anthology_MegaStone_ZERAORITE_Ready
Anthology_MegaStone_ZERAORITE_Randomized::
    goto_if_unset FLAG_CAUGHT_RANDOM_POWERPLANT_ZERAORA, Anthology_MegaStone_ZERAORITE_Locked
Anthology_MegaStone_ZERAORITE_Ready::
    finditem ITEM_ZERAORITE
    end
Anthology_MegaStone_ZERAORITE_Locked::
    msgbox Anthology_MegaStone_ZERAORITE_LockedText, MSGBOX_DEFAULT
    end
Anthology_MegaStone_ZERAORITE_LockedText::
    .string "A special MEGA STONE resonates here.\p"
    .string "Return when its guardian is yours.$"
