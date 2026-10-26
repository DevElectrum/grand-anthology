#include "global.h"
#include "battle_setup.h"
#include "battle_pike.h"
#include "battle_pyramid.h"
#include "event_data.h"
#include "fieldmap.h"
#include "fishing.h"
#include "follower_npc.h"
#include "item.h"
#include "random.h"
#include "field_player_avatar.h"
#include "link.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "ow_abilities.h"
#include "pokeblock.h"
#include "pokemon.h"
#include "random.h"
#include "roamer.h"
#include "safari_zone.h"
#include "script.h"
#include "tv.h"
#include "wild_encounter.h"
#include "battle_debug.h"
#include "battle_pike.h"
#include "battle_pyramid.h"
#include "constants/abilities.h"
#include "constants/game_stat.h"
#include "constants/item.h"
#include "constants/items.h"
#include "constants/layouts.h"
#include "constants/weather.h"

extern const u8 EventScript_SprayWoreOff[];

#define MAX_ENCOUNTER_RATE 2880

#define NUM_FEEBAS_SPOTS 6

// Number of accessible fishing spots in each section of Route 119
// Each section is an area of the route between the y coordinates in sRoute119WaterTileData
#define NUM_FISHING_SPOTS_1 131
#define NUM_FISHING_SPOTS_2 167
#define NUM_FISHING_SPOTS_3 149
#define NUM_FISHING_SPOTS (NUM_FISHING_SPOTS_1 + NUM_FISHING_SPOTS_2 + NUM_FISHING_SPOTS_3)

static u16 FeebasRandom(void);
static void FeebasSeedRng(u16 seed);
static void ApplyFluteEncounterRateMod(u32 *encRate);
static void ApplyCleanseTagEncounterRateMod(u32 *encRate);
static u8 GetMaxLevelOfSpeciesInWildTable(const struct WildPokemon *wildMon, enum Species species, enum WildPokemonArea area);
#ifdef BUGFIX
static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon *wildMon, enum Type type, enum Ability ability, u8 *monIndex, u32 size);
#else
static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon *wildMon, enum Type type, enum Ability ability, u8 *monIndex);
#endif

EWRAM_DATA static u8 sWildEncountersDisabled = 0;
EWRAM_DATA static u32 sFeebasRngValue = 0;
EWRAM_DATA bool8 gIsFishingEncounter = 0;
EWRAM_DATA bool8 gIsSurfingEncounter = 0;
EWRAM_DATA u8 gChainFishingDexNavStreak = 0;

#include "data/wild_encounters.h"

const struct WildPokemon gWildFeebas = {20, 25, SPECIES_FEEBAS};

static const u16 sRoute119WaterTileData[] =
{
//yMin, yMax, numSpots in previous sections
     0,  45,  0,
    46,  91,  NUM_FISHING_SPOTS_1,
    92, 139,  NUM_FISHING_SPOTS_1 + NUM_FISHING_SPOTS_2,
};

void DisableWildEncounters(bool8 disabled)
{
    sWildEncountersDisabled = disabled;
}

// Each fishing spot on Route 119 is given a number between 1 and NUM_FISHING_SPOTS inclusive.
// The number is determined by counting the valid fishing spots left to right top to bottom.
// The map is divided into three sections, with each section having a pre-counted number of
// fishing spots to start from to avoid counting a large number of spots at the bottom of the map.
// Note that a spot is considered valid if it is surfable and not a waterfall. To exclude all
// of the inaccessible water metatiles (so that they can't be selected as a Feebas spot) they
// use a different metatile that isn't actually surfable because it has MB_NORMAL instead.
// This function is given the coordinates and section of a fishing spot and returns which number it is.
static u16 GetFeebasFishingSpotId(s16 targetX, s16 targetY, u8 section)
{
    u16 x, y;
    u16 yMin = sRoute119WaterTileData[section * 3 + 0];
    u16 yMax = sRoute119WaterTileData[section * 3 + 1];
    u16 spotId = sRoute119WaterTileData[section * 3 + 2];

    for (y = yMin; y <= yMax; y++)
    {
        for (x = 0; x < gMapHeader.mapLayout->width; x++)
        {
            u8 behavior = MapGridGetMetatileBehaviorAt(x + MAP_OFFSET, y + MAP_OFFSET);
            if (MetatileBehavior_IsSurfableAndNotWaterfall(behavior) == TRUE)
            {
                spotId++;
                if (targetX == x && targetY == y)
                    return spotId;
            }
        }
    }
    return spotId + 1;
}

bool8 CheckFeebasAtCoords(s16 x, s16 y)
{
    u8 i;
    u16 feebasSpots[NUM_FEEBAS_SPOTS];
    u8 route119Section = 0;
    u16 spotId;

    if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_ROUTE119)
     && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_ROUTE119))
    {
        x -= MAP_OFFSET;
        y -= MAP_OFFSET;

        // Get which third of the map the player is in
        if (y >= sRoute119WaterTileData[3 * 0 + 0] && y <= sRoute119WaterTileData[3 * 0 + 1])
            route119Section = 0;
        if (y >= sRoute119WaterTileData[3 * 1 + 0] && y <= sRoute119WaterTileData[3 * 1 + 1])
            route119Section = 1;
        if (y >= sRoute119WaterTileData[3 * 2 + 0] && y <= sRoute119WaterTileData[3 * 2 + 1])
            route119Section = 2;

        // 50% chance of encountering Feebas (assuming this is a Feebas spot)
        if (Random() % 100 > 49)
            return FALSE;

        FeebasSeedRng(gSaveBlock1Ptr->dewfordTrends[0].rand);

        // Assign each Feebas spot to a random fishing spot.
        // Randomness is fixed depending on the seed above.
        for (i = 0; i != NUM_FEEBAS_SPOTS;)
        {
            feebasSpots[i] = FeebasRandom() % NUM_FISHING_SPOTS;
            if (feebasSpots[i] == 0)
                feebasSpots[i] = NUM_FISHING_SPOTS;

            // < 1 below is a pointless check, it will never be TRUE.
            // >= 4 to skip fishing spots 1-3, because these are inaccessible
            // spots at the top of the map, at (9,7), (7,13), and (15,16).
            // The first accessible fishing spot is spot 4 at (18,18).
            if (feebasSpots[i] < 1 || feebasSpots[i] >= 4)
                i++;
        }

        // Check which fishing spot the player is at, and see if
        // it matches any of the Feebas spots.
        spotId = GetFeebasFishingSpotId(x, y, route119Section);
        for (i = 0; i < NUM_FEEBAS_SPOTS; i++)
        {
            if (spotId == feebasSpots[i])
                return TRUE;
        }
    }
    return FALSE;
}

static u16 FeebasRandom(void)
{
    sFeebasRngValue = ISO_RANDOMIZE2(sFeebasRngValue);
    return sFeebasRngValue >> 16;
}

static void FeebasSeedRng(u16 seed)
{
    sFeebasRngValue = seed;
}

// NUM_LAND_MONS_ENCOUNTER_SLOTS
u32 ChooseWildMonIndex_Land(void)
{
    u8 wildMonIndex = 0;
    bool8 swap = FALSE;
    u8 rand = Random() % ENCOUNTER_CHANCE_LAND_MONS_TOTAL;

    if (rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_0)
        wildMonIndex = 0;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_1)
        wildMonIndex = 1;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_2)
        wildMonIndex = 2;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_3)
        wildMonIndex = 3;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_3 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_4)
        wildMonIndex = 4;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_4 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_5)
        wildMonIndex = 5;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_5 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_6)
        wildMonIndex = 6;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_6 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_7)
        wildMonIndex = 7;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_7 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_8)
        wildMonIndex = 8;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_8 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_9)
        wildMonIndex = 9;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_9 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_10)
        wildMonIndex = 10;
    else
        wildMonIndex = 11;

    if (LURE_STEP_COUNT != 0 && (Random() % 10 < 2))
        swap = TRUE;

    if (swap)
        wildMonIndex = 11 - wildMonIndex;

    return wildMonIndex;
}

// Mostly equivalent to ChooseWildMonIndex_Land
// NUM_LAND_MONS_ENCOUNTER_SLOTS
u8 GetLandEncounterSlotForMatchCall(void)
{
    int rand = Random() % ENCOUNTER_CHANCE_LAND_MONS_TOTAL;

    if (rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_0)
        return 0;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_1)
        return 1;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_2)
        return 2;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_3)
        return 3;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_3 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_4)
        return 4;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_4 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_5)
        return 5;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_5 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_6)
        return 6;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_6 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_7)
        return 7;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_7 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_8)
        return 8;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_8 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_9)
        return 9;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_9 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_10)
        return 10;
    else
        return 11;
}

