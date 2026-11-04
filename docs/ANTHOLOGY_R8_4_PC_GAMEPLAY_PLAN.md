# R8.4C complete PC gameplay plan (experimental)

## Baseline
- d7657044 real ARM 112-box traversal, two saves and cold restart GREEN.
- d60f2265 global 80-byte move/swap backend, populated transfer and corruption-preserving cold restart GREEN (real emulator run 41452277340).
- XGBA R8 emulator pinned 4e0f5918; 4 MiB RAM, unchanged native ABI/A2 E6 journal. Never merge main, modify Mosaic, frozen E7, a real ROM or any player save.

## Implementation gates
1. C1: checked global move/swap. Validate both global IDs 0..111, local slots 0..29, occupied target, self alias and absence before mutation. No E6 I/O. Verify full record, source empty/dest exact after cold restart (completed).
2. C2: global chooser preview. Classic popup cycles all 112. Sword/Shield-style grid stays 14 cells; L/R selects one of eight groups. Preview name, global number and count from checked RAM accessors without switching native group. Commit Jump only when nothing is being carried, then tear down icons and reinitialize. Cancel never mutates collection. Both UI modes must compile. **Source and real ARM function tests are not a controller UI acceptance.**
3. C3: GUI party deposit/withdraw and single carried-mon transfer across groups. Save held mon state to an owned 80-byte snapshot before UI teardown, preflight target, never reuse source pointer after switching, cancel must restore without duplication, prevent release of last three non-egg mons, handle eggs/mail. Destination full must not create a fake success.
4. C4: multi-move, item transfer, bulk release, favorite/legendary protection, all-or-none bag returns, all-full storage, party limits. Unavailable collection must fail closed.
5. C5: actual controller-driven scripts on both PC engines exercising 14/15, 28/29, 112/1, L/R, Jump, deposit, withdraw, cancel, occupied destination, item attach/detach. Verify two saves, two fresh emulator restarts, exact old/new records and selected index with independent E6 auditor. Add power-loss matrix, cross-region collection checks, normal/A1/A2/R8 compile and freeze R8 emulator regression.
6. Performance acceptance: no E6 journal calls during navigation; at most two 34,144-byte copies per group change; avoid per-frame 3,360-mon scans; record emulated frames/cycles separately from wall-clock.

## Non-negotiable stop gates
Do not merge or issue a playable build until graphical tests, destructive-operation safety, cross-region and crash cases pass. A backend C-call success is not proof that users can safely carry Pokémon in the GUI. Do not disable carry safety guards as a shortcut.

## C3A: no-user-input integration slice (2026-11-02)
- Direct party to first empty slot of any global box. Validate all destination/source constraints before writing complete BoxPokemon and clearing source. Keep current selected native group unchanged.
- Both PC frontends offer a remote party deposit chooser. Classic reuses its global popup; SwSh uses the existing 14-tile grid and L/R preview. Invalid/occupied/full target or B must leave party intact.
- Explicitly do **not** enable cross-group movement of a Pokémon already being carried, multi-selection, mail or item-in-hand transfers. These demand a separate rollback-aware carry-state protocol.
- Disposable real-ARM fixture attempts invalid party/global IDs and deposits a genuine game Pokémon into distant global Box 43, first free slot with exact record verification after cold restart. CI source assertions prevent accidental relaxation of the guards.
- This is source + game-function testing. No claim of visual-controller input test or C3 acceptance until independent GUI scripts execute.

## C3A accepted backend checkpoint (2026-11-03)
- The fixture used CreateMon without CalculateMonStats; the synthetic Zigzagoon had zero HP and correctly failed the last-usable-mon guard. The test-only fixture now initializes HP for Zigzagoon and Eevee, while keeping the production guard intact.
- C3A real XGBA run 41590131391 is GREEN at 1b2e30f6: Save1, checked distant Eevee deposit, cross-group moves and swaps, all 112 box selections, Save2 and independent E6 audit, then cold Continue.
- This certifies only real-ARM *function* calls, not a controller's box-screen workflow. Neither UI is accepted yet.

