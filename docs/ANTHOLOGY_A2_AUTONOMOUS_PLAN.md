# Anthology A2 autonomous execution plan (2026-11-01)

**Owner authorization:** Continue planning and implementing A2 without requiring Nyx supervision. The owner is unwell. Do not ask for routine decisions, local builds, commands or gameplay smoke tests. Work through safe remote code, CI and disposable emulation until a genuine external blocker; report blockers accurately and preserve enough handoff context to resume. This is permission to proceed on the experimental A2 branch, **not** to overwrite anyone's save, automatically merge to main, or change the accepted XGBA baseline.

## Current audited checkpoint

- Repository: `DevElectrum/grand-anthology`; isolated branch `xgba/a2-collection-lab`. Baseline before this plan: `ea589362d51dc9d714e693aa1b68076ff77533ca`.
- Latest **code** in that checkpoint is `e8f5f02a9d2f53ee4aebff9d1085adca2ddf514d`. Its CI `41297349236` has completed **successfully in both** `storage-format` and `native-a1-regression`, including independently built A1 and A2 ARM releases and temporary E6 cartridge geometry. This does NOT prove emulator gameplay.
- A1 XGBA256 tagged 256 MiB map-asset build remains independent, revision zero/native 128 KiB Flash and no A2 release.
- XGBA E7 256 MiB and E6 save protocol were accepted in their *separate* emulator/test repositories. Do **not** revalidate their already accepted capacity tests. Only Anthology A2 compatibility is unverified.

## Hard correctness invariants

1. No committed native Flash save is ever paired with an uncommitted or wrong extended PC generation. Boot, save, Hall of Fame, error recovery, link and reset paths must preserve this.
2. A crash before the final metadata marker yields the prior verified coherent cohort. A crash after a final marker yields either a verified new cohort or a verified old backup. Never choose the highest Flash counter alone; never silently fabricate blank data.
3. Native Flash 128 KiB and the E6 16 MiB envelope are distinct. Require valid E6 capability, matching version, legal offsets, CRCs, native-sector fingerprint and metadata-pinned page generations before reading a collection. No legacy 128 KiB save is opened directly as E6.
4. Resident `PokemonStorage` remains one 34,144-byte page (14 boxes / 420 slots). The other seven pages are stored externally. `TOTAL_BOXES_COUNT` must remain 14; use a virtual page index. No second collection-sized EWRAM buffer.
5. Exactly one global trainer, party, bag, Pokédex, flags/progression and cross-region PC. A2 is not Emerald's 20 independent worlds.
6. A physical Pokémon is neither silently duplicated nor lost on page switch, capture routing, deposit/withdraw, failed save, failed read, item move or bulk operation. Preserve all 80-byte BoxPokemon records, fusion slots, metadata, box names, wallpapers and held items as appropriate.
7. Legacy/incremental/special saves cannot return *apparent success* when the global transaction was skipped or failed. Callers must not continue a success path on an uncommitted A2 save. Unsupported features must visibly fail closed.
8. Failed reads, ambiguous final-byte writes or uncertain commit state disable further saves until a clean reload/reselection. A partial write cannot be retried against stale counters.
9. Every destructive feature has a reversible preparation phase, checked preview/counts, protected targets and a single verifiable commit; failure cannot silently apply a partial mutation.
10. All changes use the A2 branch with separately named output, create-only disposable saves and no published ROM/game binaries. Never touch live player saves, Mosaic, main, frozen E7, or websites.

## Ordered implementation and verification batches

### A2.1 Save-integrity closure and actual call-site audit

**Source:** `src/save.c`, `src/main.c`, `src/new_game.c`, both `hall_of_fame*.c`, `src/trade.c`, `src/start_menu.c`, `src/record_mixing.c`, `src/berry_blender.c`, direct Flash save helpers and script calls.