// NUM_WATER_MONS_ENCOUNTER_SLOTS
u32 ChooseWildMonIndex_Water(void)
{
    u32 wildMonIndex = 0;
    bool8 swap = FALSE;
    u8 rand = Random() % ENCOUNTER_CHANCE_WATER_MONS_TOTAL;

    if (rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_0)
        wildMonIndex = 0;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_1)
        wildMonIndex = 1;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_2)
        wildMonIndex = 2;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_3)
        wildMonIndex = 3;
    else
        wildMonIndex = 4;

    if (LURE_STEP_COUNT != 0 && (Random() % 10 < 2))
        swap = TRUE;

    if (swap)
        wildMonIndex = 4 - wildMonIndex;

    return wildMonIndex;
}

// Mostly equivalent to ChooseWildMonIndex_WaterRock
// NUM_WATER_MONS_ENCOUNTER_SLOTS
u8 GetWaterEncounterSlotForMatchCall(void)
{
    int rand = Random() % ENCOUNTER_CHANCE_WATER_MONS_TOTAL;

    if (rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_0)
        return 0;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_1)
        return 1;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_2)
        return 2;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_3)
        return 3;
    else
        return 4;
}


// NUM_ROCK_SMASH_MONS_ENCOUNTER_SLOTS
u32 ChooseWildMonIndex_Rocks(void)
{
    u32 wildMonIndex = 0;
    bool8 swap = FALSE;
    u8 rand = Random() % ENCOUNTER_CHANCE_ROCK_SMASH_MONS_TOTAL;

    if (rand < ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_0)
        wildMonIndex = 0;
    else if (rand >= ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_1)
        wildMonIndex = 1;
    else if (rand >= ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_2)
        wildMonIndex = 2;
    else if (rand >= ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_ROCK_SMASH_MONS_SLOT_3)
        wildMonIndex = 3;
    else
        wildMonIndex = 4;

    if (LURE_STEP_COUNT != 0 && (Random() % 10 < 2))
        swap = TRUE;

    if (swap)
        wildMonIndex = 4 - wildMonIndex;

    return wildMonIndex;
}

// NUM_FISHING_MONS_ENCOUNTER_SLOTS
static u32 ChooseWildMonIndex_Fishing(u8 rod)
{
    u8 wildMonIndex = 0;
    bool8 swap = FALSE;
    u8 rand = Random() % max(max(ENCOUNTER_CHANCE_FISHING_MONS_OLD_ROD_TOTAL, ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_TOTAL),
                             ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_TOTAL);

    if (LURE_STEP_COUNT != 0 && (Random() % 10 < 2))
        swap = TRUE;

    switch (rod)
    {
    case OLD_ROD:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_OLD_ROD_SLOT_0)
            wildMonIndex = 0;
        else
            wildMonIndex = 1;

        if (swap)
            wildMonIndex = 1 - wildMonIndex;
        break;
    case GOOD_ROD:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_2)
            wildMonIndex = 2;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_2 && rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_3)
            wildMonIndex = 3;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_3 && rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_4)
            wildMonIndex = 4;

        if (swap)
            wildMonIndex = 6 - wildMonIndex;
        break;
    case SUPER_ROD:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_5)
            wildMonIndex = 5;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_5 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_6)
            wildMonIndex = 6;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_6 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_7)
            wildMonIndex = 7;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_7 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_8)
            wildMonIndex = 8;
        if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_8 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_9)
            wildMonIndex = 9;

        if (swap)
            wildMonIndex = 14 - wildMonIndex;
        break;
    }
    return wildMonIndex;
}

u8 ChooseWildMonLevel(const struct WildPokemon *wildPokemon, u8 wildMonIndex, enum WildPokemonArea area)
{
    u8 min;
    u8 max;
    u8 range;
    u8 rand;

    if (LURE_STEP_COUNT == 0)
    {
        // Make sure minimum level is less than maximum level
        if (wildPokemon[wildMonIndex].maxLevel >= wildPokemon[wildMonIndex].minLevel)
        {
            min = wildPokemon[wildMonIndex].minLevel;
            max = wildPokemon[wildMonIndex].maxLevel;
        }
        else
        {
            min = wildPokemon[wildMonIndex].maxLevel;
            max = wildPokemon[wildMonIndex].minLevel;
        }
        range = max - min + 1;
        rand = Random() % range;

        // check ability for max level mon
        if (!GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SANITY_IS_EGG))
        {
            enum Ability ability = GetMonAbility(&gParties[B_TRAINER_PLAYER][0]);
            if (ability == ABILITY_HUSTLE || ability == ABILITY_VITAL_SPIRIT || ability == ABILITY_PRESSURE)
            {
                if (Random() % 2 == 0)
                    return max;

                if (rand != 0)
                    rand--;
            }
        }
        return min + rand;
    }
    else
    {
        // Looks for the max level of all slots that share the same species as the selected slot.
        max = GetMaxLevelOfSpeciesInWildTable(wildPokemon, wildPokemon[wildMonIndex].species, area);
        if (max > 0)
            return max + 1;
        else // Failsafe
            return wildPokemon[wildMonIndex].maxLevel + 1;
    }
}

// The seven Tanoby Chambers. Listed one by one rather than as a map-number range so that
// a future map renumber cannot silently pull a neighbouring Seven Island map into the gate.
static bool32 IsTanobyChamberMap(u8 mapGroup, u8 mapNum)
{
    if (mapGroup != MAP_GROUP(MAP_SEVEN_ISLAND_TANOBY_RUINS_MONEAN_CHAMBER))
        return FALSE;

    return mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_MONEAN_CHAMBER)
        || mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_LIPTOO_CHAMBER)
        || mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_WEEPTH_CHAMBER)
        || mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_DILFORD_CHAMBER)
        || mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_SCUFIB_CHAMBER)
        || mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_RIXY_CHAMBER)
        || mapNum == MAP_NUM(MAP_SEVEN_ISLAND_TANOBY_RUINS_VIAPOIS_CHAMBER);
}

u16 GetCurrentMapWildMonHeaderId(void)
{
    u16 i;

    for (i = 0; ; i++)
    {
        const struct WildPokemonHeader *wildHeader = &gWildMonHeaders[i];
        if (wildHeader->mapGroup == MAP_GROUP(MAP_UNDEFINED))
            break;

        if (gWildMonHeaders[i].mapGroup == gSaveBlock1Ptr->location.mapGroup &&
            gWildMonHeaders[i].mapNum == gSaveBlock1Ptr->location.mapNum)
        {
            // FRLG parity: the chambers are empty until the Tanoby Key puzzle is solved.
            // Withholding the whole header (rather than an individual table) covers every
            // chamber, both the land and hidden tables, and the FireRed/LeafGreen variants
            // alike, since the gate keys on the map rather than on which table was matched.
            if (IsTanobyChamberMap(gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum)
             && !FlagGet(FLAG_SYS_UNLOCKED_TANOBY_RUINS))
                return HEADER_NONE;

            if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_ALTERING_CAVE) &&
                gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_ALTERING_CAVE))
            {
                u16 alteringCaveId = VarGet(VAR_ALTERING_CAVE_WILD_SET);
                if (alteringCaveId >= NUM_ALTERING_CAVE_TABLES)
                    alteringCaveId = 0;

                i += alteringCaveId;
            }

            return i;
        }
    }

    return HEADER_NONE;
}

// Region merge perf: 493 headers in the 3-region build make the linear scan too hot for the
// per-frame OWE guard (it previously burned ~2% of the frame on encounter-less maps). Keyed
// on exactly the inputs the scan reads: current map + the Altering Cave table var + the
// Tanoby Key flag.
u16 GetCurrentMapWildMonHeaderIdCached(void)
{
    // All statics zero-init (.bss) — the ld script discards .data, so no nonzero initializers.
    static bool8 sCacheValid;
    static u16 sCachedHeaderId;
    static u8 sCachedMapGroup, sCachedMapNum;
    static u16 sCachedAlteringCaveId;
    static bool8 sCachedTanobyUnlocked;
    u16 alteringCaveId = VarGet(VAR_ALTERING_CAVE_WILD_SET);
    bool8 tanobyUnlocked = FlagGet(FLAG_SYS_UNLOCKED_TANOBY_RUINS);

    if (!sCacheValid
     || sCachedMapGroup != (u8)gSaveBlock1Ptr->location.mapGroup
     || sCachedMapNum != (u8)gSaveBlock1Ptr->location.mapNum
     || sCachedAlteringCaveId != alteringCaveId
     || sCachedTanobyUnlocked != tanobyUnlocked)
    {
        sCacheValid = TRUE;
        sCachedMapGroup = gSaveBlock1Ptr->location.mapGroup;
        sCachedMapNum = gSaveBlock1Ptr->location.mapNum;
        sCachedAlteringCaveId = alteringCaveId;
        sCachedTanobyUnlocked = tanobyUnlocked;
        sCachedHeaderId = GetCurrentMapWildMonHeaderId();
    }
    return sCachedHeaderId;
}

enum TimeOfDay GetTimeOfDayForEncounters(u32 headerId, enum WildPokemonArea area)
{
    const struct WildPokemonInfo *wildMonInfo;
    enum TimeOfDay timeOfDay = GetTimeOfDay();

