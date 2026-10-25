SproutTower_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SproutTower_2F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

SproutTower_2F_OnTransition:
	call SproutTower_2F_AnthologyOnTransition
	end

SproutTower_2F_EventScript_SageNico::
	trainerbattle_single TRAINER_NICO SproutTower_Text_SageNico_Seen, SproutTower_Text_SageNico_Beaten
	msgbox SproutTower_Text_SageNico_AfterBattle, MSGBOX_AUTOCLOSE
	end

SproutTower_2F_EventScript_SageEdmond::
	trainerbattle_single TRAINER_EDMOND_JT SproutTower_Text_SageEdmond_Seen, SproutTower_Text_SageEdmond_Beaten
	msgbox SproutTower_Text_SageEdmond_AfterBattle, MSGBOX_AUTOCLOSE
	end

SproutTower_2F_EventScript_Statue::
	lock
	msgbox SproutTower_Text_2F_Statue, MSGBOX_AUTOCLOSE
	release
	end

SproutTower_2F_EventScript_Xdefend::
	finditem ITEM_X_DEFEND
	end

SproutTower_Text_SageNico_Seen:
	.string "However hard we battle,\n"
	.string "the TOWER will stand strong.$"

SproutTower_Text_SageNico_Beaten:
	.string "I fought hard but I'm too weak.$"

SproutTower_Text_SageNico_AfterBattle:
	.string "The flexible pillar protects the\n"
	.string "TOWER, even from earthquakes.$"

SproutTower_Text_SageEdmond_Seen:
	.string "…Sway like leaves in the wind…$"

SproutTower_Text_SageEdmond_Beaten:
	.string "Oh, I'm weak!$"

SproutTower_Text_SageEdmond_AfterBattle:
	.string "I tried to copy BELLSPROUT's\n"
	.string "gentle movements for battle…\p"
	.string "But I didn't train well enough.$"

SproutTower_Text_2F_Statue:
	.string "A POKéMON statue…\p"
	.string "It looks very distinguished.$"

SproutTower_2F_AnthologyOnTransition::
	call_if_set FLAG_CAUGHT_ANTHOLOGY_KUBFU_WEST, SproutTower_2F_HideKubfuWest
	call_if_unset FLAG_CAUGHT_ANTHOLOGY_KUBFU_WEST, SproutTower_2F_ShowKubfuWest
	return

SproutTower_2F_HideKubfuWest::
	setflag FLAG_HIDE_ANTHOLOGY_KUBFU_WEST
	return

SproutTower_2F_ShowKubfuWest::
	clearflag FLAG_HIDE_ANTHOLOGY_KUBFU_WEST
	return

SproutTower_2F_EventScript_KubfuWest::
	lock
	faceplayer
	playmoncry SPECIES_KUBFU, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_KUBFU, 12
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, SproutTower_2F_KubfuWestRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, SproutTower_2F_KubfuWestRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, SproutTower_2F_KubfuWestCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, SproutTower_2F_KubfuWestResolved
	release
	end

SproutTower_2F_KubfuWestCaught::
	setflag FLAG_CAUGHT_ANTHOLOGY_KUBFU_WEST
	goto SproutTower_2F_KubfuWestResolved

SproutTower_2F_KubfuWestResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_KUBFU_WEST
	setflag FLAG_HIDE_ANTHOLOGY_KUBFU_WEST
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

SproutTower_2F_KubfuWestRetreated::
	release
	end
