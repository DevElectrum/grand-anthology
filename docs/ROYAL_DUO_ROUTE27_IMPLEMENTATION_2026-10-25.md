# Royal Duo: Route 27 Special Double Encounter

## Approved contract
At Johto's Route 27 near the Tohjo Falls eastern exit, Zacian and Zamazenta stand side by side after **all eight Johto Badges**, before the League. Pressing A on **either** object starts **one** double battle against both level-50 Crowned-form wolves. Poké Balls are disabled for the whole battle, including after the first opponent is defeated. The player must win the entire battle; partial defeats, running or losing do not award either. Both opponents return for another try.

Winning awards both actual **pre-battle Pokémon** (same nature, IVs, shininess, moves, personalities and held items) with HP/PP restored. Zacian holds the **Rusted Sword**, Zamazenta the **Rusted Shield**, which already provide the native Crowned battle transformations and Iron Head -> Behemoth move replacements. Gifts enter available party slots, then PC; a **two-free-slot preflight** prevents starting a battle if both cannot be awarded. The two are awarded in one script action and are permanently hidden together after success. No mandatory postgame requirements.

## Implementation
- `data/maps/Route27/map.json`: two visible adjacent objects, local IDs `LOCALID_ANTHOLOGY_ZACIAN` and `LOCALID_ANTHOLOGY_ZAMAZENTA`, at **(61,22)** and **(62,22)**. Both positions are walkable and in the same contiguous passage as Tohjo Falls's east exit, per source-map flood fill.
- `data/maps/Route27/scripts.inc`: one common interaction, eight-badge gate, preflight, double battle, full-win check, simultaneous award, and persistent completion flag `FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED`.
- `src/script_pokemon_util.c`: exact species generated with their signature held items, move-slot Iron Head, and original specimens snapshotted before the fight. Winning copies snapshots into party/storage using the existing `GiveScriptedMonToPlayer` and heals those snapshots first. Static encounter randomization does not replace the two named guardians in this fixed challenge.
- `src/battle_setup.c`: separate scripted **double Legendary wild battle**, with the standard return-from-battle flow. It clears the temporary no-catch flag even on whiteout.
- `include/config/wild_encounter.h`: `WE_FLAG_NO_CATCHING` points to the temporary Royal Duo flag under `ALL_REGIONS`; the flag is set immediately before the battle and cleared during its return callback.
- `data/maps/Route48` and `data/maps/RuinsOfAlph_Outside`: obsolete separate Zacian and Zamazenta objects/scripts removed; original unrelated route events retained. Existing Rusted Sword and Shield item definitions were found, but repository search found **no standalone world, NPC or shop distribution** to remove.
- `tools/audit_p0_acquisition.py`: includes the actual conditional victory reward as a source for both National Dex entries, preserving 1,025-species optimistic acquisition coverage.
- `test/overworld/ValidateAnthologyRoyalDuo.py`: checks both map sprites, identical interaction contracts, eight-badge gate, win-only reward, native Crowned items, old location removal, no loose signature item placements, and walkable map reachability.

## Separate validation and acceptance
Source-level and compiler checks do not prove gameplay. Later disposable-save mGBA testing: before eight badges hidden; after eight both visible; interaction with **either** leads to the **same two-opponent fight**; no Poké Balls, including after first faint; battle both Crowned; lose/run and retry; award exactly the same individual attributes after winning; with 0/1/free slots handle refusal or PC overflow; items equipped and removable; Crowned transformations and Behemoth moves; save/reload keeps both gone. Also check native Route 27 first-steps story and Tohjo Falls passage remain uninterrupted. No original story edits are intended.

Do not confuse this with Eternatus's still-separate source location/gating work. Only Royal Duo is implemented in this batch.