    if (!OW_TIME_OF_DAY_ENCOUNTERS)
        return TIME_OF_DAY_DEFAULT;

    if (InBattlePike() || CurrentBattlePyramidLocation() != PYRAMID_LOCATION_NONE)
        return OW_TIME_OF_DAY_FALLBACK;

    switch (area)
    {
    default:
    case WILD_AREA_LAND:
        wildMonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo;
        break;
    case WILD_AREA_WATER:
        wildMonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo;
        break;
    case WILD_AREA_ROCKS:
        wildMonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].rockSmashMonsInfo;
        break;
    case WILD_AREA_FISHING:
        wildMonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].fishingMonsInfo;
        break;
    case WILD_AREA_HIDDEN:
        wildMonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].hiddenMonsInfo;
        break;
    }

    if (wildMonInfo == NULL && !OW_TIME_OF_DAY_DISABLE_FALLBACK)
        return OW_TIME_OF_DAY_FALLBACK;
    else
        return GenConfigTimeOfDay(timeOfDay);
}

static u8 PickWildMonNature(enum Species species)
{
    u8 i;
    struct Pokeblock *safariPokeblock;
    u8 natures[NUM_NATURES];

    if (GetSafariZoneFlag() == TRUE && Random() % 100 < 80)
    {
        safariPokeblock = SafariZoneGetActivePokeblock();
        if (safariPokeblock != NULL)
        {
            for (i = 0; i < NUM_NATURES; i++)
                natures[i] = i;
            Shuffle(natures, NUM_NATURES, sizeof(natures[0]));
            for (i = 0; i < NUM_NATURES; i++)
            {
                if (PokeblockGetGain(natures[i], safariPokeblock) > 0)
                    return natures[i];
            }
        }
    }

    return GetSynchronizedNature(WILDMON_ORIGIN, species);
}

// P0 Anthology: natural wild encounters have a repeatable *species mapping*
// per seed, map, area and encounter table slot. No gameplay RNG is advanced,
// so reloads/revisits give the same mapping regardless of frame timing.
// Scripted legendary/static encounters, roamer/outbreak encounters, FEebas,
// trainer battles and Birch's scripted first battle never enter this function.
// Preserve special Battle Frontier rules even when wild battles call the same
// encounter-table generator as routes.
// The seed and toggle are checksum-protected SaveBlock2 data (v12).
enum Species WorldRandomizeWildSpeciesAtMap(enum Species original, u32 slot, enum WildPokemonArea area,
                                            u8 mapGroup, u8 mapNum)
{
#if ALL_REGIONS
    u32 hash;
    enum Species result;
    u32 attempts;

    if (gSaveBlock2Ptr == NULL || !gSaveBlock2Ptr->randomizerWild
     || (gMapHeader.battleType == MAP_BATTLE_SCENE_FRONTIER
      && gSaveBlock1Ptr->location.mapGroup == mapGroup
      && gSaveBlock1Ptr->location.mapNum == mapNum)
     || original == SPECIES_NONE || original == SPECIES_EGG)
        return original;

    // Jenkins-style avalanche; no global RNG and no evolving game state.
    // The ID, area, slot and map numbers independently influence the result.
    hash = 0x9E3779B9u ^ (u32)gSaveBlock2Ptr->randomizerSeed;
    hash ^= (u32)original * 0x85EBCA6Bu;
    hash ^= (slot + 1) * 0xC2B2AE35u;
    hash ^= ((u32)mapGroup << 24)
          | ((u32)mapNum << 16)
          | ((u32)area << 8);
    hash ^= hash >> 16;
    hash *= 0x7FEB352Du;
    hash ^= hash >> 15;
    hash *= 0x846CA68Bu;
    hash ^= hash >> 16;
    result = 1 + hash % (NUM_SPECIES - 1);

    // Enabled families/forms only. Battle-exclusive transformations cannot
    // naturally spawn as party Pokémon and must be acquired via their own
    // mechanics. Bounded probing guarantees no infinite loops on sparse data.
    for (attempts = 0; attempts < NUM_SPECIES - 1; attempts++)
    {
        if (WorldIsFieldStableDiscoverySpecies(result))
            return result;
        if (++result >= NUM_SPECIES)
            result = 1;
    }
#endif
    return original;
}

// The original API remains the encounter-table entrypoint.
static enum Species WorldRandomizeWildSpecies(enum Species original, u32 slot, enum WildPokemonArea area)
{
    return WorldRandomizeWildSpeciesAtMap(original, slot, area,
        gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum);
}

// Natural, pre-Champion rare habitat encounters. No original wild table is
// overwritten. Only use an extra slot when the DexNav land UI has a free icon;
// that ensures its list and the real encounter generator agree.
enum Species AnthologyGetRareHabitatSpecies(enum WildPokemonArea area, const struct WildPokemonInfo *info)
{
#if ALL_REGIONS
    u32 i, j, distinct = 0;
    enum Species rare = SPECIES_NONE;
    if (area != WILD_AREA_LAND || info == NULL || info->encounterRate == 0
     || gSaveBlock1Ptr == NULL || gSaveBlock2Ptr == NULL || gSaveBlock2Ptr->randomizerWild
     || GetSafariZoneFlag() || InBattlePike()
     || CurrentBattlePyramidLocation() != PYRAMID_LOCATION_NONE
     || gMapHeader.battleType == MAP_BATTLE_SCENE_FRONTIER)
        return SPECIES_NONE;

#define RARE_ON_MAP(map, mon) \
    if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(map) \
     && gSaveBlock1Ptr->location.mapNum == MAP_NUM(map)) rare = SPECIES_##mon;
    // Shared catalog also powers the native Pokédex area screen.
#include "data/anthology_rare_habitats.h"
#undef RARE_ON_MAP

    if (!WorldIsNaturalDiscoverySpecies(rare))
        return SPECIES_NONE;
    for (i = 0; i < NUM_LAND_MONS_ENCOUNTER_SLOTS; i++)
    {
        if (info->wildPokemon[i].species == rare)
            return SPECIES_NONE; // Already an original habitat species
        for (j = 0; j < i; j++)
            if (info->wildPokemon[j].species == info->wildPokemon[i].species)
                break;
        if (j == i)
            distinct++;
    }
    if (distinct >= NUM_LAND_MONS_ENCOUNTER_SLOTS)
        return SPECIES_NONE;
    return rare;
#else
    return SPECIES_NONE;
#endif
}

// P0 post-League discovery rotation: each third *ordinary* wild encounter
// offers another enabled, field-stable species/form, including Gen 4-9 and
// regional variants. The persisted wild-battle counter makes the rotation
// repeatable across saves without changing any cartridge save structure.
// Original regional encounter tables remain untouched before the first
// championship, with wild randomizer ON, and in facilities/Safari. Scripted
// statics, roamers, outbreaks, gifts and story battles bypass this hook.
// This is a broad catchability path, not yet a finished targeted encounter UI.
// Shared eligibility check for post-League encounter rotation and the
// player Research target. The field-stability check here intentionally remains
// broader than natural Research eligibility because randomizer uses it too.
bool8 WorldIsFieldStableDiscoverySpecies(enum Species species)
{
#if ALL_REGIONS
    if (species <= SPECIES_NONE || species >= NUM_SPECIES)
        return FALSE;
    // Battle-only transformations are not independent wild-catchable forms.
    // Their permanent bases stay obtainable, and the corresponding abilities
    // and moves still transform them in battle. In particular, Battle Bond
    // Greninja is a valid base but Ash-Greninja is a battle outcome.
    switch (species)
    {
    case SPECIES_GRENINJA_ASH:
    case SPECIES_AEGISLASH_BLADE:
    case SPECIES_WISHIWASHI_SCHOOL:
    case SPECIES_MIMIKYU_BUSTED:
    case SPECIES_PALAFIN_HERO:
    case SPECIES_CRAMORANT_GULPING:
    case SPECIES_CRAMORANT_GORGING:
    case SPECIES_MORPEKO_HANGRY:
    case SPECIES_ZYGARDE_COMPLETE:
    case SPECIES_CASTFORM_SUNNY:
    case SPECIES_CASTFORM_RAINY:
    case SPECIES_CASTFORM_SNOWY:
    case SPECIES_DARMANITAN_ZEN:
    case SPECIES_DARMANITAN_GALAR_ZEN:
    case SPECIES_CHERRIM_SUNSHINE:
    case SPECIES_EISCUE_NOICE:
    case SPECIES_MINIOR_CORE_RED:
    case SPECIES_MINIOR_CORE_ORANGE:
    case SPECIES_MINIOR_CORE_YELLOW:
    case SPECIES_MINIOR_CORE_GREEN:
    case SPECIES_MINIOR_CORE_BLUE:
    case SPECIES_MINIOR_CORE_INDIGO:
    case SPECIES_MINIOR_CORE_VIOLET:
        return FALSE;
    default:
        break;
    }
    return IsSpeciesEnabled(species)
        && !gSpeciesInfo[species].isMegaEvolution
        && !gSpeciesInfo[species].isPrimalReversion
        && !gSpeciesInfo[species].isUltraBurst
        && !gSpeciesInfo[species].isGigantamax
        && !gSpeciesInfo[species].isTeraForm
        && !gSpeciesInfo[species].isTotem;
#else
    return FALSE;
#endif
}

