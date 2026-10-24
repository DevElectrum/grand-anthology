# Pokémon Grand Anthology: scope and verification status

**Status (October 24, 2026): Public experimental source on `main`.**
The *source repository* is public. It is not a finished playable game, ROM distribution or promise of availability.

## What this project is building

Grand Anthology modifies [Pokémon World](https://github.com/evilchinesefood/PKMN-World)
to preserve the original Kanto, Johto and Hoenn adventures under one trainer,
party, Bag, Pokédex, PC and battery save. We aim for a casual, accessible journey,
modernized controls and Pokémon from generations 1–9. Later-generation regions
are outside the current scope.

## Source implemented versus gameplay verified

| Topic | Evidence and limits |
|---|---|
| National species | The October 23 static acquisition audit recognized **1,025/1,025 optimistic source paths** (regional distribution Kanto 346, Johto 352, Hoenn 327) at development code `45dc98d9`. This includes encounters, gifts, evolution and breeding **references**; it does **not** prove all are genuinely obtainable before the first Champion. |
| Specials and forms | Source has many scripted encounters and habitats, but save/revisit/catch/faint and permanent-form graph acceptance is incomplete. |
| GBA build | Initial [public main source/build CI](https://github.com/DevElectrum/grand-anthology/actions/runs/40055403327) passed both source validators and an optimized local ROM build (without distributing its output). Future WIP commits may have red CI. |
| Emulator verification | The first [public mGBA run](https://github.com/DevElectrum/grand-anthology/actions/runs/40055403335) passed title, starter, save and 27/27 party/region travel checks, but failed 5/6 in `WorldDiancieStatic` while traversing the Granite Cave north-arrow doorway: it never entered Steven's room. This has **not yet tested** the actual Diancie object or battle. Public-source availability is unrelated to this WIP test result. |
| PC storage | **420 slots (14 boxes × 30)** currently. A living collection of all 1,025 distinct species cannot fit at once; storage expansion is deferred until other gameplay is complete. Releasing a Pokémon should not erase the Pokédex caught record. |
| Gameplay readiness | Internally tracked provisional P0 readiness remains **37%**. This number is not a public playability certification, and actual campaign-wide acceptance is outstanding. |

The source-path audit is a discovery aid; it is deliberately optimistic.
Full P0 acceptance requires legitimate capture and evolution methods,
pre-Champion reachability, cross-region campaign continuity and actual
emulator save/reload testing.

## Work order

1. Finish the remaining P0 **implementation** without reopening finalized
   design decisions or expanding PC storage.
2. Run consolidated GBA/emulator checks over the three campaigns, static
   encounters, forms, breeding/evolution, randomizer settings and saves.
3. Stabilize a genuinely playable game, then consider optional P1 additions.
4. Defer storage expansion and later gimmicks (Gigantamax/Terastallization)
   until the main game is sound.

These are priorities, not delivery dates. No game download or ROM is
distributed by this repository.

## Latest public P0 engineering (October 24 UTC)

Three substantive implementation batches landed directly on public `main`:

- [`2efe308`](https://github.com/DevElectrum/grand-anthology/commit/2efe30869ff851129018acd8204c2c8b4efbb646) and [`e66fa1b`](https://github.com/DevElectrum/grand-anthology/commit/e66fa1b48a385af64c12b386c43b02e84530e0ec): added offline Linking Cord evolutions for Karrablast/Shelmet; stocked missing Gen 6/9 evolution items and Gimmighoul Coins in all three regional department stores; added an all-generation trade/item source guard (30 trade targets and 44 item types).
- [`44635fd`](https://github.com/DevElectrum/grand-anthology/commit/44635fd44b6c5924e30a2df3d5e638277fdfe1d5): Dipplin naturally learns Dragon Cheer at level 38 for offline Hydrapple evolution; validates 15 move-based evolution species against the configured Gen 9 level-up learnsets.
- [`63bea16`](https://github.com/DevElectrum/grand-anthology/commit/63bea1646b91a055175cf09ca75d302dbbfe320f): World Transit hub shop stocks 17 Arceus Plates, 17 Silvally Memories and 4 Oricorio Nectars; validates item availability and sale prices. This does not address key-item forms, Mega, Gigantamax or Terastallization.

**Build verification distinction:** The [public gameplay build at `20938787`](https://github.com/DevElectrum/grand-anthology/actions/runs/40056497817) passed. Source-validation jobs for the intermediate P0 commits passed, but full optimized GBA builds on those intermediate SHAs were cancelled by the workflow's newer-commit concurrency setting. Check the [latest `63bea16` gameplay build](https://github.com/DevElectrum/grand-anthology/actions/runs/40057275807) for its final outcome rather than assuming a pass. The [mGBA run at `20938787`](https://github.com/DevElectrum/grand-anthology/actions/runs/40056497711) still failed in the Granite Cave doorway fixture after successfully testing title, regional party travel, save/Continue, and settings; no Diancie battle result is established.

These repairs improve source-implemented accessibility, not verified end-to-end Pokédex completion. Remaining P0 includes early map/story gating, actual mart and evolution operations, breeding and static catch/faint/revisit save testing, key-item form mechanics, and consolidated emulator acceptance.

## Source history and attribution

Grand Anthology is a non-commercial hobby modification. The underlying
Pokémon World game and third-party engine, maps, artwork, sound, and tools
remain credited in [CREDITS.md](../CREDITS.md). Respect the original
authors and their component-specific license notices. The project's
documentation does not grant rights to Pokémon intellectual property.
