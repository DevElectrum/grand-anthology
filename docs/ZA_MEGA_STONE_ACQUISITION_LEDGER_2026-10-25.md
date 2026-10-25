# Legends: Z-A / Mega Dimension Stone Availability and Distribution Ledger

**Date:** 2026-10-25. **Repository:** public `DevElectrum/grand-anthology`, `main`. **Status:** read-only acquisition audit + **unapproved design ideas**. This ledger changes **no gameplay** and records **no newly placed stones**.

## Why this exists and what the maintainer just said

After the Complete Zygarde source implementation, the maintainer asked whether the Z-A Mega Stones were **available to players in the game**, rather than merely **defined in source**. The audit found that the Mega forms/items exist, but ordinary Z-A stone acquisition is not wired into the current Battle Net economy.

When Astra suggested simply adding all 45 to the vendor, the maintainer corrected the direction:

> "I think we should split them. Some should be available in battle net vendor, but the lower ones like victreebel and clefable and shit can just be placed else where no?"

**Binding design direction:** Split these Mega Stones between in-world discovery and the Battle Net (and potentially special thematic rewards). **Clefable and Victreebel are user-given examples of stones that can be placed elsewhere.** **NOT approved:** specific placements, prices, number per category, badge prerequisites, progression gates, or the other item-to-category assignments below. Discuss and let the maintainer decide before making consequential gameplay changes. Their ideal game is casually approachable and exploration-friendly, with minimal grinding and no forced team resets.

## Verified source facts vs in-game evidence

1. `include/config/species_enabled.h`: `P_MEGA_EVOLUTIONS TRUE`; `P_GEN_9_MEGA_EVOLUTIONS P_MEGA_EVOLUTIONS`. The Z-A and Mega Dimension Mega form data is compiled into this configuration.
2. `include/constants/items.h`: **45** sequential Mega Stone constants, IDs **829–873** inclusive: 26 base Z-A additions (829–854), 19 Mega Dimension additions (855–873).
3. `src/data/items.h` provides item definitions (Mega Stone hold effect, icon/text, item pocket). `src/data/pokemon/form_change_tables.h` provides corresponding Mega Evolution references. `SPECIES_ZYGARDE_MEGA` is reachable in form data from `SPECIES_ZYGARDE_COMPLETE` while holding `ITEM_ZYGARDITE` (852). This is **data/config availability**, **not confirmed gameplay access**.
4. `src/battle_net.c` has exactly **10 older stones** in `sVendorStones[]`, and **seven older starter choices** in `sStarterStones[]`. The hard-rematch `sBattleNetRewards[]` awards older signature stones, up to 30 one-time reward guard flags; **none of these tables contains the 45 Z-A item constants**. Pricing for the old vendor uses 20- and 35-Shard tiers.
5. `src/data/script_menu.h` and `data/maps/RegionHub_2F/scripts.inc` have **two menu pages, five purchasable stones each**, page-local indices 0–4 and page-toggle/Exit rows 5/6. `BattleNetVendorStoneFromChoice` hardcodes 5 rows/page, max 2 pages, and asserts exactly 10 stones. **Changing the list alone is unsafe.** Add any pages/scrolling together with matching menu routing, guarded indices, item mapping, prices, and tests.
6. `FEATURES.md` describes the real Mega Ring + Shard economy: after a regional championship, RegionHub 2F Director grants Mega Ring and starter stone; hard leader/league rematches supply Shards and older stone drops; BP can be exchanged for Shards. This is a designed in-game feature; its current Z-A inventory is empty.
7. Search of the public source for several Z-A constants, including `ITEM_CLEFABLITE`, `ITEM_VICTREEBELITE`, `ITEM_GRENINJITE`, `ITEM_ZYGARDITE`, `ITEM_RAICHUNITE_X`, and `ITEM_HEATRANITE`, found definitions and form references, **not** normal-game giving, pickup, shop, or reward references. **No ordinary Z-A acquisition paths identified in the source audit**, but exhaustive proof for every possible dynamic item source would need code/data traversal and gameplay verification. Do not overclaim a tested impossibility.
8. Player-facing `Start → CHEATS → Give Item` uses a numeric ID and `AddBagItem` (`src/debug.c`); Zygardite can in principle be requested as **item 852** there. **Cheats do not count as ordinary acquisition.** Distinguish three concepts: item data exists, cheat can generate it, normal adventure can obtain it.

## Full 45-stone roster, with *tentative discussion-only* channels

Every bucket below is **Astra's initial conversation starter**, NOT a placement decision. The player has only approved the **split principle** and specifically suggested ordinary world distribution for Clefable/Victreebel.

