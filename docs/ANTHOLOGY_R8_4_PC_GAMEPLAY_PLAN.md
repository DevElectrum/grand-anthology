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
