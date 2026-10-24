GoldenrodCity_DepartmentStore_4F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, GoldenrodDeptElevator_OnFrame
	.byte 0

GoldenrodDeptStore4_EventScript_Cooltrainer::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart GoldenrodDeptStore4_Pokemart_FeathersAndMints
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
GoldenrodDeptStore4_Pokemart_FeathersAndMints::
	@ Rose Incense is the ONLY route to Budew and therefore to Roserade: Budew appears in no wild
	@ table, and AlterEggSpeciesWithIncenseItem downgrades a Budew egg back to Roselia unless a
	@ parent holds this. It had zero references outside its own item definition -- no mart, no
	@ ground item, no gift -- so both were unobtainable. Sold here because the Day Care this
	@ serves is next door on Route 34. (The other incenses are sold at SafariZoneGate.)
	.2byte ITEM_ROSE_INCENSE
	@ Anthology accessible ability and evolution items.
	.2byte ITEM_ABILITY_CAPSULE
	.2byte ITEM_ABILITY_PATCH
	.2byte ITEM_LEADERS_CREST
	.2byte ITEM_DUSK_STONE
	@ Complete pre-Champion evolutionary supplies for Johto's new roster.
	.2byte ITEM_FIRE_STONE
	.2byte ITEM_WATER_STONE
	.2byte ITEM_THUNDER_STONE
	.2byte ITEM_LEAF_STONE
	.2byte ITEM_MOON_STONE
	.2byte ITEM_SUN_STONE
	.2byte ITEM_SHINY_STONE
	.2byte ITEM_DAWN_STONE
	.2byte ITEM_ICE_STONE
	.2byte ITEM_METAL_COAT
	.2byte ITEM_KINGS_ROCK
	.2byte ITEM_DRAGON_SCALE
	.2byte ITEM_UP_GRADE
	.2byte ITEM_DUBIOUS_DISC
	.2byte ITEM_PRISM_SCALE
	.2byte ITEM_DEEP_SEA_TOOTH
	.2byte ITEM_DEEP_SEA_SCALE
	.2byte ITEM_ELECTIRIZER
	.2byte ITEM_MAGMARIZER
	.2byte ITEM_PROTECTOR
	.2byte ITEM_REAPER_CLOTH
	.2byte ITEM_RAZOR_CLAW
	.2byte ITEM_RAZOR_FANG
	.2byte ITEM_LINKING_CORD
	@ Offline evolution access for all generations.
	.2byte ITEM_SACHET
	.2byte ITEM_WHIPPED_DREAM
	.2byte ITEM_AUSPICIOUS_ARMOR
	.2byte ITEM_MALICIOUS_ARMOR
	.2byte ITEM_GIMMIGHOUL_COIN
	.2byte ITEM_OVAL_STONE
	.2byte ITEM_TART_APPLE
	.2byte ITEM_SWEET_APPLE
	.2byte ITEM_SYRUPY_APPLE
	.2byte ITEM_GALARICA_CUFF
	.2byte ITEM_GALARICA_WREATH
	.2byte ITEM_PEAT_BLOCK
	.2byte ITEM_BLACK_AUGURITE
	.2byte ITEM_METAL_ALLOY
	.2byte ITEM_SCROLL_OF_DARKNESS
	.2byte ITEM_SCROLL_OF_WATERS
	.2byte ITEM_STRAWBERRY_SWEET
	.2byte ITEM_LOVE_SWEET
	.2byte ITEM_BERRY_SWEET
	.2byte ITEM_CLOVER_SWEET
	.2byte ITEM_FLOWER_SWEET
	.2byte ITEM_STAR_SWEET
	.2byte ITEM_RIBBON_SWEET
	.2byte ITEM_CRACKED_POT
	.2byte ITEM_CHIPPED_POT
	.2byte ITEM_UNREMARKABLE_TEACUP
	.2byte ITEM_MASTERPIECE_TEACUP
	.2byte ITEM_HEALTH_FEATHER
	.2byte ITEM_MUSCLE_FEATHER
	.2byte ITEM_RESIST_FEATHER
	.2byte ITEM_GENIUS_FEATHER
	.2byte ITEM_CLEVER_FEATHER
	.2byte ITEM_SWIFT_FEATHER
	.2byte ITEM_LONELY_MINT
	.2byte ITEM_ADAMANT_MINT
	.2byte ITEM_NAUGHTY_MINT
	.2byte ITEM_BRAVE_MINT
	.2byte ITEM_BOLD_MINT
	.2byte ITEM_IMPISH_MINT
	.2byte ITEM_LAX_MINT
	.2byte ITEM_RELAXED_MINT
	.2byte ITEM_MODEST_MINT
	.2byte ITEM_MILD_MINT
	.2byte ITEM_RASH_MINT
	.2byte ITEM_QUIET_MINT
	.2byte ITEM_CALM_MINT
	.2byte ITEM_GENTLE_MINT
	.2byte ITEM_CAREFUL_MINT
	.2byte ITEM_SASSY_MINT
	.2byte ITEM_TIMID_MINT
	.2byte ITEM_HASTY_MINT
	.2byte ITEM_JOLLY_MINT
	.2byte ITEM_NAIVE_MINT
	.2byte ITEM_SERIOUS_MINT
	pokemartlistend

GoldenrodDeptStore4_EventScript_BugCatcher::
	lock
	faceplayer
	msgbox GoldenrodDeptStore_Text_4FBugCatcher, MSGBOX_DEFAULT
	closemessage
	release
	end

GoldenrodDeptStore4_EventScript_GBAKid::
	lock
	faceplayer
	msgbox GoldenrodDeptStore_Text_4FGameboyKid, MSGBOX_DEFAULT
	closemessage
	release
	end

GoldenrodDeptStore4_EventScript_Sign::
	lock
	faceplayer
	msgbox GoldenrodDeptStore_Text_4FDirectory, MSGBOX_DEFAULT
	closemessage
	release
	end

GoldenrodDeptStore_Text_4FBugCatcher:
    .string "IRON adds to your POKéMON's\n"
    .string "DEFENSE.$"

GoldenrodDeptStore_Text_4FGameboyKid:
    .string "There were rumours that some\n"
    .string "POKéMON evolve only by\l"
    .string "being traded via a Game Link\l"
    .string "cable.\p"
    .string "Pfft…\p"
    .string "You just need a LINKING CORD.\p"
    .string "I know some of the rumoured\n"
    .string "POKéMON:\l"
    .string "MACHOKE, KADABRA, HAUNTER and,\l"
    .string "um, GRAVELER.\p"
    .string "They even sell LINKING CORDS\n"
    .string "in MAHOGANY TOWN.$"

GoldenrodDeptStore_Text_4FDirectory:
    .string "Let Us Pump Up Your POKéMON!\p"
    .string "4F MEDICINE BOX$"