- Verify early `GetSaveBlocksPointersBaseOffset` performs cohort selection before trainer-ID relocation and selects the **metadata-approved** native slot even when a newer physical Flash slot has a larger valid counter but no metadata commit.
- Verify recovery from corrupted newest metadata/page/native data selects exact older coherent state, rejects both corrupted cohorts, and preserves native save block relocation, HOF handling and flags.
- Investigate **known no-op hazard**: under A2, `LinkFullSave_Init/WriteSector/ReplaceLastSector/SetLastSectorSignature` and `WriteSaveBlock2/WriteSaveBlock1Sector` currently return `TRUE` without writing. Their callers in trade, start menu, record mixing and berry blender can then continue believing a save finished. Determine reachability and redesign either (a) a verified single A2 transaction at the correct protocol point with proper error propagation and no premature commit or (b) a clear fail-closed unsupported-feature gate before irreversible action. A silent no-op is not acceptance.
- Audit all full/partial native Flash writes and `SAVE_*` variants; identify optional auxiliary sectors (HOF) versus protected core game data. Preserve backup cohort even if auxiliary archive fails. Check both Hall of Fame scene branches propagate errors.
- Add targeted host tests with distinct trainer IDs/relocation offsets, a newer uncommitted native slot, alternating page selection, HOF auxiliary corruption, and all special-save path contracts. **Evidence:** exact commit and CI; do not infer emulator success.

### A2.2 Disposable XGBA boot/save/restart harness

**Objective:** Without requiring Nyx, compile the A2 ROM in GitHub Actions and run as much of the real XGBA emulator core as possible using a private temporary work directory.

- Pin XGBA to accepted E7 commit/tag (read-only). Check whether its headless/core test runner or scripting can boot the complete 256 MiB Anthology ROM with E6 16 MiB save. Use a separate workflow/job; avoid altering emulator source or the A1 build output.
- Build A2 in an *independent* `BUILD_NAME=xgba-a2` object tree with `XGBA_A1=1 XGBA_A2=1`; pack assets using that A2 manifest, tag the separate 256 MiB developer ROM E6 and recalculate the header complement. Never upload a game ROM, BIOS, or save artifact.
- Prepare only disposable 16 MiB E6 blank seeds via `tools/xgba_a2_storage.py blank` or equivalent create-only audited API. If using legacy migration, synthetic fixture first; later use only owner-supplied disposable copies. Avoid auto-creation of an invalid blank save by XGBA.
- Execute and inspect, in order: boot and New Game; initial full transaction; power-off and reopen; second and third save generations; page change then save and reopen; deliberately interrupted extension page write, physical native slot write and metadata marker; HOF success and archive-only failure. Check **on-disk bytes and recovered generation** after every cold restart, not just screen/UI state. Hard-stop if the emulator test harness cannot drive these operations reliably; do not assert pass from a static build.
- If the headless runner cannot interact with game menus, split verification into (1) actual emulator CPU/E6 savedata I/O with a tiny purpose-built disposable test ROM and (2) game-side host/native transaction harness, with the integration gap marked OPEN. Do not fake a full-game runtime pass.

### A2.3 112-box behavior and capacity

- Test page switching (Select+L/R over the SwSh box title), cursor/icon/selected box refresh, naming/wallpaper, move-item/multi-move lockouts, mid-flight errors, return to previous page and save/reopen.
- Exercise 420/420 occupied current page, 7 remote pages, 3359/3360 and 3360/3360 slots, gift/catch with full party, error reporting, PC box-default routing, deposit/withdraw and border cases. No false "free slot" when remote CRC/read fails.
- Verify all species/form/shiny/IV/nature/ability/held-item attributes and the four fusion records preserved across page swaps and commits. Test directly modified PC pointers/dirty detection.
- Performance budget: normal capture uses resident checks only, remote search only when resident full. No 16 MiB boot scans, no whole-bank rewrites. Set latency ceiling based on repeatable headless frame or measured 1x tests; avoid claiming host wall time is console latency.

### A2.4 Global collection user experience

