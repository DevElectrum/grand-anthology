# Grand Anthology XGBA expansion laboratory

**Status:** isolated source baseline, not an implemented 256 MiB port.

## Branches and immutable source checkpoint

- Playable canonical game and website: `main`. No experiment changes or automatic merges.
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
