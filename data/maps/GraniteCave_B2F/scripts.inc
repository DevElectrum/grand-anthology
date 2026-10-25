GraniteCave_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, GraniteCave_B2F_OnTransition
	.byte 0

GraniteCave_B2F_OnTransition::
	call GraniteCave_B2F_SecondBatchOnTransition
	end

// Independent rare statics: caught status persists in the National Dex.
// Fainting permits a rematch after map re-entry. Preserve original route story.
GraniteCave_B2F_SecondBatchOnTransition::
	getcaughtmon SPECIES_ZYGARDE
	call_if_eq VAR_RESULT, TRUE, GraniteCave_B2F_SecondBatchHideZygarde
	call_if_ne VAR_RESULT, TRUE, GraniteCave_B2F_SecondBatchShowZygarde
	return

GraniteCave_B2F_SecondBatchHideZygarde::
	setflag FLAG_HIDE_ANTHOLOGY_ZYGARDE
	return

GraniteCave_B2F_SecondBatchShowZygarde::
	clearflag FLAG_HIDE_ANTHOLOGY_ZYGARDE
	return

GraniteCave_B2F_EventScript_Zygarde::
	lock
	faceplayer
	playmoncry SPECIES_ZYGARDE, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_ZYGARDE, 50
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, GraniteCave_B2F_SecondBatchZygardeRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, GraniteCave_B2F_SecondBatchZygardeRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, GraniteCave_B2F_SecondBatchZygardeResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, GraniteCave_B2F_SecondBatchZygardeResolved
	release
	end

GraniteCave_B2F_SecondBatchZygardeResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_ZYGARDE
	setflag FLAG_HIDE_ANTHOLOGY_ZYGARDE
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

GraniteCave_B2F_SecondBatchZygardeRetreated::
	release
	end
