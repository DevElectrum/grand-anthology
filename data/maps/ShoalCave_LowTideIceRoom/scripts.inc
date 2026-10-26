ShoalCave_LowTideIceRoom_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ShoalCave_LowTideIceRoom_OnTransition
	.byte 0

ShoalCave_LowTideIceRoom_OnTransition::
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, ShoalCave_LowTideIceRoom_SeededTransition
	getcaughtmon SPECIES_GLASTRIER
	goto_if_eq VAR_RESULT, TRUE, ShoalCave_LowTideIceRoom_HideGlastrier
	clearflag FLAG_HIDE_ANTHOLOGY_GLASTRIER
	end

ShoalCave_LowTideIceRoom_SeededTransition::
	goto_if_set FLAG_CAUGHT_RANDOM_SHOALCAVE_GLASTRIER, ShoalCave_LowTideIceRoom_HideGlastrier
	clearflag FLAG_HIDE_ANTHOLOGY_GLASTRIER
	end

ShoalCave_LowTideIceRoom_HideGlastrier::
	setflag FLAG_HIDE_ANTHOLOGY_GLASTRIER
	end


@ Repeat until caught: defeating this Pokémon removes it until the player
@ re-enters, fleeing leaves it in place, and catching permanently hides it.
ShoalCave_LowTideIceRoom_EventScript_Glastrier::
	lock
	faceplayer
	playmoncry SPECIES_GLASTRIER, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_GLASTRIER, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, ShoalCave_LowTideIceRoom_LegendaryRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, ShoalCave_LowTideIceRoom_LegendaryRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, ShoalCave_LowTideIceRoom_LegendaryCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, ShoalCave_LowTideIceRoom_LegendaryResolved
	release
	end

ShoalCave_LowTideIceRoom_LegendaryCaught::
	callnative AnthologyStaticRandomizerIsOn
	call_if_eq VAR_RESULT, TRUE, ShoalCave_LowTideIceRoom_MarkSeededGlastrier
	goto ShoalCave_LowTideIceRoom_LegendaryResolved

ShoalCave_LowTideIceRoom_MarkSeededGlastrier::
	setflag FLAG_CAUGHT_RANDOM_SHOALCAVE_GLASTRIER
	return

ShoalCave_LowTideIceRoom_LegendaryResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_GLASTRIER
	setflag FLAG_HIDE_ANTHOLOGY_GLASTRIER
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

ShoalCave_LowTideIceRoom_LegendaryRetreated::
	release
	end
