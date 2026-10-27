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

The header has a keyboard-accessible **Light/Dark** toggle on every page. On first visit the site follows the operating system theme; a manual selection is saved to local storage and persists across navigation and reloads. The theme is implemented entirely in website CSS and small inline scripts, without altering any game graphics.

Both dev and build automatically invoke `scripts/generate.mjs`, which:

- reads `../data/catalog/world_acquisition.jsonl`;
- requires exactly 1,025 distinct National Pokédex entries, 874 item constants and 92 Mega Stones;
- emits `src/generated/catalog.json`, a build-time snapshot of catalog records and source-map groups;
- enriches source-indexed supplemental grass, Surfing, fishing and rare-grass encounters with their *actual native-table level brackets* without mutating `data/catalog/world_acquisition.jsonl`. Grass/Surf use the first native slot; Fishing uses individual rod slot ranges. These are ordinary encounter levels, not Lure-adjusted or randomizer outcomes;
- copies a limited set of **existing** branding art and dated mGBA screenshots into `public/visuals/`;
- copies the minimal Pokémon sprite art from the existing game asset tree into ignored `public/pokemon-sprites/` and generates `src/generated/sprites.json`. Pokédex grid entries use menu icons; profile heroes use a first-frame front sprite. Form selection swaps to available matching form artwork, otherwise explicitly labels a default-form fallback. Animation sheets are *visually clipped to frame one* by CSS and are not edited or converted.

The Randomizer Atlas exports `src/generated/randomizer-data.json` with exact internal species IDs (including forms), configured species eligibility, ordered map group/number IDs, native encounter slots and Oak/Elm/Birch source rosters. `src/lib/randomizer.mjs` mirrors C unsigned-32-bit hashing for Wild and current New Game starter choices, including National-Dex-unique roster collision handling. All 65,536 seeds are supported on demand in the browser, not by storing 65,536 worlds. `npm test` includes the entire seed range for wild hash validity, synthetic form-duplicate tests, source parser checks and the current game-data extraction. Alternate native tables (e.g. Altering Cave) are labeled explicitly. Browser UI supports a unified Pokémon Finder over **both** native wild encounters and all 28 regional starter choices, with region/source filters and link-through to species Pokédex profiles. The Starter Viewer shows sprite-backed full-card Pokédex links. The Location Explorer also shows script-matched static battle candidates, which remain conditional and do not guarantee access, capture or reward eligibility. The Atlas now additionally models current-version **scripted Statics** with all three settings (Ordinary, Legendary Shuffle and Full Chaos). It matches original Pokémon and levels to the real `setwildbattle`/`seteventmon` map scripts, retaining source file, script label, map identity and conditional/no-catch metadata. It separately models the C-owned Royal Duo fight: two combatants, slots 0/1, non-catchable battle, both awarded after victory. Every catalog scripted-battle candidate must match its map script; non-matching sources are reported in the generated `staticAudit` ledger and must not be silently guessed. These are scripted *candidate locations*, not assertions of player access/catch/quest completion. The Atlas does **not** currently claim to handle Trainers, optional Gifts, roaming/outbreak/Feebas mechanics, post-League rotation, all event prerequisites, or old save migrations. Source parity is tested; representative mGBA output checks are still required before claiming emulator acceptance.

The build also extracts configured Pokémon stats, types, abilities, forms, descriptions and evolution data from the game's Gen 1–9 C species initializers, shared Pokédex text, ability definitions and form tables. GCC resolves compile-time conditionals and a balanced initializer parser imports the relevant fields. The separate result is `src/generated/pokemon-details.json`. Generated files and copied visuals are ignored by Git. Keep the authoritative catalog in the game repository; do not edit the generated JSON.

The presentation importer conservatively merges **source aliases for the same physical event**: a special permanent Zygarde/Terapagos static plus its general scripted-battle index, or a named Mega Stone item-ball event plus its generic script reference. It retains genuinely different methods, such as Fishing versus Surfing and Battle Net free starter selection versus purchase. The raw catalog is untouched. Run `npm test` to check the normalization behavior against the current game data.

The extracted stats and biology are source-level game data, not gameplay acceptance. The site does not import outside Pokémon statistics or distribute all battle assets; the chosen existing sprite PNGs are generated for the public website and retain their original franchise/art provenance. Credit and copyright belong to their respective owners. Unsupported conditional details are reported as unavailable rather than guessed.

## Editorial and source accuracy

The catalog is a source-indexed **candidate acquisition inventory**, not a playable-world guarantee. In particular, `scripted_battle_candidate` does not prove a catchable reward. Some recorded maps lack region assignments; the site must not silently infer a region from the audit-region column. A missing item source is not evidence that an item is unobtainable.

A website importer cannot replace actual scripted-event validation or emulator tests. The homepage's AI-generated atlas artwork is **concept art**, not gameplay. The legacy emulator gallery predates the present Grand Anthology title; captures are labeled accordingly.

