> **LATEST HANDOFF, 24 October 2026: DESIGN LEDGER NOW PUBLISHED ON MAIN.** START WITH [the complete 2026-10-24 design-ready handoff](NEXT_ASTRA_HANDOFF_2026-10-24_DESIGN_READY.md), then read the [full user discussion design ledger](LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md) to its end, the [older authored source inventory](LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md), and the [preimplementation design audit](PREIMPLEMENTATION_DESIGN_AUDIT_2026-10-24.md). The later ledger sections supersede older tentative placement notes and old questions below. User authorized **these documentation commits only**. Next Astra should **read the current repository/source and report discrepancies, then wait**. **Do not start gameplay implementation, builds, emulator tests, or CI until separately requested.** The earlier "don't edit GitHub/docs" phrasing below is historical and is superseded only for these explicitly requested documentation updates.

## **CURRENT STOP-AND-DISCUSS HANDOFF: 24 October 2026 (overrides older action plans below)**

**STATUS: DOCUMENTATION / DESIGN REVIEW ONLY. DO NOT START WORK.** The user requested an explicit pause so the next Astra can read the repository, the [Legendary/Mythical location audit](LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md), and these design decisions, then **wait for discussion and authorization**. Do **not** implement, rebalance, relocate species, alter scripts, create tasks/issues/PRs, commit anything, run test or build suites, or launch CI. Do not assume old instructions below to "continue P0 engineering" are presently authorized. Reading source/documents and answering questions is allowed. No new gameplay code has been changed as part of this handoff.

**Corrected P0 acquisition rule:** All **1,025 National species** should be legitimately obtainable **eventually during the entire three-region adventure, including postgame**, without needing unavailable online trades. We previously misinterpreted this as all 1,025 before defeating a **first** Champion and made source code additions, audits, alternate evolutions and early rare placements to maximize that metric. This was *not* intended as an ironclad early availability rule. Preserve sensible natural story and late-area progression. The former "1,025/1,025 pre-Champion" audit is **historical/optimistic only**, no longer an implementation target.

**Newly clarified special-Pokémon rule:** **Legendary and Mythical Pokémon should be scripted **static encounters**, not ordinary random grass encounters.** They do **not** require an elaborate bespoke quest or post-Champion lock merely for being special. **Ultra Beasts and Paradox Pokémon are expressly exempt**: they count as rare *natural* Pokémon, even on early maps. **Kartana in Viridian Forest was specifically chosen by the user. Poipole on Route 1 is fine.** Do not silently remove or move either. Nothing else should be altered without explicit discussion.

**Read-only audit results needing discussion:** [`docs/LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md`](LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md) lists **94 distinct National Legendary/Mythical species across Gen 1–9** and their authored locations/methods; it identifies **five supplemental wild Legendary entries** (Groudon Route 111, Kyogre Seafloor Cavern Room 1, Galarian Zapdos Route 22, Galarian Articuno Seafoam B1F, Galarian Moltres Pokémon Mansion 1F), plus a **post-League every-third-wild-battle discovery system** whose whitelist includes Legendary/Mythical species. All remain in source until the user decides. The audit is source-level, not exhaustive playthrough proof; static event accessibility/gates may be unverified.

**Questions to bring to discussion, not to decide alone:** How to treat those five wild Legendary duplicates and the post-League universal wild-discovery shortcut; whether already-authored early static encounters are acceptable; and how to prioritize remaining P0 integration versus consolidated gameplay testing. The current live optimized build and nine broad emulated gameplay suites passed, but source-level all-species closure does not establish real in-game collection. The two late daycare/Diancie synthetic warp tests failed **before entering their target rooms**. A test-only debug map warp is **only a proposal**, not authorized engineering. Existing debug Warp-to-Map machinery can be reviewed but not modified now.

**Critical handoff instruction:** **Read `AGENTS.md`, this handoff, `docs/NEXT_ASTRA_PROMPT.md`, `docs/LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md`, `README.md`, `FEATURES.md`, `docs/PUBLIC_STATUS.md`, `docs/P0_PRECHAMPION_ACCESS.md`. Summarize what you understand and WAIT for the user to discuss decisions. Do not start implementing anything.**

---

# Next Astra: Grand Anthology after the PUBLIC source cutover

## Most recent validated public-main CI, 24 October UTC

