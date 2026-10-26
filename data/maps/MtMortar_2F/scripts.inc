MtMortar_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMortar_2F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SetTimeEncounters
	.byte 0

MtMortar_2F_OnTransition: 
	call MtMortar_2F_AnthologyFieldTransition
	end

MtMortar_2F_EventScript_Item_Iron::
    finditem ITEM_IRON
    end

MtMortar_2F_EventScript_Item_MaxRevive::
    finditem ITEM_MAX_REVIVE
    end

MtMortar_2F_EventScript_Item_Ether::
    finditem ITEM_ETHER
    end

MtMortar_2F_EventScript_Item_RareCandy::
    finditem ITEM_RARE_CANDY
    end

MtMortar_2F_EventScript_Item_DragonScale::
    finditem ITEM_DRAGON_SCALE
    end

MtMortar_2F_EventScript_Item_Elixir::
    finditem ITEM_ELIXIR
    end

MtMortar_2F_EventScript_Nerd::
	trainerbattle_single TRAINER_HUGH_JT MtMortar_2F_Text_SupernerdHughSeen, MtMortar_2F_Text_SupernerdHughBeaten
	msgbox MtMortar_2F_Text_SupernerdHughAfter, MSGBOX_AUTOCLOSE
	end

MtMortar_2F_Text_SupernerdHughSeen::
	.string "Yo! MARKUS!$"

MtMortar_2F_Text_SupernerdHughBeaten:
	.string "Sorry, my mistake.\n"
	.string "I misjudged you.$"

MtMortar_2F_Text_SupernerdHughAfter:
	.string "I'm investigating MT.MORTAR,\n"
	.string "but my pal's gone AWOL.\p"
	.string "Have you seen a trainer who\n"
	.string "has a SLOWPOKE that can use\l"
	.string "STRENGTH?$"

// Entei's fixed encounter lies inside Mt. Mortar beyond the mountain approach.
// The Burned Tower awakening is required; no artificial badge threshold.
MtMortar_2F_AnthologyFieldTransition::
	call MtMortar_2F_FieldSyncEnteiReserve
	return

MtMortar_2F_FieldSyncEnteiReserve::
	goto_if_unset FLAG_RELEASED_BEASTS, MtMortar_2F_FieldHideEnteiReserve
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, MtMortar_2F_FieldSyncSeededEnteiReserve
	getcaughtmon SPECIES_ENTEI
	goto_if_eq VAR_RESULT, TRUE, MtMortar_2F_FieldHideEnteiReserve
	goto MtMortar_2F_FieldShowEnteiReserve

MtMortar_2F_FieldSyncSeededEnteiReserve::
	goto_if_set FLAG_CAUGHT_RANDOM_MTMORTAR_2F_ENTEI, MtMortar_2F_FieldHideEnteiReserve
	goto MtMortar_2F_FieldShowEnteiReserve

MtMortar_2F_FieldHideEnteiReserve::
	setflag FLAG_HIDE_ANTHOLOGY_BEAST_ENTEI
	return

MtMortar_2F_FieldShowEnteiReserve::
	clearflag FLAG_HIDE_ANTHOLOGY_BEAST_ENTEI
	return

MtMortar_2F_EventScript_EnteiReserve::
	lock
	faceplayer
	playmoncry SPECIES_ENTEI, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_ENTEI, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtMortar_2F_FieldEnteiReserveRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtMortar_2F_FieldEnteiReserveRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtMortar_2F_FieldEnteiReserveCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtMortar_2F_FieldEnteiReserveResolved
	release
	end

MtMortar_2F_FieldEnteiReserveCaught::
	callnative AnthologyStaticRandomizerIsOn
	call_if_eq VAR_RESULT, TRUE, MtMortar_2F_MarkSeededEntei
	goto MtMortar_2F_FieldEnteiReserveResolved

MtMortar_2F_MarkSeededEntei::
	setflag FLAG_CAUGHT_RANDOM_MTMORTAR_2F_ENTEI
	return

MtMortar_2F_FieldEnteiReserveResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_BEAST_ENTEI
	setflag FLAG_HIDE_ANTHOLOGY_BEAST_ENTEI
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

MtMortar_2F_FieldEnteiReserveRetreated::
	release
	end
