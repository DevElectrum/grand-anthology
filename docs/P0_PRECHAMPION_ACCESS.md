# P0 before-first-Champion acquisition access audit

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
