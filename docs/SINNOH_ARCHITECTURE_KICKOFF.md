# Sinnoh architecture kickoff v0.1 (PLANNING ONLY)

**Date:** 2026-11-05. **Status:** design discovery, not approved gameplay implementation. **Base:** Grand Anthology main after R8.4 merge and handoff update, `5efa0e1`; companion XGBA master after merge and handoff update, `e9e6378`.

## Goal and default proposal

Add a **fourth complete and independently progressing campaign**: Platinum-style Sinnoh, while keeping the same player, party, National Dex, items and accepted 112-box/3,360-record A2 collection. Preserve Kanto, Johto and Hoenn exactly as established. Make the new region framework extendable to later Unova/Kalos/Alola/Galar/Paldea without redoing region storage every time. Platinum story fidelity and a GBA-style 2D presentation are *provisional recommendations*, not owner-approved decisions.

## Grounded current-source census

- `include/constants/regions.h` already declares `REGION_SINNOH`, but that is **an enum, not an implemented campaign**. There are currently 101 map groups and 1,194 map entries in `data/maps/map_groups.json`; `gMapGroup_RegionHub` is the last existing group.
- `include/regions.h:GetRegionForSectionId` recognizes Kanto and Johto section ranges, then defaults to Hoenn. Adding Sinnoh maps without fixing classification would misidentify the region.
- `src/event_data.c:GetRegionVarBase` clamps any region above Hoenn to Hoenn, while `GetRegionFlagBase` and `GetBadgeFlag` default unknown regions to native Hoenn. This would **alias Sinnoh story and badges with Hoenn**. Existing per-region vars have exactly three `0x80`-entry banks, as defined in `include/constants/region_vars.h`; the Johto flag bank and trainer banks use SaveBlock3, not an unlimited store.
- `src/story_progress.c` only supplies Kanto, Johto, Hoenn resolvers and clamps regions above Hoenn. `src/region_switch.c` has hard-coded three-region hub choices, champions, first-badge gate, intro bits, heal/whiteout respawn and a 24-badge tour counter. `src/region_map.c` defaults unrecognized regions to the Hoenn map. Region-specific HM/Fly/obedience rules need an explicit review.
- `src/load_save.c`, `include/global.h` and the A2 journal currently bind native save and E6 extension into a versioned transaction. Adding Sinnoh progression is **not** simply adding three more `u8` fields; actual SaveBlock3 slack, flags/vars/trainer banks, migration and atomicity must be measured before choosing a representation.
- R8.4 acceptance is complete: game main [PR #18](https://github.com/DevElectrum/grand-anthology/pull/18) and emulator master [PR #5](https://github.com/DevElectrum/xgba/pull/5); [C4 #41975676730](https://github.com/DevElectrum/grand-anthology/actions/runs/41975676730) and [C5 #41975048607](https://github.com/DevElectrum/grand-anthology/actions/runs/41975048607) PASS. No need to repeat R8.4 acceptance absent a specific regression.

## External reference and conversion boundary

- Upstream reference: [pret/pokeplatinum](https://github.com/pret/pokeplatinum), a WIP **Nintendo DS** Platinum decompilation, not a GBA Sinnoh port. Its [3D rendering notes](https://github.com/pret/pokeplatinum/blob/main/docs/3d_rendering.md) describe NitroSDK 3D-model pipelines, while [2D rendering notes](https://github.com/pret/pokeplatinum/blob/main/docs/2d_rendering.md) describe DS-specific rendering/VRAM. Map geometry, scripted events, encounters and narrative can inform a conversion; the NDS engine, models, 2-screen UI and binary assets cannot be linked directly as GBA content.
- The previously discussed ~10.8 MB source/content estimate has **not** been independently measured for usable converted Sinnoh assets; do not use it as a ROM size forecast. Create a representative content-to-GBA size census before committing to a pack budget. The 256 MiB XGBA ROM window removes the old 32 MiB ceiling but does not solve VRAM, OAM, runtime pointer or 8-bit group ID limits.
- Preserve source provenance, credits and public-source-only rules; do not vendor a proprietary NDS ROM, raw copyrighted asset dump or assume an upstream decomp repository automatically licenses assets for redistribution.

## Architecture contracts to decide before implementation

1. **Region registry**: centralize supported campaigns, map-section classification, home/heal/whiteout location, badge table, champion flags, intro state, field moves, Fly/map UI, story provider, hub selection. Unknown regions must reject, **not default to Hoenn**. Keep existing numeric region IDs and all existing map IDs unchanged.
2. **Progression storage**: allocate distinct Sinnoh story flags, per-trainer defeated flags, vars, eight badges and intro/champion bits. Compare (A) append-only SaveBlock3 banks with version migration against (B) a versioned region-progress area in existing E6 persistent storage committed with the native save; choose after an ABI/free-space/corruption-recovery audit. No reinterpretation of existing Kanto/Johto/Hoenn bytes. Design for at least the remaining planned regions, not just one more ad hoc bank.
3. **World content**: convert Platinum geography into the existing 2D collision/height/tileset/event schema using deterministic tooling. Maintain a declarative cross-reference table: Platinum zone/event/warp => Anthology group/map/layout/script, plus unresolved special mechanics. Map/battle assets need verified high-ROM address packing, pointers, decompression and graphics budgets.
4. **Shared state**: one trainer identity, global bag, party, Pokédex and 112-box collection across all travel. Each region retains independent gym progression and champion state. Do not wipe a campaign on re-entry; maintain correct respawn, difficulty, field-move rules, seen/caught forms and legendary/story event flags.
5. **Save/recovery**: A2 remains authoritative for journaled collection; any new Sinnoh progress must commit with coherent native-save selection. Cold restart, older-save migration and torn write recovery must be explicit. A new region should not force loss of any existing 3,360 encrypted Pokémon records.

## Proposed substantial implementation batches (not started)

**S0, decision and foundation:** source/ROM/script/asset census; map-group and map-section capacity; choose 2D art strategy and region-progress storage; implement a generic registry plus explicit Sinnoh-scoped state and reversible migration. No map import until Kanto/Johto/Hoenn preserve their IDs and saves. Output one working engine seam, not dozens of speculative micro-tests.

**S1, playable first-badge slice:** Twinleaf, Lake Verity, Route 201/202, Sandgem, Jubilife, Oreburgh and the starter/rival/Rowan opening; first badge, encounter/trainer scripts, NPC/story progress, healing, hub return and resume. One real emulator Save3/cold Continue checks Sinnoh and original three-region state and 3,360 records. This is a *vertical slice* for content conversion, not a token single map.

**S2, full main adventure in geographical clusters:** Eterna/Veilstone/Hearthome/Pastoria/Canalave/Snowpoint, Galactic plot, Mount Coronet/Spear Pillar, a feasible 2D treatment of Distortion World, eight gyms, Victory Road, Elite Four/Champion. Reuse existing battle species, moves, items and forms unless evidence shows a missing Platinum-specific behavior. Each cluster receives compile/map-graph/source validation; consolidate controller gameplay checks.

**S3, closeout and postgame:** Battle Zone and selected Platinum postgame, Sinnoh Fly/region-map completeness, return travel across four regions, Pokedex/legendary encounters, asset attribution, corrupt/torn-save recovery and one consolidated gameplay acceptance wave. Do not manufacture extra stages after the agreed acceptance contract passes.

## First conversation decisions

- **Visual direction:** provisional default is Platinum's geography and story rendered in Anthology's consistent 2D GBA style, *not* a new Nintendo DS 3D engine. If a DS-faithful look is a requirement, re-scope rendering before touching maps.
- **Content scope:** provisional default is Platinum's eight gyms and core Galactic/Giratina/Distortion World arc; optional DS minigames and full Battle Frontier should be explicitly prioritized later rather than silently promised.
- **Save architecture:** compare extension-based region progression with append-only SaveBlock3 banks. Do not pick based on imagined spare bytes; verify sizes, IDs and migration together.

## Acceptance focus

One representative fresh Sinnoh start, first badge and midgame transition; Kanto/Johto/Hoenn badges/stories untouched; existing saves continue; exactly unchanged party and all 3,360 boxed Pokémon across hub transit, actual save and cold restart; malformed new region state fails closed; no crashes or severe input/UI delay. Later full-campaign content checks belong to S2/S3, not repeated R8.4 loops.

**No Sinnoh runtime, art, map, save-schema or emulator source modifications were made in preparing this draft.**
