# Sinnoh full-content import and campaign completion plan v1.0

**Status:** planning/documentation only, 2026-11-05. **No import, source-code implementation, save conversion, build, or merge is authorized by this document.** The maintainer requested a comprehensive plan and refreshed handoffs. Execution needs a subsequent explicit go-ahead.

**Canonical inputs:** Grand Anthology `main` at `6061928f0a20d67b00b676e41c1d1d53f3c3b44c` (R8.4 merged); `sinnoh-remakes/pokeemerald-platinum` `master` at `c79120fbab7d62c66785c90b491e6a370249d5db`. Companion XGBA `master` already includes R8 and E5–E7. Pin these source commits for reproducible content extraction, not for making assumptions about future upstream updates.

**In one sentence:** Import the GBA demake's *Sinnoh-specific content and its transitive build/runtime dependencies* into Grand Anthology's existing engine; finish absent Sinnoh geography/story later; do **not** import a second battle engine, PC, Pokédex, shared bag or save implementation.

Related: [source/map inventory](SINNOH_UPSTREAM_CONTENT_MANIFEST.md), [architecture kickoff v0.1](SINNOH_ARCHITECTURE_KICKOFF.md), [active handoff](NEXT_ASTRA_HANDOFF.md), [R8.4 closeout](ANTHOLOGY_R8_4_FINAL_QUALIFICATION.md).

## A. Scope, what exists, what is not complete

The upstream project is a fork of a GBA-style pokeemerald-expansion base, not a finished Platinum conversion. Its first **13 map groups contain 93 Sinnoh maps**, with all 93 map-script paths present. **21 scripts are only minimal map-script skeletons**; some are harmless ordinary interiors, others are critical unfinished content (the four floors of the Eterna Galactic Building, for example). The first complete geographical footprint covers Twinleaf–Sandgem–Jubilife–Oreburgh–Floaroma–Valley Windworks–Eterna and the nearby routes, caves, lake, forest and interiors. It does **not** supply the complete Hearthome/Veilstone/Pastoria/Canalave/Snowpoint/Sunyshore/League/Distortion World geography or a complete eight-badge storyline.

Verified source-only comparison versus Grand Anthology:
- 93 Sinnoh map entries / 13 new map groups (existing baseline is 1,194 maps / 101 groups; append yields **1,287 maps / 114 groups**, provided all import cleanly).
- 64 unique source-only layout IDs / 128 map-and-border layout binaries.
- 23 source-only primary and 44 source-only secondary tileset **directories**, including some art for destinations not yet mapped.
- 135 new overworld picture files and 127 new overworld palette files (counts of picture/palette files, **not 135 distinct NPC characters**); 92 new trainer front-picture files and 8 back-picture/palette files.
- 60 wild encounter records across 20 Sinnoh maps, typically morning/day/night groups. Other Sinnoh maps need absence/coverage justification.
- 191 source-only MIDI files, **not 191 guaranteed link-ready songs**. Instrument/sample/voicegroup and ID dependencies must be resolved.
- `data/text/npc_sinnoh.inc` is a large, largely plain-text reference archive (~0.94 MB); `data/text/trainers_sinnoh.inc` is a separate trainer text source (~0.18 MB). Neither should be wholesale linked as active ROM dialogue.
- `src/data/trainers.party` contains Sinnoh-specific trainer entries, including 78 `TRAINER_PT_...` blocks in the source format plus other named battles; **derive the definitive reachable trainer set from map/script references**, not a misleading single prefix count.

These are available assets/data, not an assertion of game completeness, legal permission, tested asset budgets, or zero integration work.

## B. Source-to-destination import contract (no blind tree merge)

