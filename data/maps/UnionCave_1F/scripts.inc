UnionCave_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, UnionCave_1F_OnTransition
	.byte 0

UnionCave_1F_OnTransition::
	call UnionCave_1F_AnthologyOnTransition
	end


UnionCave_1F_Trigger_Monview::
	special SpawnCameraObject
	applymovement2 OBJ_EVENT_ID_CAMERA, Common_Movement_WalkRight1
	waitmovement 0
	applymovement2 OBJ_EVENT_ID_CAMERA, Common_Movement_WalkRight1
	waitmovement 0
	applymovement2 OBJ_EVENT_ID_CAMERA, Common_Movement_WalkRight1
	waitmovement 0
	applymovement2 OBJ_EVENT_ID_CAMERA, Common_Movement_WalkRight1
	waitmovement 0
	message Route29_Text_PressAorB
	waitbuttonpress
	closemessage
	applymovement2 OBJ_EVENT_ID_CAMERA, Common_Movement_WalkLeft1
	waitmovement 0
	applymovement2 OBJ_EVENT_ID_CAMERA, Common_Movement_WalkLeft1
	waitmovement 0
	applymovement2 OBJ_EVENT_ID_CAMERA, Common_Movement_WalkLeft1
	waitmovement 0
	applymovement2 OBJ_EVENT_ID_CAMERA, Common_Movement_WalkLeft1
	waitmovement 0
	special RemoveCameraObject
	applymovement2 OBJ_EVENT_ID_PLAYER, Common_Movement_WalkLeft1
	waitmovement 0
	end


UnionCave_1F_EventScript_HikerRussell::
	trainerbattle_single TRAINER_RUSSELL, UnionCave_1F_Text_HikerRussellSeen, UnionCave_1F_Text_HikerRussellBeaten
	msgbox UnionCave_1F_Text_HikerRussellAfter, MSGBOX_AUTOCLOSE
	end

UnionCave_1F_EventScript_PokemaniacLarry::
	trainerbattle_single TRAINER_LARRY_JT, UnionCave_1F_Text_PokemaniacLarrySeen, UnionCave_1F_Text_PokemaniacLarryBeaten
	msgbox UnionCave_1F_Text_PokemaniacLarryAfter, MSGBOX_AUTOCLOSE
	end

UnionCave_1F_EventScript_HikerDaniel::
	trainerbattle_single TRAINER_DANIEL, UnionCave_1F_Text_HikerDanielSeen, UnionCave_1F_Text_HikerDanielBeaten
	msgbox UnionCave_1F_Text_HikerDanielAfter, MSGBOX_AUTOCLOSE
	end

UnionCave_1F_EventScript_FirebreatherBill::
	trainerbattle_single TRAINER_BILL, UnionCave_1F_Text_FirebreatherBillSeen, UnionCave_1F_Text_FirebreatherBillBeaten
	msgbox UnionCave_1F_Text_FirebreatherBillAfter, MSGBOX_AUTOCLOSE
	end

UnionCave_1F_EventScript_FirebreatherRay::
	trainerbattle_single TRAINER_RAY, UnionCave_1F_Text_FirebreatherRaySeen, UnionCave_1F_Text_FirebreatherRayBeaten
	msgbox UnionCave_1F_Text_FirebreatherRayAfter, MSGBOX_AUTOCLOSE
	end	

UnionCave_1F_EventScript_Item_Potion::
	finditem ITEM_POTION
	end

UnionCave_1F_EventScript_Item_GreatBall::
	finditem ITEM_GREAT_BALL
	end

UnionCave_1F_EventScript_Item_Xattack::
	finditem ITEM_X_ATTACK
	end

UnionCave_1F_EventScript_Item_Awakening::
	finditem ITEM_AWAKENING
	end

UnionCave_1F_Text_HikerRussellSeen:
	.string "You're headed to AZALEA, are you?\p"
	.string "Let my POKéMON see if you are good\n"
	.string "enough to battle.$"

UnionCave_1F_Text_HikerRussellBeaten:
	.string "Oh, oh, oh!$"

UnionCave_1F_Text_HikerRussellAfter:
	.string "All right, then! I've decided.\p"
	.string "I'm not leaving until my POKéMON\n"
	.string "get tougher!$"

