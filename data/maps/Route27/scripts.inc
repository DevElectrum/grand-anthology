.set LOCALID_ROUTE27_FATMAN, 1

Route27_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route27_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

Route27_OnTransition:
    goto_if_unset FLAG_JOHTO_BADGE_8, Route27_RoyalDuoHide
    goto_if_set FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED, Route27_RoyalDuoHide
    clearflag FLAG_HIDE_ANTHOLOGY_ZACIAN
    clearflag FLAG_HIDE_ANTHOLOGY_ZAMAZENTA
    end

Route27_RoyalDuoHide::
    setflag FLAG_HIDE_ANTHOLOGY_ZACIAN
    setflag FLAG_HIDE_ANTHOLOGY_ZAMAZENTA
    end

Route27_EventScript_BaobaCall::
	pokenavcall BlackthornCity_Text_BaobaCall
	waitmessage
	delay 30
	setflag FLAG_SAFARI_ZONE_WEST_EXPANSION
    setvar VAR_ROUTE27_BAOBA_CALL, 1
	end

Route27_EventScript_FirstSteps::
    lock
    playse SE_PIN
    setflag FLAG_VISITED_KANTO
    applymovement2 LOCALID_ROUTE27_FATMAN, Common_Movement_ExclamationMark
    turnobject LOCALID_ROUTE27_FATMAN, DIR_WEST
    delay 30
    getplayerxy VAR_TEMP_0, VAR_TEMP_1
    call_if_eq VAR_TEMP_1, 24, Route27_EventScript_FirstStepsWalkTop
    call_if_eq VAR_TEMP_1, 25, Route27_EventScript_FirstStepsWalkBot

    msgbox Route27_Text_FisherHey, MSGBOX_DEFAULT
    msgbox Route27_Text_FisherMessage, MSGBOX_DEFAULT
    closemessage
    turnobject LOCALID_ROUTE27_FATMAN, DIR_SOUTH

    @integrated flagheap
    setflag FLAG_HIDE_VICTORY_ROAD_SILVER
    setflag FLAG_HIDE_INDIGO_PLATEAU_SILVER
    setflag FLAG_HIDE_INDIGO_PLATEAU_MARY_OAK
    
    clearflag FLAG_GOT_TM_SANDSTORM
    clearflag FLAG_INDIGOJUNCTION_HIDE_SILVER_GUARD
    clearflag FLAG_INDIGOJUNCTION_HIDE_KANTO_GUARD

    setvar VAR_ROUTE27_STATE, 1
    release
    end

Route27_EventScript_FirstStepsWalkTop::
    applymovement2 LOCALID_ROUTE27_FATMAN, Route27_Movement_WalkTop
    waitmovement 0
    return
Route27_EventScript_FirstStepsWalkBot::
    applymovement2 LOCALID_ROUTE27_FATMAN, Route27_Movement_WalkBot
    waitmovement 0
    return

Route27_Movement_WalkTop:
    face_left
    walk_left
    step_end

Route27_Movement_WalkBot:
    walk_down
    walk_left
    step_end

Route27_EventScript_Item_Moonstone::
    finditem ITEM_MOON_STONE
    end

Route27_EventScript_Item_RareCandy::
    finditem ITEM_RARE_CANDY
    end

Route27_EventScript_Item_SolarBeam::
    finditem ITEM_TM_SOLAR_BEAM
    end

Route27_EventScript_Item_Revive::
    finditem ITEM_REVIVE
    end

Route27_EventScript_Fatman::
    msgbox Route27_Text_FisherMessage, MSGBOX_NPC
    end

Route27_EventScript_Blake::
	trainerbattle_single TRAINER_BLAKE_JT Route27_Text_CooltrainermBlakeSeen, Route27_Text_CooltrainermBlakeBeaten
	msgbox Route27_Text_CooltrainermBlakeAfter, MSGBOX_AUTOCLOSE
	end

Route27_EventScript_Brian::
	trainerbattle_single TRAINER_BRIAN Route27_Text_CooltrainermBrianSeen, Route27_Text_CooltrainermBrianBeaten
	msgbox Route27_Text_CooltrainermBrianAfter, MSGBOX_AUTOCLOSE
	end

