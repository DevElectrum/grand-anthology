PokemonTower_7F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonTower_7F_OnTransition
	.byte 0

PokemonTower_7F_OnTransition::
	call PokemonTower_7F_AnthologyOnTransition
	end

// The Rocket/Mr. Fuji rescue is native and unchanged. Darkrai remains hidden
// until Fuji is actually rescued and Cresselia has been registered caught.
PokemonTower_7F_AnthologyOnTransition::
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, PokemonTower_7F_RandomDarkraiOnTransition
	goto_if_unset FLAG_RESCUED_MR_FUJI, PokemonTower_7F_AnthologyHideDarkrai
	getcaughtmon SPECIES_CRESSELIA
	goto_if_ne VAR_RESULT, TRUE, PokemonTower_7F_AnthologyHideDarkrai
	getcaughtmon SPECIES_DARKRAI
	call_if_eq VAR_RESULT, TRUE, PokemonTower_7F_AnthologyHideDarkrai
	call_if_ne VAR_RESULT, TRUE, PokemonTower_7F_AnthologyShowDarkrai
	return

PokemonTower_7F_RandomDarkraiOnTransition::
	goto_if_unset FLAG_RESCUED_MR_FUJI, PokemonTower_7F_AnthologyHideDarkrai
	goto_if_unset FLAG_CAUGHT_RANDOM_MTMOON_CRESSELIA, PokemonTower_7F_AnthologyHideDarkrai
	call_if_set FLAG_CAUGHT_RANDOM_POKEMONTOWER_DARKRAI, PokemonTower_7F_AnthologyHideDarkrai
	call_if_unset FLAG_CAUGHT_RANDOM_POKEMONTOWER_DARKRAI, PokemonTower_7F_AnthologyShowDarkrai
	return

PokemonTower_7F_EventScript_MrFuji::
	lock
	faceplayer
	famechecker FAMECHECKER_MRFUJI, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	setflag FLAG_HIDE_TOWER_FUJI
	clearflag FLAG_HIDE_POKEHOUSE_FUJI
	setflag FLAG_RESCUED_MR_FUJI
	msgbox PokemonTower_7F_Text_MrFujiThankYouFollowMe
	closemessage
	warp MAP_LAVENDER_TOWN_VOLUNTEER_POKEMON_HOUSE, 4, 7
	waitstate
	release
	end

PokemonTower_7F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_19, PokemonTower_7F_Text_Grunt1Intro, PokemonTower_7F_Text_Grunt1Defeat, PokemonTower_7F_EventScript_DefeatedGrunt1
	msgbox PokemonTower_7F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_7F_EventScript_DefeatedGrunt1::
	msgbox PokemonTower_7F_Text_Grunt1PostBattle
	closemessage
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_eq VAR_0x8004, 10, PokemonTower_7F_EventScript_Grunt1ExitMid
	goto_if_eq VAR_0x8004, 11, PokemonTower_7F_EventScript_Grunt1ExitRight
	goto_if_eq VAR_0x8004, 9, PokemonTower_7F_EventScript_Grunt1ExitLeft
	applymovement LOCALID_POKEMON_TOWER_GRUNT1, PokemonTower_7F_Movement_Grunt1Exit
	waitmovement 0
	goto PokemonTower_7F_EventScript_RemoveGrunt1
	end

PokemonTower_7F_EventScript_Grunt1ExitMid::
	applymovement LOCALID_POKEMON_TOWER_GRUNT1, PokemonTower_7F_Movement_Grunt1ExitMid
	waitmovement 0
	goto PokemonTower_7F_EventScript_RemoveGrunt1
	end

PokemonTower_7F_EventScript_Grunt1ExitRight::
	applymovement LOCALID_POKEMON_TOWER_GRUNT1, PokemonTower_7F_Movement_Grunt1ExitRight
	waitmovement 0
	goto PokemonTower_7F_EventScript_RemoveGrunt1
	end

PokemonTower_7F_EventScript_Grunt1ExitLeft::
	applymovement LOCALID_POKEMON_TOWER_GRUNT1, PokemonTower_7F_Movement_Grunt1ExitLeft
	waitmovement 0
	playse SE_EXIT
	delay 25
	goto PokemonTower_7F_EventScript_RemoveGrunt1
	end

