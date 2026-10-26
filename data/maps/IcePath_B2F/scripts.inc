IcePath_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, IcePath_B2F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

IcePath_B2F_OnTransition:
	call IcePath_B2F_AnthologyOnTransition
	end

IcePath_B2F_EventScript_Item_FullHeal::
	finditem ITEM_FULL_HEAL
	end

IcePath_B2F_EventScript_Item_MaxPotion::
	finditem ITEM_MAX_POTION
	end

IcePath_B2F_EventScript_Item_Carbos::
	finditem ITEM_CARBOS
	end

IcePath_B2F_AnthologyOnTransition::
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, IcePath_B2F_AnthologyRandomOnTransition
	getcaughtmon SPECIES_GLASTRIER
	goto_if_eq VAR_RESULT, TRUE, IcePath_B2F_HideGlastrierJohto
	clearflag FLAG_HIDE_ANTHOLOGY_JOHTO_GLASTRIER
	return

IcePath_B2F_AnthologyRandomOnTransition::
	goto_if_set FLAG_CAUGHT_RANDOM_ICEPATH_GLASTRIER, IcePath_B2F_HideGlastrierJohto
	clearflag FLAG_HIDE_ANTHOLOGY_JOHTO_GLASTRIER
	return

IcePath_B2F_HideGlastrierJohto::
	setflag FLAG_HIDE_ANTHOLOGY_JOHTO_GLASTRIER
	return

IcePath_B2F_EventScript_GlastrierJohto::
	lock
	faceplayer
	playmoncry SPECIES_GLASTRIER, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_GLASTRIER, 42
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, IcePath_B2F_GlastrierJohtoRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, IcePath_B2F_GlastrierJohtoRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, IcePath_B2F_GlastrierJohtoResolvedRandomCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, IcePath_B2F_GlastrierJohtoResolved
	release
	end

IcePath_B2F_GlastrierJohtoResolvedRandomCaught::
	setflag FLAG_CAUGHT_RANDOM_ICEPATH_GLASTRIER
	goto IcePath_B2F_GlastrierJohtoResolved

IcePath_B2F_GlastrierJohtoResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_JOHTO_GLASTRIER
	setflag FLAG_HIDE_ANTHOLOGY_JOHTO_GLASTRIER
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

IcePath_B2F_GlastrierJohtoRetreated::
	release
	end
