RuinsOfAlph_Outside_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RuinsOfAlph_Outside_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

RuinsOfAlph_Outside_OnTransition:
	call RuinsOfAlph_Outside_AnthologyOnTransition 
	end	

RuinsOfAlph_Outside_EventScript_Quagsire::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_QUAGSIRE, CRY_MODE_NORMAL
	msgbox RuinsOfAlph_Outside_Text_Quagsire, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RuinsOfAlph_Outside_Text_Quagsire:
	.string "QUAGSIRE: Quag Quag$"

RuinsOfAlph_Outside_EventScript_Stan::
	trainerbattle_single TRAINER_STAN_JT, RuinsOfAlph_Outside_Text_SuperNerdStan_Seen, RuinsOfAlph_Outside_Text_SuperNerdStan_Beaten
	msgbox RuinsOfAlph_Outside_Text_SuperNerdStan_After, MSGBOX_AUTOCLOSE
	end

RuinsOfAlph_Outside_EventScript_Nathan::
	trainerbattle_single TRAINER_NATHAN, RuinsOfAlph_Outside_Text_PsychicNathan_Seen, RuinsOfAlph_Outside_Text_PsychicNathan_Beaten
	msgbox RuinsOfAlph_Outside_Text_PsychicNathan_After, MSGBOX_AUTOCLOSE
	end

RuinsOfAlph_Outside_EventScript_Youngster1::
	lock
	faceplayer
	msgbox RuinsOfAlph_Outside_Text_Youngster1, MSGBOX_DEFAULT
	closemessage
	release
	end

RuinsOfAlph_Outside_EventScript_Youngster2::
	lock
	faceplayer
	msgbox RuinsOfAlph_Outside_Text_Youngster2, MSGBOX_DEFAULT
	closemessage
	release
	end

RuinsOfAlph_Outside_EventScript_Fisher1::
	lock
	faceplayer
	msgbox RuinsOfAlph_Outside_Text_Fisher1, MSGBOX_DEFAULT
	closemessage
	release
	end

RuinsOfAlph_Outside_EventScript_Fisher2::
	lock
	faceplayer
	msgbox RuinsOfAlph_Outside_Text_Fisher2, MSGBOX_DEFAULT
	closemessage
	release
	end

RuinsOfAlph_Outside_EventScript_Sign1::
	lock
	msgbox RuinsOfAlph_Outside_Text_MainSign, MSGBOX_SIGN
	closemessage
	release
	end

RuinsOfAlph_Outside_EventScript_Sign2::
	lock
	msgbox RuinsOfAlph_Outside_Text_ResearchSign, MSGBOX_SIGN
	closemessage
	release
	end

RuinsOfAlph_Outside_EventScript_Sign3::
	lock
	msgbox RuinsOfAlph_Outside_Text_MysteryChamberSign, MSGBOX_SIGN
	closemessage
	release
	end

RuinsOfAlph_Outside_Text_Scientist:
	.string "Hm? That's a POKéDEX, isn't it?\n"
	.string "May I see it?\p"
	.string "There are so many kinds of\n"
	.string "POKéMON.\p"
	.string "Hm? What's this?\p"
	.string "What is this POKéMON?\p"
	.string "It looks like the strange\n"
	.string "writing on the walls of the\p"
	.string "RUINS.\p"
	.string "If those drawings are really\n"
	.string "POKéMON, there should be many\n"
	.string "more.\p"
	.string "I know! Let me upgrade your\n"
	.string "POKéDEX. Follow me.$"

RuinsOfAlph_Outside_Text_SuperNerdStan_Seen:
	.string "What do you want? I'm studying--\n"
	.string "don't disturb me!$"

RuinsOfAlph_Outside_Text_SuperNerdStan_Beaten:
	.string "Sorry…\n"
	.string "I'm frustrated by our lack of\p"
	.string "real understanding…$"

RuinsOfAlph_Outside_Text_SuperNerdStan_After:
	.string "The RUINS are from about 1500\n"
	.string "years ago.\p"
	.string "Nobody knows who built them.\p"
	.string "It's also not known if the POKéMON\n"
	.string "statues have any meaning.\p"
	.string "It's all one big mystery…$"

RuinsOfAlph_Outside_Text_PsychicNathan_Seen:
	.string "Hmmm… This is a strange place.$"