// Eligibility for ordinary habitats and post-League Research is deliberately
// narrower than seeded randomizer eligibility. Legendary/Mythical species use
// authored statics unless natural populations make an existing wild habitat
// lore-appropriate. This does not create new encounters for these exceptions.
// Ultra Beasts and Paradox species are ordinary rare wildlife for this game.
bool8 WorldIsNaturalDiscoverySpecies(enum Species species)
{
#if ALL_REGIONS
    if (!WorldIsFieldStableDiscoverySpecies(species))
        return FALSE;
    if (gSpeciesInfo[species].isUltraBeast || gSpeciesInfo[species].isParadox)
        return TRUE;
    if (gSpeciesInfo[species].isRestrictedLegendary
     || gSpeciesInfo[species].isSubLegendary
     || gSpeciesInfo[species].isMythical)
    {
        switch (species)
        {
        // Known multi-individual populations: retain any legitimate existing
        // habitats, but never add these to unrelated routes automatically.
        case SPECIES_ZARUDE:
        case SPECIES_PHIONE:
        case SPECIES_MELTAN:
            return TRUE;
        default:
            return FALSE;
        }
    }
    return TRUE;
#else
    return FALSE;
#endif
}

// Post-League Research substitutes into unrelated routes, so its pool excludes
// all Legendary/Mythical species, including those with natural populations.
// An authored local habitat may separately allow those lore exceptions.
bool8 WorldIsPostLeagueResearchSpecies(enum Species species)
{
#if ALL_REGIONS
    if (!WorldIsNaturalDiscoverySpecies(species))
        return FALSE;
    return !gSpeciesInfo[species].isRestrictedLegendary
        && !gSpeciesInfo[species].isSubLegendary
        && !gSpeciesInfo[species].isMythical;
#else
    return FALSE;
#endif
}

// Supplemental ordinary species that were missing from original grass tables.
// These are separate from unique static encounters, fossil restoration and
// fish-only habitats. The source catalog is a small, compile-time map index.
#if ALL_REGIONS
struct AnthologyExtraHabitat
{
    u8 mapGroup;
    u8 mapNum;
    enum Species species;
};
#define ECOLOGY_ON_MAP(map, mon) {MAP_GROUP(map), MAP_NUM(map), SPECIES_##mon},
static const struct AnthologyExtraHabitat sAnthologyExtraHabitats[] =
{
#include "data/anthology_extra_habitats.h"
};
#undef ECOLOGY_ON_MAP
#endif

enum Species AnthologyGetExtraHabitatSpecies(enum WildPokemonArea area,
                                          const struct WildPokemonInfo *info, u8 slot)
{
#if ALL_REGIONS
    u32 i, j;
    // Reuse all of the original habitat's story/facility/randomizer guards.
    if (AnthologyGetRareHabitatSpecies(area, info) == SPECIES_NONE || slot >= ANTHOLOGY_EXTRA_HABITAT_SLOTS)
        return SPECIES_NONE;
    for (i = 0; i < ARRAY_COUNT(sAnthologyExtraHabitats); i++)
    {
        if (sAnthologyExtraHabitats[i].mapGroup != gSaveBlock1Ptr->location.mapGroup
         || sAnthologyExtraHabitats[i].mapNum != gSaveBlock1Ptr->location.mapNum)
            continue;
        if (slot-- != 0)
            continue;
        if (!WorldIsNaturalDiscoverySpecies(sAnthologyExtraHabitats[i].species))
            return SPECIES_NONE;
        // If any native time-of-day table acquires this species, prefer the
        // native encounter and avoid showing it twice in DexNav.
        for (j = 0; j < NUM_LAND_MONS_ENCOUNTER_SLOTS; j++)
            if (info->wildPokemon[j].species == sAnthologyExtraHabitats[i].species)
                return SPECIES_NONE;
        return sAnthologyExtraHabitats[i].species;
    }
#endif
    return SPECIES_NONE;
}

// Area-screen-safe membership: never change the live player's map to query
// a different habitat. Look up the exact header location and current land table.
// This supplements, never replaces, native Pokédex location discovery.
bool8 AnthologyMapHasHabitatSpecies(u8 mapGroup, u8 mapNum,
                                   enum Species species, const struct WildPokemonInfo *info)
{
#if ALL_REGIONS
    u32 i, j, distinct = 0;
    if (info == NULL || info->encounterRate == 0 || !WorldIsNaturalDiscoverySpecies(species))
        return FALSE;
    for (i = 0; i < NUM_LAND_MONS_ENCOUNTER_SLOTS; i++)
    {
        if (info->wildPokemon[i].species == species)
            return FALSE; // the native encounter lookup already handles this
        for (j = 0; j < i; j++)
            if (info->wildPokemon[j].species == info->wildPokemon[i].species)
                break;
        if (j == i)
            distinct++;
    }
    if (distinct >= NUM_LAND_MONS_ENCOUNTER_SLOTS)
        return FALSE;
#define RARE_ON_MAP(map, mon) \
    if (mapGroup == MAP_GROUP(map) && mapNum == MAP_NUM(map) && species == SPECIES_##mon) return TRUE;
#include "data/anthology_rare_habitats.h"
#undef RARE_ON_MAP
    // Extras exist only where a primary rare habitat is registered; their
    // generated source and CI audit enforce the same DexNav capacity invariant.
    for (i = 0; i < ARRAY_COUNT(sAnthologyExtraHabitats); i++)
        if (mapGroup == sAnthologyExtraHabitats[i].mapGroup
         && mapNum == sAnthologyExtraHabitats[i].mapNum
         && species == sAnthologyExtraHabitats[i].species)
            return TRUE;
#endif
    return FALSE;
}


#if ALL_REGIONS
struct AnthologyAquaticHabitat
{
    u8 mapGroup;
    u8 mapNum;
    enum WildPokemonArea area;
    enum Species species;
};
#define AQUATIC_ON_MAP(map, kind, mon) {MAP_GROUP(map), MAP_NUM(map), WILD_AREA_##kind, SPECIES_##mon},
static const struct AnthologyAquaticHabitat sAnthologyAquaticHabitats[] =
{
#include "data/anthology_aquatic_habitats.h"
};
#undef AQUATIC_ON_MAP
#endif

// Shared selection policy for Surf, fishing, DexNav and Pokédex area maps.
// Caller supplies a real table for the destination map, never a forged player
// location. Surf bonus must leave a free icon among the five native water slots.
static enum Species AnthologyFindAquaticHabitat(u8 mapGroup, u8 mapNum,
                                                enum WildPokemonArea area,
                                                const struct WildPokemonInfo *info)
{
#if ALL_REGIONS
    u32 i, j, distinct = 0;
    enum Species species = SPECIES_NONE;
    u32 size;

    if (info == NULL || info->wildPokemon == NULL || info->encounterRate == 0
     || (area != WILD_AREA_WATER && area != WILD_AREA_FISHING))
        return SPECIES_NONE;
    for (i = 0; i < ARRAY_COUNT(sAnthologyAquaticHabitats); i++)
    {
        if (sAnthologyAquaticHabitats[i].mapGroup == mapGroup
         && sAnthologyAquaticHabitats[i].mapNum == mapNum
         && sAnthologyAquaticHabitats[i].area == area)
        {
            species = sAnthologyAquaticHabitats[i].species;
            break;
        }
    }
    if (!WorldIsNaturalDiscoverySpecies(species))
        return SPECIES_NONE;
    size = (area == WILD_AREA_WATER) ? NUM_WATER_MONS_ENCOUNTER_SLOTS : NUM_FISHING_MONS_ENCOUNTER_SLOTS;
    for (i = 0; i < size; i++)
    {
        if (info->wildPokemon[i].species == species)
            return SPECIES_NONE;
        if (area != WILD_AREA_WATER)
            continue;
        for (j = 0; j < i; j++)
        {
            if (info->wildPokemon[j].species == info->wildPokemon[i].species)
                break;
        }
        if (j == i)
            distinct++;
    }
    if (area == WILD_AREA_WATER && distinct >= NUM_WATER_MONS_ENCOUNTER_SLOTS)
        return SPECIES_NONE;
    return species;
#else
    return SPECIES_NONE;
#endif
}

enum Species AnthologyGetAquaticHabitatSpecies(enum WildPokemonArea area,
                                              const struct WildPokemonInfo *info)
{
#if ALL_REGIONS
    if (gSaveBlock1Ptr == NULL || gSaveBlock2Ptr == NULL
     || gSaveBlock2Ptr->randomizerWild || GetSafariZoneFlag() || InBattlePike()
     || CurrentBattlePyramidLocation() != PYRAMID_LOCATION_NONE
     || gMapHeader.battleType == MAP_BATTLE_SCENE_FRONTIER)
        return SPECIES_NONE;
    return AnthologyFindAquaticHabitat(gSaveBlock1Ptr->location.mapGroup,
                                      gSaveBlock1Ptr->location.mapNum, area, info);
#else
    return SPECIES_NONE;
#endif
}

bool8 AnthologyMapHasAquaticHabitatSpecies(u8 mapGroup, u8 mapNum, enum WildPokemonArea area,
                                          enum Species species, const struct WildPokemonInfo *info)
{
    return species != SPECIES_NONE
        && AnthologyFindAquaticHabitat(mapGroup, mapNum, area, info) == species;
}

static enum Species WorldChoosePostLeagueDiscoverySpecies(enum Species original)
{
#if ALL_REGIONS
    u32 battles, attempts;
    enum Species result;

