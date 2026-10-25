# Active Astra catalog entrypoint (25 October 2026)

**Future Astras: search the current source-indexed acquisition atlas before editing Pokémon, item, shop, or reward placement.**

- [Astra's catalog instructions and query recipes](data/catalog/README.md)
- [Machine-readable acquisition index: all 1,025 National species + 874 numbered items](data/catalog/world_acquisition.jsonl)
- [Player's searchable field guide](docs/GRAND_ANTHOLOGY_FIELD_GUIDE.md)
- [Current Royal Duo contract](docs/ROYAL_DUO_ROUTE27_IMPLEMENTATION_2026-10-25.md)

The JSONL is a **candidate-path inventory**. It is not proof that each encounter is reachable, catchable, persistent or accessible at any badge count. Cross-check map scripts, current source tests and reported gameplay. Maintain the Astra index and the human field guide **together** whenever encounters, rewards, prerequisites or item sources move. Old handoffs/ledgers below may describe retired positions or proposed-only features; live source and newest approved design take precedence.

**October 25 source progress:** Royal Duo level-50 Crowned Zacian/Zamazenta double battle on Johto Route 27 after eight badges, held Rusted Sword/Shield gifts, source-validator PASS. Full optimized P0 build and source validator PASS at [run 40234899967](https://github.com/DevElectrum/grand-anthology/actions/runs/40234899967), and [mGBA baseline smoke 40231997639](https://github.com/DevElectrum/grand-anthology/actions/runs/40231997639) PASS on its earlier code SHA. No dedicated Royal Duo physical gameplay acceptance. **Eternatus remains on Route 44 in current source**; a conditional Victory Road relocation is discussed but not implemented. The catalogs are *documentation*, not an authorization to change gameplay.

---

> **25 OCT 2026, PERMANENT STELLAR TERAPAGOS SOURCE IMPLEMENTED:** [Focused engineering note](docs/PERMANENT_STELLAR_TERAPAGOS_2026-10-25.md) describes a second level-75 permanent Stellar encounter in Cave of Origin B1F post-Hoenn-Champion; original Route 114 normal Terapagos remains independently catchable even if the Stellar one was obtained first. The Stellar species now uses a no-reversion form-change table; Teraform Zero triggers on switch-in with no Tera activation. Added `test/overworld/ValidatePermanentStellarTerapagos.py`; exact optimized build and actual PC/save/battle gameplay evidence remain separate. Mega Stone source coverage is committed but map geometry and old Shoal Cave CI validation may still need later attention. Canonical public main only.

> **CURRENT ACTIVE DISCUSSION (25 OCT 2026): Z-A MEGA STONE ACQUISITION.** Read [the 45-stone availability + split-distribution ledger](docs/ZA_MEGA_STONE_ACQUISITION_LEDGER_2026-10-25.md) and [fresh next-Astra handoff](docs/NEXT_ASTRA_HANDOFF_2026-10-25_ZA_MEGA_STONES.md) before the older engineering notes below. Maintainer wants a **mix of world/NPC locations and Battle Net vendor**, explicitly mentioning Clefable and Victreebel for world discovery. **This is a discussion, not approval to implement the ledger's illustrative categories.** Complete Zygarde source is newly committed at `12f99767`; 10%/50%/Complete gameplay tests are delegated to next Astra. Do not mistake source presence for actual stone acquisition.

> **CURRENT HOENN BULK HANDOFF (24 Oct local / 25 Oct UTC):** [Read this newest verified closeout](docs/NEXT_ASTRA_HANDOFF_2026-10-24_HOENN_BULK_CLOSEOUT.md) before the older historical notes below. Latest gameplay-source HEAD `3295e1a6e26`; [P0 Gameplay Build #40159598930](https://github.com/DevElectrum/grand-anthology/actions/runs/40159598930) **PASS** (source validators + optimized ROM). Kubfu Scrolls from Chuck/Kiyo and Ruins Reins **implemented**; Hoenn Ogerpon/Loyal Three/Pecharunt, Treasures of Ruin, Jirachi, base Terapagos, Xerneas/Volcanion bulk source **implemented**. Last checked mGBA #40159186927 (earlier SHA) red on **two known artificial Diancie/daycare doorway fixtures**, not proof new gameplay failed. Keep 10–20+ encounters per ordinary regional engineering pass. Read newer handoff for precise caveats and remaining Hoenn targets. 

> **BULK JOHTO LOCATION ENGINEERING, newer than all three-at-a-time notes (24 Oct local / 25 Oct UTC):** The maintainer explicitly requires BIG, location-clustered passes (roughly 10–20 scripted encounters per regional batch), not family-by-family signoffs. Source commits on public `main`: [Forces of Nature / Enamorus gate](https://github.com/DevElectrum/grand-anthology/commit/1445e80b23fe346856a73a38888a031054a07654), [two Kubfu + Spectrier](https://github.com/DevElectrum/grand-anthology/commit/22ae8b9c72397480d079b25624743a7dd000bb3f), [Glastrier + Victini](https://github.com/DevElectrum/grand-anthology/commit/8e2c5374889ee350c508592be9a937efabb8a1fe), and [two Regis](https://github.com/DevElectrum/grand-anthology/commit/1d156588d0ecb427b1bfe9d85637057af5ef24b8). **11 source encounter relocations/gates implemented in one sweep**, prior Tao Trio was 3 more. `docs/JOHTO_BULK_2026-10-24.md` records sites, flags, CI status and remaining verification. **Do not call them gameplay PASS merely because committed.** Follow optimized ROM/source CI to repair incorrect collision coordinates or script errors. The mGBA regression contains unrelated old Diancie/daycare synthetic doorway fixture failures despite Nyx physical PASS. Avoid stalling on these. Continue substantive region-wide gameplay batches while isolating specific runtime blockers. Only canonical public repo, keep private archive untouched.

> **TAO TRIO SOURCE BATCH NOW ON PUBLIC MAIN (24 Oct 2026 local, overriding the older "Tao is next" text below):** [Reshiram lower Tin Tower 2F Lv44](https://github.com/DevElectrum/grand-anthology/commit/9dd78c8d6be77e0baa40c84f56c13d0a36672c65), [Zekrom Dark Cave NorthSide Lv44](https://github.com/DevElectrum/grand-anthology/commit/bbad6b9b24af11569f9e2c9db9aede32ffdbbdcd), [Kyurem Ice Path B1F Lv49 after BOTH dragons caught, DNA Splicers on catch with full-bag recovery](https://github.com/DevElectrum/grand-anthology/commit/db3e67e45b720bc340662442bd4cbe3f3145bd38). Old Route45/44 statics deleted. [Source inventory, bespoke validator and mGBA triggers](https://github.com/DevElectrum/grand-anthology/commit/fc2d6338ea84abcb05b731ccbc79953496b22e37); [static test interpreter crash repair](https://github.com/DevElectrum/grand-anthology/commit/79937e62b4f3326e342e3cafe51a47faf292fb69). **Source committed is NOT the same as optimized build/emulator gameplay PASS**. An early source job failed because of the test interpreter's eager string-versus-boolean numeric comparison, now fixed; rerun source CI, optimized build, and mGBA outcomes must be checked. [Implementation/evidence details](docs/JOHTO_TAO_TRIO_2026-10-24.md). Keep native Ho-Oh and Ice Path story intact. Do not redo Tao placement. Next is acceptance/repair of the new batch followed by the next approved Johto ledger relocation, not reopening settled design.

> **CURRENT COMPLETE DISCUSSION/ENGINEERING HANDOFF (24 Oct 2026):** [Read the active full handoff](docs/NEXT_ASTRA_HANDOFF_2026-10-24_ACTIVE.md) and [copy-ready continuation prompt](docs/NEXT_ASTRA_PROMPT.md). Includes approved cross-region design, Nyx manual QA, verified Johto Swords CI and next Tao trio implementation. These supersede older design-only pause notes below.

> **Current authorized gameplay head, 24 October 2026:** Johto's four independent Swords have been relocated in [`0712fb0`](https://github.com/DevElectrum/grand-anthology/commit/0712fb05627259291c1278b3bdeac34ec4f1e447), with dangling-linker repair [`6621cd8`](https://github.com/DevElectrum/grand-anthology/commit/6621cd852e09038377654611d8bd7a0a36a22db7). [Build #40151749582](https://github.com/DevElectrum/grand-anthology/actions/runs/40151749582) PASSED. [mGBA #40151749580](https://github.com/DevElectrum/grand-anthology/actions/runs/40151749580) passed nine baseline suites; red remains from the previously understood Diancie/daycare synthetic doorway tests. Next implement Tao trio with independent Reshiram/Zekrom and caught-both-gated Kyurem, not more speculative fixture tinkering. See [active handoff](docs/NEXT_ASTRA_HANDOFF.md). Old design-only stop notes below are historical.

> **CURRENT ENGINEERING OVERRIDE (24 October 2026, supersedes every historical design-pause instruction below):** The maintainer subsequently said **“Begin”** and then **“Continue”** after design review. Public `DevElectrum/grand-anthology` **`main` is authorized for ongoing P0 gameplay engineering**, grouped commits, source validation and mGBA checks. This includes the Kanto Giratina/Darkrai/Tapu Lele implementation in [`9ffe40e`](https://github.com/DevElectrum/grand-anthology/commit/9ffe40e4e308b9ae92f1860dbf9be0a37d7eb5cb). Old “documentation only / no tests / wait” notes remain historical, NOT active instructions. Honor the approved [design ledger](docs/LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md), and ask for input only if a consequential design choice is genuinely unresolved. Canonical public source only; do not touch the private predecessor.

> **Current handoff override, 24 October 2026 (newer than the design-pause note below):** The maintainer explicitly authorized **a documentation-only repository update** to publish the [full design ledger](docs/LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md), [pre-implementation design audit](docs/PREIMPLEMENTATION_DESIGN_AUDIT_2026-10-24.md), and [new handoff](docs/NEXT_ASTRA_HANDOFF_2026-10-24_DESIGN_READY.md). **The next Astra may read all public repo source to reconcile the design, but must WAIT for explicit approval before writing gameplay code, altering encounters/maps, running tests/builds/CI, or committing further engineering changes.** Earlier "do not edit docs / GitHub" instructions below refer to the superseded pre-publication discussion pause. See [prompt](docs/NEXT_ASTRA_PROMPT.md). Do not touch the private historical repo.

# Grand Anthology public development instructions

**Canonical repository:** `DevElectrum/grand-anthology`, branch `main`.
**Read [the current next-Astra handoff](docs/NEXT_ASTRA_HANDOFF.md) first.**
This is the NEW public, clean-history, source-only development repository.
Do not import or reveal private predecessor history or modify any unrelated
private repository and its original `master`.

## Goal

**Historical design-pause note (24 October 2026): superseded.** The maintainer subsequently authorized P0 engineering and, on October 25, specifically authorized this two-view catalog and README documentation update. For any **new** gameplay change, follow the latest direct user request; do not infer permission from an old handoff. Consult the source-indexed atlas above.

Build an easy, casual, approachable single-trainer Pokémon GBA adventure
covering the inherited Kanto, Johto and Hoenn campaigns. Do not pretend it
covers nine regions. All 1,025 National Pokédex species should be obtainable **eventually across the complete adventure including postgame**, not necessarily before the first Champion. Source-level 1,025 path evidence is optimistic; actual in-game acquisition/integration acceptance is **not complete**.

For future gameplay changes explicitly approved by the maintainer, prioritize substantial P0 implementation in sensible grouped batches,
then consolidated gameplay verification. Commit meaningful changes and
review existing tests; don't stall on individual encounter test cases.
Original 420-slot storage remains for now. Optional later gimmicks and
native port work are deferred. Preserve casual default gameplay and
the current project's original humor, credits and upstream acknowledgments.

## Security, redistribution, and CI

Experimental public **source only**, not a completed game or downloadable ROM.
Do not distribute ROMs, save files, asset dumps, personal data, GitHub
tokens or historical private artifacts in code, Actions, releases or issues.
Maintain original [credits](CREDITS.md) and
[third-party notices](THIRD_PARTY_NOTICES.md). Those notices do not
supply a blanket license for Pokémon IP, code and art.

Use public `main` and the two read-only workflows under
`.github/workflows/` for game-source/build and real mGBA regression.
Generated ROMs are ephemeral on runners and must never be uploaded.
Source validators, compilation and emulator gameplay have different proof
strengths. Green builds are not proof that all 1,025 Pokémon are catchable;
red emulator CI is normal unfinished development, not a publication block.

For specific current priorities and verified CI evidence, read
[docs/NEXT_ASTRA_HANDOFF.md](docs/NEXT_ASTRA_HANDOFF.md) and
[docs/PUBLIC_STATUS.md](docs/PUBLIC_STATUS.md).
