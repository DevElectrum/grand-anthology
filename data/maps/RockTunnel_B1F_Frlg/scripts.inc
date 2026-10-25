RockTunnel_B1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RockTunnel_B1F_OnTransition
	.byte 0

RockTunnel_B1F_OnTransition::
	call RockTunnel_B1F_BigBatchOnTransition
	call RockTunnel_B1F_AnthologyOnTransition
	end

RockTunnel_B1F_EventScript_Sofia::
	trainerbattle_single TRAINER_PICNICKER_SOFIA, RockTunnel_B1F_Text_SofiaIntro, RockTunnel_B1F_Text_SofiaDefeat
	msgbox RockTunnel_B1F_Text_SofiaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Martha::
	trainerbattle_single TRAINER_PICNICKER_MARTHA, RockTunnel_B1F_Text_MarthaIntro, RockTunnel_B1F_Text_MarthaDefeat
	msgbox RockTunnel_B1F_Text_MarthaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Dudley::
	trainerbattle_single TRAINER_HIKER_DUDLEY, RockTunnel_B1F_Text_DudleyIntro, RockTunnel_B1F_Text_DudleyDefeat
	msgbox RockTunnel_B1F_Text_DudleyPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Allen::
	trainerbattle_single TRAINER_HIKER_ALLEN, RockTunnel_B1F_Text_AllenIntro, RockTunnel_B1F_Text_AllenDefeat
	msgbox RockTunnel_B1F_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Eric::
	trainerbattle_single TRAINER_HIKER_ERIC, RockTunnel_B1F_Text_EricIntro, RockTunnel_B1F_Text_EricDefeat
	msgbox RockTunnel_B1F_Text_EricPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Cooper::
	trainerbattle_single TRAINER_POKEMANIAC_COOPER, RockTunnel_B1F_Text_CooperIntro, RockTunnel_B1F_Text_CooperDefeat
	msgbox RockTunnel_B1F_Text_CooperPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Steve::
	trainerbattle_single TRAINER_POKEMANIAC_STEVE, RockTunnel_B1F_Text_SteveIntro, RockTunnel_B1F_Text_SteveDefeat
	msgbox RockTunnel_B1F_Text_StevePostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Winston::
	trainerbattle_single TRAINER_POKEMANIAC_WINSTON, RockTunnel_B1F_Text_WinstonIntro, RockTunnel_B1F_Text_WinstonDefeat
	msgbox RockTunnel_B1F_Text_WinstonPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_Text_SofiaIntro::
	.string "Do you know how you can avoid\n"
	.string "getting lost in the mountains?\p"
	.string "You can bend twigs as trail\n"
	.string "markers.$"

RockTunnel_B1F_Text_SofiaDefeat::
	.string "Ohhh!\n"
	.string "I did my best!$"

RockTunnel_B1F_Text_SofiaPostBattle::
	.string "I want to go home!$"

RockTunnel_B1F_Text_DudleyIntro::
	.string "Hahaha!\n"
	.string "Can you beat my power?$"

RockTunnel_B1F_Text_DudleyDefeat::
	.string "Oops!\n"
	.string "Outmuscled!$"

RockTunnel_B1F_Text_DudleyPostBattle::
	.string "I go for power because I hate\n"
	.string "thinking.$"

RockTunnel_B1F_Text_CooperIntro::
	.string "You have a POKéDEX?\n"
	.string "I want one, too.$"

RockTunnel_B1F_Text_CooperDefeat::
	.string "Shoot!\n"
	.string "I am so jealous!$"

RockTunnel_B1F_Text_CooperPostBattle::
	.string "When you finish your POKéDEX,\n"
	.string "can I have it?$"

RockTunnel_B1F_Text_SteveIntro::
	.string "Um… Do you know about POKéMON\n"
	.string "cosplay?$"

RockTunnel_B1F_Text_SteveDefeat::
	.string "Well, that's that.$"

RockTunnel_B1F_Text_StevePostBattle::
	.string "POKéMON cosplay is dressing up as\n"
	.string "POKéMON for fun.\p"
	.string "CLEFAIRY is a favorite.$"

RockTunnel_B1F_Text_AllenIntro::
	.string "My POKéMON techniques will leave\n"
	.string "you crying!$"

RockTunnel_B1F_Text_AllenDefeat::
	.string "I give!\n"
	.string "You're a better technician!$"

RockTunnel_B1F_Text_AllenPostBattle::
	.string "In mountains, you'll often find\n"
	.string "ROCK-type POKéMON.$"