| Area | Pull from pinned Platinum | Keep in Grand Anthology | Transformation / mandatory proof |
|---|---|---|---|
| Map registry | `data/maps/map_groups.json` first 13 groups, 93 `data/maps/<Map>/map.json` | Existing 101 groups and every existing map ID | Append-only `GA_SINNOH_...` or noncolliding group names, stable generated IDs, rewrite group references and warps; confirm group/map number bounds, map sections and arrival links. |
| Layouts | 64 source-only layout entries, 128 `data/layouts/<Layout>/{map,border}.bin` | Existing layouts, address/linker/mapping model | Append stable layout IDs; convert/metatile-rebase where required; validate block sizes, dimensions, elevation, collisions and border behavior. |
| Tilesets | 23 primary / 44 secondary source-only directories, palettes, tiles, metatiles and behavior attributes | Current graphics loader/tileset registry | Import referenced assets and optionally stage future-only assets as *unlinked* source; register symbols; check palette/VRAM/tile/metatile IDs, animation, compression and material behavior. **Do not replace globally shared tilesets** by same path. |
| Map objects | Map JSON events, signposts, warps, object definitions | Existing object event runtime | Remap object graphics, local IDs, script labels, flag IDs, event templates and dynamic spawn dependencies. |
| Overworld sprites | Sinnoh-specific files beneath `graphics/object_events/pics` and `palettes` | Existing global object gfx/palette indices | Extract *referenced* sprites (Rowan, Barry, Dawn/Lucas, Galactic, leaders, props, etc.), allocate noncolliding IDs; compare frame layout/animation/palette assumptions. |
| Trainer images | `graphics/trainers/{front_pics,back_pics}` | Existing trainer art, player presentation, battle renderer | Import only referenced portraits/palettes, namespaced trainer class/pic identifiers, no replaced existing files. |
| Trainers | Sinnoh entries from `src/data/trainers.party` and referenced trainer constants | Existing battle engine, AI, mechanics | Generate deterministic global trainer ID mapping and separate defeat flags; preserve party, rematch, scaling, rival variant, AI, item, portrait and music metadata intentionally. |
| Encounters | 60 records / 20 maps from `src/data/wild_encounters.json` plus authored gifts and statics | Existing DexNav, encounter engine, species catalog, RNG, RTC | Translate map IDs, table types, rates and time variants; preserve scripted/static legendary semantics, avoid overwrite of existing regional encounter data. |
| Scripts | All 93 `data/maps/<Map>/scripts.inc`, plus exact referenced helpers (e.g. `data/scripts/honey_tree.inc`) | Existing script VM, generic game scripts, shared trainer-battle setup | Parse/namespace labels, remap flags/vars/objects/trainers/items/music/maps, rewrite incompatible specials and scratch variables; categorize 21 skeletons as harmless/shared behavior vs blocking missing event. |
| Text | Map-local dialogue + referenced passages from `npc_sinnoh.inc` / `trainers_sinnoh.inc` | Existing text codec, names, UI and language rules | Select referenced/needed passages, resolve placeholders and charmap, wrap and validate bounds. Keep massive text archive as reference only until used. |
| Audio | 191 Sinnoh MIDI candidates plus required `sound/direct_sound_samples/dp_*`, voicegroups, sound IDs/instrument config | Existing m4a mixer, sounds, other regional music | Build a referenced track graph, add nonconflicting IDs and samples, convert/compile, test high-ROM playback and transitions; do not overwrite shared audio banks. |
| Region UX | Relevant Sinnoh regional map art/data if usable; map section labels and heal/flight data | Existing Fly/Town Map UI, Hub, PC, 112-box collection, party, bag | Native Sinnoh map layer, warp/fly/heal points, arrival and return, Sinnoh-specific badge/obedience rules, unchanged incumbent three-region behavior. |
| Unique field events | Honey Trees, Cheryl escort, Galactic puzzles, Eterna gym camera, cycling, movement/cutscenes | Field engine, field moves, followers/partners where already available | Port only referenced helpers and script semantics; choose gameplay-accurate behavior case by case. Existing Rock Climb/Defog handlers are a starting point, not a guarantee all Sinnoh conditions are implemented. |
| Later art candidates | Unique extra Sinnoh tilesets such as `distortion_world` and Sunyshore/Galactic assets | Existing rendering pipeline | Preserve useful vetted art in source provenance/manifest, without claiming late maps are present or linking unused content. |

**Explicitly excluded unless a demonstrated dependency says otherwise:** generic upstream `src/`, battle AI, species/moves/abilities/evolution definitions, Pokémon icon/front/back catalogs, general item/PC/storage/save code, whole Emerald/Hoenn map replacements, version-wide build config overwrite, upstream trainer ID numbering and duplicate shared artwork. A filename that exists in both trees is *not* permission to replace Grand Anthology's implementation.

## C. Deterministic extraction pipeline (the import is generated, inspectable, reversible)

Build a small local importer/validator rather than manually copy 3,000 assorted files. It should run on a pinned upstream checkout and write a **declarative allowlist manifest**; no network dependency in CI. Source manifests and reports go into `tools/sinnoh/` (proposed path; not created yet).