Route27_EventScript_Reena::
	trainerbattle_single TRAINER_REENA Route27_Text_CooltrainerfReenaSeen, Route27_Text_CooltrainerfReenaBeaten
	msgbox Route27_Text_CooltrainerfReenaAfter, MSGBOX_AUTOCLOSE
	end

Route27_EventScript_Megan::
	trainerbattle_single TRAINER_MEGAN Route27_Text_CooltrainerfMeganSeen, Route27_Text_CooltrainerfMeganBeaten
	msgbox Route27_Text_CooltrainerfMeganAfter, MSGBOX_AUTOCLOSE
	end

Route27_EventScript_Gilbert::
	trainerbattle_single TRAINER_GILBERT_JT Route27_Text_PsychicGilbertSeen, Route27_Text_PsychicGilbertBeaten
	msgbox Route27_Text_PsychicGilbertAfter, MSGBOX_AUTOCLOSE
	end

Route27_EventScript_Jose::
	trainerbattle_single TRAINER_JOSE_JT Route27_Text_BirdKeeperJoseSeen, Route27_Text_BirdKeeperJoseBeaten
	msgbox Route27_Text_BirdKeeperJoseAfter, MSGBOX_AUTOCLOSE
	end

Route27_EventScript_Scott::
	trainerbattle_single TRAINER_SCOTT Route27_Text_FisherScottSeen, Route27_Text_FisherScottBeaten
	msgbox Route27_Text_FisherScottAfter, MSGBOX_AUTOCLOSE
	end

Route27_EventScript_TohjoFallsSign::
    msgbox TohjoFalls_Text_Sign, MSGBOX_SIGN
    end

Route27_Text_FisherHey:
    .string "Hey!$"

Route27_Text_FisherMessage:
    .string "These falls mark the edge of\n"
    .string "JOHTO-the POKéMON LEAGUE\l"
    .string "is just ahead.$"

Route27_Text_CooltrainermBlakeSeen:
    .string "You look pretty strong.\n"
    .string "Let me battle you!$"

Route27_Text_CooltrainermBlakeBeaten:
    .string "Yow!$"

Route27_Text_CooltrainermBlakeAfter:
    .string "If you prevail on this harsh trek,\n"
    .string "the truth will be revealed!\p"
    .string "Heh, sorry, I just wanted to say\n"
    .string "something cool.$"

Route27_Text_CooltrainermBrianSeen:
    .string "Hm? You're good, aren't you?$"

Route27_Text_CooltrainermBrianBeaten:
    .string "Just as I thought!$"

Route27_Text_CooltrainermBrianAfter:
    .string "A good trainer can recognize other\n"
    .string "good trainers.$"

Route27_Text_CooltrainerfReenaSeen:
    .string "You shouldn't underestimate the\n"
    .string "wild POKéMON in these parts.$"

Route27_Text_CooltrainerfReenaBeaten:
    .string "Oh! You're much too strong!$"

Route27_Text_CooltrainerfReenaAfter:
    .string "You're just a kid, but you're not\n"
    .string "to be underestimated either.$"

Route27_Text_CooltrainerfMeganSeen:
    .string "It's rare to see anyone come here.\p"
    .string "Are you training on your own?$"

Route27_Text_CooltrainerfMeganBeaten:
    .string "Oh! You're really strong!$"

Route27_Text_CooltrainerfMeganAfter:
    .string "I'm checking out pre- and post-\n"
    .string "evolution POKéMON.\p"
    .string "Evolution really does make\n"
    .string "POKéMON stronger.\p"
    .string "But evolved forms also learn\n"
    .string "moves later on.$"

Route27_Text_PsychicGilbertSeen:
    .string "Don't say a thing!\p"
    .string "Let me guess what you're thinking.\p"
    .string "Mmmmmmm…\p"
    .string "I got it! You're on the POKéMON\n"
    .string "LEAGUE challenge!$"

Route27_Text_PsychicGilbertBeaten:
    .string "You're too much!$"

Route27_Text_PsychicGilbertAfter:
    .string "With your skills, you'll do well at\n"
    .string "the LEAGUE.\p"
    .string "That's what my premonition says.$"

