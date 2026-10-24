<p align="center">
  <img src=".github/assets/grand-anthology-readme-banner.jpg" alt="Pokémon Grand Anthology: an illustrated atlas spanning a coastal city, traditional countryside and tropical islands" width="100%">
</p>

# Pokémon Grand Anthology

**Three classic regions. Nine generations. One evolving adventure.**

An **unofficial, non-commercial, experimental hobby modification** built upon
[Pokémon World](https://github.com/evilchinesefood/PKMN-World) by David A. and its contributors.

[**Origins**](#our-origins-built-upon-pokémon-world) · [**Features**](#what-grand-anthology-adds) · [**Status**](#development-status-and-verification) · [**Build**](#building-locally) · [**Credits**](#credits-and-acknowledgments) · [**Roadmap**](#development-roadmap)

> **Development preview, not a completed release.** The illustration above is original AI-generated project concept artwork, **not an in-game screenshot**. Grand Anthology is independent of and not endorsed by Pokémon World or the Pokémon rights holders.

## What is Pokémon Grand Anthology?

**Pokémon Grand Anthology** is a personal experiment in extending a classic, two-dimensional Pokémon adventure. Its goal is to preserve the recognizable **Kanto, Johto and Hoenn** campaigns while making Pokémon from **Generations 1–9** available across those three regions, alongside more convenient evolutions, collection systems and player options.

It is **not** an attempt to recreate nine full regions. The current world consists of Kanto, Johto, Hoenn and the connected travel hub inherited from Pokémon World. It is also not a polished, finished ROM hack: several mechanics are still being implemented, reconciled and tested.

## Our origins: built upon Pokémon World

**The foundation belongs to the original creators.** [Pokémon World](https://github.com/evilchinesefood/PKMN-World), developed by **David A. ([@evilchinesefood](https://github.com/evilchinesefood))** and its contributors, is the project that brings the three regional adventures into one GBA game. Its world, story integration, shared systems and substantial content are what make this extension possible.

Grand Anthology is a **separate, unofficial modification**, not an official successor, replacement, or competing claim to that work. We do not seek to undermine the creators, diminish their contributions, or suggest their endorsement. If the original three-region experience interests you, please visit the original project.

Pokémon World itself builds on [RHH's pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion), [pret's pokeemerald](https://github.com/pret/pokeemerald), and work from other community projects, including [Pokémon Heart & Soul](https://github.com/PokemonHnS-Development/pokemonHnS). Their contributions are documented in the preserved [credits](CREDITS.md). For a component-level index of the notices included with the source, see [third-party notices](THIRD_PARTY_NOTICES.md).

## A hobby project built with ChatGPT

This is a **personal, non-commercial hobby project** guided by a Pokémon enthusiast. Its direction, feature priorities and design decisions are set by a human maintainer. Much of Grand Anthology's additional code, integration work, documentation and testing assistance is produced iteratively with **OpenAI's ChatGPT**.

That enthusiast is also, **by his own admission, a coward when it comes to punishing game difficulty**. Grand Anthology is therefore being designed first and foremost as a **casual, approachable adventure**: a place to explore three familiar regions, collect favorite Pokémon and enjoy the journey without making hardcore challenge the price of admission.

**A note for the record:** said enthusiast has also undertaken **multiple Rune Level 1 (RL1) runs in *Elden Ring*** and **will not accept slander** concerning his ability to endure difficult games. He simply prefers his Pokémon adventures without that particular flavor of suffering.

Optional tougher difficulty settings or balance revisions may be explored in the future, but they are not the default design goal. This is an **intended play style**, not a claim that every battle or progression curve has already been tuned or tested.

We say that openly because authorship and provenance matter. The extensive original Pokémon World codebase and its upstream components were **not** created by this project or by ChatGPT. AI-generated code may contain mistakes, and automated checks do not guarantee a reliable gameplay experience. We aim to describe what has actually been verified and correct defects as we find them.

The project exists for enjoyment, experimentation and learning, **not** to compete commercially with or displace anyone else's work.

## Three regions, one adventure

The following foundation is inherited from Pokémon World; Grand Anthology aims to preserve and extend it rather than rebuild it:

| Region | Foundation |
| --- | --- |
| **Kanto** | The familiar FireRed-style journey, rival, gyms, Elite Four and Champion. |
| **Johto** | A Johto campaign with its own locations, characters, gyms, League and associated events. |
| **Hoenn** | The Emerald adventure, its League and the Battle Frontier. |
| **World Transit** | A hub linking the regions, with shared trainer systems and region-specific progression. |

Grand Anthology's intended continuous adventure uses **one trainer, party, bag, Pokédex, PC and save**, preserving the active team when moving between regions while keeping regional badges and story progression distinct. Our preferred journey is **Kanto → Johto → Hoenn**; extensive cross-region and save/reload acceptance remains part of the development work.

## What Grand Anthology adds

The public `main` branch contains meaningful new source-level work, but **source integration is not proof of a playable or complete feature**.

| Area | Current evidence or scope |
| --- | --- |
| **All-generation roster** | All **1,025 National Pokédex species through Generation 9** are included in an optimistic source-level acquisition audit. Actual acquisition of every species has **not** been demonstrated in gameplay. |
| **Ecological encounters** | Additional land, Surf and fishing habitats, plus planned sources for rare, legendary, mythical and other special Pokémon, are being integrated into the three existing regions. |
| **Starter choices** | Expanded professor-starter selection source work covers **10 Kanto**, **9 Johto** and **9 Hoenn** choices. Complete in-game integration remains to be accepted. |
| **Collection and evolution** | Regional Pokédex work, fossil sources, special-form acquisition and practical offline evolution methods are under development or audit. |
| **Player convenience** | Source work includes selectable shiny odds and gameplay options such as randomization, editing and cheats. Individual channels and complete save behavior still need consolidated validation. |

**Current acquisition design (24 October 2026):** all 1,025 National species should be legitimately obtainable **eventually during the full Kanto / Johto / Hoenn adventure, including postgame**, without mandatory online trades. They do **not** all have to be available before the first Champion; ordinary story and postgame gates are fine. Legendary and Mythical Pokémon are intended as scripted statics, while Ultra Beasts and Paradox Pokémon are deliberately rare natural encounters. See the [source location audit](docs/LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md). Source paths and gameplay remain unverified in part.

For an accessible summary of scope, source evidence and unverified gameplay, see the [public development status](docs/PUBLIC_STATUS.md). Development can resume from the [public next-Astra engineering handoff](docs/NEXT_ASTRA_HANDOFF.md); private historical notes and branches are not part of this repository.

## Development status and verification

**Status: public experimental source on `main`, with no completed Grand Anthology game or ROM release declared.**

Source-only publication occurred on October 23, 2026 (October 24 UTC), from a single clean-history initial commit. The [public CI workflows](https://github.com/DevElectrum/grand-anthology/actions) provide source/build and actual emulator feedback; they are **development checks, not a playable-game guarantee**. The initial optimized source/build and validation jobs passed; the initial full mGBA smoke failed on a Granite Cave doorway navigation assertion before reaching the Diancie encounter. It does not prove a Diancie spawning defect.

At the documented **October 23, 2026** source milestone, the acquisition audit reported **1,025/1,025 optimistic National species paths** (Kanto 346, Johto 352, Hoenn 327). The source-validator portion of the recorded private development CI run passed. This is an inventory of possible code paths, **not** a successful in-emulator living or completed Pokédex.

The remaining work includes verifying forms and alternate evolutions, verifying eventual obtainability while preserving legitimate post-Champion access gates, reconciling settings and gameplay integration, and then running representative end-to-end emulator tests for encounters, catching, fainting, revisits, breeding, regional travel, saving and reloading. At development commit `45dc98d9`, the optimized full-ROM build passed. A later genuine mGBA test at `5667f5f` fixed the earlier missing-symbol error but exposed an actual Diancie visibility and field-spawn failure after Save/Continue. A subsequent repair is under emulator review; this encounter is **not** accepted until the repaired build passes. Other representative regional-travel and save tests passed. Independently verify the exact published source snapshot.

A provisional internal engineering-readiness percentage exists in the tracker, but it is **not** advertised here as a measure of playable completeness.

## Screenshots

**Grand Anthology gameplay screenshots are pending consolidated emulator verification.** We will publish captures from this modification when they accurately represent the tested build.

For a preview of the **underlying** game's three-region presentation, see [Pokémon World's screenshots](https://github.com/evilchinesefood/PKMN-World/tree/master/graphics/branding/screenshots/showcase). Those are **upstream images**, not evidence that Grand Anthology's added features are functioning.

## Building locally

This repository contains GBA source, data and build tools; **it does not provide downloadable ROM files**. The current inherited Makefile still names its output `pokemonworld.gba`; that filename is a technical legacy detail, not an assertion that this project is the official Pokémon World release.

You need Git, Python 3, GNU Make, host build tools, `libpng` development headers, and a modern `arm-none-eabi` compiler/binutils/newlib toolchain. The project uses **modern compilation, not agbcc**. See [INSTALL.md](INSTALL.md) for platform-specific setup.

```sh
git clone https://github.com/DevElectrum/grand-anthology.git
cd grand-anthology
make release -j4
```

**Important:** The full-generation roster has previously required the optimized release-style build shown above to fit the GBA ROM size limit. This is **not a guarantee** that every in-progress commit compiles or runs correctly. The optimized GBA build passed on the October 23 source milestone; full gameplay validation remains unfinished. `make modern` is an inherited non-release invocation and should not be assumed to fit this development branch. Check the exact commit's CI and emulator evidence before treating a build as verified. Keep ROM outputs local; never upload them to GitHub Releases, Actions artifacts, or the repository.

For source-level checks and test tooling, consult [INSTALL.md](INSTALL.md) and the project documentation. Never interpret a green static validator alone as full gameplay acceptance.

## Saves, storage and current limitations

The current baseline PC storage remains **14 boxes × 30 slots = 420 Pokémon**. A larger external archive has been explored separately, but it is **not an integrated in-game feature**, and a 1,025-species *living* Pokédex is not currently supported by those 420 slots. National Pokédex caught registration is designed to remain recorded even if an individual Pokémon is released.

Do not assume compatibility with existing Pokémon World or earlier Grand Anthology saves. **Back up original `.sav` files before experimenting**, especially when switching commits, ROMs, or emulators. Save migration, forms, battle state, encounters and multi-region continuity still require broader validation.

Android and other emulator support are goals to check in practice, not guarantees. Details and deferred storage ideas appear in the [public development status](docs/PUBLIC_STATUS.md).

## Feedback and contributions

Thoughtful bug reports and improvements are welcome as this hobby project develops. When reporting a bug, please include the **commit or build used**, emulator and platform, whether the save is new or migrated, steps to reproduce, what you expected and what happened. Use the eventual public source repository's Issues tab when available.

Please avoid submitting copyrighted ROM binaries, ripped game assets without appropriate permission, personal save files containing sensitive information, or secrets. Proposed contributions should preserve upstream attribution and any applicable file-specific license notices. Contributions may be reviewed, revised or declined; there is no promised support schedule.

## Project principles, copyright and distribution

Grand Anthology is an **unofficial, free, non-commercial fan hobby project**. It is not affiliated with, authorized by, sponsored by, or endorsed by **Nintendo, Game Freak, The Pokémon Company, the Pokémon World team, or the other upstream project teams**. Pokémon names, characters and associated intellectual property remain the property of their respective owners.

We intend to respect the original [Pokémon World](https://github.com/evilchinesefood/PKMN-World) project's published practices and the rights and applicable terms of every upstream contributor. In particular:

- **No ROM distribution:** do not publish compiled `.gba` files through this repository, GitHub Releases, GitHub Actions artifacts, or affiliated downloads. Builds are for local use.
- **No commercial use:** the project is not for sale and does not offer paid builds, monetized access, or paid ROM downloads.
- **Attribution preserved:** retain the original project's extensive [CREDITS.md](CREDITS.md), source notices and file-specific third-party license requirements; identify Grand Anthology additions separately.
- **No implied permission:** public source availability, these acknowledgments and this README **do not create a license** to assets or code that the rights holders have not licensed. We will investigate requests and concerns about material and make corrections where appropriate.

The original project has not been verified to publish a blanket license authorizing unrestricted derivative-source redistribution. We do **not** claim such a permission here; anyone considering reuse must examine each source and its terms independently. **Respectful intent is not a substitute for permission.**

## Credits and acknowledgments

Grand Anthology would not exist without the original contributors and the community work they assembled:

- **[Pokémon World](https://github.com/evilchinesefood/PKMN-World):** David A. and contributors, for the interconnected Kanto, Johto, Hoenn adventure and the integrated game foundation.
- **[RHH / pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion):** the underlying modernized Emerald engine, battle systems and extensive community contributions.
- **[pret / pokeemerald](https://github.com/pret/pokeemerald):** the foundational Emerald decompilation project.
- **[Pokémon Heart & Soul](https://github.com/PokemonHnS-Development/pokemonHnS)** and other credited creators: source material, Johto assets, community features and art as documented by Pokémon World.
- **Grand Anthology project direction:** an independent hobby maintainer, with extensive ChatGPT-assisted implementation, iteration and documentation.

**The complete inherited credits and contributor acknowledgments remain in [CREDITS.md](CREDITS.md).** Listing groups here is not meant to replace the individual credits there or imply that any upstream contributor endorses this modification.

## Development roadmap

This is a **plan, not a promised schedule**. The [current project scope](docs/PUBLIC_STATUS.md) takes precedence over older, conflicting roadmap language. The current world's scope remains **Kanto, Johto and Hoenn**.

<details>
<summary><strong>P0: finish the playable core</strong></summary>

- Finish legitimate full-adventure acquisition routes for every National species, including postgame. Do not force all 1,025 before a first Champion.
- Reconcile meaningful permanent forms, branch evolutions, offline requirements and story gates.
- Complete essential player options and preserve original campaigns and regional continuity.
- Perform consolidated emulator acceptance for catching, encounter persistence, breeding, fossils, Pokédex rewards, travel and saves.

</details>

<details>
<summary><strong>P1: additional gameplay and quality-of-life improvements</strong></summary>

- Preserve casual, approachable play as the default design goal; harder difficulty modes or balance revisions, if added, should remain optional.
- Pursue the other approved optional improvements to progression, encounter rates, movement, speed, convenience and statistics without delaying the playable core.

</details>

<details>
<summary><strong>Deferred storage expansion</strong></summary>

- Explore significantly larger, durable storage and search only after the core experience is satisfactory.
- Maintain safe behavior for the original 420 slots and do not treat host-side prototypes as in-game functionality.

</details>

<details>
<summary><strong>Longer-term ideas</strong></summary>

- Preserve unselected suggestions as an exploratory backlog rather than promised features.
- No additional playable regions are part of the current project's target.

</details>

<details>
<summary><strong>P5: final-stage battle-mechanic experiments</strong></summary>

- Consider Dynamax, Gigantamax and Terastallization only after earlier work is satisfactory.
- Their presence in upstream engine code is not evidence that they work in Grand Anthology now.

</details>