## C3B: checked distant box-to-party withdrawal (2026-11-03)
- The game backend reads a checked BoxPokemon from any global box ID 0..111 and local position 0..29; rejects empty source, boxed mail, unavailable collection, full party, and inconsistent party tail *before any source/destination record write*.
- It copies the whole encrypted 80-byte record to an owned snapshot, converts that snapshot with BoxMonToMon, checks the result, writes a complete party struct, then clears the PC source and updates the party count. Active resident group and E6 journal are unchanged.
- The disposable ARM fixture rejects bad global IDs, bad slots, empty sources, full party, then withdraws Eevee from remote Box 43 slot 1 and deposits it back. Compares all 80 encrypted bytes and verifies identity, held item and favorite marking across Save2/cold Continue.
- Source checks are only static guards. The R8 real-emulator workflow is authoritative for game-level behavior. This backend is deliberately **not** wired to a new GUI selector yet; existing PC Jump to a distant box still uses normal local withdraw logic.

## Next gates, adversarial and performance-first
- **C3C UI acceptance harness:** safely enter the Region Hub PC at (17,9) using genuinely simulated controller input on a disposable save; verify live PC callbacks and mode selection before pressing A. Do not spoof controller acceptance via direct C calls. Test Classic and SwSh separately, cancel, full destination, full party, 14/15, 28/29, and 112/1. Assert exact record and no duplicate after restart, with independent E6 auditing.
- **C3D GUI withdrawal and held-single movement:** only use checked backend for new remote operations after C3C; design an owned 80-byte carry snapshot plus rollback-before-page-switch. Keep global movement of a carried Pokémon blocked until its cancel and teardown paths are independently proven.
- **C4 safety:** preflight bulk held-item bag capacity atomically, protect favorite/legendary, test no-op/full/eggs/mail. Include targeted crash/power-loss and stale-cache recovery matrices before release.
- **C5:** two graphical PC engines, real controller transitions, all boundary boxes, cross-region persistence, two saves/two cold restarts and emulated-frame performance evidence; freeze the pinned emulator separately. No migration or published playable ROM until every gate passes.
- Worst-case normal C3B work: 80-byte copy, at most six party-slot checks, one BoxMonToMon conversion. No E6 I/O, 34 KiB group copy, or whole-collection scan while navigating.

## C3C pilot: real controller-driven PC entry, both frontends (2026-11-03)
- Separate matrix workflow R8-PC-Controller.yml builds disposable SwSh and Classic variants. Classic mode changes only a throwaway CI checkout, never the actual user/default game configuration.
- Test-only volatile UI witness marks true game-field PC event (1), successful Classic/SwSh storage callback setup (2/3), and actual running graphical storage main-input loop (4/5). The linked ARM ELF exports its address; no guessed memory offsets.
- Lua starts from audited E6 Save1, checks real RegionHub map ID, reads the player's current coordinates from SaveBlock1, moves with emulated D-pad to (17,10), faces the PC at (17,9), then sends bounded A input until the matching frontend's live UI witness appears. The script never directly invokes game routines or declares success on a rendered animation.
- It independently audits the E6 storage again afterward, checking no unintended persisted changes. It emits exact failed coordinates/stage if movement or menu activation fails, rather than faking success.
- **Pilot is PC-entry only.** It does not by itself certify deposit, withdraw, Jump, cancel, carrying or cross-page selection. Next, extend this same real-input harness for those paths with exact record and full Save2/cold restart verification. Never remove the cross-group carry guard to make a test pass.

## C3C first controller execution diagnosed (2026-11-03)
- Run 41595855941 built both game variants, real Save1, and pinned emulator successfully, then failed *before PC entry* at player (16,7): input attempted (16,8), which the actual Region Hub 25x18 map.bin marks impassable.
- Corrected the controller-only route: (16,7) → (16,6) → (6,6) → (6,13) → (17,13) → (17,10), then face north to PC (17,9). This avoids impassable wall row 8 and counter row 9. Actual collision map is validated by a new source regression for every intermediate tile.
- No storage backend, journal, or accepted emulator changes. Controller failure was route planning, not evidence of a failing PC engine. Check the successor C3C run before claiming UI success.

