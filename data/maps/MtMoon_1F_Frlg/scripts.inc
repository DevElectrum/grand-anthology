MtMoon_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMoon_1F_OnTransition
	.byte 0

MtMoon_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_MT_MOON_1F
	call MtMoon_1F_AnthologyOnTransition
	end

MtMoon_1F_EventScript_ZubatSign::
	msgbox MtMoon_1F_Text_ZubatIsABloodsucker, MSGBOX_SIGN
	end

MtMoon_1F_EventScript_Marcos::
	trainerbattle_single TRAINER_HIKER_MARCOS, MtMoon_1F_Text_MarcosIntro, MtMoon_1F_Text_MarcosDefeat
	msgbox MtMoon_1F_Text_MarcosPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Josh::
	trainerbattle_single TRAINER_YOUNGSTER_JOSH, MtMoon_1F_Text_JoshIntro, MtMoon_1F_Text_JoshDefeat
	msgbox MtMoon_1F_Text_JoshPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Miriam::
	trainerbattle_single TRAINER_LASS_MIRIAM, MtMoon_1F_Text_MiriamIntro, MtMoon_1F_Text_MiriamDefeat
	msgbox MtMoon_1F_Text_MiriamPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Iris::
	trainerbattle_single TRAINER_LASS_IRIS, MtMoon_1F_Text_IrisIntro, MtMoon_1F_Text_IrisDefeat
	msgbox MtMoon_1F_Text_IrisPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Jovan::
	trainerbattle_single TRAINER_SUPER_NERD_JOVAN, MtMoon_1F_Text_JovanIntro, MtMoon_1F_Text_JovanDefeat
	msgbox MtMoon_1F_Text_JovanPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Kent::
	trainerbattle_single TRAINER_BUG_CATCHER_KENT, MtMoon_1F_Text_KentIntro, MtMoon_1F_Text_KentDefeat
	msgbox MtMoon_1F_Text_KentPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Robby::
	trainerbattle_single TRAINER_BUG_CATCHER_ROBBY, MtMoon_1F_Text_RobbyIntro, MtMoon_1F_Text_RobbyDefeat
	msgbox MtMoon_1F_Text_RobbyPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_Text_MarcosIntro::
	.string "WHOA!\n"
	.string "You shocked me!\l"
	.string "…Oh, you're just a kid!$"

MtMoon_1F_Text_MarcosDefeat::
	.string "Wow!\n"
	.string "Shocked again!$"

MtMoon_1F_Text_MarcosPostBattle::
	.string "Kids like you shouldn't be\n"
	.string "wandering around here in the dark.$"

MtMoon_1F_Text_JoshIntro::
	.string "Did you come to explore the cave,\n"
	.string "too?$"

MtMoon_1F_Text_JoshDefeat::
	.string "Losing stinks!\n"
	.string "It's so uncool.$"

MtMoon_1F_Text_JoshPostBattle::
	.string "I came all the way down here to\n"
	.string "show off to girls.$"

MtMoon_1F_Text_MiriamIntro::
	.string "Wow! It's way bigger in here than\n"
	.string "I thought!$"

MtMoon_1F_Text_MiriamDefeat::
	.string "Oh!\n"
	.string "I lost it!$"

MtMoon_1F_Text_MiriamPostBattle::
	.string "How do you get out of here?\n"
	.string "It's so big, I may get lost.$"

MtMoon_1F_Text_JovanIntro::
	.string "What!\n"
	.string "Don't sneak up on me!$"

MtMoon_1F_Text_JovanDefeat::
	.string "My POKéMON won't do!$"

MtMoon_1F_Text_JovanPostBattle::
	.string "I have to find stronger POKéMON.\n"
	.string "Where might they be?$"

MtMoon_1F_Text_IrisIntro::
	.string "What?\n"
	.string "I'm waiting for my friends to find\l"
	.string "me here.$"

MtMoon_1F_Text_IrisDefeat::
	.string "I lost?$"

MtMoon_1F_Text_IrisPostBattle::
	.string "I came because I heard there are\n"
	.string "some very rare fossils here.$"

