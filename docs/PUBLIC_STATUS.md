> **BULK JOHTO LOCATION ENGINEERING, newer than all three-at-a-time notes (24 Oct local / 25 Oct UTC):** The maintainer explicitly requires BIG, location-clustered passes (roughly 10–20 scripted encounters per regional batch), not family-by-family signoffs. Source commits on public `main`: [Forces of Nature / Enamorus gate](https://github.com/DevElectrum/grand-anthology/commit/1445e80b23fe346856a73a38888a031054a07654), [two Kubfu + Spectrier](https://github.com/DevElectrum/grand-anthology/commit/22ae8b9c72397480d079b25624743a7dd000bb3f), [Glastrier + Victini](https://github.com/DevElectrum/grand-anthology/commit/8e2c5374889ee350c508592be9a937efabb8a1fe), and [two Regis](https://github.com/DevElectrum/grand-anthology/commit/1d156588d0ecb427b1bfe9d85637057af5ef24b8). **11 source encounter relocations/gates implemented in one sweep**, prior Tao Trio was 3 more. `docs/JOHTO_BULK_2026-10-24.md` records sites, flags, CI status and remaining verification. **Do not call them gameplay PASS merely because committed.** Follow optimized ROM/source CI to repair incorrect collision coordinates or script errors. The mGBA regression contains unrelated old Diancie/daycare synthetic doorway fixture failures despite Nyx physical PASS. Avoid stalling on these. Continue substantive region-wide gameplay batches while isolating specific runtime blockers. Only canonical public repo, keep private archive untouched.

> **Tao Trio source implementation, 24 October 2026 local:** Reshiram in Tin Tower 2F (Lv44), Zekrom in north Dark Cave (Lv44), Kyurem in Ice Path B1F (Lv49) after both dragons' caught-dex bits, DNA Splicers upon capture with a bag-full retrieval plinth. Old Route45/44 objects removed; independent caught/revisit scripts added. See [source checkpoint](JOHTO_TAO_TRIO_2026-10-24.md) and latest source code commit [`79937e6`](https://github.com/DevElectrum/grand-anthology/commit/79937e62b4f3326e342e3cafe51a47faf292fb69). New source validators and optimized/mGBA runs are in progress; not a verified gameplay claim. Earlier “Tao is next” paragraphs below predate this work.

> **Johto Swords of Justice P0 relocation (24 October 2026):** [`0712fb0`](https://github.com/DevElectrum/grand-anthology/commit/0712fb05627259291c1278b3bdeac34ec4f1e447) places Keldeo Lv18 Route 32, Terrakion Lv20 Union Cave 1F, Virizion Lv23 National Park, and Cobalion Lv28 in Mt. Mortar 1F South reachable through its **western Route 42 entrance**, with independent catches and original locations removed. [`6621cd8`](https://github.com/DevElectrum/grand-anthology/commit/6621cd852e09038377654611d8bd7a0a36a22db7) fixed a dangling Olivine event label found by the first linker run and guarded against recurrence. [Exact source/optimized ROM #40151749582](https://github.com/DevElectrum/grand-anthology/actions/runs/40151749582) **PASS**. [Exact mGBA #40151749580](https://github.com/DevElectrum/grand-anthology/actions/runs/40151749580) passed nine baseline gameplay suites and failed only known Diancie (7/8) and daycare (3/4) synthetic navigation fixtures. **No in-game battle claim is made for these four new statics yet**. Next independent engineering batch is the Johto Tao trio: Reshiram/Zekrom late pre-League without mutual gate; Kyurem pre-League requiring *captured* both, with DNA Splicers capture reward. Preserve native Ho-Oh/Bell Tower and Ice Path story/puzzles.

> **Latest Kanto implementation and CI (24 October 2026 local):** On [`9ffe40e`](https://github.com/DevElectrum/grand-anthology/commit/9ffe40e4e308b9ae92f1860dbf9be0a37d7eb5cb), Giratina moved from Rock Tunnel to reachable Mt. Moon B2F (level 52, captured Dialga/Palkia required); Darkrai moved to Pokémon Tower 7F (level 45, Cresselia caught and Mr. Fuji rescued); Tapu Lele moved to Celadon garden near Erika's Gym (level 36, natural access). Old Rock Tunnel objects and scripts were removed. [Optimized build #40150403712](https://github.com/DevElectrum/grand-anthology/actions/runs/40150403712) **PASS** including source gate/encounter regressions. [mGBA #40150403711](https://github.com/DevElectrum/grand-anthology/actions/runs/40150403711) passed nine earlier gameplay suites and failed only the two **pre-existing synthetic navigation fixtures** (daycare 3/4, Diancie 7/8); both real doors and relevant in-game behavior were separately manually verified on Nyx as documented in [Nyx QA](NYX_MANUAL_QA_2026-10-24.md). **Do not claim the new three species have in-game battle verification yet.** Previous “design pause” notes below are historical and superseded by the user's explicit Begin/Continue authorization.

> **Nyx manual QA and cave-wild-encounter follow-up (24 October 2026):** A normal graphical mGBA playtest manually entered Steven's room from Granite Cave 1F, engaged level-18 Diancie, observed faint/revisit respawn, then caught Diancie and confirmed it stayed gone across revisit and a complete emulator restart. Story prerequisites were supplied by a temporary **local-only sign shortcut**, not natural badge/letter play; the automated synthetic-warp CI fixture remains unresolved. The user also reported wild overworld sprites moving toward a stationary player and crowding underground rooms. Cave generated-sprite suppression is the next source fix, while ordinary random-step encounters and authored statics remain. [Full Nyx manual QA record](NYX_MANUAL_QA_2026-10-24.md). This update supersedes old text below implying Diancie's encounter is wholly untested.

> **Design correction and development pause, 24 October 2026:** The current requirement is eventual access to all 1,025 species across the *complete three-region adventure, including postgame*, **not before the first Champion**. Original story gates may remain intact. Legendary and Mythical Pokémon should be static encounters; Ultra Beasts and Paradox Pokémon are intentionally natural rare encounters. The source-location [audit](LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md) identifies exceptions pending discussion. **No new gameplay work is authorized until user discussion.** Any older pre-Champion requirement or "resume immediately" below is historical and superseded.

# Pokémon Grand Anthology: scope and verification status

## Last exact optimized and mGBA results (24 October UTC)

[`10bf90d` Gameplay Build #40066826520](https://github.com/DevElectrum/grand-anthology/actions/runs/40066826520) **passed both source validation and optimized game compilation**. [Same-commit real mGBA #40066826492](https://github.com/DevElectrum/grand-anthology/actions/runs/40066826492) passed nine earlier acceptance suites, including Kanto/Johto starter scenes, Hoenn storage refusal, seven-mon gift boxing, new-game boot, Shiny options and three-region party/trainer Save/Continue. The lean release script pointer was correctly recovered as an unambiguous LTO-private symbol.

Overall mGBA still reports failure because **two independent synthetic map-navigation fixtures** could not reach their target:
- Kanto daycare smoke **3/4**, start at saved Route 5 exterior 37/23 (23,26), walking Up crossed to Cerulean group37 map11 (23,0) instead of triggering the daycare door. No interior NPCs were inspected.
- Diancie smoke **5/6**, staying on Granite Cave 24/7 rather than entering Steven's Room. No Diancie battle began.

Both are unresolved *test approach / map transition* observations, **not demonstrated gameplay regressions in the targeted Pokémon or NPC scripts**. Kanto breeding pair and actual Phione Egg pickup/hatching remain pending genuine in-game acceptance. Prioritize broader P0 implementation instead of repeatedly restarting full ROM builds to tune these isolated fixtures.



## Next P0 smoke gate adjustment (24 October UTC)

[`53aa59b` P0 gameplay build #40066330963](https://github.com/DevElectrum/grand-anthology/actions/runs/40066330963): **optimized compile passed**, while source validation failed only when checking an outdated test-comment phrase. [Release mGBA #40066331018](https://github.com/DevElectrum/grand-anthology/actions/runs/40066331018) compiled the game and mGBA itself, but halted while generating lean release symbols because the file-local `sGlobalScriptContext` was not found under its bare name. Consequently, neither run supplies new actual Route 5 NPC gameplay evidence. The next correction uses the actual exterior doorway, a stable source guard, and LTO-private alias lookup with fail-closed runtime interpretation. The mGBA workflow will also run the daycare and Diancie probes independently to avoid hiding results.



## Real daycare emulator test diagnosis (24 October UTC)

The [`d8a8cef` optimized build #40061225603](https://github.com/DevElectrum/grand-anthology/actions/runs/40061225603) **passed**. [The paired emulator run #40061225598](https://github.com/DevElectrum/grand-anthology/actions/runs/40061225598) again **passed** title (37/37), Cheats (8/8), full-party gift+save (15/15), Johto Elm (34/34), Kanto Oak (34/34), Hoenn full-PC refusal (26/26), shiny odds (26/26), first hub boot (4/4) and all three region travel+save (27/27).

The new daycare smoke failed at a **test-fixture setup flaw**: rewriting the saved map to inside Route 5 daycare left the previous World Transit overworld NPC object pool intact, so IDs 1/2 referred to hub actors and ID 3 was missing. A missing script context symbol in the lean release Lua symbol set then crashed the test. The corrected smoke stages **outside Route 5** and enters its door through regular GBA input; the symbol exporter is extended to include the actual script pointer. The original failure does **not** establish that normally-entered daycare NPCs are broken. Current actual game Egg pickup/hatching and successful field NPC interaction still need mGBA proof.




## Production emulator check and daycare smoke (24 October UTC)

The [optimized \`f971217\` gameplay build #40060366422](https://github.com/DevElectrum/grand-anthology/actions/runs/40060366422) **passed both jobs**. [Real mGBA run #40060366495](https://github.com/DevElectrum/grand-anthology/actions/runs/40060366495) then passed title (37/37), graphical Cheats (8/8), full-party gift+save (15/15), Elm (34/34), Oak (34/34), Birch storage refusal (26/26), shiny options (26/26), first hub boot (4/4), and complete three-region travel+save continuity (27/27). The overall emulator run **failed only at the last Diancie navigation test (5/6)**: the temporary fixture did not leave Granite Cave 1F for Steven's Room. No battle occurred, so this says nothing about Diancie's encounter implementation.

The later [Kanto Route 5 two-parent breeding change \`f07dff1\`](https://github.com/DevElectrum/grand-anthology/commit/f07dff10f557f93afd30dd82c87edb3ec9453170) **passed source validations and optimized compilation** in [P0 Gameplay Build #40060800408](https://github.com/DevElectrum/grand-anthology/actions/runs/40060800408), but still needs real map interaction. A new production-ROM [Kanto daycare mGBA smoke](../test/overworld/lua/WorldKantoDaycare.lua), wired into [P0 mGBA Release Smoke](../.github/workflows/P0-mGBA-Release-Smoke.yml), loads the interior via disposable battery Save/Continue and verifies the classic NPC plus the added two-parent lady/man are instantiated and reachable. This is **not** evidence of a hatched Phione or true breeding compatibility. Check its new exact commit/run before claiming an emulator PASS.




## Kanto pre-League daycare implementation update (24 October UTC)

[Gameplay Build #40060366422](https://github.com/DevElectrum/grand-anthology/actions/runs/40060366422) **passed both source validators and optimized production GBA compilation** at \`f971217\`, including the Champion-locked map audit, 313 supplemental ecology placements and restored optimistic 1,025/1,025 National access graph. This build is not an emulator gameplay proof.

The classic **Kanto Route 5 daycare** previously had only a single-Pokémon raising service; the region's normal two-parent FRLG breeding route was on post-Champion Four Island. The interior now includes two optional breeding NPCs that reuse the proven Johto two-parent daycare script handlers, beside the untouched FRLG trainer service. This makes Kanto Manaphy+Ditto→Phione breeding **source-wired before the first Champion** without adding save/party/PC fields. Kanto object approach and actual Egg generation, hatch and persistence still require consolidated mGBA verification. See [detailed P0 pre-Champion audit](P0_PRECHAMPION_ACCESS.md) and [handoff](NEXT_ASTRA_HANDOFF.md).



## Current P0 engineering addition: 24 October UTC (supersedes older CI caveats below)

The corrected Cubone evolution comma build at [`6baa615`, CI #40058999139](https://github.com/DevElectrum/grand-anthology/actions/runs/40058999139) **passed source validation and the full optimized GBA build**. This resolves the earlier reported compiler blocker.

On public `main`, the next acquisition implementation batches include [14 optional stone evolutions and unrestricted Peat Block for Ursaluna](https://github.com/DevElectrum/grand-anthology/commit/4f693f815fe32a985a113772b4b56d4fe8bbaf42), plus [eight additional pre-first-Champion regional-form/seed habitats](https://github.com/DevElectrum/grand-anthology/commit/c223458e4b8680b9138e2f138d2377c88e4a920d). A subsequent gate-aware source pass excludes **Embedded Tower**, **Meteor Falls Steven's Cave** and **Altering Cave** from purported pre-Champion routes, and introduces optional Hoenn **Groudon (Route 111)** and **Kyogre (Seafloor Cavern Room 1)** habitats. The supplemental ecology catalog is now **313** map-species entries, including Hoenn Route 115 Klawf (#950), which was previously only counted from Champion-gated Steven's Cave. Original habitats, story objects and NPC statics remain in place.

The corresponding code-specific source validators and optimized CI/mGBA outcomes must be checked on their **exact commit**, not inferred from earlier green runs. The previous optimistic 1,025/1,025 source graph still does **not** demonstrate legitimate first-Champion in-game acquisition, proper rare spawn distribution, evolution-item use, breeding, static lifecycle, save/revisit or complete three-region story access. The [technical gate audit](P0_PRECHAMPION_ACCESS.md) and [handoff](NEXT_ASTRA_HANDOFF.md) explain the remaining acceptance work.



**Status (October 24, 2026): Public experimental source on `main`.**
The *source repository* is public. It is not a finished playable game, ROM distribution or promise of availability.

## Latest development checkpoint (October 24, 2026 UTC)

P0 practical-access work continued with [12 Alola/Galar/Hisui region-gated evolution repairs](https://github.com/DevElectrum/grand-anthology/commit/6c50040bcc2242973038f8b1c1c057a3a7b1b8fd), [the accompanying source-validator fix](https://github.com/DevElectrum/grand-anthology/commit/efabe71e8115219d8828e6d06ec80de946d3642c), and [six mainland Kanto species placements](https://github.com/DevElectrum/grand-anthology/commit/23890d6674355bf0c8e47f132d02aa8f0aeabe65) that no longer rely exclusively on late Sevii island availability. Ecology catalog expanded from 296 to **302**, and the National source auditor excludes additional known post-Champion maps. Details: [pre-Champion access audit](P0_PRECHAMPION_ACCESS.md).

**CI caveat:** [P0 Gameplay Build #40058490395](https://github.com/DevElectrum/grand-anthology/actions/runs/40058490395) and [mGBA #40058490385](https://github.com/DevElectrum/grand-anthology/actions/runs/40058490385) failed at the same **C compiler syntax error**, not at an emulator encounter: a missing comma in the Cubone Alolan Marowak evolution table. [`5156d50`](https://github.com/DevElectrum/grand-anthology/commit/5156d50bcc71fd2539014f5558a55502224a1df7) corrected the separator and [`6baa615`](https://github.com/DevElectrum/grand-anthology/commit/6baa615e2413cdaf827523e49256fb133335cd39) fixed the regression assertion escaping. **The corrected optimized build must be confirmed** on [the latest build #40058999139](https://github.com/DevElectrum/grand-anthology/actions/runs/40058999139), which was queued when this note was written. [Build #40057275807](https://github.com/DevElectrum/grand-anthology/actions/runs/40057275807) is the last confirmed complete optimized build success at the time of writing.

This is an **implementation and source/CI status**, not evidence of in-game before-first-Champion access to every National species. Consolidated emulator acceptance remains to be done after the implementation batches. See [next-Astra handoff](NEXT_ASTRA_HANDOFF.md) for the priority sequence and verified boundaries.

## What this project is building

Grand Anthology modifies [Pokémon World](https://github.com/evilchinesefood/PKMN-World)
to preserve the original Kanto, Johto and Hoenn adventures under one trainer,
party, Bag, Pokédex, PC and battery save. We aim for a casual, accessible journey,
modernized controls and Pokémon from generations 1–9. Later-generation regions
are outside the current scope.

## Source implemented versus gameplay verified

| Topic | Evidence and limits |
|---|---|
| National species | The October 23 static acquisition audit recognized **1,025/1,025 optimistic source paths** (regional distribution Kanto 346, Johto 352, Hoenn 327) at development code `45dc98d9`. This includes encounters, gifts, evolution and breeding **references**; it does **not** prove all are genuinely obtainable before the first Champion. |
| Specials and forms | Source has many scripted encounters and habitats, but save/revisit/catch/faint and permanent-form graph acceptance is incomplete. |
| GBA build | Initial [public main source/build CI](https://github.com/DevElectrum/grand-anthology/actions/runs/40055403327) passed both source validators and an optimized local ROM build (without distributing its output). Future WIP commits may have red CI. |
| Emulator verification | The first [public mGBA run](https://github.com/DevElectrum/grand-anthology/actions/runs/40055403335) passed title, starter, save and 27/27 party/region travel checks, but failed 5/6 in `WorldDiancieStatic` while traversing the Granite Cave north-arrow doorway: it never entered Steven's room. This has **not yet tested** the actual Diancie object or battle. Public-source availability is unrelated to this WIP test result. |
| PC storage | **420 slots (14 boxes × 30)** currently. A living collection of all 1,025 distinct species cannot fit at once; storage expansion is deferred until other gameplay is complete. Releasing a Pokémon should not erase the Pokédex caught record. |
| Gameplay readiness | Internally tracked provisional P0 readiness remains **37%**. This number is not a public playability certification, and actual campaign-wide acceptance is outstanding. |

The source-path audit is a discovery aid; it is deliberately optimistic.
Full P0 acceptance requires legitimate capture and evolution methods,
pre-Champion reachability, cross-region campaign continuity and actual
emulator save/reload testing.

## Work order

1. Finish the remaining P0 **implementation** without reopening finalized
   design decisions or expanding PC storage.
2. Run consolidated GBA/emulator checks over the three campaigns, static
   encounters, forms, breeding/evolution, randomizer settings and saves.
3. Stabilize a genuinely playable game, then consider optional P1 additions.
4. Defer storage expansion and later gimmicks (Gigantamax/Terastallization)
   until the main game is sound.

These are priorities, not delivery dates. No game download or ROM is
distributed by this repository.

## Latest public P0 engineering (October 24 UTC)

The initial three substantive implementation batches landed directly on public `main`:

- [`2efe308`](https://github.com/DevElectrum/grand-anthology/commit/2efe30869ff851129018acd8204c2c8b4efbb646) and [`e66fa1b`](https://github.com/DevElectrum/grand-anthology/commit/e66fa1b48a385af64c12b386c43b02e84530e0ec): added offline Linking Cord evolutions for Karrablast/Shelmet; stocked missing Gen 6/9 evolution items and Gimmighoul Coins in all three regional department stores; added an all-generation trade/item source guard (30 trade targets and 44 item types).
- [`44635fd`](https://github.com/DevElectrum/grand-anthology/commit/44635fd44b6c5924e30a2df3d5e638277fdfe1d5): Dipplin naturally learns Dragon Cheer at level 38 for offline Hydrapple evolution; validates 15 move-based evolution species against the configured Gen 9 level-up learnsets.
- [`63bea16`](https://github.com/DevElectrum/grand-anthology/commit/63bea1646b91a055175cf09ca75d302dbbfe320f): World Transit hub shop stocks 17 Arceus Plates, 17 Silvally Memories and 4 Oricorio Nectars; validates item availability and sale prices. This does not address key-item forms, Mega, Gigantamax or Terastallization.

**Build verification distinction:** The [public gameplay build at `20938787`](https://github.com/DevElectrum/grand-anthology/actions/runs/40056497817) passed. Source-validation jobs for the intermediate P0 commits passed, but full optimized GBA builds on those intermediate SHAs were cancelled by the workflow's newer-commit concurrency setting. Check the [latest `63bea16` gameplay build](https://github.com/DevElectrum/grand-anthology/actions/runs/40057275807) for its final outcome rather than assuming a pass. The [mGBA run at `20938787`](https://github.com/DevElectrum/grand-anthology/actions/runs/40056497711) still failed in the Granite Cave doorway fixture after successfully testing title, regional party travel, save/Continue, and settings; no Diancie battle result is established.

These repairs improve source-implemented accessibility, not verified end-to-end Pokédex completion. Remaining P0 includes early map/story gating, actual mart and evolution operations, breeding and static catch/faint/revisit save testing, key-item form mechanics, and consolidated emulator acceptance.


### Pre-first-Champion access integration follow-up

- [`6c50040`](https://github.com/DevElectrum/grand-anthology/commit/6c50040bcc2242973038f8b1c1c057a3a7b1b8fd) and [`efabe71`](https://github.com/DevElectrum/grand-anthology/commit/efabe71e8115219d8828e6d06ec80de946d3642c): preserved original level/region evolutions while adding purchasable evolution-stone alternatives for **12 formerly impossible Alola/Galar/Hisui evolution branches**. The latter commit repaired an over-escaped source-validator regex; source tests passed afterward. See [the access audit](P0_PRECHAMPION_ACCESS.md) for the complete mapping.
- [`23890d6`](https://github.com/DevElectrum/grand-anthology/commit/23890d6674355bf0c8e47f132d02aa8f0aeabe65): placed **six Kanto evolution parent species** on mainland pre-League encounter maps rather than relying on Sevii Islands 4–7 (Aipom, Misdreavus, Murkrow, Sneasel, Yanma, Swinub). Expanded ecology entries **296 → 302**, retained original habitats/native encounters and validated DexNav slots. Updated the optimistic 1,025-source auditor to exclude four post-League Sevii islands, Desert Underpass, Artisan Cave, Marine/Terra Cave and the Battle Frontier in addition to earlier postgame maps.
- **CI:** [`63bea16` gameplay build #40057275807](https://github.com/DevElectrum/grand-anthology/actions/runs/40057275807) fully passed. For [`23890d6` gameplay build #40058490395](https://github.com/DevElectrum/grand-anthology/actions/runs/40058490395), source validators **passed**, including the strengthened region/shop/ecology acquisition checks; optimized build was running when recorded. A separate [mGBA #40058490385](https://github.com/DevElectrum/grand-anthology/actions/runs/40058490385) started at that SHA; check its final outcome, and remember the existing Granite Cave navigation fixture problem.

No single-species catch, evolution, save or story passage acceptance is established by those Python validators. Remaining practical P0 work: scrutinize scripts with internal post-Champion flags on otherwise accessible maps, verify actual purchase/evolution and rare encounter probabilities, consolidate emulator tests (including all three regions), check breeding and key-item-only form machinery, and correct issues uncovered. The source-level 1,025 total remains deliberately optimistic.

## Source history and attribution

Grand Anthology is a non-commercial hobby modification. The underlying
Pokémon World game and third-party engine, maps, artwork, sound, and tools
remain credited in [CREDITS.md](../CREDITS.md). Respect the original
authors and their component-specific license notices. The project's
documentation does not grant rights to Pokémon intellectual property.
