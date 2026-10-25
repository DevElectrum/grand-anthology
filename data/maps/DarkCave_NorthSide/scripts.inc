DarkCave_NorthSide_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, DarkCave_NorthSide_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

DarkCave_NorthSide_OnTransition:
	call DarkCave_NorthSide_AnthologyOnTransition
	end

DarkCave2_EventScript_Item_BlackFlute::
	finditem ITEM_BLACK_FLUTE
	end

DarkCave2_EventScript_Item_StarPiece::
	finditem ITEM_STAR_PIECE
	end

DarkCave2_EventScript_BlackGlasses::
	lock
	faceplayer
	goto_if_set FLAG_GOT_BLACK_GLASSES, DarkCave2_EventScript_BlackGlassesGot
	msgbox DarkCave2_Text_Nerd, MSGBOX_DEFAULT
	giveitem ITEM_BLACK_GLASSES
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_BLACK_GLASSES
	closemessage
	release
	end

DarkCave2_EventScript_BlackGlassesGot::
	msgbox DarkCave2_Text_Nerd2, MSGBOX_DEFAULT
	closemessage
	release
	end

DarkCave2_Text_Nerd:
    .string "Whoa! You startled me there!\p"
    .string "I had my BLACK-GLASSES on, so I\n"
    .string "didn't notice you at all.\p"
    .string "What am I doing here?\p"
    .string "Hey, don't you worry about it.\p"
    .string "I'll give you a pair of BLACK-\n"
    .string "GLASSES, so forget you saw me,\n"
    .string "OK?$"

DarkCave2_Text_Nerd2:
    .string "BLACK-GLASSES ups the power of\n"
    .string "DARK-type moves.$"

DarkCave_NorthSide_AnthologyOnTransition::
	getcaughtmon SPECIES_ZEKROM
	goto_if_eq VAR_RESULT, TRUE, DarkCave_NorthSide_AnthologyHideZekrom
	clearflag FLAG_HIDE_ANTHOLOGY_ZEKROM
	return

DarkCave_NorthSide_AnthologyHideZekrom::
	setflag FLAG_HIDE_ANTHOLOGY_ZEKROM
	return

DarkCave_NorthSide_EventScript_Zekrom::
	lock
	faceplayer
	playmoncry SPECIES_ZEKROM, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_ZEKROM, 44
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, DarkCave_NorthSide_AnthologyZekromRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, DarkCave_NorthSide_AnthologyZekromRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, DarkCave_NorthSide_AnthologyZekromResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, DarkCave_NorthSide_AnthologyZekromResolved
	release
	end

DarkCave_NorthSide_AnthologyZekromResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_ZEKROM
	setflag FLAG_HIDE_ANTHOLOGY_ZEKROM
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

DarkCave_NorthSide_AnthologyZekromRetreated::
	release
	end
