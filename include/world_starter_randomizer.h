#ifndef GUARD_WORLD_STARTER_RANDOMIZER_H
#define GUARD_WORLD_STARTER_RANDOMIZER_H

#include "global.h"

// Called by the Oak/Elm/Birch award paths and by starter-preview UI.
// Keeps original choice indices and rival/story variables unchanged.
// SaveBlock2's fifth independent opt-in randomizer mode controls the result.
enum Species WorldRandomizeStarterSpecies(enum Species original);
enum Species WorldRandomizeKantoStarterSpecies(enum Species original);

#endif // GUARD_WORLD_STARTER_RANDOMIZER_H
