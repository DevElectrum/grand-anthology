# A2 isolated-build verification PASS, Nov 1 2026

GitHub Actions run 41295322213 is GREEN in BOTH storage-format and native-a1-regression. The latter verified original 256 MiB A1 ROM/revision-zero, a FULL separately compiled XGBA_A2=1 ARM build using build/xgba-a2-release rather than reusing A1 objects, and a temporary development 256 MiB E6-tagged A2 cartridge with corrected GBA header checksum, full A1 high-address layout, and intact final ROM probe. CI deleted the temporary E6 ROM and did NOT upload any game binary. The first-save E6 blank template, C journal initializer and interruption matrix also passed. This is *build/format acceptance*, NOT actual XGBA emulator gameplay acceptance. Next needed: emulator boot, New Game, save, exit/reopen and interrupted native/E6 recovery on disposable saves; HOF/special-save support; cross-page search/favorites/bulk release/bag; one user smoke. Keep A1 and real player saves untouched.

---

# A2 packaging/isolation checkpoint (Nov 1, 2026)

Latest source on branch xgba/a2-collection-lab includes the newgame bootstrap, 112-box resident pager, cross-page capture fallback, and a developer-only E6 ROM tagger. The E6 tagger at tools/xgba_a2_rom.py confirms the source is a 256 MiB XGBA256 revision-zero A1-layout ROM and has a valid GBA checksum and last-byte probe, then creates a separate revision-E6 image with only header revision 0xBC and complement 0xBD changed. It refuses overwrites and does not publish binaries. Standalone unit tests prove those two bytes alone change and malformed input is refused.

CRITICAL: Makefile now rejects XGBA_A2=1 unless BUILD_NAME=xgba-a2 and XGBA_A1=1. Prior shadow builds reused build/xgba-a1-release and could retain stale objects with wrong CPPFLAGS. The new full native A2 build is ISOLATED at build/xgba-a2-release with output xgba-anthology-a2-native-release.gba. CI builds regular A1 first, independently builds A2 from a separate object directory, packs A2 high-ROM assets using build/xgba-a2-release/a1-manifest.json, validates E6 header geometry and deletes the temporary dev ROM. Nothing is uploaded, no player save is touched. Run 41295322213 is the first independent-build + developer-tag CI; check its outcome before claiming qualification. Prior build 41294626762 PASS covered guarded newgame save integration but was a same-object shadow build.

Next: after CI pass, run actual XGBA emulator first-run, save, restart and interrupted recovery using only disposable 16 MiB E6 seeds; special Hall of Fame sector policies, favorites/collection search/release, expanded bag and acceptance remain pending. Do NOT merge or publish an A2 gameplay ROM yet. A1 original 256 MiB remains revision 0/native 128 KiB Flash and untouched.

---

# A2 checkpoint: first-save bootstrap implementation (Nov 1, 2026)

Active branch: xgba/a2-collection-lab. Stable A1 and XGBA E7 remain untouched. No playable A2 ROM, player save conversion, or user playtest. A1's own gameplay acceptance is separate.

Last confirmed full ARM success: GitHub Actions 41289711143, covering unchanged A1 256 MiB ROM and guarded A2 PC UI/capture/save coordinator. Subsequent bootstrap run 41294626762 is pending: inspect its A2 shadow compile result before claiming new-game code works in the game. Storage-format and host C bootstrap crash budget matrix PASS on the new source.

Implemented: tools/xgba_a2_storage.py supports create-only blank DEST.sav and audit-blank DEST.sav developer commands, strictly 16 MiB erased E6 with two exact emulator superblocks, no native data or Pokemon, entire-file verification, refusal of existing destinations and symlinked paths. These are developer fixtures, NOT save migration.

