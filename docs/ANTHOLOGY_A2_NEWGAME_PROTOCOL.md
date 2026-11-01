# A2 checkpoint: first-save bootstrap implementation (Nov 1, 2026)

Active branch: xgba/a2-collection-lab. Stable A1 and XGBA E7 remain untouched. No playable A2 ROM, player save conversion, or user playtest. A1's own gameplay acceptance is separate.

Last confirmed full ARM success: GitHub Actions 41289711143, covering unchanged A1 256 MiB ROM and guarded A2 PC UI/capture/save coordinator. Subsequent bootstrap run 41294626762 is pending: inspect its A2 shadow compile result before claiming new-game code works in the game. Storage-format and host C bootstrap crash budget matrix PASS on the new source.

Implemented: tools/xgba_a2_storage.py supports create-only blank DEST.sav and audit-blank DEST.sav developer commands, strictly 16 MiB erased E6 with two exact emulator superblocks, no native data or Pokemon, entire-file verification, refusal of existing destinations and symlinked paths. These are developer fixtures, NOT save migration.

src/xgba_a2_bootstrap.c confirms two E6 superblocks, all 32 native erased Flash sectors and the full reserved PC/metadata area are pristine, then writes two independently checked records for 8 pages (14 boxes each) using only 128-byte chunks and the original PokemonStorage memory. Inactive pages preserve encoded names/wallpapers but zero mons and fusions. It writes/verifies physical native slot 1, counter 1, commits the first 56-byte A2 metadata record LAST and independently reselects the coherent global save. Any partial disk write blocks reinit. Native harness covers bad native/extended bytes, zero-byte failures, 15 interrupted-write budgets and successful initial commit. No second 34 KiB RAM allocation.

Under XGBA_A2=1 only, src/new_game.c grants a one-shot first-save authorization from NewGameInitData. src/save.c consumes it, binds E6, invokes bootstrap then starts the pager. Failure disallows repeated speculative writes. The existing A1 build is unaffected; unsupported HOF/direct-sector save variants still fail closed.

Pending: confirm guarded ARM success for this new-game path; verify actual E6 emulator revision/tag; create a distinct development ROM without changing A1; prove newgame/save/quit/reload and interruption rollback on XGBA using disposable template or copied v13 saves; handle Hall of Fame and special saves; global favorites/search/release/bag; one user acceptance test. Never declare A2 playable or modify a player's existing Hoenn save before those gates.

---

# A2 safe new-game bootstrap design

Status: DESIGN ONLY. Not implemented or playable. Do not modify A1, the E7 emulator release, existing player saves, or enable an E6-tagged game before this protocol passes.

## Invariants

The XGBA E6 extension needs a 16 MiB save file and two exact 24-byte emulator superblocks at 0x20000 and 0xFFF000. The game cannot initialize an unmapped 0x0F window blindly. Current guarded A2 save.c intentionally rejects blank new-game saves and refuses ClearSaveData. An offline converter already handles ONLY intact copied v13 native saves; this new-game design is separate.

## Required steps

1. A **create-only** blank E6 template builder: 16 MiB, all 0xFF except the two emulator E6 headers; no native Flash save, page records, or A2 metadata. Refuse any existing destination or unsafe file path. Test exact bytes.
2. Game-side bootstrap allowed ONLY by an explicit new-game code path, never just because metadata is absent. Bootstrap permission expires after a load, a completed bootstrap, or any fault.
3. Before writing, verify E6 handshake; both native 14-sector save slots fully erased; both metadata records and all page slots erased. Unexpected bytes may indicate interrupted writes or user data and must cause a hard refusal. No auto-erasing damaged saves.
4. Initialize all eight PC pages from actual new-game PokemonStorage: preserve legally encoded box names and wallpapers, clear 420 BoxPokemon slots and four fusion Pokémon on inactive pages. Use the single existing resident 34,144-byte buffer; no second full-page RAM allocation.
5. Write independently checked A/B records for all eight pages with final single-byte commit markers. Then write one complete physical native Flash save slot (initial counter 1 selects physical slot 1), validate all 14 sectors and SHA-256. Commit metadata containing the exact native slot, counter, page index and all eight page generations LAST. Duplicate metadata after one verified commit if needed.
6. Test interruptions at every page header/payload write, all native sectors, the first metadata commit and the second metadata copy. Before a valid metadata commit, never load a fabricated blank game; after it, select exactly the committed cohort. An ambiguous partially initialized save is rejected rather than silently repaired.
7. Update active game save state only after the new cohort is independently selected. If the final metadata byte might have committed but confirmation failed, force clean reload, not another speculative save.

## CI and release gates

- Python create-only template/no-clobber tests.
- Native C init/fault-injection harness plus ARM shadow link.
- Developer-only E6-tagged ROM with recalculated GBA header complement, no production publish. Boot, new game, save, shut down and relaunch in actual XGBA against disposable templates.
- No player tests or migration of their real Hoenn save until these pass.
- Preserve exact A1 256 MiB ROM, revision 0 and 128 KiB native Flash.

Emulator-side first-run provisioning could later replace the offline template, but must not alter accepted E7 until independently validated.
