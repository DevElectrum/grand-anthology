# Grand Anthology static encounter source sweep
**Date:** 26 October 2026  
**Repository:** `DevElectrum/grand-anthology`, public `main`  
**Scope:** scripted source repair only. **No compilation, source-validator execution, emulation, or manual gameplay tests were requested or performed in the final sweep.**

## What changed
The latest batch adds **28** dedicated captured-site flags atop the previously established **72**, for **100** independent `FLAG_CAUGHT_RANDOM_*` locations at `FLAG_WORLD_MAP_BANK + 0x12D` through `+ 0x190`. Existing `FLAG_CAUGHT_ANTHOLOGY_*` source flags for form-specific, double, and other bespoke encounters are retained; do not count them twice. The `flags.h` boundary marker is now `+ 0x191`. These are flags in the existing save capacity, not a new save structure.

**Batch A (7):** Goldenrod Genesect; Jagged Pass Gouging Fire; Olivine Lighthouse Regieleki; Sealed Chamber Chi-Yu; Dragon's Den Regidrago; Hoopa's Route 111 chamber; New Mauville Miraidon. Miraidon's duplicate transition delegates to the original gated transition.

**Batch B (7):** Indigo Plateau Arceus; Saffron Dojo Marshadow; Granite Cave Diancie; Mt. Ember Moltres; Seafoam B4F Articuno; Power Plant Zapdos; Birth Island Deoxys. Their original champion/creation-trio, Koichi, letter/badges, native-puzzle and rock-puzzle behavior is left in place. The seeded Arceus dependency uses the three Mt. Moon source captures.

**Batch C (6):** Route 32 Keldeo; Vermilion Meltan; Mt. Chimney Volcanion; Route 111 Ting-Lu; Ecruteak Theater Meloetta; Mossdeep Space Center Latios. Lilycove's ferry checks either seeded Rayquaza source rather than unrelated original-species Dex ownership. Seafoam B4F's current/boulder setup continues after seeded Articuno reconciliation.

**Batch D (8):** Navel Rock Top/Summit Ho-Oh, Bottom/Base Lugia, Southern Island's chosen Latios/Latias site, Mt. Silver Moltres, Whirl Islands Lugia, and the second FRLG Birth Island Deoxys source. Duplicated original species no longer imply the other physical site's seeded completion. Native cinematic and story state remain separate from site-caught state.

## Other native encounters audited and intentionally kept
- Route 12/16 Snorlax and Route 36 Sudowoodo: movement blockers with **per-location story hide flags**; their progress is deliberately not conditioned on capturing the Pokémon.
- Union Cave B2F Friday Lapras: already owns `FLAG_CAUGHT_UNION_CAVE_LAPRAS`.
- Aqua Hideout Electrode pairs, New Mauville Voltorb trio and Mahogany Hideout Electrodes: individual battle-resolution flags and story counters, not permanent capture-only legendary sources.
- Route 119/120 Kecleon puzzle actors and Battle Frontier Sudowoodo: original field-removal/visibility behavior is not replaced wholesale.
- Pokémon Tower Marowak, Berry Forest Hypno, and Lake of Rage red Gyarados: story fights and associated field or item progression. Requiring a capture here would break the plot.
- Already source-flagged Cosmog (Mt. Moon 1F and Seafoam 1F), Kubfu (Sprout Tower and Route 34), both Calyrex (Route 36), Zygarde variants and permanent Stellar Terapagos retain their established independent capture flags.

## Handoff and unresolved confidence
The code-indexed search found **81** map script files containing `seteventmon SPECIES_*`; other native `setwildbattle` scripts require different logic. This survey and source edits are not a proof that every static encounter in the game is correct. Older randomized saves that captured a source before its flag was introduced cannot always be migrated unambiguously because original species ownership alone does not identify the capture location.

The latest 28 source edits have **not been executed or tested**. In particular, defer any claims about save/reload, map-return scripts, scripted legendary cutscenes, or sprite-to-battle identity until an explicitly authorized testing phase. Do not touch the archived private repository or local save/worktree copies.
