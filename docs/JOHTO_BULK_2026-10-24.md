# Johto bulk encounter implementation checkpoint (24 October local / 25 October UTC)

**Canonical:** `DevElectrum/grand-anthology`, `main`. Maintainer's new policy is grouped **~10–20 location changes per batch**, with CI and short targeted QA, rather than 3 Pokémon followed by another handoff.

## Johto bulk sweep (four source commits, eleven encounter changes)

| Species / event | Final scripted site | Level | Special rules |
|---|---|---:|---|
| Tornadus | Route 44, (57,24) | 40 | Independent; moved from Route38 |
| Thundurus | Route 41, (17,56) | 36 | Independent, maritime Route 41; moved from Route38 |
| Landorus | Route 43, (14,25) | 42 | Independent; moved from Route39 |
| Enamorus | Route 39, existing (30,1) | 43 | Existing static remains, but **requires caught Pokédex** for Tornadus, Thundurus, Landorus; previously ungated |
| Kubfu source #1 | Sprout Tower 2F, (14,11) | 12 | Independent `FLAG_CAUGHT_ANTHOLOGY_KUBFU_WEST`; source moved from Route42 |
| Kubfu source #2 | Route34 near daycare, (39,33) | 18 | Independent `FLAG_CAUGHT_ANTHOLOGY_KUBFU_EAST`; source moved from Route42 |
| Spectrier | CliffEdgeCave/Route47, (22,26) | 34 | Independent; moved from Burned Tower B1F |
| Glastrier, Johto source | Ice Path B2F, (21,20) | 42 | Independent of Spectrier; moved from Route44; still shares caught dex with optional Hoenn Shoal Cave source |
| Victini | National Park Normal, (34,23) | 42 | **After ANY ONE** regional championship; native `FLAG_SYS_GAME_CLEAR` is authoritative any-region first Hall of Fame bit; moved from Route48 |
| Regieleki | Olivine Lighthouse, (151,10) | 37 | After `FLAG_AMPHAROS_HEALED`; moved from Ruins of Alph exterior |
| Regidrago | Dragon's Den Cavern, (35,43) | 46 | Independently available through native Blackthorn / Dragon's Den story; moved from Route45 |

**Interpretation of counts:** Eleven scripted sources were modified or relocated in these four commits. Enamorus's event was *gated*, not moved. Zarude was already at Ilex Forest and Calyrex already at Route36, so neither was pointlessly duplicated or relocated.

Source-of-truth `test/overworld/ValidateAnthologyHabitatRare.py` was updated for changed statics. Scripts use persistent caught-dex hide-on-revisit, KO respawn on re-entry, fleeing stays. Kubfu copies have separate captured flags and both must be obtainable in one playthrough. Existing natural regions, native cutscenes, daycare and cave puzzles remain in source. **Gameplay acceptance is NOT yet claimed**. Source CI diagnosed blocked original sites for Route34 Kubfu and Dragon's Den Regidrago; corrected them to legal tiles in a follow-up commit.

## Verification and next actions

- **Known baseline:** prior Tao Trio `79937e6` full source checks and optimized ROM build passed, while the 11 regular mGBA suites ran with 9 passing and two previously demonstrated incorrect synthetic doorway fixtures for Diancie/daycare.
- **Bulk sweep:** [Forces CI](https://github.com/DevElectrum/grand-anthology/actions/runs/40156178343) source and optimized build **PASS**. The later multi-commit bulk CI for `1d156588` must finish, and any failed map, encounter or linkage assertion must be repaired. A red mGBA run may reflect the two known synthetic fixtures, not an actual new broken doorway.
- **Runtime acceptance still needed:** each new/relocated battle's accessibility, native story gating (especially Lighthouse, Dragon's Den, Route41 surfing), faint→revisit, caught→save/reload permanence, both Kubfu captured despite shared Dex, Victini after each possible championship and Enamorus after caught trio.
- **Next wide-scope group:** finish remaining ordinary Johto ledger gaps and item rewards (Ruins of Alph Reins of Unity, Chuck Scroll of Waters, Mt Mortar Scroll of Darkness), then perform a location-grouped Hoenn pass covering approved Legendaries/Mythicals. The special Zacian/Zamazenta double battle, permanent Complete Zygarde and permanent Stellar Terapagos stay deliberately last.
- **Manual Nyx QA:** developer shortcuts only in isolated copied saves and LOCAL test builds, never in public GitHub. Use 2–5 minute sessions only if automated emulation cannot resolve an actual gameplay blocker.

Keep current user-requested **big batches** and avoid reopening settled design debates.
