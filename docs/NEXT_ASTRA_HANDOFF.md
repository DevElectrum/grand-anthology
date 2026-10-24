# Next Astra: Grand Anthology after the PUBLIC source cutover

**Read this first before editing. Updated October 23, 2026 (US Central), October 24 UTC.**

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