    if (original == SPECIES_NONE || original == SPECIES_EGG
     || gSaveBlock1Ptr == NULL || gSaveBlock2Ptr == NULL
     || !FlagGet(FLAG_SYS_GAME_CLEAR) || gSaveBlock2Ptr->randomizerWild
     || GetSafariZoneFlag() || InBattlePike()
     || CurrentBattlePyramidLocation() != PYRAMID_LOCATION_NONE
     || gMapHeader.battleType == MAP_BATTLE_SCENE_FRONTIER)
        return original;

    // Count is incremented by BattleSetup_StartWildBattle after generation.
    // Every third qualifying battle uses a rotating ID. A long, ordinary
    // postgame encounter sequence reaches every eligible index in one cycle,
    // instead of hoping an RNG happened to select a rare species.
    battles = GetGameStat(GAME_STAT_WILD_BATTLES);
    if (battles % 3 != 2)
        return original;

    // A chosen Research target is a real wild encounter, not a gift.
    // Invalid or uninitialized old-save values safely fall back to rotation.
    result = GetGameStat(GAME_STAT_WORLD_DISCOVERY_TARGET);
    if (WorldIsPostLeagueResearchSpecies(result))
        return result;
    result = 1 + (battles / 3) % (NUM_SPECIES - 1);
    for (attempts = 0; attempts < NUM_SPECIES - 1; attempts++)
    {
        // Battle-only forms need their respective battle mechanics; never
        // materialize those as permanent caught Pokémon.
        if (WorldIsPostLeagueResearchSpecies(result))
            return result;
        if (++result >= NUM_SPECIES)
            result = 1;
    }
#endif
    return original;
}

void CreateWildMon(enum Species species, u8 level)
{
    ZeroEnemyPartyMons();
    u32 personality = GetMonPersonality(species, GetSynchronizedGender(WILDMON_ORIGIN, species), PickWildMonNature(species), RANDOM_UNOWN_LETTER);
    CreateMonWithIVs(&gParties[B_TRAINER_OPPONENT_A][0], species, level, personality, OTID_STRUCT_PLAYER_ID, USE_RANDOM_IVS);
    GiveMonInitialMoveset(&gParties[B_TRAINER_OPPONENT_A][0]);
#if ALL_REGIONS
    // The Royal Duo relics belong to their species, not an original spawn
    // location. Wild-randomized Zacian/Zamazenta retain their signature items.
    if (gSaveBlock2Ptr != NULL && gSaveBlock2Ptr->randomizerWild)
    {
        enum Item item = ITEM_NONE;
        if (species == SPECIES_ZACIAN_HERO)
            item = ITEM_RUSTED_SWORD;
        else if (species == SPECIES_ZAMAZENTA_HERO)
            item = ITEM_RUSTED_SHIELD;
        if (item != ITEM_NONE)
            SetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_HELD_ITEM, &item);
    }
#endif
}

#ifdef BUGFIX
#define TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildPokemon, type, ability, ptr, count) TryGetAbilityInfluencedWildMonIndex(wildPokemon, type, ability, ptr, count)
#else
#define TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildPokemon, type, ability, ptr, count) TryGetAbilityInfluencedWildMonIndex(wildPokemon, type, ability, ptr)
#endif

bool8 TryGenerateWildMon(const struct WildPokemonInfo *wildMonInfo, enum WildPokemonArea area, u8 flags)
{
    u8 wildMonIndex = 0;
    u8 level;

    switch (area)
    {
    case WILD_AREA_LAND:
        if (TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_STEEL, ABILITY_MAGNET_PULL, &wildMonIndex, NUM_LAND_MONS_ENCOUNTER_SLOTS))
            break;
        if (TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_STATIC, &wildMonIndex, NUM_LAND_MONS_ENCOUNTER_SLOTS))
            break;
        if (OW_LIGHTNING_ROD >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_LIGHTNING_ROD, &wildMonIndex, NUM_LAND_MONS_ENCOUNTER_SLOTS))
            break;
        if (OW_FLASH_FIRE >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_FIRE, ABILITY_FLASH_FIRE, &wildMonIndex, NUM_LAND_MONS_ENCOUNTER_SLOTS))
            break;
        if (OW_HARVEST >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_GRASS, ABILITY_HARVEST, &wildMonIndex, NUM_LAND_MONS_ENCOUNTER_SLOTS))
            break;
        if (OW_STORM_DRAIN >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_WATER, ABILITY_STORM_DRAIN, &wildMonIndex, NUM_LAND_MONS_ENCOUNTER_SLOTS))
            break;

        wildMonIndex = ChooseWildMonIndex_Land();
        break;
    case WILD_AREA_WATER:
        if (TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_STEEL, ABILITY_MAGNET_PULL, &wildMonIndex, NUM_WATER_MONS_ENCOUNTER_SLOTS))
            break;
        if (TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_STATIC, &wildMonIndex, NUM_WATER_MONS_ENCOUNTER_SLOTS))
            break;
        if (OW_LIGHTNING_ROD >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_ELECTRIC, ABILITY_LIGHTNING_ROD, &wildMonIndex, NUM_WATER_MONS_ENCOUNTER_SLOTS))
            break;
        if (OW_FLASH_FIRE >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_FIRE, ABILITY_FLASH_FIRE, &wildMonIndex, NUM_WATER_MONS_ENCOUNTER_SLOTS))
            break;
        if (OW_HARVEST >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_GRASS, ABILITY_HARVEST, &wildMonIndex, NUM_WATER_MONS_ENCOUNTER_SLOTS))
            break;
        if (OW_STORM_DRAIN >= GEN_8 && TRY_GET_ABILITY_INFLUENCED_WILD_MON_INDEX(wildMonInfo->wildPokemon, TYPE_WATER, ABILITY_STORM_DRAIN, &wildMonIndex, NUM_WATER_MONS_ENCOUNTER_SLOTS))
            break;

        wildMonIndex = ChooseWildMonIndex_Water();
        break;
    case WILD_AREA_ROCKS:
        wildMonIndex = ChooseWildMonIndex_Rocks();
        break;
    default:
    case WILD_AREA_FISHING:
    case WILD_AREA_HIDDEN:
        break;
    }

    // Roll the habitat before calculating level and repel protection.
    // The rare's advertised DexNav level comes from this map's first native
    // encounter slot; a random native slot would give a misleading level range.
    enum Species rare = AnthologyGetRareHabitatSpecies(area, wildMonInfo);
    bool8 useRare = rare != SPECIES_NONE && Random() % 100 < 2;
    enum Species extra = SPECIES_NONE;
    if (!useRare)
    {
        u32 habitatSlot;
        for (habitatSlot = 0; habitatSlot < ANTHOLOGY_EXTRA_HABITAT_SLOTS; habitatSlot++)
        {
            enum Species candidate = AnthologyGetExtraHabitatSpecies(area, wildMonInfo, habitatSlot);
            // Each supplement keeps its 2% roll; original species remain dominant.
            if (candidate != SPECIES_NONE && Random() % 100 < 2)
            {
                extra = candidate;
                break;
            }
        }
    }
    enum Species aquatic = AnthologyGetAquaticHabitatSpecies(area, wildMonInfo);
    bool8 useAquatic = aquatic != SPECIES_NONE && Random() % 100 < 3;
    level = ChooseWildMonLevel(wildMonInfo->wildPokemon, (useRare || extra != SPECIES_NONE || useAquatic) ? 0 : wildMonIndex, area);
    if (flags & WILD_CHECK_REPEL && !IsWildLevelAllowedByRepel(level))
        return FALSE;
    if (gMapHeader.mapLayoutId != LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_WILD_MONS && flags & WILD_CHECK_KEEN_EYE && !IsAbilityAllowingEncounter(level))
        return FALSE;

    enum Species wildSpecies = WorldRandomizeWildSpecies(wildMonInfo->wildPokemon[wildMonIndex].species, wildMonIndex, area);
    if (useRare)
        wildSpecies = rare;
    else if (extra != SPECIES_NONE)
        wildSpecies = extra;
    else if (useAquatic)
        wildSpecies = aquatic;
    CreateWildMon(WorldChoosePostLeagueDiscoverySpecies(wildSpecies), level);
    return TRUE;
}

