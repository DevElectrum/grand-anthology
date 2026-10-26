GraniteCave_StevensRoom_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, GraniteCave_StevensRoom_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, GraniteCave_StevensRoom_OnTransition
	.byte 0

@ Steven's original letter/TM story takes priority; Diancie appears after
@ the letter and both Roxanne's and Brawly's badges, until caught.
GraniteCave_StevensRoom_OnTransition::
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, GraniteCave_StevensRoom_SeededDiancieTransition
	getcaughtmon SPECIES_DIANCIE
	goto_if_eq VAR_RESULT, TRUE, GraniteCave_StevensRoom_HideDiancie
	goto_if_unset FLAG_DELIVERED_STEVEN_LETTER, GraniteCave_StevensRoom_HideDiancie
	goto_if_unset FLAG_BADGE01_GET, GraniteCave_StevensRoom_HideDiancie
	goto_if_unset FLAG_BADGE02_GET, GraniteCave_StevensRoom_HideDiancie
	clearflag FLAG_HIDE_ANTHOLOGY_DIANCIE
	end

GraniteCave_StevensRoom_SeededDiancieTransition::
	goto_if_set FLAG_CAUGHT_RANDOM_GRANITE_DIANCIE, GraniteCave_StevensRoom_HideDiancie
	goto_if_unset FLAG_DELIVERED_STEVEN_LETTER, GraniteCave_StevensRoom_HideDiancie
	goto_if_unset FLAG_BADGE01_GET, GraniteCave_StevensRoom_HideDiancie
	goto_if_unset FLAG_BADGE02_GET, GraniteCave_StevensRoom_HideDiancie
	clearflag FLAG_HIDE_ANTHOLOGY_DIANCIE
	end

GraniteCave_StevensRoom_HideDiancie::
	setflag FLAG_HIDE_ANTHOLOGY_DIANCIE
	end

GraniteCave_StevensRoom_EventScript_Steven::
	lock
	faceplayer
	msgbox GraniteCave_StevensRoom_Text_ImStevenLetterForMe, MSGBOX_DEFAULT
	setvar VAR_0x8004, ITEM_LETTER
	call Common_EventScript_PlayerHandedOverTheItem
	setflag FLAG_DELIVERED_STEVEN_LETTER
	@ The letter is delivered *inside* this room. A transition-only guard
	@ cannot reveal Diancie until the player leaves and returns.
	@ Show it now if the Pokédex has not registered its capture.
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, GraniteCave_StevensRoom_SeededAfterLetter
	getcaughtmon SPECIES_DIANCIE
	call_if_eq VAR_RESULT, FALSE, GraniteCave_StevensRoom_RevealDiancieAfterLetter
	goto GraniteCave_StevensRoom_ContinueLetterStory

GraniteCave_StevensRoom_SeededAfterLetter::
	call_if_unset FLAG_CAUGHT_RANDOM_GRANITE_DIANCIE, GraniteCave_StevensRoom_RevealDiancieAfterLetter

GraniteCave_StevensRoom_ContinueLetterStory::
	msgbox GraniteCave_StevensRoom_Text_ThankYouTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_TM_STEEL_WING
	call_if_eq VAR_RESULT, FALSE, GraniteCave_StevensRoom_EventScript_BagFull
	msgbox GraniteCave_StevensRoom_Text_CouldBecomeChampionLetsRegister, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox GraniteCave_StevensRoom_Text_RegisteredSteven, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_REGISTERED_STEVEN_POKENAV
	msgbox GraniteCave_StevensRoom_Text_IveGotToHurryAlong, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_FACING, DIR_NORTH, GraniteCave_StevensRoom_EventScript_StevenExitNorth
	call_if_eq VAR_FACING, DIR_SOUTH, GraniteCave_StevensRoom_EventScript_StevenExitSouth
	call_if_eq VAR_FACING, DIR_WEST, GraniteCave_StevensRoom_EventScript_StevenExitWestEast
	call_if_eq VAR_FACING, DIR_EAST, GraniteCave_StevensRoom_EventScript_StevenExitWestEast
	playse SE_EXIT
	removeobject LOCALID_GRANITE_CAVE_STEVEN
	release
	end

GraniteCave_StevensRoom_RevealDiancieAfterLetter::
	goto_if_unset FLAG_BADGE01_GET, GraniteCave_StevensRoom_RevealDiancieTooEarly
	goto_if_unset FLAG_BADGE02_GET, GraniteCave_StevensRoom_RevealDiancieTooEarly
	clearflag FLAG_HIDE_ANTHOLOGY_DIANCIE
	addobject LOCALID_ANTHOLOGY_DIANCIE
