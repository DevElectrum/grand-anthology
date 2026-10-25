# Next Astra: Z-A Mega Stones, Split Acquisition Design (2026-10-25)

**Canonical repository:** `DevElectrum/grand-anthology` (public) on `main`; **never touch** archived private `DevElectrum/pokemon-grand-anthology`. The most recent gameplay implementation before this documentation handoff was Complete Zygarde `12f99767f0154fe664b72fd3c2e18936f628938e`.

## Read first

1. `AGENTS.md` for current pointers/invariants; don't assume its older October 24 head hashes are new.
2. **`docs/ZA_MEGA_STONE_ACQUISITION_LEDGER_2026-10-25.md`**. This is the **current active design discussion**.
3. `docs/ZYGARDE_COMPLETE_ROUTE101_IMPLEMENTATION_2026-10-25.md`.
4. `docs/LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md` for previously agreed encounter design.
5. `docs/NEXT_ASTRA_HANDOFF_2026-10-24_ACTIVE.md` and `docs/NEXT_ASTRA_HANDOFF_2026-10-24_HOENN_BULK_CLOSEOUT.md` for prior engineering status.
6. The actual `src/battle_net.c`, `src/data/script_menu.h`, `data/maps/RegionHub_2F/scripts.inc`, `include/constants/items.h`, `src/data/items.h`, `src/data/pokemon/form_change_tables.h`, and `include/config/species_enabled.h`.

## Most recent user words, crucial design intent

"I think we should split them. Some should be available in battle net vendor, but the lower ones like victreebel and clefable and shit can just be placed else where no?"

**The very next reply should continue that discussion.** Don't dump the 45 stones in a shop. Do not implement an arbitrary 45-stone allocation from the ledger as if signed off; it is explicitly a **draft for discussion**. Give thoughtful suggestions about which stones go in the world, the vendor, or special/late rewards, and ask any consequential design questions **before** implementation. Avoid reopening already settled major Pokémon project design choices.

## Ground truth from this session

- `P_MEGA_EVOLUTIONS=TRUE` and `P_GEN_9_MEGA_EVOLUTIONS=P_MEGA_EVOLUTIONS`; Legends Z-A / Mega Dimension forms are included in the project's enabled config.
- **45 Z-A Mega Stones, IDs 829–873**, exist as constants/item definitions/form-change references.
- Battle Net's existing **10-stone, 2-page vendor** and **30 signature older stone rematch rewards** have **no Z-A Mega Stones** in their item sets. Searching several Z-A constants revealed no ordinary shop/gift/pickup paths. Distinguish data/cheat availability from actual normal gameplay, and avoid asserting every dynamic path was exhaustively ruled out.
- `Start → CHEATS → Give Item` can make arbitrary items via IDs; `ITEM_ZYGARDITE=852`. Not a genuine story source.
- Vendor redesign requires synced changes to the source table, both 5-row menu pages, script navigation, and bounds assertions. The menu's index safety had previous bugs, so avoid sloppy extra pages.
- Proposed categories and thematic placement ideas are in the ledger. They are NOT approved except user agreement to a distribution **split** and their Clefable/Victreebel examples.

## What just happened before this discussion

- **Mew's Nine Masks:** manually accepted in graphical mGBA; capture restrictions and retry flow worked. The user's save had to be given genuine Kanto badges after an initial incorrect patch. Documentation `docs/MEW_NINE_MASKS_IMPLEMENTATION_2026-10-25.md`.
- **Zygarde 10%** (Petalburg Woods Lv10, Aura Break) and **50%** (Meteor Falls Lv44, Power Construct with transient Complete) already have encounters coded; not manually gameplay-verified.
- **Independent permanently Complete Zygarde** source-coded at Route 101 (5,16), Lv70, only after Hoenn Champion, its own caught flag; no build/emulator test yet by the user's explicit request. It should stay Complete in party/PC/battle/save, which needs verification. Mega Zygarde and Zygardite are in the engine, but no normal-game Zygardite source found. **These tests were expressly delegated to the next Astra**; coordinate efficiently instead of demanding protracted manual testing. Full details in Zygarde doc.
- The old Zygarde Cube likely allows 10% to gain Power Construct, contradicting approved Aura Break-only design. Flag for correction; do not let this derail Mega Stone planning.

## Next actions

1. **Discussion first:** review the 45-stone draft, balance early thematic exploration vs optional Battle Net progression, use story-relevant rewards for a few notable stones. Prioritize Clefablite/Victreebelite as visible, accessible world rewards. Decide if 45/45 should have ordinary obtainable paths; design against needless grinding.
2. Once agreed, write approved source-of-truth table with exact source / map+coordinate or NPC / prerequisite / availability timing / one-time flag or repeatability. Allow user to approve or alter consequential assignments. No blind edits.
3. Only upon authorization, implement in meaningful batches with safe, readable shop menus, explicit gameplay pickup/gift paths, full-bag recovery, no moving item IDs, and validated progression.
4. Zygarde capture/form tests: deferred to next Astra, keep brief and preferably automated or with prepared saves. Don't claim Complete persistence without actual evidence.

**No Z-A stone distribution implementation or new tests were made by the preceding Astra.** This handoff only records confirmed source findings and proposals.