static u16 GenerateFishingWildMon(const struct WildPokemonInfo *wildMonInfo, u8 rod)
{
    u8 wildMonIndex = ChooseWildMonIndex_Fishing(rod);
    enum Species wildMonSpecies = WorldChoosePostLeagueDiscoverySpecies(WorldRandomizeWildSpecies(wildMonInfo->wildPokemon[wildMonIndex].species, wildMonIndex, WILD_AREA_FISHING));
    u8 level = ChooseWildMonLevel(wildMonInfo->wildPokemon, wildMonIndex, WILD_AREA_FISHING);
    enum Species aquatic = AnthologyGetAquaticHabitatSpecies(WILD_AREA_FISHING, wildMonInfo);

    // Keep the selected rod's native level range, existing Feebas spot logic,
    // shiny-chain accounting and original bite mechanics unchanged.
    if (aquatic != SPECIES_NONE && Random() % 100 < 3)
        wildMonSpecies = aquatic;
    UpdateChainFishingStreak();
    CreateWildMon(wildMonSpecies, level);
    return wildMonSpecies;
}

bool8 SetUpMassOutbreakEncounter(u8 flags)
{
    u16 i;

    if (flags & WILD_CHECK_REPEL && !IsWildLevelAllowedByRepel(gSaveBlock1Ptr->outbreakPokemonLevel))
        return FALSE;

    CreateWildMon(gSaveBlock1Ptr->outbreakPokemonSpecies, gSaveBlock1Ptr->outbreakPokemonLevel);
    for (i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(&gParties[B_TRAINER_OPPONENT_A][0], gSaveBlock1Ptr->outbreakPokemonMoves[i], i);

    return TRUE;
}

bool8 DoMassOutbreakEncounterTest(void)
{
    if (gSaveBlock1Ptr->outbreakPokemonSpecies != SPECIES_NONE
     && gSaveBlock1Ptr->location.mapNum == gSaveBlock1Ptr->outbreakLocationMapNum
     && gSaveBlock1Ptr->location.mapGroup == gSaveBlock1Ptr->outbreakLocationMapGroup)
    {
        if (Random() % 100 < gSaveBlock1Ptr->outbreakPokemonProbability)
            return TRUE;
    }
    return FALSE;
}

static bool8 EncounterOddsCheck(u16 encounterRate)
{
    if (Random() % MAX_ENCOUNTER_RATE < encounterRate)
        return TRUE;
    else
        return FALSE;
}

// Returns true if it will try to create a wild encounter.
static bool8 WildEncounterCheck(u32 encounterRate, bool8 ignoreAbility)
{
    encounterRate *= 16;
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
        encounterRate = encounterRate * 80 / 100;
    ApplyFluteEncounterRateMod(&encounterRate);
    ApplyCleanseTagEncounterRateMod(&encounterRate);
    if (LURE_STEP_COUNT != 0)
        encounterRate *= 2;
    if (!ignoreAbility && !GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SANITY_IS_EGG))
    {
        enum Ability ability = GetMonAbility(&gParties[B_TRAINER_PLAYER][0]);

        if (ability == ABILITY_STENCH && gMapHeader.mapLayoutId == LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR)
            encounterRate = encounterRate * 3 / 4;
        else if (ability == ABILITY_STENCH)
            encounterRate /= 2;
        else if (ability == ABILITY_ILLUMINATE)
            encounterRate *= 2;
        else if (ability == ABILITY_WHITE_SMOKE)
            encounterRate /= 2;
        else if (ability == ABILITY_ARENA_TRAP)
            encounterRate *= 2;
        else if (ability == ABILITY_SAND_VEIL && gSaveBlock1Ptr->weather == WEATHER_SANDSTORM)
            encounterRate /= 2;
        else if (ability == ABILITY_SNOW_CLOAK && gSaveBlock1Ptr->weather == WEATHER_SNOW)
            encounterRate /= 2;
        else if (ability == ABILITY_QUICK_FEET)
            encounterRate /= 2;
        else if (ability == ABILITY_INFILTRATOR && OW_INFILTRATOR >= GEN_8)
            encounterRate /= 2;
        else if (ability == ABILITY_NO_GUARD)
            encounterRate *= 2;
    }
    if (encounterRate > MAX_ENCOUNTER_RATE)
        encounterRate = MAX_ENCOUNTER_RATE;
    return EncounterOddsCheck(encounterRate);
}

// When you first step on a different type of metatile, there's a 40% chance it
// skips the wild encounter check entirely.
static bool8 AllowWildCheckOnNewMetatile(void)
{
    if (Random() % 100 >= 60)
        return FALSE;
    else
        return TRUE;
}

bool8 AreLegendariesInSootopolisPreventingEncounters(void)
{
    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_SOOTOPOLIS_CITY)
     || gSaveBlock1Ptr->location.mapNum != MAP_NUM(MAP_SOOTOPOLIS_CITY))
    {
        return FALSE;
    }

    return FlagGet(FLAG_LEGENDARIES_IN_SOOTOPOLIS);
}

bool8 StandardWildEncounter(u16 curMetatileBehavior, u16 prevMetatileBehavior)
{
    u32 headerId;
    enum TimeOfDay timeOfDay;
    struct Roamer *roamer;

    if (sWildEncountersDisabled == TRUE)
        return FALSE;

    // Defensive: a debug-cleared or otherwise empty party cannot start a wild battle
    // with a SPECIES_NONE lead. Suppress encounters until a mon is available.
    if (gPlayerPartyCount == 0)
        return FALSE;

    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
    {
        if (gMapHeader.mapLayoutId == LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_WILD_MONS)
        {
            headerId = GetBattlePikeWildMonHeaderId();
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (prevMetatileBehavior != curMetatileBehavior && !AllowWildCheckOnNewMetatile())
                return FALSE;
            else if (WildEncounterCheck(gBattlePikeWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo->encounterRate, FALSE) != TRUE)
                return FALSE;
            else if (TryGenerateWildMon(gBattlePikeWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, WILD_CHECK_KEEN_EYE) != TRUE)
                return FALSE;
            else if (!TryGenerateBattlePikeWildMon(TRUE))
                return FALSE;

            BattleSetup_StartBattlePikeWildBattle();
            return TRUE;
        }
        if (gMapHeader.mapLayoutId == LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR)
        {
            headerId = gSaveBlock2Ptr->frontier.curChallengeBattleNum;
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (prevMetatileBehavior != curMetatileBehavior && !AllowWildCheckOnNewMetatile())
                return FALSE;
            else if (WildEncounterCheck(gBattlePyramidWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo->encounterRate, FALSE) != TRUE)
                return FALSE;
            else if (TryGenerateWildMon(gBattlePyramidWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, WILD_CHECK_KEEN_EYE) != TRUE)
                return FALSE;

            GenerateBattlePyramidWildMon(SPECIES_NONE);
            BattleSetup_StartWildBattle();
            return TRUE;
        }
    }
    else
    {
        if (MetatileBehavior_IsLandWildEncounter(curMetatileBehavior) == TRUE)
        {
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo == NULL)
                return FALSE;
            else if (prevMetatileBehavior != curMetatileBehavior && !AllowWildCheckOnNewMetatile())
                return FALSE;
            else if (WildEncounterCheck(gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo->encounterRate, FALSE) != TRUE)
                return FALSE;

            if (TryStartRoamerEncounter())
            {
                roamer = &gSaveBlock1Ptr->roamer[gEncounteredRoamerIndex];
                if (!IsWildLevelAllowedByRepel(roamer->level))
                    return FALSE;

                BattleSetup_StartRoamerBattle();
                return TRUE;
            }
            else
            {
                if (DoMassOutbreakEncounterTest() == TRUE && SetUpMassOutbreakEncounter(WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
                {
                    BattleSetup_StartWildBattle();
                    return TRUE;
                }

                // try a regular wild land encounter
                if (TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
                {
                    if (TryDoDoubleWildBattle())
                    {
                        struct Pokemon mon1 = gParties[B_TRAINER_OPPONENT_A][0];
                        TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, WILD_CHECK_KEEN_EYE);
                        gParties[B_TRAINER_OPPONENT_A][1] = mon1;
                        BattleSetup_StartDoubleWildBattle();
                    }
                    else
                    {
                        BattleSetup_StartWildBattle();
                    }
                    return TRUE;
                }

                return FALSE;
            }
        }
        else if (MetatileBehavior_IsWaterWildEncounter(curMetatileBehavior) == TRUE
                 || (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING) && MetatileBehavior_IsBridgeOverWater(curMetatileBehavior) == TRUE))
        {
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_WATER);

            if (AreLegendariesInSootopolisPreventingEncounters() == TRUE)
                return FALSE;
            else if (gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo == NULL)
                return FALSE;
            else if (prevMetatileBehavior != curMetatileBehavior && !AllowWildCheckOnNewMetatile())
                return FALSE;
            else if (WildEncounterCheck(gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo->encounterRate, FALSE) != TRUE)
                return FALSE;

            if (TryStartRoamerEncounter())
            {
                roamer = &gSaveBlock1Ptr->roamer[gEncounteredRoamerIndex];
                if (!IsWildLevelAllowedByRepel(roamer->level))
                    return FALSE;

                BattleSetup_StartRoamerBattle();
                return TRUE;
            }
            else // try a regular surfing encounter
            {
                if (TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo, WILD_AREA_WATER, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
                {
                    gIsSurfingEncounter = TRUE;
                    if (TryDoDoubleWildBattle())
                    {
                        struct Pokemon mon1 = gParties[B_TRAINER_OPPONENT_A][0];
                        TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo, WILD_AREA_WATER, WILD_CHECK_KEEN_EYE);
                        gParties[B_TRAINER_OPPONENT_A][1] = mon1;
                        BattleSetup_StartDoubleWildBattle();
                    }
                    else
                    {
                        BattleSetup_StartWildBattle();
                    }
                    return TRUE;
                }

                return FALSE;
            }
        }
    }

    return FALSE;
}

