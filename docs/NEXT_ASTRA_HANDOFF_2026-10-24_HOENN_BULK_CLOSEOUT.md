# NEXT ASTRA: Johto rewards and Hoenn bulk engineering closeout

**Written:** 24 October 2026 (US Central) / 25 October UTC.  
**Canonical repo:** `DevElectrum/grand-anthology` (**PUBLIC**, branch `main`).  
**Last gameplay HEAD before this documentation-only handoff:** `3295e1a6e266923fc6b7335747ad2bdcf38b44fb`.  
**LATEST FULLY GREEN SOURCE + OPTIMIZED BUILD:** [P0 Gameplay Build run 40159598930](https://github.com/DevElectrum/grand-anthology/actions/runs/40159598930), exact commit `3295e1a6e266923fc6b7335747ad2bdcf38b44fb`. Both jobs SUCCESS.  
**Latest independently inspected mGBA run:** [40159186927](https://github.com/DevElectrum/grand-anthology/actions/runs/40159186927), at earlier `05802eae`: ROM built and **nine baseline suites passed**, but workflow FAILED on the **pre-existing Kanto daycare synthetic doorway (3/4)** and **Diancie Granite Cave north-arrow fixture (7/8)**. No same-SHA complete runtime confirmation for `3295e1a6`; do not claim one. Nyx manual gameplay previously confirmed both real doors (daycare breeding including Manaphy+Ditto→Phione; Diancie object/battle).

## Critical user instructions
- **LARGE batches**, not three Pokémon at a time. Target about **10–20 or more ordinary scripted encounters per regional/location-grouped engineering pass**, splitting commits only to avoid API/tool limits or to preserve clean rollback. Implement, validate, then proceed, with brief meaningful progress updates. Do not turn a 1,025-species game into a lifetime of single-encounter checklists.
- **Continue substantial P0 implementation; authorization is active.** Old ledger paragraphs saying "discussion-only / do not access GitHub" reflect the historical planning date and were overtaken by the user's subsequent explicit "Begin / Continue" engineering requests. The design **decisions** in that ledger remain binding.
- Modify **only public `main`**. The older private `DevElectrum/pokemon-grand-anthology` is a historical archive: do **not** read/edit/import/cherry-pick its history.
- Preserve original Kanto/Johto/Hoenn campaigns, trainer/team/PC continuity, story beats, casual pacing, no forced level scaling/reset and no compulsory restart between regions. Global National Dex optimistic source coverage of all 1,025 species is **not** verified real playthrough acquisition. Post-Champion legendaries are fine; no blanket "all pre-first-Champion" rule.
- Legendaries/mythicals default to deliberate **static** encounters (not grass/water), except defensible lore-specific plural/wild cases. Ultra Beasts/Paradox are ordinary rare wild-eligible. Don't invent extra prerequisites. Use **caught Pokédex bits** for cross-species gates; **individual caught flags** for two independently obtainable same-species sources (Kubfu, differing Zygarde forms, etc.).
- Native static lifecycle: **faint -> hides until revisit and respawns; flee/teleport -> remains; caught -> permanently hidden across revisit/save/load**. Run test interpreters and realistic map-path/emulator checks, distinguishing evidence types.
- Defer major special engineering until other ordinary encounter coverage: **Zacian+Zamazenta single joint double battle granting BOTH after victory only** (Eternatus subsequently), **separately catchable permanently Complete Zygarde**, **separately catchable permanently Stellar Terapagos**. Ordinary Zygarde 10%/50% and regular Terapagos are **not** deferred.
- Minimize user testing. Nyx may be asked for a prepared **2–5 minute local-only** emulator test if automated tests cannot settle a concrete issue. Never commit cheats/fixtures or upload ROMs/saves/tokens. No unsolicited code/CI work on the private archive.

## Required read order
1. `AGENTS.md` and **THIS CURRENT handoff**.
2. `docs/LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md` **in full**, especially delegated location closeout at end; late entries override early tentative proposals.
3. `docs/NEXT_ASTRA_HANDOFF_2026-10-24_ACTIVE.md`, `docs/JOHTO_BULK_2026-10-24.md`, `docs/JOHTO_TAO_TRIO_2026-10-24.md`, `docs/NYX_MANUAL_QA_2026-10-24.md`.
4. `docs/PUBLIC_STATUS.md`, `docs/LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md`, `docs/PREIMPLEMENTATION_DESIGN_AUDIT_2026-10-24.md`, then relevant maps/scripts/flags/layouts and Actions run logs.
5. `docs/NEXT_ASTRA_PROMPT.md` is updated to mirror this authoritative state; older handoffs contain valuable history but **not** current next-step status.

## Work actually done since the last Astra handoff

### Johto encounter bulk
Tao trio implemented earlier: Reshiram Tin Tower 2F Lv44 (no Ho-Oh roof beast gate), Zekrom Dark Cave north Lv44, Kyurem Ice Path B1F Lv49 after **both caught**, DNA Splicers upon Kyurem catch, recovery sign if full bag.

Next **11 Johto encounter changes** (plus Enamorus gate): Tornadus Route44, Thundurus Route41, Landorus Route43, Enamorus Route39 after those three caught, TWO independently catchable Kubfu (Sprout Tower 2F and Route34), Spectrier CliffEdgeCave, Glastrier Ice Path B2F, Victini National Park after **any one** regional Champion using `FLAG_SYS_GAME_CLEAR`, Regieleki Olivine Lighthouse after `FLAG_AMPHAROS_HEALED`, Regidrago Dragon's Den. See `docs/JOHTO_BULK_2026-10-24.md` for coordinates/levels. Fixed two blocked tiles (Route34 Kubfu and Dragon's Den Regidrago), adapted static source tests to relocated maps and two individual Kubfu capture flags. Johto Swords earlier placed Keldeo Route32, Terrakion Union Cave1F, Virizion National Park, Cobalion MtMortar accessible west side.

**Johto reward engineering now IMPLEMENTED in source**, not merely planned:
- `06733077e34`: Scroll of Waters awarded after Chuck at Cianwood Gym; Scroll of Darkness reward from Kiyo in Mt Mortar; Reins of Unity hidden/exploration pickup in Ruins of Alph. Remove Kubfu scrolls from broad department-store item offerings; keep both Kubfu/Urshifu outcomes achievable in one save, no scroll exclusivity.
- `f654e842a5`: normalize scroll text escape sequences.
- `027fc9dfcf3`: preserve Burned Tower story callback and update source supply tests for rewards rather than shop availability. Latest green source CI states **Johto Kubfu scroll rewards + Ruins Reins guards PASS**, but actual use, Calyrex horse fusion, save/load still need realistic runtime acceptance.

### Hoenn location-grouped bulk, now authored in public source
Group commits from `81d7adcc8e` through `3295e1a6e2`:
- Ogerpon Route117; Loyal Three **Okidogi Route112**, **Fezandipiti Route113**, **Munkidori Route114** independently after Ogerpon caught; **Pecharunt Route117** after all three caught. Optional early/midgame hunting chain, not compulsory storyline. `3295e1a6` validated source-level capture/faint/revisit/prerequisite sequencing and explicit Route117 transition.
- Volcanion Mt Chimney, Xerneas Route119 (preserve story/event ecology and nearby original items).
- Treasures of Ruin in separate ancient-site environs alongside normal Regi puzzle geography: **Ting-Lu Desert Ruins, Chien-Pao Island Cave, Wo-Chien Ancient Tomb area/Route120, Chi-Yu Sealed Chamber**. Do not overwrite original Regirock/Regice/Registeel.
- Early Jirachi Route102 and regular **base Terapagos Route114** (around Hoenn gym 4, no special Champion requirement). Its battle-form mechanics are not proven by static existence.
- `f6a175042ba` corrected **eight blocked Hoenn static tiles** diagnosed by the inventory collision test. `4d8a44ad921` strengthened route reachability diagnostics. `05802eaeae2` connected Route119 Xerneas and Route120 Wo-Chien to walkable route components. `47e6fca25b4` expanded lifecycle coverage. **Latest** `3295e1a6` source checks verified optional Ogerpon/Loyal3/Pecharunt chain independently.
- At exact `3295e1a6`, optimized 32 MiB GBA production build and all P0 source validators **PASS**. Latest source job reports **87 field-object battle scripts** interpreted for faint/catch/flee/teleport and the Ogerpon hunt prerequisite/save/revisit interpreter **PASS**. This is **not** proof all new statics were traversed/caught on a real emulator.
- Earlier Hoenn CI failures were real blocked event tiles/transition tests, **fixed** at latest green source head. Do not "fix" the already fixed coordinates using old CI logs. Read latest current source.

## Immediate next engineering sequence (substantial, NOT tiny)
1. **Refresh `main` and check latest Actions before editing.** Green gameplay-build baseline is `3295e1a6`, and accompanying mGBA remains a separate incomplete category. Establish that new commits build before moving on. Do not restart every previously resolved source error.
2. **Audit remaining approved Hoenn Legendary/Mythical location gaps as one table, comparing final design vs current authored statics; then implement the next location-grouped 10–20 straightforward encounters in coherent map batches.** Good targets from final ledger: accessible permanent Route111 Hoopa ring chamber despite Mirage Tower collapse, Deoxys Birth Island ferry after Rayquaza caught (without AuroraTicket/post-Champion condition), Eon duo, Hoenn native Regi/Weather Trio gating and separate Ruin coexistence, **Koraidon Route128 Dive grotto after Gym7**, **Miraidon New Mauville independent generator annex after Gym7 and Space Center**, Yveltal, etc. Some species may already have valid source statics: **audit first and avoid duplicating**. For new rooms preserve original fossil choice, Dive/access story, Weather Trio events and neighboring puzzle scripts.
3. Add reusable source assertions for every new static: exact script reference, **real binary tile collision + elevation**, connected reachable component from natural warp, no occupied story/warp tile, no old duplicate object, accurate state prerequisite, catch/faint/retry and non-exclusivity. Bulk validator lives at `test/overworld/ValidateAnthologyHabitatRare.py`; caught-flow interpreter `test/overworld/ValidateAnthologyStaticLifecycle.py`. If the CI diagnostic prints `INVALID BULK TILE`, use all sites at once before pushing a repair.
4. **Strengthen targeted real mGBA coverage** of one representative newly authored static (prefer new Hoenn hunt chain or Kyurem rewards). Existing universal mGBA baseline tests are green except two known **synthetic navigation fixtures**; do not treat baseline mGBA coverage as new encounter gameplay acceptance. Local-only prepared Nyx check when necessary. Protect original saves.
5. Document grouped commits and next state, then continue to next region/location group until ordinary P0 acquisitions implemented. Do not stall on cosmetic alternate forms, per-species telemetry, deferred permanent forms/double battles, Grand Vault capacity, porting, publication, or repeated decision questionnaires.

### Source vs gameplay integrity cautions
- `FLAG_SYS_GAME_CLEAR` is the any-region first-Hall-of-Fame bit; region-specific Champion bits also exist. Victini should open after **any** Championship.
- Enamorus/Pecharunt/Kyurem prerequisites are caught dex, **not party ownership**.
- Kubfu **two independent captured flags**; both scroll rewards in same save. Form and fusion items must survive save and not consume Pokémon.
- Hoenn alternate encounters/forms may require separate capture flags even though National Dex species is shared; don't hide one from caught-dex registration of its other form without intended design.
- User's project is a **casual continuous anthology** of original games with modern mechanics, not a new narrative or mandatory difficulty/level-scaling project.
- The mGBA regression at `05802eae` has old Kanto daycare/Diancie fixture failures; Nyx previously manually passed their real interactions. Preserve as tracked test debt, do not falsely announce all emulator checks pass.

## Evidence links
- [Exact newest fully passing Gameplay Build](https://github.com/DevElectrum/grand-anthology/actions/runs/40159598930), `3295e1a6`.
- [Most recent inspected mGBA failure](https://github.com/DevElectrum/grand-anthology/actions/runs/40159186927), earlier `05802eae`; nine baseline suites pass, only daycare/Diancie synthetic fixture failures.
- Latest source location chain commits: `06733077e34` (items), `81d7adcc8e` (Ogerpon beginning), `00e541417c` (hunt), `1f3a299abf` (Volcanion/Xerneas), `e601eff110`/`8f752db9e0` (Treasures of Ruin), `df95361b8d` (Jirachi/Terapagos), `f6a175042ba` (collision repairs), `05802eaeae2` (reachability), `3295e1a6` (chain verification).

**You have authorization to continue.** Make meaningful engineering progress in large cohorts and commit on public `main`. Ask only about genuinely new, consequential design decisions; don't re-ask the settled ledger. Keep the user briefly informed as you work. **Big steps, better mistakes, even better solutions.**
