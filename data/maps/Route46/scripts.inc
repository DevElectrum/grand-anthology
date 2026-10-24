Route46_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route46_AnthologyTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

Route46_OnTransition: 
	end

Route46_EventScript_Bailey::
	trainerbattle_single TRAINER_BAILEY, Route46_Text_HikerBaileySeen, Route46_Text_HikerBaileyBeaten
	msgbox Route46_Text_HikerBaileyAfterBattle, MSGBOX_AUTOCLOSE
	end

Route46_EventScript_Ted::
	trainerbattle_single TRAINER_TED_JT, Route46_Text_CamperTedSeen, Route46_Text_CamperTedBeaten
	msgbox Route46_Text_CamperTedAfterBattle, MSGBOX_AUTOCLOSE
	end

Route46_EventScript_Erin::
	trainerbattle_single TRAINER_ERIN, Route46_Text_PicnickerErinSeen, Route46_Text_PicnickerErinBeaten
	msgbox Route46_Text_PicnickerErinAfterBattle, MSGBOX_AUTOCLOSE
	end

Route46_EventScript_Sign::
	lock
	msgbox Route46_Text_Sign, MSGBOX_SIGN
	closemessage
	release
	end

Route46_EventScript_Item_Revive::
	finditem ITEM_REVIVE
	end

Route46_Text_HikerBaileySeen:
	.string "Awright! I'll show you the\n"
	.string "power of mountain POKéMON!$"

Route46_Text_HikerBaileyBeaten:
	.string "Mercy! You showed me your\n"
	.string "power!$"

Route46_Text_HikerBaileyAfterBattle:
	.string "It's over. I don't mind.\n"
	.string "We HIKERS are like that.$"

Route46_Text_CamperTedSeen:
	.string "I'm raising POKéMON too!\p"
	.string "Will you battle with me?$"

Route46_Text_CamperTedBeaten:
	.string "Wha…?$"

Route46_Text_CamperTedAfterBattle:
	.string "I did my best but came up\n"
	.string "short.\p"
	.string "No excuses--I admit I lost.$"

Route46_Text_PicnickerErinSeen:
	.string "I raise POKéMON too!\p"
	.string "Will you battle with me?$"

Route46_Text_PicnickerErinBeaten:
	.string "Oh, rats!$"

Route46_Text_PicnickerErinAfterBattle:
	.string "I've been to many GYMS, but the\n"
	.string "GYM in GOLDENROD is my favorite.\p"
	.string "It's filled with pretty flowers!$"

Route46_Text_PicnickerErin2Beaten:
	.string "Aww… I keep losing all the time!\p"
	.string "I'll just have to try harder!\n"
	.string "Anyway, thanks for battling\n"
	.string "me again and again.\p"
	.string "Here's that present from the\n"
	.string "other time.$"

Route46_Text_Sign:
	.string "ROUTE 46\n"
	.string "MOUNTAIN RD. AHEAD$"

// Optional new form or post-release roamer safety-net encounter.
Route46_AnthologyTransition::
	call Route46_AnthologyFieldTransition
	goto Route29_OnTransition

Route46_AnthologyFieldTransition::
	call Route46_FieldSyncEnteiReserve
	return

Route46_FieldSyncEnteiReserve::
	goto_if_unset FLAG_RELEASED_BEASTS, Route46_FieldHideEnteiReserve
	getcaughtmon SPECIES_ENTEI
	goto_if_eq VAR_RESULT, TRUE, Route46_FieldHideEnteiReserve
	goto Route46_FieldShowEnteiReserve

Route46_FieldHideEnteiReserve::
	setflag FLAG_HIDE_ANTHOLOGY_BEAST_ENTEI
	return

Route46_FieldShowEnteiReserve::
	clearflag FLAG_HIDE_ANTHOLOGY_BEAST_ENTEI
	return

Route46_EventScript_EnteiReserve::
	lock
	faceplayer
	playmoncry SPECIES_ENTEI, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_ENTEI, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route46_FieldEnteiReserveRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route46_FieldEnteiReserveRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route46_FieldEnteiReserveResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route46_FieldEnteiReserveResolved
	release
	end

Route46_FieldEnteiReserveResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_BEAST_ENTEI
	setflag FLAG_HIDE_ANTHOLOGY_BEAST_ENTEI
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

Route46_FieldEnteiReserveRetreated::
	release
	end
