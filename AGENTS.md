> **Current authorized gameplay head, 24 October 2026:** Johto's four independent Swords have been relocated in [`0712fb0`](https://github.com/DevElectrum/grand-anthology/commit/0712fb05627259291c1278b3bdeac34ec4f1e447), with dangling-linker repair [`6621cd8`](https://github.com/DevElectrum/grand-anthology/commit/6621cd852e09038377654611d8bd7a0a36a22db7). [Build #40151749582](https://github.com/DevElectrum/grand-anthology/actions/runs/40151749582) PASSED. [mGBA #40151749580](https://github.com/DevElectrum/grand-anthology/actions/runs/40151749580) passed nine baseline suites; red remains from the previously understood Diancie/daycare synthetic doorway tests. Next implement Tao trio with independent Reshiram/Zekrom and caught-both-gated Kyurem, not more speculative fixture tinkering. See [active handoff](docs/NEXT_ASTRA_HANDOFF.md). Old design-only stop notes below are historical.

> **CURRENT ENGINEERING OVERRIDE (24 October 2026, supersedes every historical design-pause instruction below):** The maintainer subsequently said **“Begin”** and then **“Continue”** after design review. Public `DevElectrum/grand-anthology` **`main` is authorized for ongoing P0 gameplay engineering**, grouped commits, source validation and mGBA checks. This includes the Kanto Giratina/Darkrai/Tapu Lele implementation in [`9ffe40e`](https://github.com/DevElectrum/grand-anthology/commit/9ffe40e4e308b9ae92f1860dbf9be0a37d7eb5cb). Old “documentation only / no tests / wait” notes remain historical, NOT active instructions. Honor the approved [design ledger](docs/LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md), and ask for input only if a consequential design choice is genuinely unresolved. Canonical public source only; do not touch the private predecessor.

> **Current handoff override, 24 October 2026 (newer than the design-pause note below):** The maintainer explicitly authorized **a documentation-only repository update** to publish the [full design ledger](docs/LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md), [pre-implementation design audit](docs/PREIMPLEMENTATION_DESIGN_AUDIT_2026-10-24.md), and [new handoff](docs/NEXT_ASTRA_HANDOFF_2026-10-24_DESIGN_READY.md). **The next Astra may read all public repo source to reconcile the design, but must WAIT for explicit approval before writing gameplay code, altering encounters/maps, running tests/builds/CI, or committing further engineering changes.** Earlier "do not edit docs / GitHub" instructions below refer to the superseded pre-publication discussion pause. See [prompt](docs/NEXT_ASTRA_PROMPT.md). Do not touch the private historical repo.

# Grand Anthology public development instructions

**Canonical repository:** `DevElectrum/grand-anthology`, branch `main`.
**Read [the current next-Astra handoff](docs/NEXT_ASTRA_HANDOFF.md) first.**
This is the NEW public, clean-history, source-only development repository.
Do not import or reveal private predecessor history or modify any unrelated
private repository and its original `master`.

## Goal

**Current override (24 October 2026): DESIGN DISCUSSION PAUSE.** Read this file, `docs/NEXT_ASTRA_HANDOFF.md` and `docs/LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md`, then wait for the user. Do not write code, relocate encounters, run tests, or commit additional changes before explicit user approval. Older implementation-first instructions below describe future workflow, not present authorization.

Build an easy, casual, approachable single-trainer Pokémon GBA adventure
covering the inherited Kanto, Johto and Hoenn campaigns. Do not pretend it
covers nine regions. All 1,025 National Pokédex species should be obtainable **eventually across the complete adventure including postgame**, not necessarily before the first Champion. Source-level 1,025 path evidence is optimistic; actual in-game acquisition/integration acceptance is **not complete**.

**Only after discussion and express user go-ahead**, prioritize substantial P0 implementation in sensible grouped batches,
then consolidated gameplay verification. Commit meaningful changes and
review existing tests; don't stall on individual encounter test cases.
Original 420-slot storage remains for now. Optional later gimmicks and
native port work are deferred. Preserve casual default gameplay and
the current project's original humor, credits and upstream acknowledgments.

## Security, redistribution, and CI

Experimental public **source only**, not a completed game or downloadable ROM.
Do not distribute ROMs, save files, asset dumps, personal data, GitHub
tokens or historical private artifacts in code, Actions, releases or issues.
Maintain original [credits](CREDITS.md) and
[third-party notices](THIRD_PARTY_NOTICES.md). Those notices do not
supply a blanket license for Pokémon IP, code and art.

Use public `main` and the two read-only workflows under
`.github/workflows/` for game-source/build and real mGBA regression.
Generated ROMs are ephemeral on runners and must never be uploaded.
Source validators, compilation and emulator gameplay have different proof
strengths. Green builds are not proof that all 1,025 Pokémon are catchable;
red emulator CI is normal unfinished development, not a publication block.

For specific current priorities and verified CI evidence, read
[docs/NEXT_ASTRA_HANDOFF.md](docs/NEXT_ASTRA_HANDOFF.md) and
[docs/PUBLIC_STATUS.md](docs/PUBLIC_STATUS.md).