src/xgba_a2_bootstrap.c confirms two E6 superblocks, all 32 native erased Flash sectors and the full reserved PC/metadata area are pristine, then writes two independently checked records for 8 pages (14 boxes each) using only 128-byte chunks and the original PokemonStorage memory. Inactive pages preserve encoded names/wallpapers but zero mons and fusions. It writes/verifies physical native slot 1, counter 1, commits the first 56-byte A2 metadata record LAST and independently reselects the coherent global save. Any partial disk write blocks reinit. Native harness covers bad native/extended bytes, zero-byte failures, 15 interrupted-write budgets and successful initial commit. No second 34 KiB RAM allocation.

Under XGBA_A2=1 only, src/new_game.c grants a one-shot first-save authorization from NewGameInitData. src/save.c consumes it, binds E6, invokes bootstrap then starts the pager. Failure disallows repeated speculative writes. The existing A1 build is unaffected; unsupported HOF/direct-sector save variants still fail closed.

Pending: confirm guarded ARM success for this new-game path; verify actual E6 emulator revision/tag; create a distinct development ROM without changing A1; prove newgame/save/quit/reload and interruption rollback on XGBA using disposable template or copied v13 saves; handle Hall of Fame and special saves; global favorites/search/release/bag; one user acceptance test. Never declare A2 playable or modify a player's existing Hoenn save before those gates.

---

# A2 checkpoint: page UI, safe blank pages, 112-box capture routing (Nov 1, 2026)

Branch: xgba/a2-collection-lab. Source as of 5b18818f and this documentation-only commit. No A2 ROM offered to user, no player saves touched. A1 gameplay acceptance remains pending.

- Original full A1 256 MiB ROM and A2 save.c shadow ARM build PASS in GitHub Actions 41281719510. The first ARM build containing guarded SwSh page UI and cross-page capture is run 41288887853. Check result before claiming these new paths compile.
- A2-only Select+L/R when cursor is on SwSh PC box title cycles the 8 pages of 14 boxes. Handler rejects moving mons/items and multi-move. On success the UI is fully reinitialized (icons, cursor, wallpaper, title) after fading; a read fault disables further saving until reload. Titles have /1 through /8 suffixes. Source remains guarded XGBA_A2=1, not yet gameplay-qualified.
- Capture handling in CopyMonToPC, IsPokemonStorageFull, CheckFreePokemonStorageSpace: fast original 14-box path unchanged. Only if resident page is full do routines CRC-validate other pages and inspect 80-byte BoxPokemon records without allocating any second 34 KiB resident PC. Capture can advance to another page before deposit. Any failed read locks saves and refuses to advertise free space.
- Offline converter tools/xgba_a2_storage.py initializes empty pages by cloning original legal GBA box names and wallpapers and clearing ONLY 420 BoxPokemon and 4 fusion Pokemon, rather than blanket-zeroing whole PokemonStorage. Regression test seeds name/wallpaper/fusion bytes into valid native Flash sector; verifies names and wallpaper survive, but Pokemon do not get cloned.
- CI storage-format includes Python corruption/migration, C CRC/pager/native fingerprint/cohort/transaction tests. Native job checks unchanged A1 256 MiB ROM and separately shadow-recompiles save.c, swsh_storage_system.c, pokemon.c, pokemon_storage_system.c in experimental A2 mode. Still no E6-tagged A2 playable release.

Next gates: wait for and FIX latest guarded ARM build; then create safe E6 new-game bootstrap (new-game saves are currently intentionally rejected), handle special Hall-of-Fame save paths, add in-emulator E6 save/exit/relaunch testing on copy-only v13 saves, check PC UI transitions and capture when the active page is full. Expanded search, favorites/release protections across pages, bag expansion remain pending. Never mark A2 complete, merge into A1, or ask user to test a live save until all gates pass.

---

# A2 checkpoint: full save coordinator + guarded game integration (Nov 1, 2026)

Active development branch: xgba/a2-collection-lab. No commit was made to main, the stable A1 expansion branch, or the XGBA emulator.

