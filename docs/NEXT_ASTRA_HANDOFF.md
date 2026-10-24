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

## Collaboration and workflow precautions

- Make changes and commits **only in `DevElectrum/grand-anthology:main`** unless the user directs otherwise. Do not import old refs, merges, issue artifacts or GitHub Actions outputs from private history.
- A public source repository may attract contributions. Respect third-party component notices and authorship and do not upload any `.gba`, personal `.sav`, confidential credentials, copyrighted base-game dumps or derived full-ROM archives. Public source visibility does **not** imply a blanket code/asset license. See [THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md).
- Read-only `GITHUB_TOKEN`, `actions/checkout` with `persist-credentials: false`, no `upload-artifact` for copyrighted ROMs/saves and no write-token builds from untrusted outside pull requests.
- The public [`.gitignore`](../.gitignore) now allows new `docs/` and `AGENTS.md` content to be tracked; other inherited generated-file exclusions still apply.
- No extra repository creation, visibility change or playability announcement is required. This file is the public continuity document.

**Next-Astra opening instruction:** Read `AGENTS.md` and this handoff on PUBLIC `main`, inspect latest [GitHub Actions](https://github.com/DevElectrum/grand-anthology/actions), then continue substantive P0 gameplay engineering with large commits and brief progress updates.