| Item ID | Stone constant | Suggested channel ONLY |
|---:|---|---|
| 829 | `ITEM_CLEFABLITE` | World / NPC candidate |
| 830 | `ITEM_VICTREEBELITE` | World / NPC candidate |
| 831 | `ITEM_STARMINITE` | World / NPC candidate |
| 832 | `ITEM_DRAGONINITE` | Battle Net candidate |
| 833 | `ITEM_MEGANIUMITE` | Battle Net candidate |
| 834 | `ITEM_FERALIGITE` | Battle Net candidate |
| 835 | `ITEM_SKARMORITE` | World / NPC candidate |
| 836 | `ITEM_FROSLASSITE` | World / NPC candidate |
| 837 | `ITEM_EMBOARITE` | Battle Net candidate |
| 838 | `ITEM_EXCADRITE` | Battle Net candidate |
| 839 | `ITEM_SCOLIPITE` | World / NPC candidate |
| 840 | `ITEM_SCRAFTINITE` | World / NPC candidate |
| 841 | `ITEM_EELEKTROSSITE` | World / NPC candidate |
| 842 | `ITEM_CHANDELURITE` | Battle Net candidate |
| 843 | `ITEM_CHESNAUGHTITE` | Battle Net candidate |
| 844 | `ITEM_DELPHOXITE` | Battle Net candidate |
| 845 | `ITEM_GRENINJITE` | Battle Net candidate |
| 846 | `ITEM_PYROARITE` | World / NPC candidate |
| 847 | `ITEM_FLOETTITE` | Battle Net candidate |
| 848 | `ITEM_MALAMARITE` | World / NPC candidate |
| 849 | `ITEM_BARBARACITE` | World / NPC candidate |
| 850 | `ITEM_DRAGALGITE` | World / NPC candidate |
| 851 | `ITEM_HAWLUCHANITE` | Battle Net candidate |
| 852 | `ITEM_ZYGARDITE` | Special / late reward candidate |
| 853 | `ITEM_DRAMPANITE` | World / NPC candidate |
| 854 | `ITEM_FALINKSITE` | World / NPC candidate |
| 855 | `ITEM_HEATRANITE` | Special / late reward candidate |
| 856 | `ITEM_DARKRANITE` | Special / late reward candidate |
| 857 | `ITEM_ZERAORITE` | Special / late reward candidate |
| 858 | `ITEM_RAICHUNITE_X` | Battle Net candidate |
| 859 | `ITEM_RAICHUNITE_Y` | Battle Net candidate |
| 860 | `ITEM_CHIMECHITE` | World / NPC candidate |
| 861 | `ITEM_ABSOLITE_Z` | Battle Net candidate |
| 862 | `ITEM_STARAPTITE` | World / NPC candidate |
| 863 | `ITEM_GARCHOMPITE_Z` | Battle Net candidate |
| 864 | `ITEM_LUCARIONITE_Z` | Battle Net candidate |
| 865 | `ITEM_GOLURKITE` | World / NPC candidate |
| 866 | `ITEM_MEOWSTICITE` | World / NPC candidate |
| 867 | `ITEM_CRABOMINITE` | World / NPC candidate |
| 868 | `ITEM_GOLISOPITE` | Battle Net candidate |
| 869 | `ITEM_MAGEARNITE` | Special / late reward candidate |
| 870 | `ITEM_SCOVILLAINITE` | World / NPC candidate |
| 871 | `ITEM_BAXCALIBRITE` | Battle Net candidate |
| 872 | `ITEM_TATSUGIRINITE` | World / NPC candidate |
| 873 | `ITEM_GLIMMORANITE` | World / NPC candidate |

**Bucket counts of this *proposal only*:** world/NPC 22; Battle Net vendor 18; thematic/late special 5. Do not infer the user approved those numbers. Change every row freely during discussion. All 45 must eventually have a genuine ordinary-game path unless the player decides otherwise.

## World discovery concepts to discuss, not implement yet