## C3C second controller diagnosis and safe prompt progression
- On a7a0f225, both matrix jobs reached Region Hub PC tile, but the pilot rejected at *A press 5* with trace=0. The same game has a multi-stage PC script: boot text → Choose PC multichoice → login text → storage opened text → ShowPokemonStorageSystemPC → actual storage screen. Five presses were not enough evidence of a failure.
- A test-only witness at DoPCTurnOnEffect now records stage 6, independently proving the physical PC trigger accepted A. The Lua script logs witness changes and allows the bounded multi-prompt sequence to finish, while rejecting a genuinely inert PC after 3 attempts and a script that never reaches storage after 14 attempts.
- Workflow failure output filters actual controller diagnostics before noisy BIOS logs. No production PC behavior, RAM-backend, or journal change. C3C remains nonaccepted until both actual UI variants pass.

## C3C Jump preview/cancel real-controller extension (2026-11-03)
- The C3C two-frontend entry smoke passed real XGBA run 41607139661 at 1a2f78b7. This is accepted only as *genuine GUI entry*.
- A second script mode re-enters each real PC on its own fresh emulator process, uses UP from verified new UI cursor slot zero to reach the box title, A to open options, A to select first safe Jump option, then R/L to preview global Box 15 and return to Box 1, finally B to cancel.
- Test-only witnesses distinguish classic/SwSh option menus (7/8), actual global Jump choosers (9/10), active resident box (bit 30), and preview global box (bit 31), with selected index and resident page encoded independently. The live script fails closed if any witness is missing or the selection is unexpected; never presses A inside the actual Jump chooser.
- Before, during and after preview, the resident page must stay zero. After B, selected box must remain local Box 1, and the independent E6 auditor output must match the exact before snapshot. No C3 backend, journal, or emulator changes.
- Passing preview/cancel is not Jump commit acceptance, not remote party transfer acceptance, and not a playable build gate. Follow it with an independent controller-driven global Jump commit to Box 15, Save2/cold Continue and exact record verification.

## C3C actual global Jump commit controller gate
- A third isolated controller mode restarts audited Save1, enters real PC, opens Jump using real UP/A/A input, previews global Box 15 via R, commits it via A, and waits for the real PC to finish tearing down/reopening instead of assuming a fixed frame count.
- Test-only resident UI marker must report page=1, local box=0; preview before commit must report page=0, global selection 14. The independent E6 auditor must remain byte-identical in decoded audit to Save1, since navigating a box without an explicit game Save must never write the sidecar.
- This is **in-session UI commit only**. Next C3C acceptance must explicitly Save2 in-game *after* Jump and cold Continue with the selected group and record provenance independently verified. Do not claim a save-persistence victory from a transient RAM-only page switch.

## C3C Save3 controller-and-cold acceptance gate (experimental, 2026-11-03)
- Base checkpoint: dcc19fa2 run 41609705834 passed real graphical Jump commit on both PC engines; real Save2 and independent cold persistence still passed separately.
- Run a *third* isolated test on disposable media. Build from same pinned R8 XGBA. Save1 via real game; Save2 via actual Save menu and checked RAM fixture; cold-load Save2 with seeded Bulbasaur, Charmander, Squirtle, Pidgey and Eevee and global Box 30 selected (page 2/local 1).
- Navigate the real PC in Classic and SwSh variants. Open Jump from the true box-title menu, preview Box 30, press L to Box 16, LEFT to Box 15, then A to commit. Verify active page 1/local box 0 after graphical teardown/rebuild. B twice only through the verified storage Continue/No state; require return to genuine PC main menu. B closes it, Start opens the actual graphical Start menu, and Save icon is verified by live task+state pointer before any A.
- Complete the real third Save using journal completion stage 8 and native counter 3; audit independent E6 metadata. A fresh emulator process cold-loads Save3. The **test-only** A2 load verifier checks expected page 1/local 0 and the five original game Pokémon in their precise global box slots, including held items, marking, origin personality and empty previous slots. This prevents false success on a new-game fallback or wrong resident page.
- All internal traces and the counter-sensitive verifier live under XGBA_R8_TEST, never a player build. Existing Save2 requirement page 2/local 1 remains independently strict. No emulator, Mosaic, native save format or production storage change.
- A controller callback is not an independent snapshot of every record's 80 bytes; full byte-for-byte disk provenance across Save2 and Save3 remains an additional hardening opportunity. A green run proves selected box plus record identity/count and E6 consistency, not complete graphical transfer support.
- Do not label gate passed until **both frontend matrix jobs**, real XGBA Save3, cold Continue, and source regression are green.