MtMoon_1F_Text_KentIntro::
	.string "Suspicious men are in the cave.\n"
	.string "What about you?$"

MtMoon_1F_Text_KentDefeat::
	.string "You got me!$"

MtMoon_1F_Text_KentPostBattle::
	.string "I saw them!\n"
	.string "I'm sure they're from TEAM ROCKET!$"

MtMoon_1F_Text_RobbyIntro::
	.string "You need to go through this cave\n"
	.string "to get to CERULEAN CITY.$"

MtMoon_1F_Text_RobbyDefeat::
	.string "I lost.$"

MtMoon_1F_Text_RobbyPostBattle::
	.string "ZUBAT is tough!\n"
	.string "But if you can catch one, you'll\l"
	.string "be able to count on it.$"

MtMoon_1F_Text_ZubatIsABloodsucker::
	.string "Beware!\n"
	.string "ZUBAT is a bloodsucker!$"

MtMoon_1F_Text_BrockHelpsExcavateFossils::
	.string "Hi, I'm excavating for fossils here\n"
	.string "under MT. MOON.\p"
	.string "Sometimes, BROCK of PEWTER GYM\n"
	.string "lends me a hand.$"


// Restore uncaught statics on entry; let the original map scripts run.
MtMoon_1F_AnthologyOnTransition::
	call_if_set FLAG_CAUGHT_ANTHOLOGY_COSMOG_MOON, MtMoon_1F_AnthologyHideCosmogMoon
	call_if_unset FLAG_CAUGHT_ANTHOLOGY_COSMOG_MOON, MtMoon_1F_AnthologyShowCosmogMoon
	getcaughtmon SPECIES_MARSHADOW
	call_if_eq VAR_RESULT, TRUE, MtMoon_1F_AnthologyHideMarshadow
	call_if_ne VAR_RESULT, TRUE, MtMoon_1F_AnthologyShowMarshadow
	return

MtMoon_1F_AnthologyHideCosmogMoon::
	setflag FLAG_HIDE_ANTHOLOGY_COSMOG_MOON
	return

MtMoon_1F_AnthologyShowCosmogMoon::
	clearflag FLAG_HIDE_ANTHOLOGY_COSMOG_MOON
	return

// Caught: permanently gone. Fainted: returns on re-entry. Run: stays here.
MtMoon_1F_EventScript_CosmogMoon::
	lock
	faceplayer
	playmoncry SPECIES_COSMOG, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_COSMOG, 15
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtMoon_1F_AnthologyCosmogMoonRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtMoon_1F_AnthologyCosmogMoonRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtMoon_1F_AnthologyCosmogMoonCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtMoon_1F_AnthologyCosmogMoonResolved
	release
	end

MtMoon_1F_AnthologyCosmogMoonCaught::
	setflag FLAG_CAUGHT_ANTHOLOGY_COSMOG_MOON
	goto MtMoon_1F_AnthologyCosmogMoonResolved

MtMoon_1F_AnthologyCosmogMoonResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_COSMOG_MOON
	setflag FLAG_HIDE_ANTHOLOGY_COSMOG_MOON
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

MtMoon_1F_AnthologyCosmogMoonRetreated::
	release
	end

MtMoon_1F_AnthologyHideMarshadow::
	setflag FLAG_HIDE_ANTHOLOGY_MARSHADOW
	return

MtMoon_1F_AnthologyShowMarshadow::
	clearflag FLAG_HIDE_ANTHOLOGY_MARSHADOW
	return

// Caught: permanently gone. Fainted: returns on re-entry. Run: stays here.
MtMoon_1F_EventScript_Marshadow::
	lock
	faceplayer
	playmoncry SPECIES_MARSHADOW, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_MARSHADOW, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtMoon_1F_AnthologyMarshadowRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtMoon_1F_AnthologyMarshadowRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtMoon_1F_AnthologyMarshadowResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtMoon_1F_AnthologyMarshadowResolved
	release
	end

MtMoon_1F_AnthologyMarshadowResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_MARSHADOW
	setflag FLAG_HIDE_ANTHOLOGY_MARSHADOW
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

MtMoon_1F_AnthologyMarshadowRetreated::
	release
	end