**Implemented since the previous A2 checkpoint:**
- C cohort verifier checks all 14 logical sectors per native save slot, rotating physical ids, Flash1M additive sector checksums, consistent counter, SHA-256 hash of the exact physical slot, all metadata-pinned PC page generations and the byte-exact native PC against the selected extended page. Newest uncommitted Flash counters are ignored.
- C transaction coordinator stages dirty PC pages, writes *only the alternate* native Flash slot via a callback, independently checks native sectors and SHA, then commits a 56-byte metadata payload using the one-byte final marker. It refuses partial writes and retries without replacing the previous committed state. End-to-end C harness exercises partial native writes, retry, successful commit and rollback after corruption. Python matrix separately checks partial page/native/metadata writes. No user saves involved.
- Performance: normal cohort selection checks newest metadata first, scans only metadata-referenced PC copies, and probes the older cohort only on recovery; a C test enforces under 400,000 bytes of extension reads on an intact initial save. No 16 MiB boot scan. The pager still holds only the original 34,144-byte resident PC; no 112-box array exists in GBA RAM.
- Experimental guarded adapter in src/save.c under XGBA_A2=1. When enabled, it probes the E6 bus, selects the coherent native slot for loading, binds the one-page pager, and routes SAVE_NORMAL/SAVE_LINK/SAVE_EREADER through native+extended commit. It refuses unsupported saves, partial old save entrypoints, and any native-only fallback. It refuses ClearSaveData instead of erasing a protected live file. The A1/default build is completely unaffected. Makefile opt-in XGBA_A2=1 requires XGBA_A1=1.
- The CI storage-contract and native C coordinator tests have passed. Full A1 native 256 MiB ROM/link builds were green on commits 90320ed6 and earlier. A separate shadow A2 ARM compile/link was added to the A2 native CI job on the latest commits; verify its status before claiming it passed. It never publishes an A2 ROM, uploads binaries or touches player saves.

**Remaining release gates:** successful shadow ARM link; audited E6 ROM packaging/revision and safe new-game bootstrap; handle Hall of Fame/special save entrypoints through one global transaction; actual in-game 112-box page controls, durable favorites, protected release, index/search, and bag; runtime emulator restart/recovery with copied v13 save; A1's own gameplay acceptance remains pending. The shadow build **must not be distributed as playable A2**. A2 is not complete. Do not enable tag E6 in the ROM merely because C host tests pass.

**Engineering caveat:** if a metadata commit's final byte persisted but its readback failed, the transaction deliberately returns error and a subsequent save in that session must reselect/reload rather than blindly advancing the stale in-memory counter. Never tell the user that the save succeeded without a verified coherent cohort.

---

A2 2026-10-31 latest addition: src/xgba_a2_fingerprint.c and include/xgba_a2_fingerprint.h implement a bounded guest-side SHA-256 fingerprint of exactly one 14-sector physical native Flash slot (56 KiB), using the game's caller-owned existing 4 KiB flash sector buffer, not allocating a second save. The source matches the offline Python slot-prefix reference for both physical slots; failure to read any sector fails closed. Native unit test test/overworld/xgba_a2_fingerprint_harness.c and CI job storage-format PASS on source b7d6f16f (18 Python scenarios including full transaction crash matrix, plus C journal/pager and SHA tests). Full native ARM 256 MiB A1 link regression is separately running in CI; inspect latest result. Still NOT wired into src/save.c: the save load selector, coherent native+metadata commit, real PC page menu, favorites, bag and playable A2 E6 ROM remain unfinished. The A1 ROM still uses native revision 0 and no real user saves were touched.

# A2 ENGINEERING CHECKPOINT: protected journal, byte-only bus, eight-page pager

As of 2026-10-31, the A2 source is implemented on branch xgba/a2-collection-lab, but it is NOT a playable A2 ROM and it has NOT been tested with real user saves. It is not merged into main, A1 or Mosaic.

