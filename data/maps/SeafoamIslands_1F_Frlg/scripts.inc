SeafoamIslands_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SeafoamIslands_1F_OnTransition
	.byte 0

SeafoamIslands_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SEAFOAM_ISLANDS_1F
	call SeafoamIslands_1F_AnthologyOnTransition
	end


// Restore uncaught statics on entry; let the original map scripts run.
SeafoamIslands_1F_AnthologyOnTransition::
	call_if_set FLAG_CAUGHT_ANTHOLOGY_COSMOG_SEAFOAM, SeafoamIslands_1F_AnthologyHideCosmogSeafoam
	call_if_unset FLAG_CAUGHT_ANTHOLOGY_COSMOG_SEAFOAM, SeafoamIslands_1F_AnthologyShowCosmogSeafoam
	return

SeafoamIslands_1F_AnthologyHideCosmogSeafoam::
	setflag FLAG_HIDE_ANTHOLOGY_COSMOG_SEAFOAM
	return

SeafoamIslands_1F_AnthologyShowCosmogSeafoam::
	clearflag FLAG_HIDE_ANTHOLOGY_COSMOG_SEAFOAM
	return

// Caught: permanently gone. Fainted: returns on re-entry. Run: stays here.
SeafoamIslands_1F_EventScript_CosmogSeafoam::
	lock
	faceplayer
	playmoncry SPECIES_COSMOG, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_COSMOG, 15
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, SeafoamIslands_1F_AnthologyCosmogSeafoamRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, SeafoamIslands_1F_AnthologyCosmogSeafoamRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, SeafoamIslands_1F_AnthologyCosmogSeafoamCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, SeafoamIslands_1F_AnthologyCosmogSeafoamResolved
	release
	end

SeafoamIslands_1F_AnthologyCosmogSeafoamCaught::
	setflag FLAG_CAUGHT_ANTHOLOGY_COSMOG_SEAFOAM
	goto SeafoamIslands_1F_AnthologyCosmogSeafoamResolved

SeafoamIslands_1F_AnthologyCosmogSeafoamResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_COSMOG_SEAFOAM
	setflag FLAG_HIDE_ANTHOLOGY_COSMOG_SEAFOAM
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

SeafoamIslands_1F_AnthologyCosmogSeafoamRetreated::
	release
	end
