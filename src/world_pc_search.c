#include "global.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "world_pc_search.h"

// P0 non-destructive global storage search. Capacity is bounded by the actual
// save-file box layout. The index is rebuilt from valid stored Pokémon rather
// than saved, so there is no cache to become stale after deposits, evolutions,
// withdrawals, trading, or traveling between the original three regions.
static s32 WorldPcCompare(const struct WorldPcSearchResult *a,
                          const struct WorldPcSearchResult *b,
                          enum WorldPcSortMode mode)
{
    s32 diff = 0;

    if (mode == WORLD_PC_SORT_SPECIES)
        diff = (s32)a->species - b->species;
    else if (mode == WORLD_PC_SORT_LEVEL)
        diff = (s32)b->level - a->level; // highest level first
    else if (mode == WORLD_PC_SORT_ITEM)
        diff = (s32)a->heldItem - b->heldItem;

    // Exact original box/slot position is the stable tie-breaker.
    if (diff == 0)
        diff = (s32)a->boxId - b->boxId;
    if (diff == 0)
        diff = (s32)a->boxSlot - b->boxSlot;
    return diff;
}

// Heap sorting keeps RAM O(1) and avoids GBA libc qsort/stack recursion.
// No mutation of the save data: only caller-owned search-result references.
static void WorldPcSiftDown(struct WorldPcSearchResult *a, u16 root, u16 count,
                            enum WorldPcSortMode mode)
{
    while ((u32)root * 2 + 1 < count)
    {
        u16 child = (u16)((u32)root * 2 + 1);
        u16 candidate = root;
        struct WorldPcSearchResult temp;

        if (WorldPcCompare(&a[candidate], &a[child], mode) < 0)
            candidate = child;
        if (child + 1 < count && WorldPcCompare(&a[candidate], &a[child + 1], mode) < 0)
            candidate = child + 1;
        if (candidate == root)
            break;
        temp = a[root];
        a[root] = a[candidate];
        a[candidate] = temp;
        root = candidate;
    }
}

static void WorldPcSortResults(struct WorldPcSearchResult *results, u16 count,
                               enum WorldPcSortMode mode)
{
    u16 i;
    if (mode == WORLD_PC_SORT_LOCATION || count < 2)
        return;

    for (i = count / 2; i > 0; i--)
        WorldPcSiftDown(results, i - 1, count, mode);

    for (i = count - 1; i > 0; i--)
    {
        struct WorldPcSearchResult temp = results[0];
        results[0] = results[i];
        results[i] = temp;
        WorldPcSiftDown(results, 0, i, mode);
    }
}

u16 WorldPcSearch(const struct WorldPcSearchFilter *filter,
                  enum WorldPcSortMode sortMode,
                  struct WorldPcSearchResult *out, u16 capacity)
{
    u16 total = 0;
    u16 stored = 0;
    u8 box, slot;

    if (filter == NULL || gPokemonStoragePtr == NULL)
        return 0;
    if (out == NULL)
        capacity = 0;
    if (capacity > TOTAL_BOXES_COUNT * IN_BOX_COUNT)
        capacity = TOTAL_BOXES_COUNT * IN_BOX_COUNT;

    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (slot = 0; slot < IN_BOX_COUNT; slot++)
        {
            struct BoxPokemon *mon = GetBoxedMonPtr(box, slot);
            enum Species species;
            bool8 egg, shiny;
            u8 level;
            enum Item item;

            if (mon == NULL
             || !GetBoxMonData(mon, MON_DATA_SANITY_HAS_SPECIES)
             || GetBoxMonData(mon, MON_DATA_SANITY_IS_BAD_EGG))
                continue;
            egg = GetBoxMonData(mon, MON_DATA_SANITY_IS_EGG);
            if (egg && !filter->includeEggs)
                continue;
            species = GetBoxMonData(mon, MON_DATA_SPECIES);
            if (filter->species != SPECIES_NONE && species != filter->species)
                continue;
            item = GetBoxMonData(mon, MON_DATA_HELD_ITEM);
            if (filter->heldItem != ITEM_NONE && item != filter->heldItem)
                continue;
            shiny = GetBoxMonData(mon, MON_DATA_IS_SHINY);
            if (filter->shinyOnly && !shiny)
                continue;
            level = egg ? 0 : GetLevelFromBoxMonExp(mon);
            if (filter->minLevel != 0 && level < filter->minLevel)
                continue;
            if (filter->maxLevel != 0 && level > filter->maxLevel)
                continue;

            total++;
            if (stored < capacity)
            {
                out[stored].species = species;
                out[stored].heldItem = item;
                out[stored].boxId = box;
                out[stored].boxSlot = slot;
                out[stored].level = level;
                out[stored].shiny = shiny;
                stored++;
            }
        }
    }
    WorldPcSortResults(out, stored, sortMode);
    return total;
}
