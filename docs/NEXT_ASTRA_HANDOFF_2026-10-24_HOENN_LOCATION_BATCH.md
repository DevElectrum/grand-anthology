# NEXT ASTRA: Hoenn location and form engineering batch

**Written:** October 24, 2026 US Central / October 25 UTC.
**Canonical repository:** `DevElectrum/grand-anthology`, PUBLIC, `main`.
**Never inspect, modify, or import from the archived private `DevElectrum/pokemon-grand-anthology`.**

## Start here

1. Read `AGENTS.md`, this handoff, `docs/NEXT_ASTRA_HANDOFF_2026-10-24_HOENN_BULK_CLOSEOUT.md`, `docs/NEXT_ASTRA_HANDOFF_2026-10-24_ACTIVE.md`, and `docs/NEXT_ASTRA_HANDOFF.md`.
2. Read **ALL** of `docs/LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md`, especially final delegated Hoenn locations, then `docs/NYX_MANUAL_QA_2026-10-24.md` and `docs/PUBLIC_STATUS.md`.
3. Verify HEAD and fresh GitHub Actions before claiming build success or gameplay acceptance. **Last confirmed fully green source validators + optimized build at the time this handoff was written remains `3295e1a6e266923fc6b7335747ad2bdcf38b44fb`, run `40159598930`.** The source commits below were not yet finally accepted by an optimized build plus emulator at handoff-writing time. Update this statement only after checking the workflows.
4. The prior checked mGBA release smoke `40159186927` compiled and passed nine baseline cases but failed two known artificial Diancie/daycare doorway fixtures; the interactions were separately verified on Nyx. Never call all gameplay accepted merely because source validators and optimized build are green.

## Source changes now on public main

Four substantial gameplay commits:
- `3a49f634766391396851f71918ff25d0d9c58d1e`: Eon duo, Deoxys ferry, native weather gates, mythic level adjustments.
- `b8e4ad0f240233fdb9994443a91119079c68fd8d`: independent 10% and 50% Zygarde catch flows and saved capture flags, old Granite Cave duplicate removed.
- `709607f377d6291a0b3a850d33f8d78009258a9e`: Koraidon Route 128 dive approach and Miraidon New Mauville interior, story gates, corrected Meteor Falls tile.
- `30b38b8d01d7b30b73723fac39ad56298bafb686`: removed pre-League Groudon/Kyogre duplicates that violated the final postgame design; updated static inventory and regressions.
- `b9b4bc5c20dbfb6ae1f15e55f7f20cc0176289fd`: corrected caught native-weather-trio hide precedence and added state-transition tests for Hoenn story and form gates.
- The commit adding this handoff also explicitly branches the two Zygarde caught scripts to their resolved labels so the source lifecycle interpreter and in-game scripts agree.

### Encounter inventory and source-level gates

| Encounter | Current source location / level | Gate, persistence, notes |
|---|---|---|
| Latias | Route 120, (25,61), Lv35 | Caught-bit static; old Route 118 duplicate removed |
| Latios | Mossdeep Space Center 1F, (9,4), Lv40 | Defeat Space Center Magma; old Route 120 Latios replaced with Latias. Interior is near, not outside, the Space Center; revisit exterior location fidelity if desired |
| Deoxys | Native Birth Island triangle puzzle, Lv30 | Ordinary Lilycove harbor sailor offers ferry after **Rayquaza caught**; no AuroraTicket, ferry departure state recorded; old Space Center duplicate replaced with Latios; caught-dex hide and defeat retry |
| Zygarde 10% | Petalburg Woods, (16,19), Lv10 | Aura Break, separate `FLAG_CAUGHT_ANTHOLOGY_ZYGARDE_10`; first-gym access |
| Zygarde 50% | Meteor Falls 1F, (14,27), Lv44 | Badge 6 gate, **Power Construct**, independent `FLAG_CAUGHT_ANTHOLOGY_ZYGARDE`; default Granite Cave placement deleted |
| Koraidon | Underwater Route 128, (42,27), Lv45 | Badge 7 and Space Center Magma victory; placed by seafloor approach, not yet a separate dedicated grotto-room map |
| Miraidon | New Mauville Inside, (34,24), Lv45 | Badge 7, Space Center Magma victory, and Wattson generator quest switched off (state 2); still same original map, not a new annex-room map |
| Hoopa (Confined) | Route 111 Mirage Tower-side permanent exterior, (30,56), Lv30 | Route 111 remains available with either Mirage Tower layout; **still needs the dedicated ring chamber** if the full approved architecture is to be realized |
| Yveltal | Mt. Pyre Exterior, (13,17), Lv39 | Midgame static; source level lowered from 50 |
| Groudon | Native Terra Cave | Postgame native story, **Rayquaza caught required**, permanent caught hide; Fiery Path duplicate removed |
| Kyogre | Native Marine Cave | Postgame native story, **Rayquaza caught required**, permanent caught hide; Route 125 duplicate removed |

These are encounter-source changes, not proof the map animations, conversations, capture, faint, save, and re-entry sequences have been accepted in the emulator. The binary tile candidate search in GitHub Actions found the initial Meteor Falls Zygarde (15,23) was blocked; source adjusted to legal (14,27). Other new map tiles passed the validator stage in run `40161612805`, but later validators initially stopped at outdated source contracts before these repairs.

### Tests and validation

- `test/overworld/ValidateAnthologyHabitatRare.py` now tracks **85** optional static/backup field objects, fewer than before **because redundant Deoxys, Groudon, Kyogre and old locations are removed**, not because species are lost.
- `test/overworld/ValidateAnthologyStaticLifecycle.py` covers source-level catches, faint/re-entry, escapes, separate Zygarde flags, Mossdeep unlocks, Koraidon/Miraidon requirements, native Groudon/Kyogre caught and Rayquaza prerequisites, and Deoxys ferry source presence.
- Test logic is not an emulator substitute. Examine newest `P0 Gameplay Build` and `P0 mGBA Release Smoke` runs for actual pass/fail and full logs.
- Fix any compiler, binary tile, script label, or regression issues revealed by fresh Actions before declaring build green. Previous intermediate commits had expected red CI while coordinated changes and validators were being updated.

### Remaining approved engineering priorities

1. Finish Hoopa's **permanent, Mirage Tower-independent themed ring chamber**. The current exterior is persistent, but not the full planned chamber. Do not put it behind the collapsing fossil tower.
2. Refine Koraidon's dive site into a distinct Route 128 grotto and Miraidon's New Mauville placement into an annex, without disrupting the Seafloor Cavern entrance or Wattson's switch puzzle. Current source encounters are in the approved geographic areas but not separate interiors.
3. Run targeted emulator path/catch/faint/re-enter/save tests for the above batch, especially Deoxys ferry and triangle, independent two Zygarde captures, Regi/Weather Trio original stories, and the underwater and generator routes. The user can help with one prepared 2–5 minute Nyx test only if automation genuinely cannot settle it.
4. Audit remaining approved Hoenn ledger entries against current source **before** implementing another 10–20+ ordinary location-grouped batch. Never duplicate existing encounters, remove native story or alter progression inadvertently.
5. Reserve **Zacian + Zamazenta special paired double battle**, **permanent Complete Zygarde**, and **permanent Stellar Terapagos** for late implementation. Ordinary early/mid counterparts should not be deferred with those special features.

**Do not reopen settled design decisions.** The design ledger is final, and the archived private repository is out of scope. Big steps, better mistakes, even better solutions.
