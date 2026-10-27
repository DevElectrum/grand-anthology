// Reuse a small existing cave layout; this independent map is appended to JohtoLeague.
JohtoEternatusChamber_MapScripts::
    map_script MAP_SCRIPT_ON_TRANSITION, JohtoEternatusChamber_AnthologyOnTransition
    .byte 0

JohtoEternatusChamber_AnthologyOnTransition:
    goto_if_unset FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED, JohtoEternatusChamber_Hide
    callnative AnthologyStaticRandomizerIsOn
    goto_if_eq VAR_RESULT, TRUE, JohtoEternatusChamber_SeededOnTransition
    getcaughtmon SPECIES_ETERNATUS
    call_if_eq VAR_RESULT, TRUE, JohtoEternatusChamber_Hide
    call_if_ne VAR_RESULT, TRUE, JohtoEternatusChamber_Show
    end

JohtoEternatusChamber_SeededOnTransition:
    call_if_set FLAG_CAUGHT_RANDOM_ROUTE44_ETERNATUS, JohtoEternatusChamber_Hide
    call_if_unset FLAG_CAUGHT_RANDOM_ROUTE44_ETERNATUS, JohtoEternatusChamber_Show
    end

JohtoEternatusChamber_Hide:
    setflag FLAG_HIDE_ANTHOLOGY_ETERNATUS
    return

JohtoEternatusChamber_Show:
    clearflag FLAG_HIDE_ANTHOLOGY_ETERNATUS
    return

JohtoEternatusChamber_EventScript_Eternatus::
    lock
    faceplayer
    goto_if_unset FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED, JohtoEternatusChamber_NotReady
    playmoncry SPECIES_ETERNATUS, CRY_MODE_ENCOUNTER
    waitmoncry
    seteventmon SPECIES_ETERNATUS, 55
    special BattleSetup_StartLegendaryBattle
    waitstate
    specialvar VAR_RESULT, GetBattleOutcome
    goto_if_eq VAR_RESULT, B_OUTCOME_RAN, JohtoEternatusChamber_Retreated
    goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, JohtoEternatusChamber_Retreated
    goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, JohtoEternatusChamber_Caught
    goto_if_eq VAR_RESULT, B_OUTCOME_WON, JohtoEternatusChamber_Resolved
    release
    end

JohtoEternatusChamber_Caught:
    callnative AnthologyStaticRandomizerIsOn
    call_if_eq VAR_RESULT, TRUE, JohtoEternatusChamber_MarkSeededCatch
    goto JohtoEternatusChamber_Resolved

JohtoEternatusChamber_MarkSeededCatch:
    setflag FLAG_CAUGHT_RANDOM_ROUTE44_ETERNATUS
    return

JohtoEternatusChamber_Resolved:
    fadescreenswapbuffers FADE_TO_BLACK
    removeobject LOCALID_ANTHOLOGY_ETERNATUS
    setflag FLAG_HIDE_ANTHOLOGY_ETERNATUS
    fadescreenswapbuffers FADE_FROM_BLACK
    release
    end

JohtoEternatusChamber_NotReady:
    msgbox JohtoEternatusChamber_Text_NotReady, MSGBOX_DEFAULT
    release
    end

JohtoEternatusChamber_Retreated:
    release
    end

JohtoEternatusChamber_Text_NotReady:
    .string "The chamber still rejects you.$"