RuinsOfAlph_Outside_Text_PsychicNathan_Beaten:
	.string "…$"

RuinsOfAlph_Outside_Text_PsychicNathan_After:
	.string "I like thinking here.$"

RuinsOfAlph_Outside_Text_MysteryChamberSign:
	.string "MYSTERY STONE PANEL CHAMBER$"

RuinsOfAlph_Outside_Text_MainSign:
	.string "RUINS OF ALPH\n"
	.string "VISITORS WELCOME$"

RuinsOfAlph_Outside_Text_ResearchSign:
	.string "RUINS OF ALPH RESEARCH CENTER\p"
	.string "THE AUTHORITY ON THE\n"
	.string "RUINS OF ALPH$"

RuinsOfAlph_Outside_Text_Fisher1:
	.string "While exploring the RUINS, we\n"
	.string "suddenly noticed an odd presence.\p"
	.string "We all got scared and ran away.\n"
	.string "You should be careful too.$"

RuinsOfAlph_Outside_Text_Fisher2:
	.string "The RUINS hide a huge secret!\p"
	.string "…I think…$"

RuinsOfAlph_Outside_Text_Youngster1:
	.string "There are many kinds of UNOWN, so\n"
	.string "we use them for our secret codes.$"

RuinsOfAlph_Outside_Text_Youngster2:
	.string "A… H… E… A… D… Hmm…\p"
	.string "What?\p"
	.string "I'm decoding this message!$"

// Independent special encounter flags; keep native events authoritative.
RuinsOfAlph_Outside_AnthologyOnTransition::
	getcaughtmon SPECIES_ZAMAZENTA
	call_if_eq VAR_RESULT, TRUE, RuinsOfAlph_Outside_AnthologyHideZamazenta
	call_if_ne VAR_RESULT, TRUE, RuinsOfAlph_Outside_AnthologyShowZamazenta
	getcaughtmon SPECIES_REGIELEKI
	call_if_eq VAR_RESULT, TRUE, RuinsOfAlph_Outside_AnthologyHideRegieleki
	call_if_ne VAR_RESULT, TRUE, RuinsOfAlph_Outside_AnthologyShowRegieleki
	return

RuinsOfAlph_Outside_AnthologyHideZamazenta::
	setflag FLAG_HIDE_ANTHOLOGY_ZAMAZENTA
	return

RuinsOfAlph_Outside_AnthologyShowZamazenta::
	clearflag FLAG_HIDE_ANTHOLOGY_ZAMAZENTA
	return

RuinsOfAlph_Outside_EventScript_Zamazenta::
	lock
	faceplayer
	playmoncry SPECIES_ZAMAZENTA, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_ZAMAZENTA, 50
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, RuinsOfAlph_Outside_AnthologyZamazentaRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, RuinsOfAlph_Outside_AnthologyZamazentaRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, RuinsOfAlph_Outside_AnthologyZamazentaResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RuinsOfAlph_Outside_AnthologyZamazentaResolved
	release
	end

RuinsOfAlph_Outside_AnthologyZamazentaResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_ZAMAZENTA
	setflag FLAG_HIDE_ANTHOLOGY_ZAMAZENTA
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

RuinsOfAlph_Outside_AnthologyZamazentaRetreated::
	release
	end

RuinsOfAlph_Outside_AnthologyHideRegieleki::
	setflag FLAG_HIDE_ANTHOLOGY_REGIELEKI
	return

RuinsOfAlph_Outside_AnthologyShowRegieleki::
	clearflag FLAG_HIDE_ANTHOLOGY_REGIELEKI
	return

RuinsOfAlph_Outside_EventScript_Regieleki::
	lock
	faceplayer
	playmoncry SPECIES_REGIELEKI, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_REGIELEKI, 45
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, RuinsOfAlph_Outside_AnthologyRegielekiRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, RuinsOfAlph_Outside_AnthologyRegielekiRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, RuinsOfAlph_Outside_AnthologyRegielekiResolved
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RuinsOfAlph_Outside_AnthologyRegielekiResolved
	release
	end

RuinsOfAlph_Outside_AnthologyRegielekiResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_REGIELEKI
	setflag FLAG_HIDE_ANTHOLOGY_REGIELEKI
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

RuinsOfAlph_Outside_AnthologyRegielekiRetreated::
	release
	end