PokemonTower_7F_EventScript_RemoveGrunt1::
	removeobject LOCALID_POKEMON_TOWER_GRUNT1
	release
	end

PokemonTower_7F_Movement_Grunt1ExitMid::
	walk_down
	walk_right
	walk_down
	walk_down
	walk_down
	walk_right
	walk_down
	walk_down
	step_end

PokemonTower_7F_Movement_Grunt1ExitRight::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_down
	walk_down
	step_end

PokemonTower_7F_Movement_Grunt1Exit::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

PokemonTower_7F_Movement_Grunt1ExitLeft::
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

PokemonTower_7F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_20, PokemonTower_7F_Text_Grunt2Intro, PokemonTower_7F_Text_Grunt2Defeat, PokemonTower_7F_EventScript_DefeatedGrunt2
	msgbox PokemonTower_7F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_7F_EventScript_DefeatedGrunt2::
	famechecker FAMECHECKER_MRFUJI, 1
	msgbox PokemonTower_7F_Text_Grunt2PostBattle
	closemessage
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_eq VAR_0x8004, 12, PokemonTower_7F_EventScript_Grunt2ExitLeft
	goto_if_eq VAR_0x8004, 13, PokemonTower_7F_EventScript_Grunt2ExitRight
	applymovement LOCALID_POKEMON_TOWER_GRUNT2, PokemonTower_7F_Movement_Grunt2Exit
	waitmovement 0
	goto PokemonTower_7F_EventScript_RemoveGrunt2
	end

PokemonTower_7F_EventScript_Grunt2ExitLeft::
	applymovement LOCALID_POKEMON_TOWER_GRUNT2, PokemonTower_7F_Movement_Grunt2ExitLeft
	waitmovement 0
	goto PokemonTower_7F_EventScript_RemoveGrunt2
	end

PokemonTower_7F_EventScript_Grunt2ExitRight::
	applymovement LOCALID_POKEMON_TOWER_GRUNT2, PokemonTower_7F_Movement_Grunt2ExitRight
	waitmovement 0
	goto PokemonTower_7F_EventScript_RemoveGrunt2
	end

PokemonTower_7F_EventScript_RemoveGrunt2::
	removeobject LOCALID_POKEMON_TOWER_GRUNT2
	release
	end

PokemonTower_7F_Movement_Grunt2ExitLeft::
	walk_down
	walk_down
	walk_down
	walk_left
	walk_down
	walk_down
	walk_down
	step_end

PokemonTower_7F_Movement_Grunt2Exit::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

PokemonTower_7F_Movement_Grunt2ExitRight::
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

PokemonTower_7F_EventScript_Grunt3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_21, PokemonTower_7F_Text_Grunt3Intro, PokemonTower_7F_Text_Grunt3Defeat, PokemonTower_7F_EventScript_DefeatedGrunt3
	msgbox PokemonTower_7F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_7F_EventScript_DefeatedGrunt3::
	msgbox PokemonTower_7F_Text_Grunt3PostBattle
	closemessage
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_eq VAR_0x8004, 10, PokemonTower_7F_EventScript_Grunt3ExitRight
	goto_if_eq VAR_0x8004, 9, PokemonTower_7F_EventScript_Grunt3ExitLeft
	applymovement LOCALID_POKEMON_TOWER_GRUNT3, PokemonTower_7F_Movement_Grunt3Exit
	waitmovement 0
	goto PokemonTower_7F_EventScript_RemoveGrunt3
	end

PokemonTower_7F_EventScript_Grunt3ExitRight::
	applymovement LOCALID_POKEMON_TOWER_GRUNT3, PokemonTower_7F_Movement_Grunt3ExitRight
	waitmovement 0
	goto PokemonTower_7F_EventScript_RemoveGrunt3
	end

PokemonTower_7F_EventScript_Grunt3ExitLeft::
	applymovement LOCALID_POKEMON_TOWER_GRUNT3, PokemonTower_7F_Movement_Grunt3ExitLeft
	waitmovement 0
	goto PokemonTower_7F_EventScript_RemoveGrunt3
	end

PokemonTower_7F_EventScript_RemoveGrunt3::
	removeobject LOCALID_POKEMON_TOWER_GRUNT3
	release
	end

PokemonTower_7F_EventScript_Unused::
	release
	end

PokemonTower_7F_Movement_Grunt3ExitRight::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_down
	step_end