## C3C offline exact-record digest hardening (staged after Save3 acceptance)
- The independent E6 cohort auditor now computes SHA-256 over the concatenated 80-byte encrypted BoxPokemon records in all 8 selected, CRC-validated native/page generations: exactly 3,360 slots (268,800 bytes). It intentionally excludes mutable box index/name/wallpaper fields.
- The Save3 matrix extracts the canonical digest after populated Save2 and again after controller Jump+Save3 and demands an exact match, before cold Continue. This is an independent byte-for-byte proof that GUI navigation did not silently rewrite or lose any Pokémon, stronger than the test-only game fixture's species/personality/item/marking checks.
- Only offline auditing and CI assertions change; no production storage, A2 format, emulator, or user saves change. Require both matrix branches green again before full C3C acceptance.

## C3D1 complete transaction and cold-save integration batch (2026-11-04)

One atomic candidate batch, **not accepted until exact two-engine CI verdict**:
- Replace hard-coded directional sequences after graphical withdrawal with a bounded state-aware controller route. Classic follows box rows into the bottom Party button; SwSh walks to the leftmost box column then crosses into party. Both use actual sampled cursor area/slot/party count, never direct game-memory cursor writes.
- Preserve the original unsaved cancel-and-redeposit check, then exercise an independent saved transaction on a **cloned Save2 disposable E6 sidecar**. Real controller moves Squirtle Box15 slot6 to Box43 slot2, cancels one chooser, reopens, deposits, exits PC, uses actual in-game Start/Save, commits Save3, then cold Continues in a fresh XGBA process.
- The test-only cold-load fixture recognizes two distinct valid Save3 scenarios: unchanged collection after a Jump-only Save3, or the remote-deposit collection. It never relaxes Save2's original populated-record checks. The independent Python auditor reconstructs eight CRC-pinned committed pages and requires **only** the two exact 80-byte BoxPokemon slots to differ; all 3,358 other slots remain bit-identical.
- The existing Jump Save3 test continues to use the untouched Save2 sidecar. The roundtrip sidecar is deleted on all outcomes and never touches a user's save, main, Mosaic or the pinned XGBA build.
- Stop after the first complete controller-matrix result. Analyze evidence before any corrective commit; do not push another speculative key-sequence patch. C3D1 remains pending until both frontends, exact offline audit and fresh cold Continue pass.

## C3D1 post-matrix diagnostic consolidation (2026-11-04, corrective candidate)

Initial coherent candidate `e43d9699` produced a completed five-workflow matrix: collection source, storage contract/native A1, real-Pokémon cold persistence and boot all green. The dual-PC controller matrix still failed, but reached new acceptance checkpoints: Classic unsaved deposit/cancel/Box43 roundtrip passed; its separate real Save3 flow stopped because Classic B from the party first hides the party instead of exiting storage. SwSh entered the party via left-edge navigation, proved cancellation preserved Squirtle, then failed the final Box43 record check after attempted deposit.

Corrective architecture is grounded in those logs, not speculative key sequences: (1) require test-only proof that the cursor actually selects the original Squirtle before first chooser and after cancellation; (2) instrument the post-deposit exact remote-record fields (source clear, destination species/personality/item/marking, party conservation) for failure diagnosis, without touching production logic; (3) drive Classic party hide or SwSh box-cursor transition before the already accepted storage B/B exit protocol. The transaction/save3/cold/audit workflow remains unchanged. Run one full controller matrix, inspect its exact result, then decide whether gameplay logic needs repair. No CI-chasing pushes while it runs.

## C3D1 SwSh source-index correction (2026-11-04, CI candidate)