The latest code commit [`10bf90d`](https://github.com/DevElectrum/grand-anthology/commit/10bf90dd12446547f469b3aaa9bc58abbb669a0f) **PASSED both independent source validations and the full optimized GBA compile** in [Gameplay Build #40066826520](https://github.com/DevElectrum/grand-anthology/actions/runs/40066826520). The optimistic 1,025/1,025 National source graph and 313 supplemental habitats remain green. The [paired mGBA Release Smoke #40066826492](https://github.com/DevElectrum/grand-anthology/actions/runs/40066826492) succeeded in generating the release Lua symbol profile: `sGlobalScriptContext` was found with its compiler-renamed `sGlobalScriptContext.lto_priv.0` name. It passed **all nine earlier broad gameplay suites again**: title 37/37, Cheats 8/8, party-gift/save 15/15, Elm 34/34, Oak 34/34, Birch storage refusal 26/26, shiny odds 26/26, new Hub 4/4 and three-region party/save continuity 27/27.

Both late independent probes **failed to reach their target map through their synthetic Save/Continue fixture**, with no target interaction attempted:
- `WorldKantoDaycare.lua` **3/4**: real flash Continue restored outside Route 5 at (23,26) (map 37/23); repeated Up input from this artificially restored position travelled north through the Route 5/Cerulean border to **map 37/11 at (23,0)**, rather than activating the daycare building's (23,25) warp. It never initialized interior map 51/0 or tested the three actor interactions, so don't infer a missing breeder.
- `WorldDiancieStatic.lua` **5/6**: synthetic Continue restored Granite Cave 1F but its north-arrow approach stayed in map 24/7 rather than transitioning to Steven's Room 24/10. No Diancie encounter/battle was exercised.

**Do not spend the next engineering batch endlessly adjusting either doorway fixture.** The grouped P0 implementations are source/optimized-build verified, but runtime wild catch, new daycare, Egg generation, evolutions, encounter persistence and ordinary story passage remain unproved. Continue broad functional implementation and consolidated representative gameplay; a later robust fixture should arrive through genuine regional progression or another validated in-engine map transition, not a synthetic SaveBlock1 map rewrite that leaves stale map/object state. Do not equate green build with completed Pokémon acquisition.



## Latest blocked release smoke and corrective follow-up (24 October UTC)

At [`53aa59b`](https://github.com/DevElectrum/grand-anthology/commit/53aa59bda8493879a3299f00ea1f30f63e861959), the optimized ROM compile **succeeded** in [P0 Gameplay Build #40066330963](https://github.com/DevElectrum/grand-anthology/actions/runs/40066330963), but source validation **failed on an obsolete test-text assertion** for the phrase `real flash save` after the fixture comments had changed. All preceding P0 audit checks, including 1,025/1,025 optimistic numbered paths, passed. The source guard is corrected to assert the actual `stageAtRoute5Door()` procedure instead of that brittle prose. [Same-code mGBA #40066331018](https://github.com/DevElectrum/grand-anthology/actions/runs/40066331018) **compiled optimized ROM and mGBA**, but **stopped before running any gameplay** at lean Lua release-symbol export: the `static` `sGlobalScriptContext` C symbol was no longer present under its bare name in the LTO ELF. Do not conflate this with Route 5 gameplay failure. Release symbol lookup now permits an unambiguous `.lto_priv.N` alias and, if LTO truly erases it, exposes zero with an explicit warning; the NPC script assertion safely fails closed rather than aborting all test startup.

The corrected [P0 mGBA workflow](../.github/workflows/P0-mGBA-Release-Smoke.yml) independently executes **both** late probes (Kanto two-parent daycare actor/dialogue and the existing Diancie cave fixture), collecting failure status without hiding the second result. Critical title, starter, travel and save suites remain fail-fast. This work does **not** claim a completed Egg creation/hatch, and no ROM/save is published.



## October 24 real-mGBA daycare fixture failure and correction

The [full P0 Gameplay Build #40061225603](https://github.com/DevElectrum/grand-anthology/actions/runs/40061225603) at `d8a8cef` **PASSED both source checks and optimized compilation**. Its paired [release mGBA #40061225598](https://github.com/DevElectrum/grand-anthology/actions/runs/40061225598) passed the nine existing broad title/cheat/party-gift/Oak/Elm/Birch-refusal/shiny/first-boot/three-region continuity suites, but **failed in the newly added WorldKantoDaycare fixture** before reaching the older Diancie test. The temporary Save/Continue had been relocated **directly inside Route 5's daycare**, leaving the old World Transit ObjectEvents at coordinates (13,10)/(14,10) and without a third actor, exactly the stale-object-template pitfall warned about by the Diancie fixture. The smoke also referenced `S.sGlobalScriptContext`, missing from the lean `--title` Lua symbol profile, causing a Lua error. Neither observation proves an actual Route 5 NPC spawn bug in normal travel.

**Corrective implementation:** stage the disposable flash Save/Continue **outside the Route 5 house** (map group 37, index 23), then walk through its real exterior doorstep warp at (23,25) into the daycare interior (group 51, index 0). Only assert the three new/live map NPCs after that actual world transition. Export the existing `sGlobalScriptContext` global into release Lua symbols to distinguish script interaction from mere tile collision. The acquisition validator now checks the pair of map-group IDs, correct exterior doorway warp and release-symbol contract. **Wait for exact corrected commit's CI and mGBA outcome before claiming success.** A further obstruction might indicate genuine wrong actor placement; use emulator results to decide. The old Diancie warp remains a separate issue, not evidence of Diancie broken battle code.




## Latest real mGBA evidence + Kanto breeding acceptance target (24 October UTC)

The [mGBA release run #40060366495](https://github.com/DevElectrum/grand-anthology/actions/runs/40060366495) from [\`f971217\`](https://github.com/DevElectrum/grand-anthology/commit/f971217f898af09fd58a178fbaf1f20a82d629ac) **compiled the real optimized game and reached emulator tests**. **All broad release suites passed**: title 37/37, cheats 8/8, global gift Save/Continue 15/15, Elm 34/34, Oak 34/34, Birch full-PC refusal 26/26, shiny odds 26/26, fresh hub 4/4, three-region party roundtrip and Save/Continue 27/27. **Diancie 5/6 failed exactly at the previously documented Granite Cave north-arrow warp**. The fixture remained on map 24/7 rather than entering Steven's room 24/10; no Diancie object or battle outcome was reached. Do not misreport this as a broken Diancie encounter.

The [Kanto Route 5 breeding NPC commit \`f07dff1\`](https://github.com/DevElectrum/grand-anthology/commit/f07dff10f557f93afd30dd82c87edb3ec9453170) **passed BOTH source checks and complete optimized compilation** in [P0 Gameplay Build #40060800408](https://github.com/DevElectrum/grand-anthology/actions/runs/40060800408); This run is green; evaluate the newer daycare mGBA job separately. The original Route 5 FRLG daycare NPC persists with separate \`SaveBlock3.route5DayCareMon\`, while the two new optional breeders point at the same \`SaveBlock1.daycare\` used by Johto/Hoenn. A new [WorldKantoDaycare.lua](../test/overworld/lua/WorldKantoDaycare.lua) production-ROM smoke stages a disposable flash Save/Continue into Route 5, checks all three active original/map NPCs and attempts both breeding dialogue interactions. It **does not** verify Egg creation or Phione hatching. The P0 mGBA workflow now triggers on relevant daycare files and runs this smoke **before** the already known failing Diancie navigation fixture.

**Next-Astra priority:** inspect exact optimized build and mGBA outcomes of these latest commits. Fix any real Route 5 NPC geometry/interaction bug if the new mGBA smoke exposes one. If all broad features pass except Diancie, advance grouped practical acquisition, source-gate and evolution/capture tests; do not spend an entire session retuning the Diancie warp. Later consolidated genuine Kanto Manaphy+Ditto deposit → egg → hatch → Save/Continue and per-region statics/rare catches must be proved before P0 acceptance. Preserve source-only public distribution.




## Kanto daycare breeding implementation and CI evidence (24 October UTC)

The strict gate-audit and last Klawf repair commit [\`f971217\`](https://github.com/DevElectrum/grand-anthology/commit/f971217f898af09fd58a178fbaf1f20a82d629ac) **passed both source checks and full optimized ROM build** in [P0 Gameplay Build #40060366422](https://github.com/DevElectrum/grand-anthology/actions/runs/40060366422). The separate [mGBA run #40060366495](https://github.com/DevElectrum/grand-anthology/actions/runs/40060366495) was still running at the initial checkpoint; query its true final status.

The next functional P0 batch corrects Kanto **two-parent breeding before Champion**. Old Route 5 FRLG daycare supports only single Pokémon training; Kanto Four Island breeding is post-League. To make Manaphy+Ditto→Phione locally actionable, added a breeding lady and man inside \`data/maps/Route5_PokemonDayCare_Frlg/map.json\` next to the original caretaker, with tiny cross-region wrappers to the existing **Johto daycare** scripts in \`data/maps/Route34_DayCare/scripts.inc\`. The shared \`gSaveBlock1Ptr->daycare\` state and compatible Egg pickup need no new save fields. \`ValidateAnthologyEvolutionAccess.py\` covers this source topology.

**Exact next checks:** inspect latest P0 Gameplay Build on the new breeding commit and latest P0 mGBA smoke. If the new building objects have collision/interaction problems, fix the layout/position as a functional batch; do not claim the Phione Egg was produced without an mGBA deposit/compatibility/Egg/hatch test. Continue grouped story and unique-event access auditing and consolidate real gameplay proof after implementation. Do not alter or import private history, distribute ROM/saves, expand 420 PC storage, or switch focus to native Linux/gimmicks.



## Newest Oct 24 checkpoint: grouped gameplay acquisition and gate corrections

Continue only on **public `DevElectrum/grand-anthology:main`**. Do not modify/merge the historical private repository. Inspect live `main` HEAD and exact CI checks at handoff time.

- [`4f693f8`](https://github.com/DevElectrum/grand-anthology/commit/4f693f815fe32a985a113772b4b56d4fe8bbaf42) adds **14** optional stone evolution branches for 12 parents, retaining original special clock/weather/spin mechanisms; Peat Block Ursaluna also drops night-only restriction. `ValidateAnthologyEvolutionAccess.py` source-checked this batch, but a complete release build was superseded by the next push.
- [`c223458`](https://github.com/DevElectrum/grand-anthology/commit/c223458e4b8680b9138e2f138d2377c88e4a920d) adds **eight** regional-form/rare-seed Kanto/Johto habitats (Galarian Farfetch'd/Meowth/birds, Paldean Wooper, Own-Tempo Rockruff and Alola-cap Pikachu); its [P0 gameplay source validators #40060011972](https://github.com/DevElectrum/grand-anthology/actions/runs/40060011972) passed. `DARMANITAN_GALAR_ZEN` is explicitly excluded from the collectible-form audit because Zen Mode is a battle-only form.
- Latest bundled gate-fix changes exclude **Embedded Tower**, **Meteor Falls Steven's Cave** and **Altering Cave** from the pre-Champion source closure. They are locked by Johto/Hoenn Champion flags in map scripts. Adds first-Champion Hoenn natural sources for **Groudon** (Route 111) and **Kyogre** (Seafloor Cavern Room 1), preserving the original postgame script objects. Ecology grows **302 → 310 → 312 → 313**; strict Champion-gate CI exposed a real **Klawf #950** gap (1,024/1,025 until fixed). Added a reachable Hoenn Route 115 rocky habitat while retaining the original Steven's Cave source.
- The old [Cubone syntax fix on `6baa615` passed optimized CI #40058999139](https://github.com/DevElectrum/grand-anthology/actions/runs/40058999139); both full build and validators were green. **Look up the latest new-gameplay-sha P0 Gameplay Build and P0 mGBA Release Smoke** before claiming complete optimized compilation or emulator acceptance. New code commits deliberately trigger both CI pipelines. The existing mGBA Diancie test cannot reach Steven's Room from Granite Cave; it does not demonstrate a defective Diancie object.
- Keep the core design: Kanto/Johto/Hoenn only, one saved trainer/party/PC, casual difficulty, 420 PC slots for now. No living-Dex completeness claims, ROM distribution, native Linux, storage expansion, Gigantamax or Terastallization.

**Next engineering:** Finish reviewing internal story flags, map connectivity, special encounter prerequisites, breeding egg sources, expensive rare odds and permanent-form paths. **Then run consolidated mGBA gameplay**, representing all three regional departures, genuine catches, item/time/weather evolutions, daycare egg generation/hatching (including Manaphy→Phione), static catch/faint/revisit/reload and shared-party continuity. Do not sink cycles into a single Diancie fixture before broad implementation is done. Track source-test evidence separately from optimized-build and actual emulated gameplay outcomes.



**Read this first before editing. Updated October 23, 2026 (US Central), October 24 UTC.**

## CURRENT HANDOFF CHECKPOINT: gameplay repair and C build unblock (Oct 24 UTC)

**Primary live branch:** public `DevElectrum/grand-anthology:main`. **Latest gameplay/test repair before this documentation update:** [`6baa615e2413cdaf827523e49256fb133335cd39`](https://github.com/DevElectrum/grand-anthology/commit/6baa615e2413cdaf827523e49256fb133335cd39). The previous code fix was [`5156d50`](https://github.com/DevElectrum/grand-anthology/commit/5156d50bcc71fd2539014f5558a55502224a1df7). Inspect live `main` before modifying anything; do not use an old SHA as a branch target. The shorter [next-Astra prompt](NEXT_ASTRA_PROMPT.md) is maintained alongside this handoff.

### Completed P0 acquisition implementation

- [`2efe308`](https://github.com/DevElectrum/grand-anthology/commit/2efe30869ff851129018acd8204c2c8b4efbb646): all 30 trade-evolution paths have local offline item alternatives and all 44 item-evolution types are supplied regionally. Source audit only, not battle acceptance.
- [`44635fd`](https://github.com/DevElectrum/grand-anthology/commit/44635fd44b6c5924e30a2df3d5e638277fdfe1d5): Dragon Cheer at level 38 on Dipplin; source guard for 15 move-requisite evolutions.
- [`63bea16`](https://github.com/DevElectrum/grand-anthology/commit/63bea1646b91a055175cf09ca75d302dbbfe320f): 17 Arceus Plates, 17 Silvally Memories and four Oricorio Nectars stocked in World Transit.
- [`6c50040`](https://github.com/DevElectrum/grand-anthology/commit/6c50040bcc2242973038f8b1c1c057a3a7b1b8fd), [`efabe71`](https://github.com/DevElectrum/grand-anthology/commit/efabe71e8115219d8828e6d06ec80de946d3642c): alternate, locally purchasable evolution stones for 12 inherited Alola/Galar/Hisui-only branches and validator escaping repair. The three playable regions never set those foreign `IF_REGION` predicates. Preserve ordinary base-form evolutions.
- [`23890d6`](https://github.com/DevElectrum/grand-anthology/commit/23890d6674355bf0c8e47f132d02aa8f0aeabe65): six formerly post-Champion Sevii-locked Kanto evolution parents gained mainland pre-League ecology: Aipom, Misdreavus, Murkrow, Sneasel, Yanma, Swinub. Extra-habitat entries now total **302**, and the source audit excludes additional clearly post-League maps. See [detailed pre-Champion access audit](P0_PRECHAMPION_ACCESS.md).

### Critical CI distinction and corrective action

- The fully optimized [`63bea16` P0 Gameplay Build #40057275807](https://github.com/DevElectrum/grand-anthology/actions/runs/40057275807) **PASSED**.
- The [`23890d6` Gameplay Build #40058490395](https://github.com/DevElectrum/grand-anthology/actions/runs/40058490395) **FAILED compilation**, although the independent **source-validation job PASSED**. The [same-SHA mGBA run #40058490385](https://github.com/DevElectrum/grand-anthology/actions/runs/40058490385) **FAILED before launching gameplay**, at compilation. This is **not** a new Diancie/encounter verdict.
- Exact compiler error: `src/data/pokemon/species_info/gen_1_families.h:13697:370: error: expected '}' before '{' token`. In Cubone's `EVOLUTION(...)`, the new Dusk Stone Alolan Marowak branch lacked the **comma before the following Totem `EVO_NONE` branch**.
- [`5156d50`](https://github.com/DevElectrum/grand-anthology/commit/5156d50bcc71fd2539014f5558a55502224a1df7) adds the missing comma and a source assertion; [`6baa615`](https://github.com/DevElectrum/grand-anthology/commit/6baa615e2413cdaf827523e49256fb133335cd39) repairs that assertion's raw-regex escaping. **Recheck the latest CI at HEAD before declaring this fixed in the optimized build.** [`6baa615` Gameplay Build #40058999139](https://github.com/DevElectrum/grand-anthology/actions/runs/40058999139) had been queued at this checkpoint. Note that the workflow cancels superseded earlier runs; a cancellation is not a failing compile.
- Earlier [mGBA #40056497711](https://github.com/DevElectrum/grand-anthology/actions/runs/40056497711) did execute title, first boot, region-party continuity and save/Continue and then stopped at Granite Cave north-arrow navigation. It never entered Steven's Room, and therefore cannot establish Diancie's actual spawn behavior. Do not get stuck on this one fixture.

### Next-Astra priority, ordered

1. **Check latest public-main P0 Gameplay Build first.** If the `6baa615` optimized build fails, inspect the new compiler logs and correct the remaining source error(s), then rerun CI. Keep all fixes on public `main`; no ROM or save artifacts.
2. **Continue substantive P0 practical pre-first-Champion acquisition gating audit across all three regions**, grouping fixes. Check scripts with hidden Champion flags on accessible maps, map/warp/habitat access, native + rare + ecology + aquatic rolls, trade/item/move/stones, breeding and alternate forms. The `tools/audit_p0_acquisition.py` **1,025/1,025** result is an *optimistic source closure*, not an accepted runtime catchability certificate. Prioritize real blockers rather than extra docs or per-species fixtures.
3. **After substantial implementation**, consolidate emulator testing for regional access, shops, species captures, evolution branches, breeding, catches/faints/revisits, Save/Continue and party continuity. Fix the Granite Cave navigation fixture when testing Diancie, distinguishing warp behavior from encounter behavior.
4. Maintain easy/casual gameplay, three-region Kanto/Johto/Hoenn scope, **420 PC slots**, and defer Dynamax/Gigantamax/Terastallization, PC expansion, native Linux port, and public release claims. Preserve upstream authorship/credits/notices; never touch the private precursor repository.

This section supersedes earlier time-stamped CI descriptions below. No runtime proof of all 1,025 is claimed.

## Canonical repository, branch and intent

- **PUBLIC active repository:** [`DevElectrum/grand-anthology`](https://github.com/DevElectrum/grand-anthology)
- **Development branch:** `main` in THIS public repository. `main` is a genuine fresh-history root: initial commit [`27703e9dbcc69ff50a0cb51a88775c3e3a37ad40`](https://github.com/DevElectrum/grand-anthology/commit/27703e9dbcc69ff50a0cb51a88775c3e3a37ad40) has **zero parents**.
- The source-only import selected **33,649 allowlisted files**, preserved upstream [credits](../CREDITS.md) and the [nine inherited tool notices](../THIRD_PARTY_NOTICES.md), and omitted historical engineering records, private Git history, binary ROMs and saves.
- The older repository remains a **private archive** with its previous `master` unchanged. **Do not push to it, clone/merge its Git history into public `main`, make it public or run expensive private Actions there.** Work in this public repo.
- The user published the *source* to obtain public GitHub Actions minutes for ongoing development. **Source publication is done.** Do not propose another source-publication audit or demand a green build/gameplay test before working.

## Current project and fixed design boundaries

Pokémon Grand Anthology is an experimental, unofficial, noncommercial GBA modification of [Pokémon World](https://github.com/evilchinesefood/PKMN-World). The objective is one accessible continuous journey across **Kanto, Johto and Hoenn**, with one trainer/party/PC/save and source-integrated Pokémon from Generations 1–9. No nine-region expansion is planned. Original upstream authors and all historic contributors must remain credited.

The intended before-first-Champion National Pokédex comprises **1,025 distinct species**; the source audit reported **1,025/1,025 optimistic acquisition paths**, including wild/static/gifts/evolution/breeding references. **This is NOT proof of actual pre-Champion capture, evolution, save, revisit or full-game acceptance.** There are 420 existing PC slots. Leave expansion until after P0 implementation and representative testing. Alternate forms are not new National Dex IDs. No invented claims of complete playability.

The maintainer prefers **casual, approachable Pokémon difficulty** while also having completed multiple Elden Ring RL1 runs; keep this playful distinction in the README. Do not reopen settled design debates. Keep credits, unofficial/no-ROM/attribution notices, and truthful WIP labels.

## Public GitHub Actions baseline

At the exact *initial public source commit* `27703e9d`:

- **[P0 Gameplay Build run #40055403327](https://github.com/DevElectrum/grand-anthology/actions/runs/40055403327): SUCCESS.** Both source validators and optimized full-roster ROM compilation passed, with no ROM uploaded.
- **[P0 mGBA Release Smoke run #40055403335](https://github.com/DevElectrum/grand-anthology/actions/runs/40055403335): FAILURE** only in the late `WorldDiancieStatic.lua` navigation. Earlier title, Kanto/Johto/Birch starter, save and [regional travel `27/27 PASS`](https://github.com/DevElectrum/grand-anthology/actions/runs/40055403335) checks completed.
- Actual Diancie log: `BOOTED grp=24 map=7 pos=(5,9)`; entered north-arrow tile `(5,10)`; turning Up in place was **PASS**, then `real overworld north-arrow warp entered Steven room` **FAIL**, final `map=24/7 pos=(5,9)`; `VERDICT WorldDiancieStatic: 5/6 PASS`. The fixture moved back to (5,9), so it **never reached Steven's room**. This is not evidence for or against Diancie visibility/battle.
- Subsequent workflow-maintenance changes use **public `main`** for both retained read-only workflows, [`Gameplay-P0.yml`](../.github/workflows/Gameplay-P0.yml) and [`P0-mGBA-Release-Smoke.yml`](../.github/workflows/P0-mGBA-Release-Smoke.yml). They build local temporary ROMs but **never publish ROMs, saves or screenshots**. Recheck the newest run SHA after new code commits. Red gameplay CI is normal WIP work, not a reason to hide source.

## What to do next (priority order)

1. **Resume substantive P0 implementation,** not another publication process. Inspect [README](../README.md), [PUBLIC_STATUS](PUBLIC_STATUS.md), [FEATURES](../FEATURES.md) and relevant implementation/validation code on this public `main` first. Focus practical before-first-Champion capture/evolution/breeding/alternate-evolution paths, scripted encounter integration, relevant save/revisit behaviors and any remaining P0 mechanics. Group substantial changes by functional or geographical area and commit meaningful batches.
2. Use **quick source validators/optimized build** between sizeable implementation batches. Do **consolidated emulator testing after meaningful implementation is in place**, not a mandatory one-by-one gameplay test for every species. Do not be blocked by the known Diancie doorway fixture while doing unrelated P0 source engineering.
3. When addressing the Diancie acceptance failure, first debug the **north-arrow warp control fixture** and distinguish navigation from the in-room field object. The failed assertion did not enter the room; don't assert a Diancie spawn defect on that evidence.
4. After real P0 implementation and targeted fixes, run wider Kanto/Johto/Hoenn gameplay acceptance, save/reload, encounters, forms, settings, persistence and region continuity. Report **what was actually demonstrated**, not source reachability.
5. Keep the original **420-slot PC** for now; storage enlargement and Dynamax/Gigantamax/Terastallization are deferred. Native Linux port and unrelated infrastructure features are not current priority.


## Implementation update: three P0 gameplay batches on public main

After the initial `20938787` handoff, the following **public-main** changes were committed:

| Commit | Working feature | Verification boundary |
|---|---|---|
| [`2efe308`](https://github.com/DevElectrum/grand-anthology/commit/2efe30869ff851129018acd8204c2c8b4efbb646) | Karrablast and Shelmet Linking Cord evolution routes; Sachet, Whipped Dream, Auspicious/Malicious Armor and Gimmighoul Coins in all Kanto/Johto/Hoenn department stores | Code scan: all 30 trade edges have offline item routes; all 44 item-evolution types stocked per region |
| [`e66fa1b`](https://github.com/DevElectrum/grand-anthology/commit/e66fa1b48a385af64c12b386c43b02e84530e0ec) | Correct escaping in evolution-source test | The source-validation CI job passed on a later commit; these are still not emulator assertions |
| [`44635fd`](https://github.com/DevElectrum/grand-anthology/commit/44635fd44b6c5924e30a2df3d5e638277fdfe1d5) | Dipplin learns required Dragon Cheer at level 38; move-based evolution source guard checks all 15 cases | Intermediate source-validation job passed; full build superseded |
| [`63bea16`](https://github.com/DevElectrum/grand-anthology/commit/63bea1646b91a055175cf09ca75d302dbbfe320f) | 17 Arceus Plates, 17 Silvally Memories, 4 Oricorio Nectars in shared hub store and a buyability source guard | Latest gameplay-build run [`40057275807`](https://github.com/DevElectrum/grand-anthology/actions/runs/40057275807) was running when recorded; verify final result |

**Freshly confirmed emulator baseline:** [mGBA `20938787` #40056497711](https://github.com/DevElectrum/grand-anthology/actions/runs/40056497711) finished **FAIL** at the *same Granite Cave doorway navigation*. Before that failure it logged `WorldShinyOdds: 26/26 PASS`, `WorldFirstBoot: 4/4 PASS`, and `WorldRegionPartyTravel: 27/27 PASS`. Do not call Diancie's spawn broken: the emulator never entered its room. Do not repeatedly rebuild that fixture at the expense of feature implementation.

**Next engineering work:** Check `63bea16` CI first, then group remaining P0 by actual pre-Champion gating or forms subsystem. Confirm real mart purchases and offline evolution for this batch, breeding and key-item-only form access (e.g. Rotom Catalog, Zygarde Cube, Reveal Glass) without assuming price-zero key items can be sold in regular shops. Audit save/faint/revisit for representative scripted battles, then run consolidated mGBA play acceptance. Keep 420 PC slots and all upstream notices; do not touch archived private repo.

## Follow-up: pre-Champion P0 acquisition repairs (public main)

**Latest gameplay commit:** [`23890d6674355bf0c8e47f132d02aa8f0aeabe65`](https://github.com/DevElectrum/grand-anthology/commit/23890d6674355bf0c8e47f132d02aa8f0aeabe65), following [`6c50040`](https://github.com/DevElectrum/grand-anthology/commit/6c50040bcc2242973038f8b1c1c057a3a7b1b8fd) and [validator fix `efabe71`](https://github.com/DevElectrum/grand-anthology/commit/efabe71e8115219d8828e6d06ec80de946d3642c).

- Fixed 12 evolution branches that demanded `REGION_ALOLA`, `REGION_GALAR` or `REGION_HISUI` (all impossible from `GetCurrentRegion()` in this three-region game). Added distinct purchasable Evo Stone alternatives while leaving base evolutions and original branch mechanics intact. A new test enumerates all 12 `IF_REGION` cases, enforces the alternative, shops and runtime item-use mode. See [P0 access details](P0_PRECHAMPION_ACCESS.md).
- Six Gen IV Kanto-owned evolution lines were counted due to base encounters on *post-Champion* Sevii islands 4–7. Added Aipom, Misdreavus, Murkrow, Sneasel, Yanma and Swinub on selected mainland maps, no native replacements, with DexNav capacity guards; ecology now 302 entries. Tightened the Python National source-audit blacklist for postgame islands, postgame Hoenn caves and Battle Frontier. Baseline static audit still remains optimistic because script-local guards, map passage and gameplay aren't proven.
- [`63bea16` full gameplay build](https://github.com/DevElectrum/grand-anthology/actions/runs/40057275807) succeeded. **Source validation succeeded** for new [`23890d6` Gameplay Build](https://github.com/DevElectrum/grand-anthology/actions/runs/40058490395), with full compilation ongoing when recorded. [Same-SHA mGBA](https://github.com/DevElectrum/grand-anthology/actions/runs/40058490385) ongoing; expect possible previous Granite Cave fixture failure, not a Diancie object verdict. Do not confuse intermediate cancelled full builds with failed code.
- Follow-on auditing confirmed the Alcremie `EVO_SPIN` has a real field-avatar input trigger in `src/field_control_avatar.c` calling `TrySpecialOverworldEvo`, and move-use evolution bookkeeping is present in `src/battle_move_resolution.c`. **These are code connections, not emulator acceptance.**

**Next step:** continue real P0 availability/story-gate audit and repair grouped issues if found, then consolidated emulator proof for rare ecology, trade-stone and alternate-form evolutions, breeding, static catches and persist/revisit. Do not drift into gimmicks, PC growth, private repo or publication work.

## Collaboration and workflow precautions

- Make changes and commits **only in `DevElectrum/grand-anthology:main`** unless the user directs otherwise. Do not import old refs, merges, issue artifacts or GitHub Actions outputs from private history.
- A public source repository may attract contributions. Respect third-party component notices and authorship and do not upload any `.gba`, personal `.sav`, confidential credentials, copyrighted base-game dumps or derived full-ROM archives. Public source visibility does **not** imply a blanket code/asset license. See [THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md).
- Read-only `GITHUB_TOKEN`, `actions/checkout` with `persist-credentials: false`, no `upload-artifact` for copyrighted ROMs/saves and no write-token builds from untrusted outside pull requests.
- The public [`.gitignore`](../.gitignore) now allows new `docs/` and `AGENTS.md` content to be tracked; other inherited generated-file exclusions still apply.
- No extra repository creation, visibility change or playability announcement is required. This file is the public continuity document.

**Next-Astra opening instruction:** Read `AGENTS.md` and this handoff on PUBLIC `main`, inspect latest [GitHub Actions](https://github.com/DevElectrum/grand-anthology/actions), then continue substantive P0 gameplay engineering with large commits and brief progress updates.
