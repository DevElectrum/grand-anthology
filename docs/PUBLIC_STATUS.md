# Pokémon Grand Anthology: scope and verification status

**Status (October 24, 2026): Experimental source development.**
Not a finished game, public release, or promise of availability.

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
| GBA build | The optimized full-roster GBA build has passed at previous development commits. A public WIP source snapshot is not a promise that every commit builds; CI may be red while development continues. |
| Emulator verification | Targeted regional starter, party, cross-region travel and flash-save tests have previously passed; other tests, including Diancie field/battle integration, have exposed unresolved failures. This is active game development, not a completion gate for making clearly labeled source available. |
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

## Source history and attribution

Grand Anthology is a non-commercial hobby modification. The underlying
Pokémon World game and third-party engine, maps, artwork, sound, and tools
remain credited in [CREDITS.md](../CREDITS.md). Respect the original
authors and their component-specific license notices. The project's
documentation does not grant rights to Pokémon intellectual property.