PokemonTower_7F_Movement_Grunt3Exit::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

PokemonTower_7F_Movement_Grunt3ExitLeft::
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

PokemonTower_7F_Text_Grunt1Intro::
	.string "What do you want?\n"
	.string "Why are you here?$"

PokemonTower_7F_Text_Grunt1Defeat::
	.string "I give up!$"

PokemonTower_7F_Text_Grunt1PostBattle::
	.string "I'm not going to forget this!$"

PokemonTower_7F_Text_Grunt2Intro::
	.string "This old guy marched right up to\n"
	.string "our HIDEOUT.\p"
	.string "Then, he starts ranting about how\n"
	.string "TEAM ROCKET's abusing POKéMON.\p"
	.string "So, we're just talking it over as\n"
	.string "adults.$"

PokemonTower_7F_Text_Grunt2Defeat::
	.string "Please!\n"
	.string "No more!$"

PokemonTower_7F_Text_Grunt2PostBattle::
	.string "POKéMON are only good for making\n"
	.string "money. Why not use them?\p"
	.string "You stay out of our business!$"

PokemonTower_7F_Text_Grunt3Intro::
	.string "You're not saving anyone, kid!$"

PokemonTower_7F_Text_Grunt3Defeat::
	.string "Don't fight us ROCKETS!$"

PokemonTower_7F_Text_Grunt3PostBattle::
	.string "You're not getting away with this!$"

PokemonTower_7F_Text_MrFujiThankYouFollowMe::
	.string "MR. FUJI: Heh?\n"
	.string "You came to save me?\p"
	.string "Thank you. But, I came here of my\n"
	.string "own free will.\p"
	.string "I came to calm the spirit of\n"
	.string "CUBONE's mother.\p"
	.string "I think MAROWAK's spirit has\n"
	.string "finally left us.\p"
	.string "I must thank you for your kind\n"
	.string "concern.\p"
	.string "Follow me to my home, POKéMON\n"
	.string "HOUSE, at the foot of this tower.$"


// Static encounter is on the 7F public aisle, separate from Fuji's script.
PokemonTower_7F_AnthologyHideDarkrai::
	setflag FLAG_HIDE_ANTHOLOGY_DARKRAI
	return

PokemonTower_7F_AnthologyShowDarkrai::
	clearflag FLAG_HIDE_ANTHOLOGY_DARKRAI
	return

PokemonTower_7F_EventScript_Darkrai::
	lock
	faceplayer
	playmoncry SPECIES_DARKRAI, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_DARKRAI, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PokemonTower_7F_AnthologyDarkraiRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PokemonTower_7F_AnthologyDarkraiRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PokemonTower_7F_AnthologyDarkraiCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PokemonTower_7F_AnthologyDarkraiResolved
	release
	end

PokemonTower_7F_AnthologyDarkraiCaught::
	setflag FLAG_CAUGHT_RANDOM_POKEMONTOWER_DARKRAI
	goto PokemonTower_7F_AnthologyDarkraiResolved

PokemonTower_7F_AnthologyDarkraiResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_DARKRAI
	setflag FLAG_HIDE_ANTHOLOGY_DARKRAI
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

PokemonTower_7F_AnthologyDarkraiRetreated::
	release
	end


Anthology_MegaStone_DARKRANITE::
    callnative AnthologyStaticRandomizerIsOn
    goto_if_eq VAR_RESULT, TRUE, PokemonTower_7F_MegaStoneDarkraiRandom
    getcaughtmon SPECIES_DARKRAI
    goto_if_ne VAR_RESULT, TRUE, Anthology_MegaStone_DARKRANITE_Locked
    finditem ITEM_DARKRANITE
    end
PokemonTower_7F_MegaStoneDarkraiRandom::
    goto_if_unset FLAG_CAUGHT_RANDOM_POKEMONTOWER_DARKRAI, Anthology_MegaStone_DARKRANITE_Locked
    finditem ITEM_DARKRANITE
    end
Anthology_MegaStone_DARKRANITE_Locked::
    msgbox Anthology_MegaStone_DARKRANITE_LockedText, MSGBOX_DEFAULT
    end
Anthology_MegaStone_DARKRANITE_LockedText::
    .string "A special MEGA STONE resonates here.\p"
    .string "Return when its guardian is yours.$"