- **Clefablite:** Mt. Moon / its outskirts, in a reachable side nook or via a moon-themed NPC. Avoid replacing the original fossil story or Legendary static.
- **Victreebelite:** Kanto's botanical/Celadon environs, a plant-focused trainer/NPC reward or off-path visible pickup, with a reasonably early-to-mid Kanto opportunity.
- **Starminite:** a coastal/water-focused nook or fisher reward, without requiring postgame on principle.
- **Skarmorite / Froslassite:** region-appropriate mountain/cold-route exploration; choose accessible routes on their respective timelines.
- **Scraftinite / Malamarite:** distinctive trainer gift or themed urban/port encounter.
- **Falinks / Drampa / Chimecho / Toxtricity-type lower tier examples:** use direct in-world finds or small side challenges rather than a Shard tax. (Only actual stone names in the roster are candidates; these are thematic suggestions.)
- Hidden pickups are okay sometimes, but don't bury all easier stones behind random invisible tiles or the Itemfinder; visible sprites, clear NPC clues, and rewarding detours are better for a casual anthology. Preserve native items/warps and do not pile several gimmick rewards on a single early route.
- World stone placements should be spread over **Kanto, Johto and Hoenn** where sensible, considering when the associated Pokémon is actually obtainable. Stone availability does **not** require arbitrary possession of that Pokémon.

## Battle Net and special-reward concepts to discuss, not implement yet

- Battle Net suits high-demand starter stones (Meganium, Feraligatr, Emboar, Chesnaught, Delphox, Greninja) and some stronger/sought-after Megas. It should remain an **option to acquire**, not a mandatory grind for every weaker form.
- Review whether the existing starter-stone free-choice deserves a broader three-region starter roster before redesigning it; avoid changing the initial seven choices silently.
- Offer a modest price ladder (for example existing 20/35 Shards, perhaps an extra 50 tier for high-end forms) **only as a proposal**. Do not silently introduce 100-Shard grinds when hard rematches yield 1–2 Shards. Check actual player effort against acquisition stage.
- The existing rematch signature-stone rewards are gameplay-progression-linked. Preserving them avoids accidentally invalidating the Battle Net's /30 records or running out of stone flag bits.
- **Zygardite** is a strong thematic candidate for a post-Champion guardian encounter reward or nearby Route 101 follow-up, tied to **permanent Complete Zygarde**. That is **NOT approved** and requires an explicit decision on whether catching it should grant the stone, whether there should be a late-game shop alternative, and how full-bag/one-time flags work. **Do not retroactively change the already implemented Complete catch or gate Mega behind catching Doggo/50%** without approval.
- Heatranite, Darkranite, Zeraorite, Magearnite might fit dedicated lore/quest rewards better than a generic stall. **Do not create new elaborate hunts unless desired.**

## Engineering constraints for later approved implementation

- Build a table-driven, maintained **45/45 acquisition ledger** with one or more explicit sources per stone and no duplicates/softlocks. Classify actual normal-game path separately from cheat availability.
- If adding overworld/NPC rewards: use one-time persistent flags or native item-ball handling; enforce full-bag retry; verify no existing triggers, pickups, static encounters, map entry warps, or lore quests are overwritten.
- If adding vendor goods: extend `src/battle_net.c`, `src/data/script_menu.h`, and `data/maps/RegionHub_2F/scripts.inc` **together**. Respect screen height/menu indexes and the already corrected page-local input guard. Preserve the existing 10 old stones and current economy.
- Verify 45 items have proper gItems entries/icons and associated form changes in the enabled build; a constant alone is insufficient. Do **not** renumber existing item IDs, change save layout, or introduce gratuitous save migrations.
- Check Mega Ring acquisition and use for normal gameplay; no promises of every new Mega being usable without actual in-game validation.
- No permission to implement the speculative placement categories was given in this exchange. Next Astra should **discuss choices first**, then work in meaningful regional/batch groups once explicitly authorized.

## Related milestone handoff

The separately catchable **permanent Zygarde Complete** source implementation is committed, final fix `12f99767f0154fe664b72fd3c2e18936f628938e` (Route 101 west clearing, level 70, Hoenn Champion flag, independent catch flag). It was **not built or manually tested** in that implementation pass, by player request. See [Complete Zygarde handoff](ZYGARDE_COMPLETE_ROUTE101_IMPLEMENTATION_2026-10-25.md).

Zygarde 10% (Petalburg Woods, Lv10, Aura Break) and 50% (Meteor Falls, Lv44, Power Construct) are also source-implemented but **not yet manually accepted**. The older **Zygarde Cube** implementation may let 10% gain Power Construct, bypassing its intended Aura Break restriction; flag for later repair with explicit scope approval. Do not conflate that with the Complete boss.

**Mew Nine Masks** was reported manually working by the player on Nyx (Tapu Bulu → Volcanion → Rayquaza → catchable Mew, successful eventual capture). Its QA record is [here](MEW_NINE_MASKS_IMPLEMENTATION_2026-10-25.md).

**No code, ROM, maps, save files, or tests were changed for this ledger.**