- First define stable Pokémon identity versus box coordinates. Preserve favorites with an identity-linked, versioned persistence model; consider unique IDs and compatibility with identical species/OT/PID and old-save imports. No favorite may jump to another mon when sorted, traded, evolved, moved, or reboxed.
- Collection-wide bounded search/filter/sort as an **index/view** without moving physical records. Off-page scan/index rebuild only on relevant mutations and on initial requested search. Search fields: species, types, shiny, legendary/mythical, favorites, item and box/page location (implement UI affordances consistent with 256 KiB EWRAM).
- Bulk release stages a preview and confirms before mutation, preserving favorite and Legendary/Mythical Pokémon; shiny status alone is not protection per approved policy. Protect from accidental releases while navigating pages, held-item return must check bag capacity and rollback if it cannot fit. Never treat failure as successful count; backup cohort survives power loss.
- Include regular one-by-one release. Test zero/all-protected/mixed/last-party/eggs/held items/full bag, rollback, duplicate entries and maximum 3,360 slots.

### A2.5 Global expanded bag

- Trace the actual `SaveBlock1` bag ABI, pockets, `SavePlayerBag`/`LoadPlayerBag` and all stash/trade/menu paths before changing sizes. Bag changes must not alter trainer/PC save pointers unpredictably.
- Choose a single bounded extension-owned bag schema with A/B versioning or a smaller compatible solution justified by capacity. Use one global bag; no region-specific duplicates. Define unique key items, maximum item count, pocket expansion, sorting/filtering, item receipt from releases, NPC rewards, held-item moves and failure behavior.
- Build a create-only migration/checker from legacy bag data as necessary. Test full pockets, duplicates, cross-region events, first game, old format, crashes during updates and cold restart.
- No speculative bag schema changes before completing the persistence and collection gates.

### A2.6 Adversarial integration, regressions and performance

- Automated cases: every critical write boundary, truncated/bad header/CRC/generation/hash, older/newer native counter conflicts, missing/mismatched metadata, empty/half-initialized E6, double-corrupt cohort, RTC trailer, interrupted HOF, game restart, high-address map fetch.
- Resource budgets: EWRAM/IWRAM, native link `<32 MiB`, overall 256 MiB ROM, E6 range bounds, intermediate 4 KiB save scratch, 34,144-byte resident PC, 128-byte bus chunks, no per-frame global scans, batched operations with bounded work.
- Verify existing A1 ROM byte/geometry invariants remain unchanged in own build. Cross-region regression Kanto/Johto/Hoenn, progress flags, daycare, party, inventory, static encounters, box operations. No hidden UI "success" after a refused save.
- Require source/host CI, isolated full ARM link and XGBA runtime evidence **separately**. If one fails, isolate the smallest reproducible input and fix that class before adding features.
- Risk-order: DATA LOSS > wrong save generation > memory corruption > feature correctness > interaction/performance polish.

### A2.7 Release gate, handoff and one user acceptance

- Only after all automated critical gates: prepare a short self-contained user test on Nyx using a **distinct** test ROM name and a pre-backed-up disposable save. No need to run anything until the owner is healthy. Prefer one concise session: boot, capture, page change, save, exit/reopen, one protected bulk action, item check, region travel.
- Do not publish A2 as a gameplay release or merge to main until test accepted. If local machine access is unavailable, leave gameplay acceptance explicitly **PENDING**, while continuing safe CI/docs/code work.
- Update `AGENTS.md` and `docs/NEXT_ASTRA_HANDOFF.md` at each meaningful checkpoint: branch/head, precise files changed, CI URL and conclusion, test classification, remaining risks, next unblocked action. Limit each handoff to current facts; older material remains archival.

## Autonomous operating rules

- **Do not stop for routine status questions or minor implementation choices.** Resolve within established requirements, preserve working code, group coherent changes and run CI after substantial batches.
- At each milestone: architect the data/latency/error paths, implement a coherent batch, test adversarially, inspect results and record a checkpoint. Never call compile success gameplay success.
- On a genuine blocker (missing emulator automation, inaccessible private machine, repeated failing CI that cannot be diagnosed safely): preserve branch and last green commit, attach exact repro evidence in the handoff, then pursue other independent safe work. Do not repeatedly ask the sick owner for Nyx access.
- Remote GitHub access does not provide access to Nyx's process, files or UI. Don't claim local test execution or operate against the player's real data. Work proceeds only during active assistant/tool execution, not silently in the background after a turn ends.

**Guiding rule:** big steps, better mistakes, even better solutions. Protect the collection first.
