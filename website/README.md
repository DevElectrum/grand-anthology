# Grand Anthology website

The public site is an independent Astro + TypeScript project inside the canonical game repository. It never edits gameplay data. This is the **first website foundation**, not a published game release.

## Local development

From the repository root:

```sh
cd website
npm install
npm run dev
```

Astro requires Node.js 22.12 or newer. The Pokémon reference-data extractor also requires Python 3 and GCC's C preprocessor (`gcc`). Visit the URL printed by the development server. For a production build run `npm run build`; output is written to `website/dist/`.

Both dev and build automatically invoke `scripts/generate.mjs`, which:

- reads `../data/catalog/world_acquisition.jsonl`;
- requires exactly 1,025 distinct National Pokédex entries, 874 item constants and 92 Mega Stones;
- emits `src/generated/catalog.json`, a build-time snapshot of catalog records and source-map groups;
- enriches source-indexed supplemental grass, Surfing, fishing and rare-grass encounters with their *actual native-table level brackets* without mutating `data/catalog/world_acquisition.jsonl`. Grass/Surf use the first native slot; Fishing uses individual rod slot ranges. These are ordinary encounter levels, not Lure-adjusted or randomizer outcomes;
- copies a limited set of **existing** branding art and dated mGBA screenshots into `public/visuals/`.

The build also extracts configured Pokémon stats, types, abilities, forms, descriptions and evolution data from the game's Gen 1–9 C species initializers, shared Pokédex text, ability definitions and form tables. GCC resolves compile-time conditionals and a balanced initializer parser imports the relevant fields. The separate result is `src/generated/pokemon-details.json`. Generated files and copied visuals are ignored by Git. Keep the authoritative catalog in the game repository; do not edit the generated JSON.

The presentation importer conservatively merges **source aliases for the same physical event**: a special permanent Zygarde/Terapagos static plus its general scripted-battle index, or a named Mega Stone item-ball event plus its generic script reference. It retains genuinely different methods, such as Fishing versus Surfing and Battle Net free starter selection versus purchase. The raw catalog is untouched. Run `npm test` to check the normalization behavior against the current game data.

The extracted stats and biology are source-level game data, not gameplay acceptance. The site does not import outside Pokémon statistics or publish all game sprites. Unsupported conditional details are reported as unavailable rather than guessed.

## Editorial and source accuracy

The catalog is a source-indexed **candidate acquisition inventory**, not a playable-world guarantee. In particular, `scripted_battle_candidate` does not prove a catchable reward. Some recorded maps lack region assignments; the site must not silently infer a region from the audit-region column. A missing item source is not evidence that an item is unobtainable.

A website importer cannot replace actual scripted-event validation or emulator tests. The homepage's AI-generated atlas artwork is **concept art**, not gameplay. The legacy emulator gallery predates the present Grand Anthology title; captures are labeled accordingly.

Do not distribute compiled ROMs, save files, or claim endorsement by Pokémon rights holders. Preserve upstream credits and review artwork provenance before expanding website usage.

## Planned, not yet implemented

The dedicated Randomizer Atlas will prioritize **Wild, Static, and Starter** seeded placements. It must reproduce the exact game's integer hashing, species-enumeration order, map and encounter slot identities, eligibility filters, static mode, and algorithm version, and be verified against the game before returning placement results. Gifts and NPC trainers are later priorities. The static wiki does not alter itself according to randomizer settings.

The website has no hosting/deployment configured yet. Do not claim a public site or download exists.
