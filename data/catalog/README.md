# World Acquisition Catalog: Astra's Search Index

**Canonical repository:** `DevElectrum/grand-anthology`, public `main`. The catalog is a **source-level acquisition index**, not a verified strategy guide, a proof of full-game obtainability, or a substitute for current map scripts.

## The two reader views

- **Astra / machine:** [`world_acquisition.jsonl`](world_acquisition.jsonl). One JSON object per line: a metadata header, exactly **1,025** `kind=pokemon` records in National Dex order, and **874** `kind=item` records for explicitly numbered item constants. The item list includes all **92** defined Mega Stones.
- **Human:** [Grand Anthology Field Guide](../../docs/GRAND_ANTHOLOGY_FIELD_GUIDE.md), which has Kanto → Johto → Hoenn progression, a complete Pokédex lookup, 455 location groups, the 92-stone acquisition list and an item lookup.
- **Public introduction:** [README](../../README.md). Keep its progression and development status synchronized.

## Search examples

From the repository root:

```sh
rg -n '"symbol":"SPECIES_ZACIAN"' data/catalog/world_acquisition.jsonl
rg -n '"symbol":"ITEM_RUSTED_SWORD"' data/catalog/world_acquisition.jsonl
rg -n 'Route27|ROUTE27' data/catalog/world_acquisition.jsonl
```

If `jq` is installed:

```sh
jq -c 'select(.kind=="pokemon" and .dex==888)' data/catalog/world_acquisition.jsonl
jq -c 'select(.kind=="item" and .mega_stone==true)' data/catalog/world_acquisition.jsonl
jq -r 'select(.kind=="pokemon" and (.sources | length)==0) | .name' data/catalog/world_acquisition.jsonl
```

## Format and evidence rules

- `kind="pokemon"`: `dex`, `symbol`, `name`, `audit_region`, `evolves_from`, `sources`, `known_path_status`.
- `kind="item"`: `id`, `symbol`, `name`, `mega_stone`, `sources`, `known_path_status`.
- A source has `method`, usually `map` and `source` (repo path), optionally `form`, `level` or level range, `script`, `flag`, `required`, `detail`, `note` and `gameplay_verified`.
- **`scripted_battle_candidate`** is not necessarily a catchable prize. A battle script can be a temporary opponent. Inspect its completion and reward branches.
- **`evolves_from`** lists source relationships found in species-info files; it is not a full recipe or guaranteed obtainable form. Trade/item/move/level conditions require separate review.
- **`audit_region`** describes how the earlier 1,025-species optimistic coverage audit assigns entries to Kanto, Johto and Hoenn. It is not the only region where the species appears.
- Wild catalog levels are source-table ranges, not guaranteed level windows after randomization or runtime modifiers.
- **`source_candidate_only`** means precisely that. A missing source or item entry is not proof of unavailability: ordinary shop inventories, dynamic gifts, crafting, item drops, all inherited event branches, full-bag paths, and exact navigation may be incompletely indexed.

## Source inputs and maintenance

This index was assembled from current `include/constants/pokedex.h`, `include/constants/items.h`, the Gen 1–9 species-info evolution links, `src/data/wild_encounters.json`, three `src/data/anthology_*_habitats.h` catalogs, map script code-search matches, world Mega Stone item-ball references, `src/battle_net.c`, and bespoke Royal Duo / Mew / Terapagos / Zygarde source paths. **The search extraction is conservative and incomplete for arbitrary script branches.**

When source changes:

1. Read the actual map/script/item/reward implementation and its flags, rather than copying retired placement notes.
2. Update the affected JSONL line's source list and evidence labels. If a record is a candidate, leave it a candidate until gameplay acceptance is recorded.
3. Update the corresponding player's Field Guide entry and map/item indexes, plus README progression when relevant. The JSONL is the canonical machine inventory; do not let the guides drift.
4. Validate **1,025 unique dex entries**, **92 Mega Stones**, valid item IDs and no invented source locations. The independent `test/overworld/ValidateAnthologyMegaStoneAcquisition.py` confirms 92/92 *source-level* Mega paths (30 rematch, 21 vendor, 41 world), not walkability.
5. Consult `docs/ROYAL_DUO_ROUTE27_IMPLEMENTATION_2026-10-25.md` and `docs/PERMANENT_STELLAR_TERAPAGOS_2026-10-25.md` for special cases. **Eternatus remains on Route 44 in present source**, though a Victory Road relocation was discussed.

The human guide is currently a documented rendering of this machine inventory with editorial progression notes. There is **no automatic keep-in-sync build step yet**; future developers must update both views together or add a deterministic generator before relying on automation.

**Do not modify `DevElectrum/pokemon-grand-anthology`**, the private historical archive. Do not upload ROMs or saves.