RockTunnel_B1F_Text_MarthaIntro::
	.string "I don't often come here, but I will\n"
	.string "battle you.$"

RockTunnel_B1F_Text_MarthaDefeat::
	.string "Oh!\n"
	.string "I lost!$"

RockTunnel_B1F_Text_MarthaPostBattle::
	.string "I like tiny POKéMON.\n"
	.string "Big ones are too scary!$"

RockTunnel_B1F_Text_EricIntro::
	.string "Hit me with your best shot!$"

RockTunnel_B1F_Text_EricDefeat::
	.string "Fired away!$"

RockTunnel_B1F_Text_EricPostBattle::
	.string "I'll raise my POKéMON to beat\n"
	.string "yours, kid.$"

RockTunnel_B1F_Text_WinstonIntro::
	.string "I draw illustrations of POKéMON\n"
	.string "when I'm home.$"

RockTunnel_B1F_Text_WinstonDefeat::
	.string "Whew…\n"
	.string "I'm exhausted…$"

RockTunnel_B1F_Text_WinstonPostBattle::
	.string "I'm an artist, not a fighter.\n"
	.string "I'll go home to draw.$"

// Regigigas requires all three Hoenn Regis registered caught, not in party.
RockTunnel_B1F_AnthologyOnTransition::
	getcaughtmon SPECIES_REGIROCK
	goto_if_ne VAR_RESULT, TRUE, RockTunnel_B1F_AnthologyRegigigasLocked
	getcaughtmon SPECIES_REGICE
	goto_if_ne VAR_RESULT, TRUE, RockTunnel_B1F_AnthologyRegigigasLocked
	getcaughtmon SPECIES_REGISTEEL
	goto_if_ne VAR_RESULT, TRUE, RockTunnel_B1F_AnthologyRegigigasLocked
	getcaughtmon SPECIES_REGIGIGAS
	call_if_eq VAR_RESULT, TRUE, RockTunnel_B1F_AnthologyHideRegigigas
	call_if_ne VAR_RESULT, TRUE, RockTunnel_B1F_AnthologyShowRegigigas
	goto RockTunnel_B1F_AnthologyOtherStatics

RockTunnel_B1F_AnthologyRegigigasLocked::
	setflag FLAG_HIDE_ANTHOLOGY_REGIGIGAS
RockTunnel_B1F_AnthologyOtherStatics::
	getcaughtmon SPECIES_UXIE
	call_if_eq VAR_RESULT, TRUE, RockTunnel_B1F_AnthologyHideUxie
	call_if_ne VAR_RESULT, TRUE, RockTunnel_B1F_AnthologyShowUxie
	getcaughtmon SPECIES_MESPRIT
	call_if_eq VAR_RESULT, TRUE, RockTunnel_B1F_AnthologyHideMesprit
	call_if_ne VAR_RESULT, TRUE, RockTunnel_B1F_AnthologyShowMesprit
	getcaughtmon SPECIES_AZELF
	call_if_eq VAR_RESULT, TRUE, RockTunnel_B1F_AnthologyHideAzelf
	call_if_ne VAR_RESULT, TRUE, RockTunnel_B1F_AnthologyShowAzelf
	return

RockTunnel_B1F_AnthologyHideRegigigas::
	setflag FLAG_HIDE_ANTHOLOGY_REGIGIGAS
	return

RockTunnel_B1F_AnthologyShowRegigigas::
	clearflag FLAG_HIDE_ANTHOLOGY_REGIGIGAS
	return

RockTunnel_B1F_EventScript_Regigigas::
	lock
	faceplayer
	playmoncry SPECIES_REGIGIGAS, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_REGIGIGAS, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, RockTunnel_B1F_AnthologyRegigigasRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, RockTunnel_B1F_AnthologyRegigigasRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, RockTunnel_B1F_AnthologyRegigigasResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RockTunnel_B1F_AnthologyRegigigasResolved
	release
	end

RockTunnel_B1F_AnthologyRegigigasResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_REGIGIGAS
	setflag FLAG_HIDE_ANTHOLOGY_REGIGIGAS
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

RockTunnel_B1F_AnthologyRegigigasRetreated::
	release
	end

RockTunnel_B1F_AnthologyHideUxie::
	setflag FLAG_HIDE_ANTHOLOGY_UXIE
	return

RockTunnel_B1F_AnthologyShowUxie::
	clearflag FLAG_HIDE_ANTHOLOGY_UXIE
	return

