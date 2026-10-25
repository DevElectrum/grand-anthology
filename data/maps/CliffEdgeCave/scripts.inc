CliffEdgeCave_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CliffEdgeCave_OnTransition
	.byte 0

CliffEdgeCave_OnTransition::
	call CliffEdgeCave_AnthologyOnTransition
	end

CliffEdgeCave_EventScript_BaldMan::
	lock
	faceplayer
	msgbox CliffEdgeCave_Text_Baldman, MSGBOX_DEFAULT
	closemessage
	release
	end

CliffEdgeCave_Text_Baldman:
	.string "If you see a fork in the road\n"
	.string "inside a cave, most likely one\l"
	.string "way is right and the other leads\l"
	.string "to a dead end.\p"
	.string "but people can't help trying both.$"

CliffEdgeCave_AnthologyOnTransition::
	getcaughtmon SPECIES_SPECTRIER
	goto_if_eq VAR_RESULT, TRUE, CliffEdgeCave_HideSpectrier
	clearflag FLAG_HIDE_ANTHOLOGY_SPECTRIER
	return

CliffEdgeCave_HideSpectrier::
	setflag FLAG_HIDE_ANTHOLOGY_SPECTRIER
	return

CliffEdgeCave_EventScript_Spectrier::
	lock
	faceplayer
	playmoncry SPECIES_SPECTRIER, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_SPECTRIER, 34
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, CliffEdgeCave_SpectrierRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, CliffEdgeCave_SpectrierRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, CliffEdgeCave_SpectrierResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, CliffEdgeCave_SpectrierResolved
	release
	end

CliffEdgeCave_SpectrierResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_SPECTRIER
	setflag FLAG_HIDE_ANTHOLOGY_SPECTRIER
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

CliffEdgeCave_SpectrierRetreated::
	release
	end