Route27_Text_BirdKeeperJoseSeen:
    .string "Tweet! Tweet! Tetweet!$"

Route27_Text_BirdKeeperJoseBeaten:
    .string "Tweet!$"

Route27_Text_BirdKeeperJoseAfter:
    .string "BIRD KEEPERS like me mimic bird\n"
    .string "whistles to command POKéMON.$"

TohjoFalls_Text_Sign:
    .string "TOHJO FALLS\n"
    .string "GATEWAY TO THE POKéMON LEAGUE$"

Route27_Text_FisherScottSeen:
    .string "I'm feeling great today!\p"
    .string "I feel like I could boot even the\n"
    .string "LEAGUE CHAMP!$"

Route27_Text_FisherScottBeaten:
    .string "No! Not in this battle!$"

Route27_Text_FisherScottAfter:
    .string "Just like in fishing, it's all over\n"
    .string "in POKéMON if you give up.$"


@ The Route 27 Royal Duo: either sprite begins the same uncapturable double battle.
@ Both held-item Crowned guardians must fall before either one joins the trainer.
Route27_EventScript_RoyalDuoZacian::
    goto Route27_EventScript_RoyalDuo

Route27_EventScript_RoyalDuoZamazenta::
    goto Route27_EventScript_RoyalDuo

Route27_EventScript_RoyalDuo::
    lock
    faceplayer
    goto_if_unset FLAG_JOHTO_BADGE_8, Route27_RoyalDuoNotReady
    goto_if_set FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED, Route27_RoyalDuoAlreadyClaimed
    special RoyalDuoCheckCapacity
    goto_if_eq VAR_RESULT, FALSE, Route27_RoyalDuoNoRoom
    msgbox Route27_Text_RoyalDuoChallenge, MSGBOX_DEFAULT
    closemessage
    special RoyalDuoPrepareBattle
    setflag FLAG_ANTHOLOGY_ROYAL_DUO_NO_CATCH
    special BattleSetup_StartRoyalDuoBattle
    waitstate
    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_ne VAR_RESULT, B_OUTCOME_WON, Route27_RoyalDuoRetry
    special RoyalDuoAwardVictory
    goto_if_eq VAR_RESULT, FALSE, Route27_RoyalDuoNoRoom
    setflag FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED
    fadescreenswapbuffers FADE_TO_BLACK
    removeobject LOCALID_ANTHOLOGY_ZACIAN
    removeobject LOCALID_ANTHOLOGY_ZAMAZENTA
    setflag FLAG_HIDE_ANTHOLOGY_ZACIAN
    setflag FLAG_HIDE_ANTHOLOGY_ZAMAZENTA
    fadescreenswapbuffers FADE_FROM_BLACK
    msgbox Route27_Text_RoyalDuoGift, MSGBOX_DEFAULT
    release
    end

Route27_RoyalDuoNoRoom::
    msgbox Route27_Text_RoyalDuoNoRoom, MSGBOX_DEFAULT
    release
    end

Route27_RoyalDuoRetry::
    msgbox Route27_Text_RoyalDuoRetry, MSGBOX_DEFAULT
    release
    end

Route27_RoyalDuoAlreadyClaimed::
    msgbox Route27_Text_RoyalDuoGift, MSGBOX_DEFAULT
    release
    end

Route27_RoyalDuoNotReady::
    msgbox Route27_Text_RoyalDuoNotReady, MSGBOX_DEFAULT
    release
    end

Route27_Text_RoyalDuoChallenge:
    .string "ZACIAN and ZAMAZENTA guard the\l"
    .string "road to the POKéMON LEAGUE!\p"
    .string "Defeat them both to earn their trust!$"

Route27_Text_RoyalDuoGift:
    .string "ZACIAN and ZAMAZENTA join you!\p"
    .string "Both carry their sacred relics.$"

Route27_Text_RoyalDuoNoRoom:
    .string "You need space for TWO POKéMON.\p"
    .string "Free up party or PC storage first!$"

Route27_Text_RoyalDuoRetry:
    .string "The two guardians await a rematch.$"

Route27_Text_RoyalDuoNotReady:
    .string "Eight JOHTO BADGES are required.$"
