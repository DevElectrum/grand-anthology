.set LOCALID_ICEPATH2_BOULDER1, 3
.set LOCALID_ICEPATH2_BOULDER2, 4
.set LOCALID_ICEPATH2_BOULDER3, 2
.set LOCALID_ICEPATH2_BOULDER4, 1

IcePath_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CaveHole_CheckFallDownHole
	map_script MAP_SCRIPT_ON_TRANSITION, IcePath_B1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, IcePath_B1F_SetHoleWarp
	.byte 0

IcePath_B1F_OnTransition:
	copyvar VAR_ICE_STEP_COUNT, 1
	call IcePath_B1F_AnthologyOnTransition
	end

IcePath_B1F_SetHoleWarp:
	setstepcallback STEP_CB_CRACKED_FLOOR
	setholewarp MAP_ICE_PATH_B2F
	end

IcePath_B1F_EventScript_PuzzleSkipButton::
	lock
	msgbox IcePath_B1F_Text_PuzzleSkipButton, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, FALSE, IcePath_B1F_EventScript_PuzzleSkipButtonNo
	warp MAP_ICE_PATH_B1F, 15, 12
	waitstate
	release
	end

IcePath_B1F_EventScript_PuzzleSkipButtonNo::
	release
	end

IcePath_B1F_Text_PuzzleSkipButton::
	.string "Would you like to skip this\n"
	.string "puzzle?$"

IcePath_B1F_EventScript_Item_Iron::
	finditem ITEM_IRON
	end
IcePath_B1F_EventScript_Item_Revive::
	finditem ITEM_REVIVE
	end

IcePath_B1F_EventScript_Sign::
	msgbox IcePath_B1F_Text_Sign, MSGBOX_SIGN
	end

IcePath_B1F_Text_Sign:
	.string "DANGER CRACKED ICE\n"
	.string "GO FAST ON BICYCLE\l"
	.string "SWITCH GEARS BY\l"
	.string "PRESSING …\l"
	.string "The sign is faded…$"


IcePath_B1F_AnthologyOnTransition::
	getcaughtmon SPECIES_KYUREM
	goto_if_eq VAR_RESULT, TRUE, IcePath_B1F_AnthologyHideKyurem
	getcaughtmon SPECIES_RESHIRAM
	goto_if_ne VAR_RESULT, TRUE, IcePath_B1F_AnthologyHideKyurem
	getcaughtmon SPECIES_ZEKROM
	goto_if_ne VAR_RESULT, TRUE, IcePath_B1F_AnthologyHideKyurem
	clearflag FLAG_HIDE_ANTHOLOGY_KYUREM
	return

IcePath_B1F_AnthologyHideKyurem::
	setflag FLAG_HIDE_ANTHOLOGY_KYUREM
	return

IcePath_B1F_EventScript_Kyurem::
	lock
	faceplayer
	playmoncry SPECIES_KYUREM, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_KYUREM, 49
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, IcePath_B1F_AnthologyKyuremRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, IcePath_B1F_AnthologyKyuremRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, IcePath_B1F_AnthologyKyuremCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, IcePath_B1F_AnthologyKyuremResolved
	release
	end

IcePath_B1F_AnthologyKyuremResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_KYUREM
	setflag FLAG_HIDE_ANTHOLOGY_KYUREM
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

IcePath_B1F_AnthologyKyuremCaught::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_KYUREM
	setflag FLAG_HIDE_ANTHOLOGY_KYUREM
	fadescreenswapbuffers FADE_FROM_BLACK
	checkitem ITEM_DNA_SPLICERS
	goto_if_eq VAR_RESULT, TRUE, IcePath_B1F_AnthologyKyuremRetreated
	giveitem ITEM_DNA_SPLICERS
	goto_if_eq VAR_RESULT, TRUE, IcePath_B1F_AnthologyKyuremRetreated
	msgbox IcePath_B1F_Text_SplicersBagFull, MSGBOX_DEFAULT
	closemessage
	release
	end

IcePath_B1F_AnthologyKyuremRetreated::
	release
	end

// After a full bag, the adjacent plinth restores uncaptured DNA Splicers.
IcePath_B1F_EventScript_DNASplicersPlinth::
	lock
	getcaughtmon SPECIES_KYUREM
	goto_if_ne VAR_RESULT, TRUE, IcePath_B1F_EventScript_PlinthSealed
	checkitem ITEM_DNA_SPLICERS
	goto_if_eq VAR_RESULT, TRUE, IcePath_B1F_EventScript_PlinthEmpty
	giveitem ITEM_DNA_SPLICERS
	goto_if_eq VAR_RESULT, FALSE, IcePath_B1F_EventScript_PlinthBagFull
	release
	end

IcePath_B1F_EventScript_PlinthSealed::
	msgbox IcePath_B1F_Text_PlinthSealed, MSGBOX_DEFAULT
	release
	end

IcePath_B1F_EventScript_PlinthEmpty::
	msgbox IcePath_B1F_Text_PlinthEmpty, MSGBOX_DEFAULT
	release
	end

IcePath_B1F_EventScript_PlinthBagFull::
	msgbox IcePath_B1F_Text_SplicersBagFull, MSGBOX_DEFAULT
	release
	end

IcePath_B1F_Text_PlinthSealed:
	.string "Two distant dragons must answer\n"
	.string "this frozen inscription.$"

IcePath_B1F_Text_PlinthEmpty:
	.string "The frozen plinth is silent.$"

IcePath_B1F_Text_SplicersBagFull:
	.string "Your BAG is full! Return to\n"
	.string "this plinth for DNA SPLICERS.$"