Each imported entity should carry: `kind`, `source_path`, `source_blob_sha`, `target_path`, `target_symbol`, `dependent_symbols/files`, `selected_reason`, `origin/credit/license_review`, and `status` (imported/adapted/deferred/replaced). Generate four explicit translation tables:
1. `map/group/layout/section/warp`: 93 map identities plus all dependent layout/tileset/warp labels.
2. `flag/variable/trainer/object`: all Sinnoh-specific script/event IDs, explicit exceptions for global/special temporary IDs.
3. `music/graphics/text/item`: transitive C/ASM/scripts and resource symbol registration; no missing refs.
4. `progression graph`: prerequisite flag/var, trigger/warp/trainer, resulting state, mandatory/optional, and intended revisit behavior.

Required static validators:
- **Coverage:** every source Sinnoh map, layout and selected event has exactly one manifest disposition; every imported reference resolves; no duplicate target IDs; every deferred mandatory story dependency fails the completeness gate.
- **Legacy preservation:** record entire pre-import group order, map-number pairs, layout numeric IDs and key flag/trainer/save offsets. Assert existing references/values are bit-for-bit unchanged after generation. New Sinnoh map groups are appended, never prepended as upstream does.
- **Script audit:** normalize local labels and register all used `FLAG_*`, `VAR_*`, `TRAINER_*`, `MUS_*`, `ITEM_*`, specials and object graphics. Every reference must be mapped or named on a reasoned global allowlist. Check no unintended `FLAG_BADGE01_GET` / `FLAG_BADGE02_GET` use in Sinnoh.
- **Map graph:** verify reciprocal travel, every warp destination valid, walkable starter->Roark->Gardenia path, all story-locked doors eventually open; tile collision/height/surf/climb/door behavior; disallow spawns into void.
- **Sound/art/link:** test all required palette indices, sprite formats, VRAM/OAM budgets, charmap, MIDI->voicegroup->sample closure, and linker symbols. Verify high-ROM pointer/address/decompression constraints; 256 MiB emulator window does **not** mean every source pointer is safely high-addressable.
- **Dependency checks:** no code from another battle/save engine lands in production just to satisfy an imported story helper. Any such dependency becomes an explicit adapter or rejected resource.

Write both machine-readable manifest and human-readable collision report; review those **before** the actual bulk import. Re-running extraction from the same commit should have identical outputs.

## D. Integration foundation required *before* enabling imported maps

