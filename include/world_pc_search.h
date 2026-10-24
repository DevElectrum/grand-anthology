#ifndef GUARD_WORLD_PC_SEARCH_H
#define GUARD_WORLD_PC_SEARCH_H

#include "global.h"

// P0 PC index: an ephemeral VIEW of the existing saved BoxPokemon records.
// It never modifies BoxPokemon, save blocks, the current box or OT/personality.
enum WorldPcSortMode
{
    WORLD_PC_SORT_LOCATION,
    WORLD_PC_SORT_SPECIES,
    WORLD_PC_SORT_LEVEL,
    WORLD_PC_SORT_ITEM,
};

struct WorldPcSearchFilter
{
    enum Species species;  // SPECIES_NONE means any species/form
    enum Item heldItem;    // ITEM_NONE means any held item
    u8 minLevel;           // 0 means no minimum
    u8 maxLevel;           // 0 means no maximum
    bool8 shinyOnly;
    bool8 includeEggs;
};

struct WorldPcSearchResult
{
    u16 species;           // exact species/form, not just National Dex ID
    u16 heldItem;
    u8 boxId;
    u8 boxSlot;
    u8 level;
    bool8 shiny;
};

// Returns total matches across ALL boxes; writes at most capacity results.
// For fully sorted results, allocate TOTAL_BOXES_COUNT*IN_BOX_COUNT entries.
// Limited buffers contain the first capacity matching storage locations.
// Sorting rearranges only the output view, never the Pokémon or box slots.
u16 WorldPcSearch(const struct WorldPcSearchFilter *filter,
                  enum WorldPcSortMode sortMode,
                  struct WorldPcSearchResult *out, u16 capacity);

#endif // GUARD_WORLD_PC_SEARCH_H
