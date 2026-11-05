# LATEST SINNOH ENGINEERING CHECKPOINT: BULK STAGING VERIFIED, LINK PROBE IN PROGRESS

**Authorization remains: complete the full usable Sinnoh import, not only documentation.** Current development branch \`sinnoh/full-import\`; permanent game \`main\` unchanged. The batch importer and tests are committed and the 2,758 map/layout/asset source files can be cloned and staged reproducibly in temporary CI. \`docs/SINNOH_PINNED_SYMBOL_AUDIT.json\` freezes **663 source symbols** (including 147 flags, 21 vars, 66 trainers, 62 object graphics, 30 music IDs); crucial Hoenn aliases \`FLAG_BADGE01_GET\`, \`FLAG_BADGE02_GET\`, \`VAR_STARTER_MON\` still require dedicated remapping/persistence before activation.

**Evidence:** pinned import [#41995977966](https://github.com/DevElectrum/grand-anthology/actions/runs/41995977966) PASS, subsequent [#41996897101](https://github.com/DevElectrum/grand-anthology/actions/runs/41996897101) PASS, after canonical Git CRLF/palette fix. Disposable full-content installation/link probe [#41996360025](https://github.com/DevElectrum/grand-anthology/actions/runs/41996360025) failed only at first imported MIDI missing \`midi.cfg\` recipe. Importer added safe source-only MIDI recipe merge and tests; [#41996897190](https://github.com/DevElectrum/grand-anthology/actions/runs/41996897190) then exposed that a comment line in \`midi.cfg\` is forbidden by \`audio_rules.mk\`. This was fixed at commit \`22efdad0fa882d9975b262bba30fedc7df8d99fc\`. **Current isolated integration probe** [#41997175641](https://github.com/DevElectrum/grand-anthology/actions/runs/41997175641) was compiling at this checkpoint; inspect its result/logs. Do not say it passed unless verified. This is **not yet a full usable or publishable Sinnoh game**.

**Next:** Resolve actual compile/link errors by resource class in one batch, apply explicit namespace/ID mapping and independent SaveBlock/E6 progression, then activate maps. An ephemeral runner installs the assets and appends 13 groups/64 layouts only for a build probe; no third-party art/audio was committed as files to the public project. The main branch never received these assets. After complete import, perform a gap analysis before using Platinum \`.nds\` as a reference for later missing campaign content. Update this handoff on substantial milestones, not after every test.

---

# CURRENT EXECUTION PROMPT: SINNOH FULL IMPORT AUTHORIZED

The maintainer authorized execution of the full import on November 5, 2026. Continue \`sinnoh/full-import\`, not the old documentation-only pause. Work in substantial batches and do not stop at the inventory. The CI [pinned-source staging audit #41995500653](https://github.com/DevElectrum/grand-anthology/actions/runs/41995500653) passed; map/content activation and regional persistence are not implemented. \`src/event_data.c\` fail-closed region fence exists on the branch but still needs build qualification. Follow the latest \`AGENTS.md\` and \`NEXT_ASTRA_HANDOFF.md\` top entries, and the canonical I0–I5 plan. Do not merge \`main\`, publish unlicensed assets/ROMs, or modify user saves.

---

# CURRENT ASTRA RESTART PROMPT: GRAND ANTHOLOGY SINNOH (2026-11-05)

The historical prompt below is from **October 24** and is **not the current task**. Do not resume its P0 Hoenn encounter batch or treat it as higher precedence than current handoffs.

**Repository:** [DevElectrum/grand-anthology](https://github.com/DevElectrum/grand-anthology) `main`, with R8.4 and XGBA's 112-box integration already merged/accepted. `DevElectrum/pokemon-mosaic` is a separate project. No new Sinnoh code has been authorized or implemented.

**Read in this order:**
1. `AGENTS.md` newest 2026-11-05 Sinnoh override.
2. `docs/NEXT_ASTRA_HANDOFF.md` newest 2026-11-05 entry.
3. `docs/SINNOH_FULL_IMPORT_EXECUTION_PLAN.md` canonical I0-I5 design.
4. `docs/SINNOH_UPSTREAM_CONTENT_MANIFEST.md` actual 93-map / 21-skeleton / tileset / audio inventory.
5. `docs/SINNOH_ARCHITECTURE_KICKOFF.md` (historical region/save risks only) and `docs/ANTHOLOGY_R8_4_FINAL_QUALIFICATION.md`.

**Primary upstream:** `sinnoh-remakes/pokeemerald-platinum@c79120fbab7d62c66785c90b491e6a370249d5db` (GBA demake). Import **Sinnoh content** and needed helper dependencies, not generic `pokeemerald-expansion` internals. The demake covers Twinleaf through Eterna with gaps; late Platinum geography and core story must be newly authored. All new flags/vars/trainer IDs and append-only map IDs must remain independent of existing Kanto/Johto/Hoenn, and native/A2/R8 save compatibility must hold.

**Current authorization:** planning and documentation only. No speculative implementation, branches, builds, CI, merge or published assets until maintainer explicitly requests execution. On authorization, I0 is source manifest/ABI/legal/budget proof before bulk integration; then implement substantial batches and use bounded acceptance tests.

---

## ARCHIVED 2026-10-24 PROMPT BELOW (HISTORICAL ONLY)

Astra, continue **Pokémon Grand Anthology**! 🎮

**Canonical public repository:** `DevElectrum/grand-anthology`
**Branch:** `main`
**Last VERIFIED gameplay-code head (before handoff documentation):** `3295e1a6e266923fc6b7335747ad2bdcf38b44fb`
**CI: P0 Gameplay Build #40159598930 PASSED** source validators and optimized GBA ROM build.
**Never touch/import the old private repo:** `DevElectrum/pokemon-grand-anthology`.

### First, read directly from GitHub
1. `AGENTS.md`
2. **`docs/NEXT_ASTRA_HANDOFF_2026-10-24_HOENN_BULK_CLOSEOUT.md`** (newest complete handoff, higher precedence than older "next" notes)
3. `docs/NEXT_ASTRA_HANDOFF_2026-10-24_ACTIVE.md` and `docs/NEXT_ASTRA_HANDOFF.md`
4. **ALL** of `docs/LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md`, especially FINAL delegated locations, not earlier tentative suggestions
5. `docs/JOHTO_BULK_2026-10-24.md`, `docs/JOHTO_TAO_TRIO_2026-10-24.md`, `docs/NYX_MANUAL_QA_2026-10-24.md`, `docs/PUBLIC_STATUS.md`
6. Inspect latest `main`, map events/scripts/layouts, `test/overworld/ValidateAnthologyHabitatRare.py`, lifecycle validator and Actions jobs.

### Priority: substantial P0 engineering, large groups
We **JUST FINISHED** source/optimized CI green for Johto's Tao Trio, another 11 Johto encounter changes, the Kubfu scroll/Calyrex Reins rewards, and a substantial Hoenn Ogerpon hunt + early encounter + Treasures of Ruin batch. **Do not duplicate already implemented encounters or spend an entire response on three Pokémon.**

Audit remaining approved Hoenn static/gating gaps against CURRENT public source, group them by actual location/access, then **implement roughly 10–20+ ordinary encounter/reward changes per batch**, with meaningful commits and coherent bulk source validation. Next candidates include Hoopa's persistent Mirage Tower base room, Deoxys Birth Island ferry after Rayquaza caught, Koraidon Route128 Dive grotto, Miraidon New Mauville annex, other unresolved Hoenn approved entries. Preserve original story/puzzles. Follow the ledger, including catch-based gates and independent repeated-species/form flags. Native P0 faint/revisit/catch/save semantics matter.

**Evidence honesty:** Exact Gameplay Build #40159598930 is all green. Latest checked mGBA run #40159186927 (an EARLIER commit) is red ONLY for the old Kanto daycare/Diancie synthetic doorway fixtures after nine baseline suites passed; those doors were manually verified on Nyx. New Hoenn statics are SOURCE VERIFIED, **NOT** independently end-to-end emulator-caught. Run targeted mGBA where feasible and only summon the user for a preprepared 2–5 minute local Nyx check when genuinely blocked.

**Do not implement now:** Zacian+Zamazenta joint double battle, permanently Complete Zygarde, permanently Stellar Terapagos. Ordinary Zygarde 10%/50% and regular Terapagos remain normal-priority.

Update public `main`, keep docs/handoff current, report concrete progress and commit SHAs. No private archive access, no ROM/save uploads, no repeat of settled design debate. The owner wants **BIG engineering steps, not little ones**.

Big steps, better mistakes, even better solutions.