- On-disk A2 record format v2: the 16 MiB E6 envelope retains the original 128 KiB native Flash. Each 34,144-byte PC page has two independently CRC32-protected generations. The 56-byte metadata payload records a SHA-256-prefix of ONE specific native Flash save slot, its save counter, active page, slot number and exact generations of all eight PC pages. Two 128-byte metadata A/B copies ensure previous committed references survive incomplete saves. This resolves the previous unsafe whole-128-KiB hash: a partial native write into the alternate slot must NOT invalidate the older committed slot.
- The C journal supports exact-generation reads, staging writes that protect the most recently committed generation, ROM-resident fast CRC32, no-op detection and a one-byte final commit marker. The one-byte fix was demanded by injected crash tests: a partially written multi-byte magic was not an adequate commit boundary.
- The byte-only guest bus probes the exact E6 superblock and never performs 16/32-bit guest save accesses. The pager holds only one existing 34,144-byte PokemonStorage in RAM, detects changes without relying on callers setting dirty flags, stages changed pages and keeps uncommitted generations distinct from metadata-approved versions.
- Host Python CI tests include native-slot corruption/migration, page/meta A/B fallback, interrupted PC writes, each of 14 native-sector writes, partial metadata commit, re-staging after aborted writes, direct-write change detection and guest save bounds. Runs 41272929676, 41272850160 and 41273096767 passed. A full ARM 256 MiB A1 build regression has been added to the same A2 workflow; inspect its latest CI run before claiming that gate is green.
- Not implemented yet: a live game save coordinator that chooses the last metadata-approved native slot during loading, atomically binds new Flash saves to page generation metadata, and blocks saving on any reconciliation failure; the actual in-game page selector; search, favorites and bulk release protection across pages; expanded bag; E6-tagged A2 ROM. A2 cannot be called complete until all those paths and an actual emulator save/restart check pass.

The current APIs in src/xgba_a2_pager.c DO NOT themselves durably commit the game's native Flash; PagerCommit is an acknowledgment after an EXTERNAL global transaction, not a transaction implementation. Do not set the ROM revision to E6 or give real player saves to the converter yet. The original A1 gameplay acceptance remains independently pending.

---

# Anthology A2: Collection and persistence engineering record

**Status:** A2 Stage 1 source implemented and CI green; in-game A2 functionality NOT implemented. Active branch: xgba/a2-collection-lab (based on A1 documentation head cf4cc98d). Protect main, xgba/expansion-lab, the A1 ROM and current player saves.

## Implemented now (not a paper-only plan)

1. tools/xgba_a2_storage.py creates a NEW 16 MiB XGBA E6 save from one intact native v13 save; it refuses target overwrite, mismatched save format, partial sectors, bad region checksum, unexpected native size, or mismatched PC data. Existing test/overworld/SavePatch.py validates the live 14-sector rotating Flash layout, complete active slot, source and exact no-op roundtrip.
2. The native 128 KiB Flash at file offsets 0..0x1FFFF remains unchanged. E6 24-byte superblocks at 0x20000 and 0xFFF000 match the accepted emulator. The initial A2 metadata SHA256-prefix and counter tie a collection snapshot to that exact native save.
3. PC pages occupy file 0x200000..0x28FFFF as 8 pages × 2 A/B records × 0x9000. Each header is 32 bytes with magic, version, page, monotonic generation, byte length, payload CRC32 and header CRC32. Each stores exactly the native 34,144-byte PokemonStorage ABI, including box names, wallpapers and four fusions. Page one copies the native PC byte for byte; other pages start empty and valid. 112 global boxes = 3,360 slots.
4. Metadata uses two 64-byte slots starting at 0x290000. Each stores a 24-byte payload: 16-byte truncated SHA-256 of native flash, native save counter, selected page. The converter writes independent A/B generations to PC pages and metadata, audits the temporary image, fsyncs, then atomically publishes with a create-only hardlink. It optionally relocates a 16-byte RTC trailer unchanged to the end of the 16 MiB E6 image.
5. An offline audit selects the newest **valid** A/B record after verifying both CRCs, recovers a corrupted latest copy via its older backup, and refuses missing double copies, bad extension header, changed native generation or page-one/native mismatch. Both emulator E6 headers must match for the current offline image policy. No real user save was used in tests.