// Optional excavation alongside Rock Smash battles. These are normal Bag items,
// not story flags: multiple fragments can be gathered and revived independently.
// Keep the original Rock Smash encounter and Rusturf Tunnel puzzle paths intact.
#if ALL_REGIONS
static const enum Item sAnthologyExcavationFossils[] =
{
    ITEM_ARMOR_FOSSIL,
    ITEM_SKULL_FOSSIL,
    ITEM_COVER_FOSSIL,
    ITEM_PLUME_FOSSIL,
    ITEM_JAW_FOSSIL,
    ITEM_SAIL_FOSSIL,
    ITEM_FOSSILIZED_BIRD,
    ITEM_FOSSILIZED_FISH,
    ITEM_FOSSILIZED_DRAKE,
    ITEM_FOSSILIZED_DINO,
};
#endif

void RockSmashWildEncounter(void)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    enum TimeOfDay timeOfDay;

    // VAR_0x8004 was used for the follower move before this point; reuse it
    // only after the obstacle has been cleared, as an optional fossil output.
    gSpecialVar_0x8004 = ITEM_NONE;

    if (headerId != HEADER_NONE)
    {
        timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_ROCKS);

        const struct WildPokemonInfo *wildPokemonInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].rockSmashMonsInfo;

        if (wildPokemonInfo == NULL)
        {
            gSpecialVar_Result = FALSE;
        }
        else if (WildEncounterCheck(wildPokemonInfo->encounterRate, TRUE) == TRUE
         && TryGenerateWildMon(wildPokemonInfo, WILD_AREA_ROCKS, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE) == TRUE)
        {
            if (TryDoDoubleWildBattle())
            {
                struct Pokemon mon1 = gParties[B_TRAINER_OPPONENT_A][0];
                TryGenerateWildMon(wildPokemonInfo, WILD_AREA_ROCKS, WILD_CHECK_REPEL | WILD_CHECK_KEEN_EYE);
                gParties[B_TRAINER_OPPONENT_A][1] = mon1;
                BattleSetup_StartDoubleWildBattle();
                gSpecialVar_Result = TRUE;
            }
            else {
                BattleSetup_StartWildBattle();
                gSpecialVar_Result = TRUE;
            }
        }
        else
        {
            gSpecialVar_Result = FALSE;
        }
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }

#if ALL_REGIONS
    if (!gSpecialVar_Result && gSaveBlock1Ptr != NULL && gSaveBlock2Ptr != NULL
     && !GetSafariZoneFlag() && !InBattlePike()
     && CurrentBattlePyramidLocation() == PYRAMID_LOCATION_NONE
     && gMapHeader.battleType != MAP_BATTLE_SCENE_FRONTIER
     && Random() % 100 < 12)
    {
        enum Item fossil = sAnthologyExcavationFossils[Random() % ARRAY_COUNT(sAnthologyExcavationFossils)];
        if (AddBagItem(fossil, 1))
            gSpecialVar_0x8004 = fossil;
    }
#endif
}

bool8 SweetScentWildEncounter(void)
{
    s16 x, y;
    u32 headerId;
    enum TimeOfDay timeOfDay;

    PlayerGetDestCoords(&x, &y);
    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
    {
        if (gMapHeader.mapLayoutId == LAYOUT_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_WILD_MONS)
        {
            headerId = GetBattlePikeWildMonHeaderId();
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (TryGenerateWildMon(gBattlePikeWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, 0) != TRUE)
                return FALSE;

            TryGenerateBattlePikeWildMon(FALSE);
            BattleSetup_StartBattlePikeWildBattle();
            return TRUE;
        }
        if (gMapHeader.mapLayoutId == LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR)
        {
            headerId = gSaveBlock2Ptr->frontier.curChallengeBattleNum;
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (TryGenerateWildMon(gBattlePyramidWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, 0) != TRUE)
                return FALSE;

            GenerateBattlePyramidWildMon(SPECIES_NONE);
            BattleSetup_StartWildBattle();
            return TRUE;
        }
    }
    else
    {
        if (MetatileBehavior_IsLandWildEncounter(MapGridGetMetatileBehaviorAt(x, y)) == TRUE)
        {
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

            if (gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo == NULL)
                return FALSE;

            if (TryStartRoamerEncounter())
            {
                BattleSetup_StartRoamerBattle();
                return TRUE;
            }

            if (DoMassOutbreakEncounterTest() == TRUE)
                SetUpMassOutbreakEncounter(0);
            else
                TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, 0);

            BattleSetup_StartWildBattle();
            return TRUE;
        }
        else if (MetatileBehavior_IsWaterWildEncounter(MapGridGetMetatileBehaviorAt(x, y)) == TRUE)
        {
            timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_WATER);

            if (AreLegendariesInSootopolisPreventingEncounters() == TRUE)
                return FALSE;
            if (gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo == NULL)
                return FALSE;

            if (TryStartRoamerEncounter())
            {
                BattleSetup_StartRoamerBattle();
                return TRUE;
            }

            TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo, WILD_AREA_WATER, 0);
            BattleSetup_StartWildBattle();
            return TRUE;
        }
    }

    return FALSE;
}

bool8 DoesCurrentMapHaveFishingMons(void)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    enum TimeOfDay timeOfDay;

    // HEADER_NONE is an *expected* input here — detecting the no-table case is what this function
    // is for — and GetTimeOfDayForEncounters() indexes gWildMonHeaders[headerId], so the guard has
    // to come before that call, not after it. The old `headerId != HEADER_NONE && gWildMonHeaders[
    // headerId]...` test was fine on its own (&& short-circuits, so the index was never reached and
    // the returned bool was always correct); what it could not protect was the call in the
    // initializer above it. gWildMonHeaders[0xFFFF] is +5.25 MB, which still lands inside the 32 MB
    // cartridge image, so it silently returns arbitrary ROM rather than faulting. Maps with fishable
    // water but no encounter header (both harbors, Battle Frontier Outside West/East, Aqua Hideout
    // 1F) reach here every time the player casts a rod.
    if (headerId == HEADER_NONE)
        return FALSE;

    timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_FISHING);

    if (gWildMonHeaders[headerId].encounterTypes[timeOfDay].fishingMonsInfo != NULL)
        return TRUE;
    else
        return FALSE;
}

void FishingWildEncounter(u8 rod)
{
    enum Species species;
    u32 headerId;
    s16 x, y;
    enum TimeOfDay timeOfDay;

    gIsFishingEncounter = TRUE;
    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    if (CheckFeebasAtCoords(x, y) == TRUE)
    {
        u8 level = ChooseWildMonLevel(&gWildFeebas, 0, WILD_AREA_FISHING);

        species = gWildFeebas.species;
        CreateWildMon(species, level);
    }
    else
    {
        headerId = GetCurrentMapWildMonHeaderId();
        timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_FISHING);
        species = GenerateFishingWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].fishingMonsInfo, rod);
    }

    IncrementGameStat(GAME_STAT_FISHING_ENCOUNTERS);
    SetPokemonAnglerSpecies(species);
    BattleSetup_StartWildBattle();
}

