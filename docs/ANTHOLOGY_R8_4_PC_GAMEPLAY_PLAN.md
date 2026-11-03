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