Do not distribute compiled ROMs, save files, or claim endorsement by Pokémon rights holders. Preserve upstream credits; the referenced Pokémon sprite graphics are fan-project/game-derived assets, not original website artwork, and should not be presented as separately licensed original work.

## Next Atlas engineering stages

**Phase 1 implemented:** source-indexed Wild encounter replacements and unique regional starter choices for current New Game settings; all 65,536 seed numbers supported. **Not yet accepted in an emulator:** spot checks of source-derived Atlas output against real in-game results. **Phase 2 calculation implemented:** three v13 scripted-static policies, script-matched source sites, Royal Duo special constructor, and static searches in the Finder and Location Explorer. **Still pending:** mGBA parity tests for representative conditional encounters, end-to-end quest/story gating, source-site persistence/caught behavior, and auditing special-event constructors beyond the current script-match index. **Phase 3:** optional Gifts and Trainers, and legacy-version compatibility if needed. The ordinary wiki remains independent of the selected seed.

## Cloudflare Pages launch (prepared; account connection pending)

This website is ready for Cloudflare Pages Git integration, but hosting has **not** been activated. Do not claim a public site or give out a domain until the first production deployment succeeds.

Go to [Cloudflare Workers & Pages](https://dash.cloudflare.com/?to=/:account/workers-and-pages) → Create application → Pages → Connect to Git, and select the public `DevElectrum/grand-anthology` GitHub repository. Configure:

| Setting | Value |
| --- | --- |
| Production branch | `main` |
| Framework preset | Astro |
| Root directory | `website` |
| Build command | `npm run build` |
| Build output directory | `dist` (relative to `website`) |
| Node version | 22 or newer; optional `NODE_VERSION=22` |
| Build prerequisites | `python3`, `gcc` (C preprocessing) |

**Keep the full repository checked out.** The website's build uses `../src`, `../data`, `../graphics` and `../.github/assets` even though it executes inside `website`. Do not deploy only the website folder or change all URLs to a GitHub Pages project subpath. No adapter, API server, database or Cloudflare secret is required.

From `website`, run `npm install`, `npm test`, `npm run build` and `npm run audit:deploy` before a production push. The audit verifies internal links and local assets, checks that key pages and metadata exist, and enforces Cloudflare Pages Free's 20,000-file limit and 25 MiB maximum single-file size. The website GitHub workflow runs the same deployment audit after every production build. Cloudflare's 20-minute build timeout also applies.

After publishing, verify `/`, `/pokedex/`, `/pokedex/133/`, `/pokedex/718/`, `/locations/`, `/randomizer/?seed=02026`, `/mega-stones/`, and `/about/`. Check a Pokémon sprite, a regional starter, static shuffle v1 and v2, and light/dark mode. Once the production `*.pages.dev` name is confirmed, link it from the repository README, configure canonical SEO URLs and a sitemap, and optionally set a custom domain. Preview deployments are not production.

The deployed `public/_headers` file provides conservative security headers. We deliberately avoid an untested Content Security Policy: the theme uses inline JavaScript and Google Fonts. `robots.txt` permits indexing, while the small favicon echoes the site's four-point star. Do not confuse website publication with a finished ROM release.


## Legendary Shuffle algorithm v2 and save compatibility

From 27 October 2026, new games set `SaveBlock2.randomizerVersion = 2` and Legendary Shuffle uses **a global seeded Fisher–Yates permutation** over every eligible Legendary/Mythical Species enum entry. The pool includes species from all generations and is ordered by internal Species ID. Initialize a private xorshift32 state using `mix32(0xA511E9B3 ^ seed)` (nonzero fallback `0x6D2B79F5`); iterate from `count-1` down to 1, choose `j = next32() % (i+1)`, swap positions `i` and `j`, and map each original species's pre-shuffle index to the shuffled slot. It is independent of map and encounter order and remains a one-to-one mapping. The game and browser use exactly the same 32-bit operations.

**Existing v13 saves with randomizerVersion 1 retain the old cyclic rotation**, and migrated v12 saves (version 0) keep their existing Ordinary-mode rules. We did **not** increase SAVE_FORMAT_VERSION or rewrite saves: the existing persisted three-bit `randomizerVersion` differentiates algorithms. The website's **Static algorithm version** selector defaults to v2 and supports v1 for existing saves; shareable Atlas URLs preserve the selected version. Wild/Starter mappings and Full Chaos remain unchanged for the same seed, and so does Ordinary mode. A build of the new ROM is required to use v2 gameplay; merely refreshing the website does not change an existing ROM.

The seeded permutation has automated full-pool bijection and legacy-rotation tests; player-visible validation in mGBA still needs a representative fresh v2 New Game test and an existing v1 save reload.
