# Grand Anthology public development instructions

**Canonical repository:** `DevElectrum/grand-anthology`, branch `main`.
**Read [the current next-Astra handoff](docs/NEXT_ASTRA_HANDOFF.md) first.**
This is the NEW public, clean-history, source-only development repository.
Do not import or reveal private predecessor history or modify any unrelated
private repository and its original `master`.

## Goal

Build an easy, casual, approachable single-trainer Pokémon GBA adventure
covering the inherited Kanto, Johto and Hoenn campaigns. Do not pretend it
covers nine regions. National species reachability across 1,025 IDs has
optimistic static source evidence; practical pre-Champion acquisition and
actual in-game catch/evolution/save acceptance are **not complete**.

Prioritize substantial P0 implementation in sensible grouped batches,
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
