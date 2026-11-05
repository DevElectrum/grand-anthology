# Grand Anthology R8.4 final release qualification (experimental)

Status: **ALL AUTOMATED C4/C5 FINAL RELEASE GATES GREEN; NOT FROZEN, NOT MERGED.** Owner release-freeze approval remains outstanding. This is a release-gate ledger, not a claim of a shipped player ROM.

## Immutable compatibility baseline

- Game worktree: `DevElectrum/grand-anthology@xgba/a2-collection-lab`. Never update `main`, Mosaic, or existing user ROMs/saves from this gate.
- Emulator: `DevElectrum/xgba@xgba/r8-4mib-ram`, pinned to `4e0f59185e2c7d7ed5333d8adc20e8d3ad94d05c`. Frozen E7 and XGBA `master` stay untouched.
- Existing baseline evidence: dual PC controller run [41965926821](https://github.com/DevElectrum/grand-anthology/actions/runs/41965926821) passed protected global Clear All refusal, transfer and graphical Save3/cold Continue; real game [41965846161](https://github.com/DevElectrum/grand-anthology/actions/runs/41965846161) passed in-RAM travel for Kanto, Johto and Hoenn, all 3,360 boxed records unchanged, and fresh-process crash recovery; native A1 and A2 contract [41965833487](https://github.com/DevElectrum/grand-anthology/actions/runs/41965833487) passed.
- Experimental C4/C5 implementation is feature complete, but successful *graphical* destructive confirmation and regional *Save3 + cold Continue* must be independently proven.

## Current acceptance evidence (2026-11-05 UTC)

- **C5 regional Save3 and fresh-process cold Continue: PASSED**, [Actions #41975048607](https://github.com/DevElectrum/grand-anthology/actions/runs/41975048607) on game commit `5f48d539367bd00cf269d7a3667123bd09841e12`. The real regional travel, regional Save3+cold step, independent storage audit, torn page7 rollback and torn metadata rollback all completed successfully.
- **C4 successful destructive GUI and Save3: PASSED**, [Actions #41975676730](https://github.com/DevElectrum/grand-anthology/actions/runs/41975676730) at code commit `9b997a0ab2884d785bc47543388e5a4cde6e3868`. **Both Classic and SwSh matrix jobs are green.** Each passed the positive graphical Clear All planner, two explicit Yes confirmations, an actual destructive global release, exact Save2->Save3 full-collection audit (one changed record, 3,359 unchanged), and fresh-XGBA cold Continue. The first attempt confirmed a test-harness error, not a game safety failure: both real dialogs default to **No**. The fixed controller explicitly selects Yes twice.
- **Native A1 and A2 storage-format compatibility: PASSED**, [Actions #41974502402](https://github.com/DevElectrum/grand-anthology/actions/runs/41974502402).
- **Emulator core: PASSED**, [XGBA Actions #41439796163](https://github.com/DevElectrum/xgba/actions/runs/41439796163), 16/16 tests.
- **Performance/compatibility source review: COMPLETE**. Page switching uses two 34,144-byte RAM copies without E6 I/O; test-only 18,000-frame travel bound, completed native A1/A2 jobs and XGBA CPU/DMA/rewind/bounds checks. This does not imply a measured device FPS or smartphone performance benchmark.

## Final release gates (never infer a green check from an implementation commit)

1. **C4 successful graphical destructive release**. Run Classic and SwSh UI on separate disposable saves. An isolated `XGBA_R8_C4_GUI_TEST=1` cartridge keeps one eligible, unmarked Pidgey holding a Potion in global Box56 slot4; the other five fixture species remain protected. Genuine controller input must reach the positive global planner and execute both Yes confirmations. The shared transaction erases exactly the target 80-byte record and returns its item to the bag atomically; an independently audited Save2->Save3 comparison must show exactly one changed slot and 3,359 byte-identical slots. A fresh XGBA process must Continue Save3 and verify the same committed cohort. The negative protected-refusal proof remains separately green. Any failed step blocks the release.
2. **C5 regional Save3/cold Continue**. Start three independent game-generated, audited Save2 clones. Controller-driven transit must enter Kanto, Johto and Hoenn; actual graphical Start Save must commit native Save3 and A2 stage 8 *in the target region*, without altering any of 3,360 boxed records or the 600-byte party. A new XGBA process per region must resume counter 3 in the expected region, with the same party fingerprint and CRC-verified collection SHA-256. The ordinary cross-region RAM-only transit gate remains separate. Never weaken the Hub Box15 Save3 fixture to accept a regional Save3.
3. **Performance and compatibility**. Verify both R8 PC frontend controller jobs and XGBA 16/16 core tests; confirm A2 journal and native A1 compatibility, standard non-R8 build, all R8 opt-in bounds and savestate/rewind tests. Bounded controller/script frame counts are useful engineering diagnostics, **not a measured FPS or host 5x-speed benchmark**. Do not claim measured latency without runtime data.

## Source-derived engineering budget

- R8 maps **4,194,304 bytes** at `0x20000000..0x203FFFFF`. Eight 34,144-byte group images require **273,152 bytes**, approximately **6.51%** of that window before other allocations. Keep these separate from native EWRAM and the on-disk 16 MiB E6 journal.
- `src/xgba_a2_pager.c:XgbaA2PagerSwitch` uses two `memcpy` operations of 34,144 bytes each for a true group switch (**68,288 bytes copied**), then switches page index. The R8 path issues **zero E6 journal reads/writes while browsing**. This is a source-level behavior, not a wall-clock measurement.
- `XgbaA2PagerPrepareSave` reconciles the native resident group, compares cached page generations and writes *changed* groups; it does not need to stage all eight unchanged groups. It scans up to eight page images on a save, not every rendered frame.
- `src/save.c` intentionally scans up to **3,360 slots** to plan/revalidate a destructive global release. This is a user-requested destructive transaction, not a per-frame navigation loop. It preserves exact 80-byte selected source images, validates them again, stages all returned held items in a separate bag, publishes only an all-or-nothing result, then erases selected records.
- Region controller presently applies a **maximum 18,000 emulated frames** to its end-to-end travel gate (including scripted interaction). This is a test deadline and should be tuned based on observed runs, not represented as pure travel latency.
- `DevElectrum/xgba` R8 head [41439796163](https://github.com/DevElectrum/xgba/actions/runs/41439796163) passed 16/16 CPU, DMA, bounds, reset, E5/E6/E7, versioned full-state and rewind suites. That does not certify GUI framerate on the user's actual machine or phone.
- Native A1 and A2 storage source and link compatibility is independently covered by the game `A2-XGBA-Integration.yml` workflow. For the current qualification branch, inspect exact-head results before freeze.

## Release decision protocol

Record an exact **commit SHA + GitHub Actions run ID + job and step results** for every final gate. Any red, missing, or incomplete workflow is **pending/fail**, not accepted. CI must not publish a disposable ROM or user save. Do not merge to `main`, modify frozen XGBA, or start Sinnoh until all release-qualification checks pass and the human owner approves the release freeze. A single optional, focused personal menu-feel playtest may follow automated acceptance; player debugging is not a substitute for automation.