**CI:** A2 XGBA Collection Storage Contract workflow, run 41270716312, 11 synthetic migration/failure tests PASS plus SavePatch ABI check. Source code commit 897468fa; tests+workflow commit 8dd5d61d.

## Next necessary integrated implementation batch

- **Guest runtime / 0x0F byte bus:** Implement a single resident 34,144-byte PokemonStorage page only, CRC32 table in ROM, two copy headers with independent validation, bounded metadata, dirty-page tracking, and a fail-closed E6 capability handshake. No 16/32-bit guest extension access. Do not clone the 20-world Emerald game's 0x380000 sidecar.
- **Native/extended transaction coordinator:** This is the difficult atomicity problem. The initial converter binds page-zero and metadata to the native save SHA; an in-game save will change native Flash. The runtime must advance a coherent native + extension generation and choose a recoverable old/new pair on interrupted saves. Do not simply flush page then call SaveGame, or permanently require the original flash hash after legitimate saves. Use an explicit two-phase commit, recovery journal/superblock, and deterministic crash-point tests that preserve either a fully old or a fully new state. A/B page-level validity alone is not enough.
- **Global 112-box PC:** Retain the 14-box resident PokemonStorage and all 420 native slot APIs; use 8 virtual pages, explicit page switch/capture/withdraw/move hooks, read-your-writes, and a safely defined mapping for native sector 5..13. Preserve BoxPokemon ABI and save migration; never raise TOTAL_BOXES_COUNT to 112.
- **Search/favorites/bulk protections:** Bounded indices, sorted view not moving original mons; persistent protection that moves with record, not box coordinates. Egg, favorites and legendary/mythical protections, and held item return checks must survive page transitions.
- **Bag:** One shared global bag. Decide whether true expandable pocket arrays belong in extension or whether new UI and key-item dedup suffice; make bounded, tested capacity changes with explicit ABI migration.
- **ROM packaging:** Only after runtime and native coupling work. New opt-in make xgba-a2 must emit XGBA256 with software revision E6 and correctly recomputed GBA complement, while make xgba-a1 must keep revision 0 and native 128 KiB Flash. Do not change A1 default output.
- **CI / acceptance:** Use source layout fixtures, C guest/emulator save persistence tests and injected interruptions after each write boundary. One user-facing combined gameplay smoke with a copied save only after all stages pass. A1's own gameplay acceptance remains independently pending. No repeated E7 256 MiB capacity test.

## Boundaries and known design constraints

- Existing native GBA Flash uses dual rotating 14-sector saves. Source includes SaveBlock1 14,752 B, SaveBlock2 3,884 B, SaveBlock3 1,264 B, PokemonStorage 34,144 B, format v13.
- The E6 emulator does not automatically know Anthology's transaction format. 24-byte superblocks activate guest 0x0F access, but do **not** implement atomicity, game-level CRC, schema migration, or in-game page switching.
- EWRAM 256 KiB, IWRAM 32 KiB, native linked ROM 32 MiB remain hard budgets. No full 16 MiB scans at boot or O(3,360 × full-Pokemon) searches per menu frame.
- Keep the original global traveling party, global dex, global bag and global collection. Nine separate regions is not twenty virtual copies of Hoenn.
- A2 is not completed until the in-game 112 boxes, collection UI, bag decisions, safe migration, runtime save/restart/recovery and one user smoke are actually passed.

## Internal commands (developer-only, DO NOT use on actual saves yet)

python3 tools/xgba_a2_storage.py layout
python3 tools/xgba_a2_storage.py init pristine-v13.sav NEW-a2.sav
python3 tools/xgba_a2_storage.py audit NEW-a2.sav

Never run conversion on a live emulator-managed save; always close emulator and work from a backed-up copy.