Real controller run 41807570886 proved Classic Save3 and cold audit green; SwSh's diagnostic `0x61` showed the wrong Zigzagoon in Box43 while the original Squirtle remained in party. Root cause in `src/swsh_storage_system.c`: `ChooseBoxGrid_UpdateHover` moves the *shared* `sCursorPosition` to a chooser tile (0..13); R8 remote deposit case 18 then mistakenly used that tile as a party index. The existing `sChooseBoxMenu->savedCursorPosition`, captured at chooser entry, is the correct party source. Fix the *production* R8 deposit to use the saved party slot for both backend move and icon removal, with a fail-closed source-area/count check. Do not change Classic, A2 journal, XGBA or controller key sequences. The unchanged dual-UI controller matrix must verify cancel, exact remote deposit, Save3, independent whole-cohort 3,360-record audit, and new-process cold Continue. This is a candidate, not accepted until CI is green.

## C3D1 final accepted result (2026-11-04)

Production SwSh source cursor alias fixed at `33169f0d`. A 14-cell global chooser controls `sCursorPosition` (box tile); source party identity survives in `sChooseBoxMenu->savedCursorPosition` and is now validated and used for backend transfer and sprite teardown. No other production backend or emulator change. [Controller matrix 41811066427](https://github.com/DevElectrum/grand-anthology/actions/runs/41811066427) **PASSED BOTH Classic and SwSh**, including unsaved cancellation/roundtrip, real Save3 and exact offline record proof, fresh cold Continue, and original Jump Save3 regression. The three source/storage/cold companion runs are green as linked in the active handoff. C3D1 is closed.

## Remaining R8.4 gates before Sinnoh: consolidated engineering order

1. **C3D2 safe remote box-to-box carry.** Existing checked `XgbaA2GameMoveGlobalBoxMon` and `XgbaA2GameSwapGlobalBoxMons` are the commit mechanisms, not pointer-carrying across pages. Source location is stable `(globalBox, slot)` and its entire encrypted 80-byte identity remains owned/canonical until a destination preflight commits; own any graphical carry snapshot outside the destroyed UI. Cancel, unavailable collection, occupied/full/invalid destination, UI teardown and source identity mismatch must leave original intact. Keep existing 14-box local drag behavior; add remote UI path for both Classic and SwSh without changing journal/ABI. One build/real-controller matrix tests 14/15, 28/29, 112/1, swap, cancel and exact cold-save provenance.
2. **C4 destructive and item correctness.** Audit single/multi selection, item movement and bulk release against all 112 boxes. Preserve favorites and protected legendary/mythical records, mail/key-item constraints, party last-usable guard, whole-bag preflight before any bulk held-item return, and precise no-op on full/invalid. Unsupported high-risk cross-page modes stay disabled until truly implemented, never silently reduced to current 14 boxes. Test a populated/full box, protected records, full bag and cancellation with exact disk audit.
3. **C5 recovery, compatibility and regional acceptance.** Force failed/torn E6 transaction at staged pages, native-slot write and metadata publish; verify fallback to last committed cohort, no partial virtual collection and no user-save migration. Traverse Kanto/Johto/Hoenn with one trainer's party/bag/112 boxes unchanged. Require 0 E6 writes during box navigation, bounded RAM copies and practical response time, A1/A2/native/accepted E7 compatibility, and a single concise Nyx gameplay check on a disposable save. Freeze accepted R8.4, then begin Sinnoh map/story integration.

**Release stop conditions:** No merge or usable artifact while a real controller path, destructive-record safety or cold recovery gate is red. Avoid one-commit-per-keypress CI loops: pinpoint a failing invariant, make one evidence-based repair, then rerun affected gates. Defer optional search/sort cosmetics and expanded-bag capacity (A2.5) rather than allowing them to block Sinnoh unless a correctness dependency is demonstrated.

## Detailed two-large-batch plan accepted as design (2026-11-04)

C3D2 and C4 are now fully scoped in [ANTHOLOGY_R8_4_C3D2_C4_EXECUTION_PLAN.md](ANTHOLOGY_R8_4_C3D2_C4_EXECUTION_PLAN.md). This is documentation only. Key engineering choice: virtual, source-preserving transfer + direct checked move/swap instead of passing a purged legacy held pointer through native-page/UI teardown. C4 adds a globally indexed 3,360-slot release selection, consistent single/bulk/held-item safety, all-or-nothing bag return, and two-frontend controller Save/cold audit. Do not split these into repeated one-keypatch CI cycles.
