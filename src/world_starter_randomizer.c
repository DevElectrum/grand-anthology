#include "global.h"
#include "pokemon.h"
#include "world_starter_randomizer.h"
#include "constants/maps.h"

// The ordering here must match sStarterMon in src/starter_choose.c. A
// randomized choice retains its original position for story/rival branches.
#if ALL_REGIONS
static const enum Species sKantoStarterSlots[] =
{
    SPECIES_BULBASAUR, SPECIES_SQUIRTLE, SPECIES_CHARMANDER,
    SPECIES_TURTWIG, SPECIES_PIPLUP, SPECIES_CHIMCHAR,
    SPECIES_ROWLET, SPECIES_POPPLIO, SPECIES_LITTEN, SPECIES_POIPOLE,
};

static const enum Species sJohtoStarterSlots[] =
{
    SPECIES_CHIKORITA, SPECIES_CYNDAQUIL, SPECIES_TOTODILE,
    SPECIES_SNIVY, SPECIES_TEPIG, SPECIES_OSHAWOTT,
    SPECIES_GROOKEY, SPECIES_SCORBUNNY, SPECIES_SOBBLE,
};

static const enum Species sHoennStarterSlots[] =
{
    SPECIES_TREECKO, SPECIES_TORCHIC, SPECIES_MUDKIP,
    SPECIES_CHESPIN, SPECIES_FENNEKIN, SPECIES_FROAKIE,
    SPECIES_SPRIGATITO, SPECIES_FUECOCO, SPECIES_QUAXLY,
};

static bool32 WorldIsValidRandomizedStarter(enum Species species)
{
    return species != SPECIES_EGG && IsSpeciesEnabled(species)
        && !gSpeciesInfo[species].isMegaEvolution
        && !gSpeciesInfo[species].isPrimalReversion
        && !gSpeciesInfo[species].isUltraBurst
        && !gSpeciesInfo[species].isGigantamax
        && !gSpeciesInfo[species].isTeraForm
        && !gSpeciesInfo[species].isTotem;
}

// Preserve the original seeded hash and candidate scan. Reject Pokémon
// already used by earlier slots by National Pokédex number, not just species
// enum ID. Unown letters and other forms have different IDs but represent
// the same species in the menu.
static enum Species WorldPickUniqueStarter(enum Species original, u16 map,
                                           const enum Species *previous, u32 previousCount)
{
    u32 hash = 0x9E3779B9u ^ (u32)gSaveBlock2Ptr->randomizerSeed;
    u32 attempts;
    enum Species result;

    hash ^= (u32)original * 0x85EBCA6Bu;
    hash ^= ((u32)map << 8) | 0x5300u;
    hash ^= hash >> 16;
    hash *= 0x7FEB352Du;
    hash ^= hash >> 15;
    hash *= 0x846CA68Bu;
    hash ^= hash >> 16;
    result = 1 + hash % (NUM_SPECIES - 1);
    for (attempts = 0; attempts < NUM_SPECIES - 1; attempts++)
    {
        if (WorldIsValidRandomizedStarter(result))
        {
            u32 i;
            for (i = 0; i < previousCount; i++)
                if (gSpeciesInfo[previous[i]].natDexNum == gSpeciesInfo[result].natDexNum)
                    break;
            if (i == previousCount)
                return result;
        }
        if (++result >= NUM_SPECIES)
            result = 1;
    }
    return original;
}
#endif

// Resolve slots in canonical menu order. Previews, player awards and Kanto
// Blue's out-of-map starter lookup must share one seed-specific roster.
static enum Species WorldRandomizeStarterSpeciesAtMap(enum Species original, u16 map)
{
#if ALL_REGIONS
    const enum Species *slots;
    u32 slotCount;
    u32 i;
    enum Species selected[ARRAY_COUNT(sKantoStarterSlots)];

    if (gSaveBlock2Ptr == NULL || !gSaveBlock2Ptr->randomizerStarters)
        return original;

    switch (map)
    {
    case MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB:
        slots = sKantoStarterSlots;
        slotCount = ARRAY_COUNT(sKantoStarterSlots);
        break;
    case MAP_NEW_BARK_TOWN_LAB:
        slots = sJohtoStarterSlots;
        slotCount = ARRAY_COUNT(sJohtoStarterSlots);
        break;
    case MAP_ROUTE101:
        slots = sHoennStarterSlots;
        slotCount = ARRAY_COUNT(sHoennStarterSlots);
        break;
    default:
        return original;
    }

    for (i = 0; i < slotCount; i++)
    {
        selected[i] = WorldPickUniqueStarter(slots[i], map, selected, i);
        if (slots[i] == original)
            return selected[i];
    }
#endif
    return original;
}

// Rival teams are generated outside Oak's lab. Use the exact same Kanto
// starter slot mapping without reading the player's current map.
enum Species WorldRandomizeKantoStarterSpecies(enum Species original)
{
    return WorldRandomizeStarterSpeciesAtMap(original, MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB);
}

enum Species WorldRandomizeStarterSpecies(enum Species original)
{
    u16 map;
    if (gSaveBlock1Ptr == NULL)
        return original;
    map = ((u16)(u8)gSaveBlock1Ptr->location.mapGroup << 8)
        | (u8)gSaveBlock1Ptr->location.mapNum;
    return WorldRandomizeStarterSpeciesAtMap(original, map);
}
