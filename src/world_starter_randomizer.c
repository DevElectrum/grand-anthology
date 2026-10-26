#include "global.h"
#include "pokemon.h"
#include "world_starter_randomizer.h"
#include "constants/maps.h"

// Preserve original starter scripts, rival choices and story progress. Each
// canonical choice has one stable species result per seed and map, independent
// of RNG frame order. This function is used ONLY when granting a starter.
//
// Safety: do not change any other scripted Pokémon, such as optional gift
// starters, scripted legendaries, trainer Pokémon, eggs or sidequest rewards.
static enum Species WorldRandomizeStarterSpeciesAtMap(enum Species original, u16 map)
{
#if ALL_REGIONS
    u32 hash;
    u32 attempts;
    enum Species result;

    if (gSaveBlock2Ptr == NULL || !gSaveBlock2Ptr->randomizerStarters)
        return original;

    switch (map)
    {
    case MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB:
        if (original != SPECIES_BULBASAUR && original != SPECIES_SQUIRTLE
         && original != SPECIES_CHARMANDER && original != SPECIES_TURTWIG
         && original != SPECIES_PIPLUP && original != SPECIES_CHIMCHAR
         && original != SPECIES_ROWLET && original != SPECIES_POPPLIO
         && original != SPECIES_LITTEN && original != SPECIES_POIPOLE)
            return original;
        break;
    case MAP_NEW_BARK_TOWN_LAB:
        if (original != SPECIES_CHIKORITA && original != SPECIES_CYNDAQUIL
         && original != SPECIES_TOTODILE && original != SPECIES_SNIVY
         && original != SPECIES_TEPIG && original != SPECIES_OSHAWOTT
         && original != SPECIES_GROOKEY && original != SPECIES_SCORBUNNY
         && original != SPECIES_SOBBLE)
            return original;
        break;
    case MAP_ROUTE101:
        if (original != SPECIES_TREECKO
         && original != SPECIES_TORCHIC
         && original != SPECIES_MUDKIP
         && original != SPECIES_CHESPIN
         && original != SPECIES_FENNEKIN
         && original != SPECIES_FROAKIE
         && original != SPECIES_SPRIGATITO
         && original != SPECIES_FUECOCO
         && original != SPECIES_QUAXLY)
            return original;
        break;
    default:
        return original;
    }

    hash = 0x9E3779B9u ^ (u32)gSaveBlock2Ptr->randomizerSeed;
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
        if (result != SPECIES_EGG && IsSpeciesEnabled(result)
         && !gSpeciesInfo[result].isMegaEvolution
         && !gSpeciesInfo[result].isPrimalReversion
         && !gSpeciesInfo[result].isUltraBurst
         && !gSpeciesInfo[result].isGigantamax
         && !gSpeciesInfo[result].isTeraForm
         && !gSpeciesInfo[result].isTotem)
            return result;
        if (++result >= NUM_SPECIES)
            result = 1;
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
