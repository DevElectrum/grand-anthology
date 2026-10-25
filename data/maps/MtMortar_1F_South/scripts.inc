MtMortar_1F_South_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMortar_1F_South_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

MtMortar_1F_South_OnTransition: 
	call MtMortar_1F_South_AnthologyOnTransition
	end



MtMortar_1F_South_EventScript_Item_Revive::
    finditem ITEM_REVIVE
    end

MtMortar_1F_South_EventScript_Item_Elixir::
    finditem ITEM_ELIXIR
    end

// Independent early/mid-Johto Sword of Justice static. No sibling capture gate.
MtMortar_1F_South_AnthologyOnTransition::
	getcaughtmon SPECIES_COBALION
	call_if_eq VAR_RESULT, TRUE, MtMortar_1F_South_AnthologyHideCobalion
	call_if_ne VAR_RESULT, TRUE, MtMortar_1F_South_AnthologyShowCobalion
	return

MtMortar_1F_South_AnthologyHideCobalion::
	setflag FLAG_HIDE_ANTHOLOGY_COBALION
	return

MtMortar_1F_South_AnthologyShowCobalion::
	clearflag FLAG_HIDE_ANTHOLOGY_COBALION
	return

MtMortar_1F_South_EventScript_Cobalion::
	lock
	faceplayer
	playmoncry SPECIES_COBALION, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_COBALION, 28
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtMortar_1F_South_AnthologyCobalionRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtMortar_1F_South_AnthologyCobalionRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtMortar_1F_South_AnthologyCobalionResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtMortar_1F_South_AnthologyCobalionResolved
	release
	end

MtMortar_1F_South_AnthologyCobalionResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_COBALION
	setflag FLAG_HIDE_ANTHOLOGY_COBALION
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

MtMortar_1F_South_AnthologyCobalionRetreated::
	release
	end
