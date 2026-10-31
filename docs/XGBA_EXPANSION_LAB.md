# Grand Anthology XGBA expansion laboratory

## Updated charter: TWO PROJECTS, ONE EXPERIMENTAL EMULATOR (31 October 2026)

**This supersedes the original isolation-only intent below.** The source transfer is complete. The playable, standard-emulator three-region project is now in independent `DevElectrum/pokemon-mosaic`. The user intentionally retained `DevElectrum/grand-anthology` as the true **XGBA-only** game, not as a branch-based variation of Mosaic. The source currently inherited here still contains only Kanto, Johto and Hoenn; the next goals are entirely new software, not claims of existing features.

**Full target:** 9 actual distinct campaigns and regions (Kanto, Johto, Hoenn, Sinnoh, Unova, Kalos, Alola, Galar, Paldea), designed to add new generations/regions later. Implement contemporary battle rules, moves, abilities, items, form mechanics and any chosen coexistence policies for gimmicks; extensive PC and bag capacity with search, sorting, filters, favorites, safe bulk operations and collection UX; meaningful independent regional progress plus durable saves. A first implementation can stage these incrementally but the ENGINE must be designed for the full target.

**E7 evidence:** the custom emulator at `DevElectrum/xgba` and the Emerald 20-world/256 MiB experiment at `DevElectrum/xgba-emerald-test` both have frozen `xgba/e7-accepted-20261031` baselines. CI and actual gameplay proved the full ROM address window and last byte `0x1FFFFFFF`, with World 20 rendering using assets in the 255 MiB slot. Experimental 16 MiB extension save/112 PC boxes per world were also shown to work in the E6 game. These facts **prove possibilities**, not Grand Anthology compatibility or the capacity required for nine wholly distinct regions. Do not transplant E6 save structs wholesale without a real ABI/map/content audit.

**Next engineering gate:** aggressively audit all source ROM/linker size assumptions, map tables, asset indices, script pointers, battle engine versioning, and current PC/bag/save ABI. Then design a reusable larger-than-32 MiB asset allocator/packer and migration/recovery scheme for the actual nine-region game. Compare static 256 MiB pack, banked content, modular overlays, or possible future larger ROM addressing as justified by measured region content. Implement large integration batches only after accounting for performance, worst-case RAM, interrupted saves, bad CRC/version migration, backup and rollback. Keep regression tests deterministic/branch-specific, with **one focused manual acceptance** after each genuine milestone. Avoid reconfirming the E7 256 MiB emulator experiment.

**Branch discipline:** `xgba/expansion-lab` is the engineering branch. `main` remains an original source anchor and website for now, while `xgba/pre-expansion-20261031` anchors `ee129df0`. No automatic merges, updates to Mosaic, website redeployments, uploaded ROMs or user-save changes. Keep source attribution and noncommercial public status. A separate Git worktree with an independently named test ROM and disposable .sav is strongly preferred. Rebranding of Mosaic is deliberately delegated away from this work.


**Status:** isolated source baseline, not an implemented 256 MiB port.

## Branches and immutable source checkpoint

- Grand Anthology source anchor and existing website: `main`, currently still three-region source. Standard-emulator playable game development is now **separately** in `DevElectrum/pokemon-mosaic`. No experiment changes or automatic merges.
- Exact pre-expansion source snapshot: `xgba/pre-expansion-20261031`.
- New independent source experiment: `xgba/expansion-lab`.
- Both branches were created at `ee129df07e5d8b4ce8d6ebe134fa4399eb921997` from current `main` on 2026-10-31.
- The experimental XGBA emulator and proven 256 MiB Emerald capacity demonstrator live in separate repos, both with frozen `xgba/e7-accepted-20261031` branches. The original E6 16 MiB save functionality is preserved there.

## Boundary of this first step

The user's request here was to **isolate the current Grand Anthology into an expansion test**, not yet to replace its linker, restructure its multi-region resources, convert any saved game or merge functionality into `main`. Do not imply this branch already supports 256 MiB or 2,240 PC boxes. In particular, Grand Anthology's current native PC format is separate from the experimental Emerald E6 format.

**Never edit `main` or merge into it automatically.** Keep all experimentation on this branch until the user explicitly approves production integration. Keep the website, existing three-region gameplay and live saves isolated.

## Next engineering work, when authorized

1. Audit the present source-level ROM limits (linker scripts, literal pointers, GBA header, assets, decompression and save ABI). Check maps/regions and generated layout arrays before choosing a port.
2. Reuse XGBA E7's verified CPU address map (0x10000000–0x1FFFFFFF) and its versioned `XGBA256` cartridge header only when fully compatible. Do not assume Emerald's 20-world model maps 1:1 to the three-region Grand Anthology.
3. Prefer moving a representative **real Anthology asset** beyond 32 MiB, then move large content groups in bulk. Use an audited single manifest/packer with deterministic offsets, overlap, capacity and read-back tests. Keep game startup and hot paths O(1) where possible; avoid whole-ROM scans and unnecessary RAM copies.
4. Analyze exact save compatibility (old 128 KiB flash and any project save extensions) and explicitly choose whether PC expansion is independent of ROM expansion. Do not modify/convert real user saves as part of a ROM-only test.
5. Use a **separate local Git worktree**, ROM basename and disposable save for the experiment. Never build over the installed phone ROM or the user's working `main` tree.
6. Add a dedicated branch-scoped CI workflow for the XGBA experiment, not release CI, and do not upload built ROM binaries. Existing `Gameplay-P0` and website workflows are for other scopes and must not become a vehicle for deploying experiments.
7. Preserve real `main` behavior and make integration optional. Report separately: source implemented, native build passed, XGBA CPU-tested, and in-game verified. User wants large, preplanned increments and one focused acceptance wave, not endless small test cycles.

## Local isolation recipe

In a clean local checkout of Grand Anthology, fetch the remote branches and add a **second working directory** (prefer a local tracking branch, not a detached HEAD). The resulting main and expansion source directories have independent build outputs, generated files and saves. Leave the original tree exactly where it is.

Do not commit copyrighted ROMs, save files or extracted assets. Preserve the public source-only posture, provenance, credits and third-party notices.
