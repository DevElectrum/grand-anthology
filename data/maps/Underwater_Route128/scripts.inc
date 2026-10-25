Underwater_Route128_MapScripts::
	.byte 0

Underwater_Route128_EventScript_GrottoEntrance::
	lock
	faceplayer
	goto_if_unset FLAG_BADGE07_GET, Underwater_Route128_EventScript_GrottoSealed
	goto_if_unset FLAG_DEFEATED_MAGMA_SPACE_CENTER, Underwater_Route128_EventScript_GrottoSealed
	msgbox Underwater_Route128_Text_GrottoEntrance, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, Underwater_Route128_EventScript_GrottoCancel
	closemessage
	warp MAP_ROUTE128_ANCIENT_GROTTO, 7, 4
	waitstate
	end

Underwater_Route128_EventScript_GrottoSealed::
	msgbox Underwater_Route128_Text_GrottoSealed, MSGBOX_DEFAULT
	release
	end

Underwater_Route128_EventScript_GrottoCancel::
	release
	end

Underwater_Route128_Text_GrottoEntrance:
	.string "Behind the sea-worn rock, an\\n"
	.string "ancient cavern lies hidden.\\p"
	.string "Enter the grotto?$"

Underwater_Route128_Text_GrottoSealed:
	.string "A deep current blocks the grotto.\\n"
	.string "Its ancient seal has not faded.$"
