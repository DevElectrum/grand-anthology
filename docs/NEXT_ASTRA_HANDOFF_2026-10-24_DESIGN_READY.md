# Next Astra: Design Ledger Handoff and Read-Only Engineering Reconnaissance
**Date:** 2026-10-24
**Canonical public repository:** DevElectrum/grand-anthology
**Branch:** main

## First and absolute priority: understand the current design

The maintainer has explicitly authorized **this documentation update and repo handoff** after a long design discussion. This permission **does not authorize gameplay implementation, source/map/script changes, tests, CI runs, issues, PRs, or other engineering changes**. The next Astra may **READ** the current public repository thoroughly, compare it to the decisions, report findings, ask consequential questions only where truly necessary, and **WAIT for an explicit go-ahead before implementing**.

Read in this order:

1. [AGENTS.md](../AGENTS.md)
2. [This handoff](NEXT_ASTRA_HANDOFF_2026-10-24_DESIGN_READY.md)
3. [FULL discussion design ledger](LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md) **(read ALL THE WAY THROUGH; later dated sections supersede earlier sections)**
4. [Read-only source Legendary/Mythical location audit](LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md) **(historical authored state, not present gameplay proof)**
5. [Pre-implementation DESIGN audit](PREIMPLEMENTATION_DESIGN_AUDIT_2026-10-24.md)
6. [Current handoff index](NEXT_ASTRA_HANDOFF.md) and [next-Astra prompt](NEXT_ASTRA_PROMPT.md)
7. [README.md](../README.md), [FEATURES.md](../FEATURES.md), [PUBLIC_STATUS.md](PUBLIC_STATUS.md)
8. [Historical P0 pre-Champion audit](P0_PRECHAMPION_ACCESS.md) **(superseded target, don't reinstate it)**.
9. Relevant actual source: scripts in data/maps, special encounter scripts, wild encounter and research selection, roamers, region-specific flags, items, abilities/form changes, multi-region party/PC, and save/PC persistence. READ ONLY until go-ahead.

**Security:** Do not touch the private predecessor DevElectrum/pokemon-grand-anthology or reproduce its history. Public source only; no ROMs or save files committed or released.

## Human maintainer's non-negotiable vision

- Preserve familiar **Kanto, Johto, Hoenn** original adventures and transit with a single persistent trainer, party, box/save system, and the choice to carry an overpowered team between regions. No compulsory region-based level reset, scaling or new-game restart. Casual play by default.
- **1,025 National species obtainable eventually during the complete adventure, including postgame**, not all before the first Champion. Old source graph “1,025/1,025” is optimistic, not proven playable, not a mandate to stuff pre-League maps.
- Legendary/Mythical encounters are principally **deliberate static encounters**, with lore-based exceptions only where multiple wild individuals make sense. Ultra Beasts and Paradox Pokémon are treated as rare **natural wild species**, including intentionally early **Kartana in Viridian Forest**, and Poipole's route/starter sources. Do not undo those.
- Distribute exciting discoveries naturally throughout journeys, with a few lore-linked caught-Pokédex prerequisites. Do not arbitrarily lock powerful Pokémon late just because they're strong.
- No mandatory Legendary roaming. Preserve original story cutscenes where appropriate; convert acquisition to stable statics.
- Static behavior: catch = permanently resolved; faint/flee = repeatable upon leaving and reentering, including across saves. No permanently missable captures due to incidental story map closure; protect original cutscenes, puzzles and quests.
- Maintainer often requests **discussion first**, rather than coding from speculative assumptions. Avoid restarting settled design debates or grinding isolated synthetic emulator tests.

## Kanto: condensed settled design

- Native Articuno Seafoam, Zapdos Power Plant, Moltres Mt. Ember; Mew Pokémon Mansion, **Mewtwo Cerulean Cave only after Kanto Champion + Mew caught**. Remove pre-League Mansion Mewtwo duplicate; retain existing Sevii/Celio cave restriction.
- **Shaymin Viridian Forest before Brock**. Lake spirits + Heatran Rock Tunnel, **Regigigas Rock Tunnel after the three Hoenn Regis caught**. Dialga and Palkia Mt. Moon; **Giratina Mt. Moon B2F after both caught**. Cresselia Mt. Moon; **Darkrai Lavender Tower 7F after Cresselia caught and Fuji rescued**. Manaphy Seafoam; Phione by Manaphy+Ditto breeding, actual Egg gameplay not yet demonstrated. Arceus Indigo Plateau exterior after Champion and creation trio caught.
- Two Cosmog statics (Mt. Moon and Seafoam) permitting Solgaleo and Lunala. Tapu Koko Power Plant; Bulu Route 5; **Lele Celadon gardens**; Fini Seafoam. Necrozma **Cerulean Cave 2F postgame**; Type: Null Mansion. **Magearna replaces original Silph Co Lapras gift** at level 25 after rival battle, while Lapras stays obtainable by Surf on **Route 20** and via the inherited Icefall Cave encounter. Marshadow Fighting Dojo after Karate Master, preserving the Hitmon gift. Meltan **Vermilion construction site early level 15**; Melmetal level 40 with Metal Coat. Zeraora Power Plant.
- Five erroneous Legendary wild supplemental habitats to remove/reconcile: Groudon Route 111, Kyogre Seafloor Room 1, Galarian Zapdos Route 22, Galarian Articuno Seafoam B1F, Galarian Moltres Mansion 1F. **Separately inspect** global post-League discovery rotation / Research substitutions; don't indiscriminately delete game systems.

## Johto: condensed settled design

- **Celebi Ilex shrine** early, **Zarude another Ilex static**. Awaken beasts via native Burned Tower event, then **Raikou Route 38**, **Suicune Lake of Rage after Rocket Hideout**, **Entei deep Waterfall Mt. Mortar**. **Ho-Oh Bell Tower only after all three caught**; Lugia stays Whirl Islands.
- Victini National Park **after any ONE regional championship**, not all three. Swords independent: **Keldeo Route 32**, **Terrakion Union Cave**, **Virizion National Park**, **Cobalion accessible west Mt. Mortar**. Tornadus Route 44; Thundurus Route 41; Landorus Route 43; Enamorus Route 39 after all three caught. Meloetta Ecruteak Dance Theater; Genesect Goldenrod workshop.
- Reshiram **lower Bell Tower** / Zekrom **Blackthorn-side Dark Cave**, around 7–8 badges, independent. **Kyurem Ice Path B1F after both caught**, before Champion; DNA Splicers rewarded; Glastrier occupies separate Ice Path B2F. Native story gate for Bell Tower must be checked read-only before engineering.
- **Zacian+Zamazenta together in a special dual battle** at Route 27 near Tohjo Falls after gym 8: win by defeating both to be awarded **both** simultaneously, not ordinary catch-ball encounters; losing or only one beaten awards neither. **ENTIRE MECHANIC VERY LATE IMPLEMENTATION PRIORITY**. Eternatus hidden Victory Road cavern after both wolves obtained; Eternamax later TBD.
- Regieleki Olivine Lighthouse after Ampharos is healed; Regidrago Dragon's Den. Calyrex grassy Route 36; Glastrier Ice Path B2F; Spectrier Cliff Cave Route 47; **Reins of Unity Ruins of Alph** discovery allows both horse fusions. Two early Kubfu **Sprout Tower** and **Route 34 beside Day-Care**, scrolls **Chuck** (Water) and **Mt. Mortar Karate King** (Dark) to allow both Urshifu types. Both styles accessible same playthrough.

## Hoenn Gen 3, 6, 9: condensed settled design

- **Original three Regi ruins/puzzles unchanged**. Rayquaza accessible/catchable during native pre-Champion story, **Groudon and Kyogre postgame after Rayquaza caught** independently; do not lose original Sootopolis event. Latias static Route 120 near pond; Latios static Mossdeep Space Center. **Jirachi level 8–10 at Route 102 pond before gym 1**. **Deoxys Birth Island original triangle puzzle after Rayquaza caught**; Lilycove ferry access then, no Aurora Ticket/post-Champion S.S. Ticket restriction.
- **Diancie Granite Cave Steven's Room after two Hoenn badges and delivering his letter**, around level 18. Xerneas Route 119 forest (gym 5–6), Yveltal Mt. Pyre exterior (gym 6). Hoopa **permanent Route 111 Mirage Tower base ring chamber** at ~gym 4 that cannot vanish with the fossil tower; Volcanion Mt. Chimney (~gym 4–5).
- Zygarde 10% **Petalburg Woods**, early level 8–10, **Aura Break only**. Zygarde 50% **Meteor Falls**, ~6–7 badges; **Power Construct immediately usable**, triggers temporary Complete transformation at half HP and reverts after battle. **Third, separately catchable PERMANENT Complete Zygarde** Route 101 near Littleroot **after Hoenn Champion**; all three can coexist. Permanent Complete implementation deliberately **VERY LATE**. Normal ability-change items must not circumvent form restrictions.
- Koraidon **Route 128 Dive grotto**, Miraidon **separate sealed New Mauville annex**, both around gym 7 and independently catchable; preserve existing Wattson and Space Center quests. Treasures of Ruin are added near Regi historical sites without interfering with puzzles: **Ting-Lu Desert Ruins**, **Chien-Pao Island Cave**, **Wo-Chien Ancient Tomb**, **Chi-Yu Sealed Chamber (requires Dive)**. Their statics must be in adjacent chambers if original puzzles would otherwise be disturbed.
- **Ogerpon Route 117 grove catch triggers optional hunt**; then catch **Okidogi Route 112**, **Fezandipiti Route 113**, **Munkidori Route 114** in any order; then **Pecharunt Route 117 berry grove** unlocked. Badges 2–5, no huge DLC story, no forced party.
- **Regular Terapagos** around gym 4 in a new **Route 114 crystal grotto**. A **second, separately catchable permanent 700-BST Stellar Terapagos** in **Cave of Origin deepest chamber** after Hoenn Champion (reopen cave without harming story). Both simultaneously ownable. Permanent Stellar will need custom form/ability/move/save battle logic and is **VERY LATE** engineering priority, despite regular turtle's early-mid acquisition.

## Engineering priority separation: do not confuse with player unlock timing

1. **Only after explicit user go-ahead:** inspect code and prepare targeted encounter/location/dedup/dependency change list. Do not code or run CI during initial reconnaissance.
2. Once authorized for actual engineering: normal scripted static encounters, story and map access, wild habitat cleanup and post-League Research/discovery filter, native gifts/evolution/held-item/scroll/ferry flags. Implement in substantive **geographic batches**, preserving original game scenarios.
3. Representative gameplay checks: legitimate traversal; static catch/faint/flee/revisit/save; party full/PC overflow; evolving/fusing; the few gating prerequisites. Source validators/builds alone do not certify those.
4. **Last, after every ordinary Legendary/Mythical fix:** (i) Zacian+Zamazenta special award-on-dual-victory encounter; (ii) permanently Complete Zygarde; (iii) permanently Stellar Terapagos. User explicitly deferred all three; DON'T let them derail work earlier. Tera-wide features, Gmax/Dmax and expanded PC storage remain separately postponed.

## Explicit evidence and uncertainty guardrails

- The 2026-10-24 full ledger documents progressive decisions; read later revisions as superseding earlier proposals, even if an early heading says “unresolved”.
- Selected map locations are **design targets**, not claims a ready-to-use cave room or warp already exists. Route 114 crystal grotto, Koraidon Dive grotto, New Mauville annex, Hoopa ring chamber and Cave of Origin reentry may require new map or script support.
- Do not conflate per-encounter completion with national species caught registration; Zygarde/regular+Stellar Terapagos/multiple Cosmog and Kubfu require independent encounters.
- Distinguish prior source snapshot from current runtime truth: historical optimized CI passed, nine broad emulator suites passed, but daycare and Diancie synthetic map navigation failed **without actually testing their target NPC/encounter**. Neither full Phione breeding nor all 1,025 acquisitions are proven.
- Story/progression conflicts to examine: Bell Tower access versus Reshiram placement; birth/ferry availability; Mirage Tower disappearance; Regi room overlays; Cave of Origin reentry; Pokémon Mansion duplicate Mewtwo; Rayquaza/Groudon/Kyogre native scripts; park and forest NPC/tile collisions; old roaming paths.

## Instructions for first response in next conversation

Confirm you have read the public repo, **all sections** of the full design ledger and both audits. Report the most meaningful discrepancies between current code and agreed design, grouped by region/feature, distinguishing documented proposal, source evidence, and unverified gameplay. Present a concise recommended batch plan. **Stop and wait for the maintainer's explicit implementation authorization**. Do not ask to relitigate decisions already settled, and do not silently code because an older file says “resume immediately.”

This is a documentation handoff only. No gameplay code was modified when this file was created.