RockTunnel_B1F_EventScript_Uxie::
	lock
	faceplayer
	playmoncry SPECIES_UXIE, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_UXIE, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, RockTunnel_B1F_AnthologyUxieRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, RockTunnel_B1F_AnthologyUxieRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, RockTunnel_B1F_AnthologyUxieResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RockTunnel_B1F_AnthologyUxieResolved
	release
	end

RockTunnel_B1F_AnthologyUxieResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_UXIE
	setflag FLAG_HIDE_ANTHOLOGY_UXIE
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

RockTunnel_B1F_AnthologyUxieRetreated::
	release
	end

RockTunnel_B1F_AnthologyHideMesprit::
	setflag FLAG_HIDE_ANTHOLOGY_MESPRIT
	return

RockTunnel_B1F_AnthologyShowMesprit::
	clearflag FLAG_HIDE_ANTHOLOGY_MESPRIT
	return

RockTunnel_B1F_EventScript_Mesprit::
	lock
	faceplayer
	playmoncry SPECIES_MESPRIT, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_MESPRIT, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, RockTunnel_B1F_AnthologyMespritRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, RockTunnel_B1F_AnthologyMespritRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, RockTunnel_B1F_AnthologyMespritResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RockTunnel_B1F_AnthologyMespritResolved
	release
	end

RockTunnel_B1F_AnthologyMespritResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_MESPRIT
	setflag FLAG_HIDE_ANTHOLOGY_MESPRIT
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

RockTunnel_B1F_AnthologyMespritRetreated::
	release
	end

RockTunnel_B1F_AnthologyHideAzelf::
	setflag FLAG_HIDE_ANTHOLOGY_AZELF
	return

RockTunnel_B1F_AnthologyShowAzelf::
	clearflag FLAG_HIDE_ANTHOLOGY_AZELF
	return

RockTunnel_B1F_EventScript_Azelf::
	lock
	faceplayer
	playmoncry SPECIES_AZELF, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_AZELF, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, RockTunnel_B1F_AnthologyAzelfRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, RockTunnel_B1F_AnthologyAzelfRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, RockTunnel_B1F_AnthologyAzelfResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RockTunnel_B1F_AnthologyAzelfResolved
	release
	end

RockTunnel_B1F_AnthologyAzelfResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_AZELF
	setflag FLAG_HIDE_ANTHOLOGY_AZELF
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

RockTunnel_B1F_AnthologyAzelfRetreated::
	release
	end

// Multi-location acquisition batch: preserve canonical map scripts.
RockTunnel_B1F_BigBatchOnTransition::
	getcaughtmon SPECIES_HEATRAN
	call_if_eq VAR_RESULT, TRUE, RockTunnel_B1F_BigBatchHideHeatran
	call_if_ne VAR_RESULT, TRUE, RockTunnel_B1F_BigBatchShowHeatran
	return

RockTunnel_B1F_BigBatchHideHeatran::
	setflag FLAG_HIDE_ANTHOLOGY_HEATRAN
	return

RockTunnel_B1F_BigBatchShowHeatran::
	clearflag FLAG_HIDE_ANTHOLOGY_HEATRAN
	return

RockTunnel_B1F_EventScript_Heatran::
	lock
	faceplayer
	playmoncry SPECIES_HEATRAN, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_HEATRAN, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, RockTunnel_B1F_BigBatchHeatranRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, RockTunnel_B1F_BigBatchHeatranRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, RockTunnel_B1F_BigBatchHeatranResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RockTunnel_B1F_BigBatchHeatranResolved
	release
	end

RockTunnel_B1F_BigBatchHeatranResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_HEATRAN
	setflag FLAG_HIDE_ANTHOLOGY_HEATRAN
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

RockTunnel_B1F_BigBatchHeatranRetreated::
	release
	end

Anthology_MegaStone_HEATRANITE::
    getcaughtmon SPECIES_HEATRAN
    goto_if_ne VAR_RESULT, TRUE, Anthology_MegaStone_HEATRANITE_Locked
    finditem ITEM_HEATRANITE
    end
Anthology_MegaStone_HEATRANITE_Locked::
    msgbox Anthology_MegaStone_HEATRANITE_LockedText, MSGBOX_DEFAULT
    end
Anthology_MegaStone_HEATRANITE_LockedText::
    .string "A special MEGA STONE resonates here.\p"
    .string "Return when its guardian is yours.$"