UnionCave_1F_Text_PokemaniacLarrySeen:
	.string "I roam far and wide in search of\n"
	.string "POKéMON.\p"
	.string "Are you looking for POKéMON too?\p"
	.string "Then you're my collecting rival!$"

UnionCave_1F_Text_PokemaniacLarryBeaten:
	.string "Ugh. My poor POKéMON…$"

UnionCave_1F_Text_PokemaniacLarryAfter:
	.string "Every Friday, you can hear POKéMON\n"
	.string "roars from deep inside the cave.$"

UnionCave_1F_Text_HikerDanielSeen:
	.string "Whoa! What a surprise!\p"
	.string "I didn't expect to see anyone here!$"

UnionCave_1F_Text_HikerDanielBeaten:
	.string "Whoa! I'm beaten big time!$"

UnionCave_1F_Text_HikerDanielAfter:
	.string "I was conned into buying a\n"
	.string "SLOWPOKETAIL.\p"
	.string "I feel sorry for the poor POKéMON.$"

UnionCave_1F_Text_FirebreatherBillSeen:
	.string "ZUBAT's SUPERSONIC keeps confusing\n"
	.string "my POKéMON.\p"
	.string "I'm seriously upset about that!$"

UnionCave_1F_Text_FirebreatherBillBeaten:
	.string "I flamed out!$"

UnionCave_1F_Text_FirebreatherBillAfter:
	.string "On weekends, you can hear strange\n"
	.string "roars from deep in the cave.$"

UnionCave_1F_Text_FirebreatherRaySeen:
	.string "If it's light, a cave isn't scary.\p"
	.string "If you're strong, POKéMON aren't\n"
	.string "scary.$"

UnionCave_1F_Text_FirebreatherRayBeaten:
	.string "FLASH!$"

UnionCave_1F_Text_FirebreatherRayAfter:
	.string "It's my POKéMON's fire that lights\n"
	.string "up this cave.$"

// Independent early/mid-Johto Sword of Justice static. No sibling capture gate.
UnionCave_1F_AnthologyOnTransition::
	callnative AnthologyStaticRandomizerIsOn
	goto_if_eq VAR_RESULT, TRUE, UnionCave_1F_AnthologyRandomOnTransition
	getcaughtmon SPECIES_TERRAKION
	call_if_eq VAR_RESULT, TRUE, UnionCave_1F_AnthologyHideTerrakion
	call_if_ne VAR_RESULT, TRUE, UnionCave_1F_AnthologyShowTerrakion
	return

UnionCave_1F_AnthologyRandomOnTransition::
	call_if_set FLAG_CAUGHT_RANDOM_UNIONCAVE_1F_TERRAKION, UnionCave_1F_AnthologyHideTerrakion
	call_if_unset FLAG_CAUGHT_RANDOM_UNIONCAVE_1F_TERRAKION, UnionCave_1F_AnthologyShowTerrakion
	return

UnionCave_1F_AnthologyHideTerrakion::
	setflag FLAG_HIDE_ANTHOLOGY_TERRAKION
	return

UnionCave_1F_AnthologyShowTerrakion::
	clearflag FLAG_HIDE_ANTHOLOGY_TERRAKION
	return

// Defeated => map re-entry rematch. Caught => permanent for this site.
// Flee/teleport => stays.
UnionCave_1F_EventScript_Terrakion::
	lock
	faceplayer
	playmoncry SPECIES_TERRAKION, CRY_MODE_ENCOUNTER
	waitmoncry
	seteventmon SPECIES_TERRAKION, 20
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, UnionCave_1F_AnthologyTerrakionRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, UnionCave_1F_AnthologyTerrakionRetreated
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, UnionCave_1F_AnthologyTerrakionResolvedRandomCaught
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, UnionCave_1F_AnthologyTerrakionResolved
	release
	end

UnionCave_1F_AnthologyTerrakionResolvedRandomCaught::
	setflag FLAG_CAUGHT_RANDOM_UNIONCAVE_1F_TERRAKION
	goto UnionCave_1F_AnthologyTerrakionResolved

UnionCave_1F_AnthologyTerrakionResolved::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_ANTHOLOGY_TERRAKION
	setflag FLAG_HIDE_ANTHOLOGY_TERRAKION
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

UnionCave_1F_AnthologyTerrakionRetreated::
	release
	end