1. **Regional registry:** use canonical region IDs already declared in `include/constants/regions.h`. Add explicit per-region descriptors for map-section ownership, hub destination, home/arrival/whiteout, healing/Fly, badges/champion, field moves, story, intro/started state and regional map. Unsupported regions return error/none; **no fallback to Hoenn**.
2. **Isolated progress:** current `src/event_data.c` aliases unsupported regions to Hoenn; `src/story_progress.c`, `src/region_switch.c`, `src/region_map.c` and `include/regions.h` encode 3-region defaults. Build stable Sinnoh flags/vars/trainer-defeated state, eight badge flags, intro/champion records, local story states. Keep older three banks byte-identical.
3. **Save ABI gate:** measure actual `SaveBlock2`/`SaveBlock3` sizes, existing offsets, spare sectors/E6 pages and R8 cohort compatibility, including checksums and `RegionSaveWidthForVersion`. Choose an append-only versioned bank *only if proven to fit and migrate atomically*, otherwise design a journal-coherent XGBA extension area with matching selected native save generation. No unguarded bank resizing; upgrade older saves with initially-unvisited Sinnoh and zero damage to existing Pokémon/progression.
4. **Trainer/flag budget:** allocate explicit nonoverlapping numeric ranges and check 16-bit IDs, script assembler encodings, trainer defeat-state storage and existing inline-vs-SaveBlock3 routing. Map conversion must be driven by name+source namespace, never by copying upstream numeric IDs.
5. **Map IDs:** target initial 114 groups / 1,287 maps. Test the actual generated group/map fields, per-group map numbering and all consumers before assuming `u8` supports every case. Append sections/heal IDs and preserve native map ID values.
6. **Region gameplay:** single shared party/bag/National Dex/112-box collection; independent Sinnoh progress; region-specific obedience, gym gating, Fly/whiteout, save/Continue return, Hall of Fame and rival starter logic. World Tour already assumes 24 badges; retain previously earned rewards and set an explicit policy for 32+ badges, not a silent threshold rewrite.
7. **High-ROM:** keep Grand Anthology XGBA-specific A1/A2/R8 loader and journal as the only save/storage authority. Place *actual measured* packed content using source build map; check DMA/decompression/map and music reads from each required bank. Do not assert 10.8 MB cost or assume the 256 MiB window resolves RAM/VRAM/OAM constraints.
8. **Interaction compatibility:** existing Rock Climb and Defog field-move handlers exist; verify field behavior and Sinnoh badge permissions. Check Honey Trees (source helper triggers an immediate encounter rather than necessarily reproducing Platinum's timed tree behavior), Cheryl partner interactions, Eterna camera/gym effects, daily/RTC and event revisit semantics separately.

**Known upstream ID collision:** Roark's script calls `setflag FLAG_BADGE01_GET`, Gardenia calls `FLAG_BADGE02_GET`, and those names currently belong to Hoenn's native badges in Anthology. This is a hard fail for unadapted scripts.

## E. Full import execution in substantial batches (future implementation, not begun)

### I0 — Lock source manifest, design/state budgets, permissions
- Pin both input commits; generate every-map dependency closure, source hashes, per-file disposition and crosswalk.
- Measure group/map/layout/trainer IDs, legacy save ABI, spare state, script VM constraints, linker/high-ROM placement, graphics/audio runtime budgets.
- Verify authorship and redistribution basis of all copied content. The inspected `pokeemerald-platinum` root has a `CREDITS.md` but **no top-level LICENSE**; public source visibility is not itself an open license. Hold any uncertain content out of published distribution until permissions/provenance are resolved. Record source URLs and attribution in notices.
- Output: signed-off source manifest + collision/ABI report, extraction tooling design and tested reversible migration specification.
- **Gate:** no unresolved collisions, no guessed save offsets, no unvetted asset licensing for publishable builds.

### I1 — One reusable four-region platform change
- Implement a registry and explicit rejection of unsupported regions; append nonconflicting map/section/trainer/flag namespaces; select coherent, versioned progress storage with cold recovery and migration.
- Add Sinnoh hub/arrival/return/first-badge/obedience/Fly interfaces behind disabled placeholders, *without* enabling incomplete maps.
- Keep no unintended changes to native three-region gameplay/flags, party, bag, PC and R8.
- **Gate:** compile/source assertions, clean old-save migration and actual XGBA Continue + A2 save selection, with pre/post identity of incumbent regions and all 3,360 encrypted records. Do not replay the entire historical R8.4 suite unless dependent code changes.

### I2 — Bulk import the entire available Sinnoh content set
- Import/convert all **93 existing maps, 64 layouts, 13 groups**, events/warps, referenced primary/secondary tilesets and graphics. Preserve later-only unique art as dormant resources if provenance allows.
- Import all Sinnoh-specific trainer records reachable from these scripts (including rival forms/rematches), all 60 encounter records, map-local text, referenced archive dialogue, all needed portraits/sprites/music with transitive samples/voicegroups.
- Transform all 93 map scripts, including explicit classification of 21 skeletons. Add targeted field helpers/animations only where a Sinnoh script requires them. Disable/restrict unfinished mission areas until I3 completion.
- Single integrated source validation cycle for maps, resource references, scripts, link and real XGBA graphics/music smoke. Do not call this stage two-badge-playable merely because it compiles.
- **Gate:** complete disposition of every source Sinnoh map/asset/special; no unresolved label or ID collisions; all imported maps render and connect; high-ROM/runtime and legacy ID checks clean.

### I3 — Complete and accept the imported **Twinleaf-to-Eterna** campaign
- Connect proper Rowan/Barry/player starter introduction to hub travel, allowing an existing global party without losing it or duplicating starter/catch records. Resolve visit/re-entry cases and full collection/bag where gifts are offered.
- Complete rival battles, first two gyms/Coal and Forest Badges, Jubilife/Looker, Valley Windworks/Mars, Floaroma, Eterna Forest/Cheryl, Gardenia and the **unfinished Eterna Galactic four floors**. Apply correct Sinnoh-specific encounter/rematch/quest/gift/flag semantics.
- Finish accessible shops, heal/whiteout, early Fly destinations, field abilities, and Sinnoh map interface through Eterna. Decide whether Honey Tree behavior should be immediate (source) or timed (Platinum fidelity) before implementation.
- **Gate:** real controller route from Hub -> Sinnoh opening -> Roark -> Gardenia -> Galactic/Eterna -> Hub -> Sinnoh return; actual saves/cold Continue with independent Sinnoh flags and unchanged other regional progress, party and 3,360 boxed records.

### I4 — Finish the **missing** remainder of Platinum in geographic/story clusters
These are **new content authoring**, not files falsely claimed to exist upstream:
1. **Central Sinnoh:** Cycling Road/Wayward Cave/Mt. Coronet connections, Hearthome and Fantina, Route 209–215, Veilstone and Maylene, Galactic expansion.
2. **South/west:** Pastoria and Crasher Wake, Great Marsh, Canalave/Iron Island and Byron, lake guardians and expanding Galactic plot.
3. **North/endgame:** Snowpoint and Candice, lake crisis, Mt. Coronet/Spear Pillar, Cyrus/Giratina and an intentional GBA-2D Distortion World design; Sunyshore and Volkner.
4. **League/postgame:** Victory Road, Elite Four/Cynthia, Battle Zone, postgame legends/rematches and selected optional Platinum facilities.
For each cluster: create missing maps/layouts/events/trainer/encounter tables and unique art; draw from the upstream *unmapped* Sinnoh art where it fits, then author what is absent. Add an explicit location/quest inventory and continuity/flag graph before coding the cluster. Preserve trainer/encounter balance policy with Grand Anthology's existing all-generation species catalog.

**Gym sequence:** Roark -> Gardenia -> Fantina -> Maylene -> Crasher Wake -> Byron -> Candice -> Volkner. No claim that the upstream source already contains those last six fully implemented.

### I5 — One consolidated full-campaign qualification and documentation
- End-to-end fresh Sinnoh 8 badges / Galactic / Distortion / League/Champion / intended postgame, all essential locations traversable with correct escape/whiteout and Fly.
- Old Kanto/Johto/Hoenn saves upgrade and retain every region-specific flag/var/badge, Champion and original map ID; save after Sinnoh, travel away and return, cold Continue, torn-write rollback, malformed-state refusal.
- Byte-for-byte invariants: six-Pokémon party records and all 3,360 boxed 80-byte records unaffected by regional travel and migrations unless intentionally mutated through an accepted action. No item/favorite/held-item duplication/loss.
- Deterministic build validation for map graph, script symbols, collision, audio, VRAM/OAM, linker/ROM segment budgets. One bounded real-controller acceptance wave for every new major subsystem; focused retests only on impacted failures.
- Final public-source attribution audit, game docs, explicit build requirements, reproducible source manifest, no user save/ROM overwrites, no Mosaic changes. User gameplay acceptance on a **separate disposable build** before any release claims.

## F. Outstanding decisions, not blockers to planning

- **Regional story fidelity:** default Platinum main quest and eight gyms, with one unified Grand Anthology player; preserve current battle mechanics even where Gen-4 balance differs. Do not promise every DS minigame.
- **Postgame target:** Battle Zone and key legendary/rematch quests are planned. Full Underground, Super Contests, Pokétch, accessory/dress-up systems and an additional Battle Frontier are *optional separate scope* unless a mandatory progression dependency makes a small portion necessary.
- **Honey Tree policy:** need owner decision between time-accurate wait and immediately usable source helper; ensure both use the persistent story/encounter rules chosen.
- **Progress storage:** pick only after actual byte/sector and E6 atomicity measurements. A design recommendation is not a verified free-space claim.
- **Level/catch economy:** choose whether a region begins near starter levels with the globally retained team, or scales trainer rosters. Maintain existing globally shared party and no forced reset.
- **Audio policy:** use source MIDI candidates with verified soundbank dependencies; degrade only by deliberate replacement and attribution.
- **License/provenance:** source visibility does not settle redistribution permissions. Do not publish ambiguous externally sourced art/music/dialogue without resolving provenance.

## G. Execution guardrails and handoff

**At every milestone:** design adversarially up front (save/ID collision, map graph, rollback, VRAM/OAM, map/event script, ROM bank, performance); implement substantial coherent batches; run deterministic validation followed by bounded real-controller proof. Fix only true red gates. No cycles of repeatedly retesting already-green R8.4 or recreating old 112-box persistence work.

**Stop without implementation until explicitly authorized.** No branches, import scripts, runtime code, save migrations, builds or tests were started when authoring this v1.0 plan. On approval the first execution step is I0 import manifest/ABI/licensing signoff, *then* I1/I2 in substantial batches. Keep `DevElectrum/pokemon-mosaic` independent. Never commit ROM binaries, player save data or extracted commercial ROM dumps.

**Definition of success:** Grand Anthology has one shared player ecosystem and four independent completed regional campaigns; Sinnoh has the eight-gym Platinum story, real geography and content from the demake wherever available, no inherited game/save subsystem duplication, and a reusable region framework for future Unova/Kalos/Alola/Galar/Paldea. This is a target, not current functionality.
