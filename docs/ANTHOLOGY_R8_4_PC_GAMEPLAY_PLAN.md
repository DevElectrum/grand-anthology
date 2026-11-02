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
