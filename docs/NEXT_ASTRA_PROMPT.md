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