GraniteCave_StevensRoom_RevealDiancieTooEarly::
	return

GraniteCave_StevensRoom_EventScript_StevenExitNorth::
	applymovement LOCALID_GRANITE_CAVE_STEVEN, GraniteCave_StevensRoom_Movement_StevenExit
	waitmovement 0
	return

GraniteCave_StevensRoom_EventScript_StevenExitWestEast::
	applymovement LOCALID_PLAYER, GraniteCave_StevensRoom_Movement_PlayerTurnTowardExit
	applymovement LOCALID_GRANITE_CAVE_STEVEN, GraniteCave_StevensRoom_Movement_StevenExit
	waitmovement 0
	return

GraniteCave_StevensRoom_EventScript_StevenExitSouth::
	applymovement LOCALID_PLAYER, GraniteCave_StevensRoom_Movement_PlayerTurnTowardExit
	applymovement LOCALID_GRANITE_CAVE_STEVEN, GraniteCave_StevensRoom_Movement_StevenExitSouth
	waitmovement 0
	return

GraniteCave_StevensRoom_EventScript_BagFull::
	msgbox GraniteCave_StevensRoom_Text_OhBagIsFull, MSGBOX_DEFAULT
	return

GraniteCave_StevensRoom_Movement_StevenExit:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	delay_8
	step_end

GraniteCave_StevensRoom_Movement_PlayerTurnTowardExit:
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_up
	step_end

GraniteCave_StevensRoom_Movement_StevenExitSouth:
	walk_left
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	delay_8
	step_end

GraniteCave_StevensRoom_Text_ImStevenLetterForMe:
	.string "My name is STEVEN.\p"
	.string "I'm interested in rare stones,\n"
	.string "so I travel here and there.\p"
	.string "Oh?\n"
	.string "A LETTER for me?$"

GraniteCave_StevensRoom_Text_ThankYouTakeThis:
	.string "STEVEN: Okay, thank you.\p"
	.string "You went through all this trouble to\n"
	.string "deliver that. I need to thank you.\p"
	.string "Let me see…\n"
	.string "I'll give you this TM.\p"
	.string "It contains my favorite move,\n"
	.string "STEEL WING.$"

GraniteCave_StevensRoom_Text_CouldBecomeChampionLetsRegister:
	.string "STEVEN: Your POKéMON appear quite\n"
	.string "capable.\p"
	.string "If you keep training, you could even\n"
	.string "become the CHAMPION of the POKéMON\l"
	.string "LEAGUE one day. That's what I think.\p"
	.string "I know, since we've gotten to know each\n"
	.string "other, let's register one another in\l"
	.string "our POKéNAVS.\p"
	.string "… … … … … …$"

GraniteCave_StevensRoom_Text_RegisteredSteven:
	.string "Registered STEVEN\n"
	.string "in the POKéNAV.$"

GraniteCave_StevensRoom_Text_IveGotToHurryAlong:
	.string "Now, I've got to hurry along.$"

GraniteCave_StevensRoom_Text_OhBagIsFull:
	.string "Oh, your BAG is full…\n"
	.string "That's too bad, then.$"


@ Repeat until caught: defeating this Pokémon removes it until the player
@ re-enters, fleeing leaves it in place, and catching permanently hides it.
GraniteCave_StevensRoom_EventScript_Diancie::
	lock
	faceplayer
	playmoncry SPECIES_DIANCIE, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_DIANCIE, 18
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, GraniteCave_StevensRoom_LegendaryRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, GraniteCave_StevensRoom_LegendaryRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, GraniteCave_StevensRoom_SeededDiancieCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, GraniteCave_StevensRoom_LegendaryResolved
	release
	end

GraniteCave_StevensRoom_SeededDiancieCaught::
	callnative AnthologyStaticRandomizerIsOn
	call_if_eq VAR_RESULT, TRUE, GraniteCave_StevensRoom_MarkSeededDiancie
	goto GraniteCave_StevensRoom_LegendaryResolved

GraniteCave_StevensRoom_MarkSeededDiancie::
	setflag FLAG_CAUGHT_RANDOM_GRANITE_DIANCIE
	return

GraniteCave_StevensRoom_LegendaryResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_DIANCIE
	setflag FLAG_HIDE_ANTHOLOGY_DIANCIE
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

GraniteCave_StevensRoom_LegendaryRetreated::
	release
	end
