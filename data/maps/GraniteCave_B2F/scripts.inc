GraniteCave_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, GraniteCave_B2F_OnTransition
	.byte 0

GraniteCave_B2F_OnTransition::
	call GraniteCave_B2F_SecondBatchOnTransition
	call GraniteCave_B2F_AnthologyOnTransition
	end

// Independent rare statics: caught status persists in the National Dex.
// Fainting permits a rematch after map re-entry. Preserve original route story.
GraniteCave_B2F_AnthologyOnTransition::
	getcaughtmon SPECIES_TING_LU
	call_if_eq VAR_RESULT, TRUE, GraniteCave_B2F_AnthologyHideTingLu
	call_if_ne VAR_RESULT, TRUE, GraniteCave_B2F_AnthologyShowTingLu
	return

GraniteCave_B2F_AnthologyHideTingLu::
	setflag FLAG_HIDE_ANTHOLOGY_TING_LU
	return

GraniteCave_B2F_AnthologyShowTingLu::
	clearflag FLAG_HIDE_ANTHOLOGY_TING_LU
	return

GraniteCave_B2F_EventScript_TingLu::
	lock
	faceplayer
	playmoncry SPECIES_TING_LU, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_TING_LU, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, GraniteCave_B2F_AnthologyTingLuRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, GraniteCave_B2F_AnthologyTingLuRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, GraniteCave_B2F_AnthologyTingLuResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, GraniteCave_B2F_AnthologyTingLuResolved
	release
	end

GraniteCave_B2F_AnthologyTingLuResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_TING_LU
	setflag FLAG_HIDE_ANTHOLOGY_TING_LU
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

GraniteCave_B2F_AnthologyTingLuRetreated::
	release
	end

// Hoenn pre-League sites independent of the original story interactions.
GraniteCave_B2F_SecondBatchOnTransition::
	getcaughtmon SPECIES_CHIEN_PAO
	call_if_eq VAR_RESULT, TRUE, GraniteCave_B2F_SecondBatchHideChienPao
	call_if_ne VAR_RESULT, TRUE, GraniteCave_B2F_SecondBatchShowChienPao
	getcaughtmon SPECIES_ZYGARDE
	call_if_eq VAR_RESULT, TRUE, GraniteCave_B2F_SecondBatchHideZygarde
	call_if_ne VAR_RESULT, TRUE, GraniteCave_B2F_SecondBatchShowZygarde
	return

GraniteCave_B2F_SecondBatchHideChienPao::
	setflag FLAG_HIDE_ANTHOLOGY_CHIEN_PAO
	return

GraniteCave_B2F_SecondBatchShowChienPao::
	clearflag FLAG_HIDE_ANTHOLOGY_CHIEN_PAO
	return

GraniteCave_B2F_EventScript_ChienPao::
	lock
	faceplayer
	playmoncry SPECIES_CHIEN_PAO, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_CHIEN_PAO, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, GraniteCave_B2F_SecondBatchChienPaoRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, GraniteCave_B2F_SecondBatchChienPaoRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, GraniteCave_B2F_SecondBatchChienPaoResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, GraniteCave_B2F_SecondBatchChienPaoResolved
	release
	end

GraniteCave_B2F_SecondBatchChienPaoResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_CHIEN_PAO
	setflag FLAG_HIDE_ANTHOLOGY_CHIEN_PAO
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

GraniteCave_B2F_SecondBatchChienPaoRetreated::
	release
	end

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