u16 GetLocalWildMon(bool8 *isWaterMon)
{
    u32 headerId;
    enum TimeOfDay timeOfDay;
    const struct WildPokemonInfo *landMonsInfo;
    const struct WildPokemonInfo *waterMonsInfo;

    *isWaterMon = FALSE;
    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == HEADER_NONE)
        return SPECIES_NONE;

    timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);
    landMonsInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo;

    timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_WATER);
    waterMonsInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo;

    // Neither
    if (landMonsInfo == NULL && waterMonsInfo == NULL)
        return SPECIES_NONE;
    // Land Pokémon
    else if (landMonsInfo != NULL && waterMonsInfo == NULL)
        return landMonsInfo->wildPokemon[ChooseWildMonIndex_Land()].species;
    // Water Pokémon
    else if (landMonsInfo == NULL && waterMonsInfo != NULL)
    {
        *isWaterMon = TRUE;
        return waterMonsInfo->wildPokemon[ChooseWildMonIndex_Water()].species;
    }
    // Either land or water Pokémon
    if ((Random() % 100) < 80)
    {
        return landMonsInfo->wildPokemon[ChooseWildMonIndex_Land()].species;
    }
    else
    {
        *isWaterMon = TRUE;
        return waterMonsInfo->wildPokemon[ChooseWildMonIndex_Water()].species;
    }
}

u16 GetLocalWaterMon(void)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    enum TimeOfDay timeOfDay;

    if (headerId != HEADER_NONE)
    {
        timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_WATER);

        const struct WildPokemonInfo *waterMonsInfo = gWildMonHeaders[headerId].encounterTypes[timeOfDay].waterMonsInfo;

        if (waterMonsInfo)
            return waterMonsInfo->wildPokemon[ChooseWildMonIndex_Water()].species;
    }
    return SPECIES_NONE;
}

bool8 UpdateRepelCounter(void)
{
    u16 repelLureVar = VarGet(VAR_REPEL_STEP_COUNT);
    u16 steps = REPEL_LURE_STEPS(repelLureVar);
    bool32 isLure = IS_LAST_USED_LURE(repelLureVar);

    if (InBattlePike() || CurrentBattlePyramidLocation() != PYRAMID_LOCATION_NONE)
        return FALSE;
    if (InUnionRoom() == TRUE)
        return FALSE;

    if (steps != 0)
    {
        steps--;
        if (!isLure)
        {
            VarSet(VAR_REPEL_STEP_COUNT, steps);
            if (steps == 0)
            {
                ScriptContext_SetupScript(EventScript_SprayWoreOff);
                return TRUE;
            }
        }
        else
        {
            VarSet(VAR_REPEL_STEP_COUNT, steps | REPEL_LURE_MASK);
            if (steps == 0)
            {
                ScriptContext_SetupScript(EventScript_SprayWoreOff);
                return TRUE;
            }
        }

    }
    return FALSE;
}

bool8 IsWildLevelAllowedByRepel(u8 wildLevel)
{
    u8 i;

    if (!REPEL_STEP_COUNT)
        return TRUE;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (I_REPEL_INCLUDE_FAINTED == GEN_1 || I_REPEL_INCLUDE_FAINTED >= GEN_6 || GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_HP))
        {
            if (!GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_IS_EGG))
                return wildLevel >= GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL);
        }
    }

    return FALSE;
}

bool8 IsAbilityAllowingEncounter(u8 level)
{
    enum Ability ability;

    if (GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SANITY_IS_EGG))
        return TRUE;

    ability = GetMonAbility(&gParties[B_TRAINER_PLAYER][0]);
    if (ability == ABILITY_KEEN_EYE || ability == ABILITY_INTIMIDATE)
    {
        u8 playerMonLevel = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL);
        if (playerMonLevel > 5 && level <= playerMonLevel - 5 && !(Random() % 2))
            return FALSE;
    }

    return TRUE;
}

static bool8 TryGetRandomWildMonIndexByType(const struct WildPokemon *wildMon, enum Type type, u8 numMon, u8 *monIndex)
{
    u8 validIndexes[numMon]; // variable length array, an interesting feature
    u8 i, validMonCount;

    for (i = 0; i < numMon; i++)
        validIndexes[i] = 0;

    for (validMonCount = 0, i = 0; i < numMon; i++)
    {
        if (GetSpeciesType(wildMon[i].species, 0) == type || GetSpeciesType(wildMon[i].species, 1) == type)
            validIndexes[validMonCount++] = i;
    }

    if (validMonCount == 0 || validMonCount == numMon)
        return FALSE;

    *monIndex = validIndexes[Random() % validMonCount];
    return TRUE;
}

#include "data.h"

static u8 GetMaxLevelOfSpeciesInWildTable(const struct WildPokemon *wildMon, enum Species species, enum WildPokemonArea area)
{
    u8 i, maxLevel = 0, numMon = 0;

    switch (area)
    {
    case WILD_AREA_LAND:
        numMon = NUM_LAND_MONS_ENCOUNTER_SLOTS;
        break;
    case WILD_AREA_WATER:
        numMon = NUM_WATER_MONS_ENCOUNTER_SLOTS;
        break;
    case WILD_AREA_ROCKS:
        numMon = NUM_ROCK_SMASH_MONS_ENCOUNTER_SLOTS;
        break;
    default:
    case WILD_AREA_FISHING:
    case WILD_AREA_HIDDEN:
        break;
    }

    for (i = 0; i < numMon; i++)
    {
        if (wildMon[i].species == species && wildMon[i].maxLevel > maxLevel)
            maxLevel = wildMon[i].maxLevel;
    }

    return maxLevel;
}

#ifdef BUGFIX
static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon *wildMon, enum Type type, enum Ability ability, u8 *monIndex, u32 size)
#else
static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon *wildMon, enum Type type, enum Ability ability, u8 *monIndex)
#endif
{
    if (GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SANITY_IS_EGG))
        return FALSE;
    else if (GetMonAbility(&gParties[B_TRAINER_PLAYER][0]) != ability)
        return FALSE;
    else if (Random() % 2 != 0)
        return FALSE;

#ifdef BUGFIX
    return TryGetRandomWildMonIndexByType(wildMon, type, size, monIndex);
#else
    return TryGetRandomWildMonIndexByType(wildMon, type, NUM_LAND_MONS_ENCOUNTER_SLOTS, monIndex);
#endif
}

static void ApplyFluteEncounterRateMod(u32 *encRate)
{
    if (FlagGet(FLAG_SYS_ENC_UP_ITEM) == TRUE)
        *encRate += *encRate / 2;
    else if (FlagGet(FLAG_SYS_ENC_DOWN_ITEM) == TRUE)
        *encRate = *encRate / 2;
}

static void ApplyCleanseTagEncounterRateMod(u32 *encRate)
{
    enum Item heldItem = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HELD_ITEM);
    if (gItemsInfo[heldItem].holdEffect == HOLD_EFFECT_REPEL)
        *encRate = *encRate * 2 / 3;
}

bool8 TryDoDoubleWildBattle(void)
{
    if (GetSafariZoneFlag()
      || (WE_DOUBLE_WILD_REQUIRE_2_MONS && GetMonsStateToDoubles() != PLAYER_HAS_TWO_USABLE_MONS))
        return FALSE;
    if (FollowerNPCIsBattlePartner() && FNPC_FLAG_PARTNER_WILD_BATTLES != 0
     && (FNPC_FLAG_PARTNER_WILD_BATTLES == FNPC_ALWAYS || FlagGet(FNPC_FLAG_PARTNER_WILD_BATTLES)) && FNPC_NPC_FOLLOWER_WILD_BATTLE_VS_2 == TRUE)
        return TRUE;
    else if (FlagGet(WE_FLAG_FORCE_DOUBLE_WILD))
        return TRUE;
    else if (RandomPercentage(RNG_NONE, WE_DOUBLE_WILD_CHANCE))
        return TRUE;
    return FALSE;
}

bool8 StandardWildEncounter_Debug(void)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();
    enum TimeOfDay timeOfDay;

    // Same shape as DoesCurrentMapHaveFishingMons(): guard before indexing gWildMonHeaders[].
    if (headerId == HEADER_NONE)
        return FALSE;

    timeOfDay = GetTimeOfDayForEncounters(headerId, WILD_AREA_LAND);

    if (TryGenerateWildMon(gWildMonHeaders[headerId].encounterTypes[timeOfDay].landMonsInfo, WILD_AREA_LAND, 0) != TRUE)
        return FALSE;

    DoStandardWildBattle_Debug();
    return TRUE;
}

u32 ChooseHiddenMonIndex(void)
{
    #ifdef ENCOUNTER_CHANCE_HIDDEN_MONS_TOTAL
        u8 rand = Random() % ENCOUNTER_CHANCE_HIDDEN_MONS_TOTAL;

        if (rand < ENCOUNTER_CHANCE_HIDDEN_MONS_SLOT_0)
            return 0;
        else if (rand >= ENCOUNTER_CHANCE_HIDDEN_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_HIDDEN_MONS_SLOT_1)
            return 1;
        else
            return 2;
    #else
        return 0xFF;
    #endif
}

bool32 MapHasNoEncounterData(void)
{
    return (GetCurrentMapWildMonHeaderId() == HEADER_NONE);
}
