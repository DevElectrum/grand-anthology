# P0 before-first-Champion acquisition access audit


## Kanto breeding accessibility repair (October 24 UTC)

The classic FRLG Route 5 daycare raises **one** Pokémon and cannot produce Eggs. Its original two-parent Four Island daycare is gated behind the Kanto League. This is material for the Kanto-origin pre-first-Champion path to **Phione**, which specifically requires breeding a **Manaphy** with Ditto; simply listing Manaphy as a source does not complete a playable Phione breeding route.

Kanto Route 5's existing interior now contains an optional **two-person breeding service** alongside the original FRLG one-parent caretaker. The new lady/man forward into the established Johto **shared** two-parent daycare scripts: compatible pair deposit/retrieval, earned Egg pickup, rejection/full-party safeguards and the usual save-backed storage. No new breeding implementation, save structure, PC slots or party schema are introduced. The legacy one-parent caretaker remains functional. The Route 5 entrance is part of ordinary pre-Champion Kanto travel.

\`ValidateAnthologyEvolutionAccess.py\` now checks the two new NPC event objects, wrapper links to Johto's working handlers, unmodified original caretaker, Kanto location and exits, absence of a Kanto-Champion gate, and the source wiring for the Manaphy+Ditto→Phione breeding branch. **This is source wiring, not an mGBA-confirmed Egg or newly-hatched Phione.** Map object approach tiles, dialog deposit/pickup, actual compatibility, save/reload and birth remain in the consolidated emulator acceptance queue.



## October 24 follow-up: real-time, form and Champion-gate repairs

The [first new acquisition batch](https://github.com/DevElectrum/grand-anthology/commit/4f693f815fe32a985a113772b4b56d4fe8bbaf42) retains ordinary level/time/weather/spin methods while adding **14 purchasable-stone alternatives** across 12 evolution parents: Alolan Rattata, Hisuian Sneasel, Galarian Linoone, Amaura, both Sliggoo forms, both Rockruff variants, Cosmoem (two branches), Snom, Greavard and Milcery. Peat Block also works on Ursaring independent of night. The explicit sources and item usability are checked by `ValidateAnthologyEvolutionAccess.py`. This does not verify party evolution animations or used-item consumption in mGBA.

The [second batch](https://github.com/DevElectrum/grand-anthology/commit/c223458e4b8680b9138e2f138d2377c88e4a920d) introduced eight geographically meaningful supplemental habitats: Galarian Farfetch'd (Kanto Route 8), Galarian Meowth (Kanto Route 11), Galarian Zapdos (Kanto Route 22), Galarian Articuno (Seafoam B1F), Galarian Moltres (Pokémon Mansion 1F), Paldean Wooper (Johto Route 30), Own-Tempo Rockruff (Kanto Route 9) and Alola-cap Pikachu (Kanto Route 6). Original Johto Galarian-bird statics and Hoenn Paldean Wooper Surf remain. Source guards check natural land map requirements, unique slots, default/native tables and DexNav capacity. Galarian Darmanitan's Zen form is a *battle-only* transformation, not a collectible wild form, so the optimistic regional-form report now excludes it.

A third gated-map audit identified three erroneously *countable* pre-Champion areas: **Embedded Tower** (Johto Champion flag at Route 47), **Meteor Falls Steven's Cave** (Hoenn Champion entrance metatiles), and **Altering Cave** (Route 103 opens only after Hoenn Champion). They are now blacklisted from `tools/audit_p0_acquisition.py`'s pre-Champion source closure. In particular, **Groudon** and **Kyogre** were otherwise represented by postgame Terra/Marine Caves or Champion-gated Embedded Tower; they now have rare supplemental Hoenn natural hunts on **Route 111** and in **Seafloor Cavern Room 1**, respectively. The new ecology total is **313** (up from 302), following the Route 115 Klawf relocation below; it is still a source catalog, not recorded catches.

The corrected Cubone syntax regression was conclusively build-verified at [P0 Gameplay Build #40058999139](https://github.com/DevElectrum/grand-anthology/actions/runs/40058999139): **both validation and optimized ROM compilation passed**. The 14-stone batch source validation passed at [#40059747896](https://github.com/DevElectrum/grand-anthology/actions/runs/40059747896); its optimized build was later superseded. The eight-form batch source validator passed at [#40060011972](https://github.com/DevElectrum/grand-anthology/actions/runs/40060011972). **Verify the final code commit's exact optimized build and mGBA workflow before making stronger claims**. Real capture/egg/evolution, story passage, save/reload and encounter persistence acceptance remain open.



**Scope:** source repairs on public `DevElectrum/grand-anthology:main`, October 24, 2026. A source route is not proof of legitimate playthrough acquisition.

## Twelve unreachable inherited regional evolution branches

The inherited Pokémon species tables use `IF_REGION, REGION_ALOLA/GALAR/HISUI` for alternate forms. The actual `GetCurrentRegion()` in `src/region_switch.c` selects Kanto, Johto or Hoenn only. `src/pokemon.c` therefore cannot select those alternate evolutions on level-up or original stone use. The previous base-form rules were retained, and an independent `EVO_ITEM` method was added to each alternative, with stones stocked in all three regional department stores.

| Source Pokémon | Alternate result | Added stone |
|---|---|---|
| Pikachu | Alolan Raichu | Shiny Stone |
| Exeggcute | Alolan Exeggutor | Sun Stone |
| Cubone | Alolan Marowak | Dusk Stone |
| Koffing | Galarian Weezing | Shiny Stone |
| Mime Jr. | Galarian Mr. Mime | Ice Stone |
| Quilava | Hisuian Typhlosion | Dusk Stone |
| Dewott | Hisuian Samurott | Dusk Stone |
| Petilil | Hisuian Lilligant | Dawn Stone |
| Rufflet | Hisuian Braviary | Dawn Stone |
| Goomy | Hisuian Sliggoo | Shiny Stone |
| Bergmite | Hisuian Avalugg | Ice Stone |
| Dartrix | Hisuian Decidueye | Dawn Stone |

These alternatives are deliberately accessible and distinct from normal branch stones or levels. Using a stone triggers `EVO_MODE_ITEM_USE`; the exact species, form, bag transaction and evolution animation still require emulator testing. Subsequent evolution of Hisuian Sliggoo to Hisuian Goodra remains weather/level gated.

## Mainland alternatives to post-Champion Sevii sources

The Kanto National allocation includes Gen IV evolutions of species originally placed on Sevii Islands 4–7, which the original FRLG story unlocks only after a Champion. Optional legacy postgame locations remain intact, but new mainland wild ecology makes the full lines source-reachable before the regional League.

| Parent | New mainland Kanto source | Unlocks National entry |
|---|---|---|
| Aipom | Route 16 | Ambipom |
| Misdreavus | Pokémon Tower 4F | Mismagius |
| Murkrow | Pokémon Tower 6F | Honchkrow |
| Sneasel | Seafoam Islands B3F | Weavile |
| Yanma | Route 12 | Yanmega |
| Swinub | Seafoam Islands B4F | Mamoswine |

Each map already has land wild encounters and a curated rare habitat. The added ecology sources do not replace native species or the original Sevii sources; each chosen map has space among the 12 DexNav land icons and stays below the four-extra-slot limit. The `tools/audit_p0_acquisition.py` location blacklist now excludes other obviously postgame maps from its optimistic reachability closure.

## Evidence and remaining risks

- Source tests: `ValidateAnthologyEvolutionAccess.py`, `ValidateAnthologyHabitatRare.py` and `audit_p0_acquisition.py` passed in [Gameplay Build #40058490395](https://github.com/DevElectrum/grand-anthology/actions/runs/40058490395) validation job.
- The prior [Gameplay Build #40057275807](https://github.com/DevElectrum/grand-anthology/actions/runs/40057275807) succeeded, including optimized ROM compilation. The new head's compilation was still underway at documentation time. No ROMs or saves are distributed.
- Still unverified: per-map walkable grass, all story and script-local gating, actual wild rolls, all twelve stone interactions, breeding, gift bag-full cases, static battle/catch/revisit, region-switch persistence and total in-game Pokédex completion. Do not label 1,025/1,025 as runtime complete.

## Post-audit CI finding and repair (October 24 UTC)

The [`23890d6` optimized build #40058490395](https://github.com/DevElectrum/grand-anthology/actions/runs/40058490395) **failed C compilation** even though all its source validators passed. The corresponding [mGBA #40058490385](https://github.com/DevElectrum/grand-anthology/actions/runs/40058490385) did not reach emulator gameplay. The culprit was a missing comma between the new Alolan Marowak Dusk Stone evolution and the following `EVO_NONE` Totem branch in `gen_1_families.h`; the compiler reported `expected '}' before '{'` at line 13697. The missing comma was fixed in [`5156d50`](https://github.com/DevElectrum/grand-anthology/commit/5156d50bcc71fd2539014f5558a55502224a1df7) with a source assertion, whose escaping was corrected in [`6baa615`](https://github.com/DevElectrum/grand-anthology/commit/6baa615e2413cdaf827523e49256fb133335cd39). Verify the [latest optimized CI #40058999139](https://github.com/DevElectrum/grand-anthology/actions/runs/40058999139) before claiming that this syntax fix compiles. The previous [`63bea16` full build](https://github.com/DevElectrum/grand-anthology/actions/runs/40057275807) was green.

## Source-gate regression caught and repaired: Klawf #950

The first strict run against Champion-locked Steven's Cave exposed an actual Hoenn source gap, **Klawf #950** (optimistic National source graph dropped to 1,024/1,025). Klawf had previously been placed in `MAP_METEOR_FALLS_STEVENS_CAVE`, whose entrance opens only after a Hoenn Champion. Its original late source is retained for optional revisits, and **Hoenn Route 115** supplies an additional pre-Champion cliff habitat. `ValidateAnthologyHabitatRare.py` now guards the new natural entry and the locked-map exclusion. This returns the *candidate* graph to 1,025/1,025 if the final source validator passes; it is not gameplay acceptance.
