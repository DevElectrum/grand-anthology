#ifndef GUARD_WORLD_STARTER_RANDOMIZER_H
#define GUARD_WORLD_STARTER_RANDOMIZER_H

#include "global.h"

// Only the actual Oak/Elm/Birch award paths call this. It deliberately does
// not change the visible Poké Ball choice, rival species, or story script vars.
// SaveBlock2's fifth independent opt-in randomizer mode controls the result.
enum Species WorldRandomizeStarterSpecies(enum Species original);

#endif // GUARD_WORLD_STARTER_RANDOMIZER_H
