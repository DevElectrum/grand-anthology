# Next Astra: **READ ONLY, WAIT FOR DESIGN DISCUSSION** (24 October 2026)

Astra, this is the user's explicit handoff. **DO NOT START ANY WORK.** Your only job on entering this chat is to read the relevant files in the canonical public GitHub repo, understand the existing source and the corrected decisions below, give the user a concise summary, and **WAIT** for further discussion and instructions. **Do not edit files, commit, code, test, invoke CI, open PRs/issues, relocate Pokémon, or act on ideas**. The user wants questions and mutual agreement **before** large engineering changes. Earlier handoff paragraphs telling you to "continue implementation" are superseded.

**Canonical repo:** [`DevElectrum/grand-anthology`](https://github.com/DevElectrum/grand-anthology), branch `main`. The historical private repository `DevElectrum/pokemon-grand-anthology` must **not** be opened, changed, merged or published.

**Read these files directly from public GitHub first**, in order:
1. `AGENTS.md`
2. `docs/NEXT_ASTRA_HANDOFF.md` (the new first section supersedes old tasks)
3. `docs/LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md` (the location list the user requested in a file)
4. `README.md`
5. `FEATURES.md`
6. `docs/PUBLIC_STATUS.md`
7. `docs/P0_PRECHAMPION_ACCESS.md` (historic audit only)

**Newly settled design:** All 1,025 National species need to be legitimately obtainable **eventually across the complete Kanto/Johto/Hoenn adventure including postgame**; there is **NO requirement** for all of them before the first Champion. The previous pre-Champion fixation was an unwanted assistant assumption. Ordinary campaign/story gates should be respected. **Legendary and Mythical Pokémon should be static scripted encounters**, but don't assume they all must be late-game or require elaborate quests. **Ultra Beasts and Paradox Pokémon are NOT covered by the static-only request**: they are deliberately natural rare encounters. **Kartana in Viridian Forest was deliberately placed at the user's request, and Poipole on Route 1 is accepted.** Avoid deleting or "fixing" these. The user has **not** approved any encounter changes yet.

**Audit issues for discussion only:** Five Legendary form/base species are also in supplemental **wild** habitat code: Groudon (Route 111), Kyogre (Seafloor Cavern Room 1), Galarian Zapdos (Route 22), Galarian Articuno (Seafoam B1F) and Galarian Moltres (Pokémon Mansion 1F). All have static sources too. A post-first-Hall-of-Fame rotation can put Legendary/Mythical species into every third qualifying wild encounter anywhere, or repeatedly substitute a player-chosen Research target. Discuss whether that shortcut should be kept, constrained or removed. Many scripted statics appear relatively early, but user has not objected to early **static** Pokémon or approved relocation. Full map/species inventory and limits are in the audit file.

**Existing project state, not a request to work:** 1,025/1,025 source paths are optimistic, not verified collection. The latest pre-handoff gameplay commit `10bf90d` [passed source validation and optimized ROM build](https://github.com/DevElectrum/grand-anthology/actions/runs/40066826520). [mGBA run](https://github.com/DevElectrum/grand-anthology/actions/runs/40066826492) passed nine broad gameplay suites, with two separate synthetic door navigation failures that never reached daycare/Diancie targets. Remaining P0 normally includes source/progression review, integration and playthrough acceptance, but **do none of it until explicitly requested**. Debug Warp-to-Map in `src/debug.c` is existing machinery; building a QA warp harness was only suggested, not authorized. PC expansion, Dynamax/Gigantamax, Tera and native port remain deferred. Public source only, never ROMs or private history.

**Your first response:** Confirm you have read the repo and audit, briefly surface the decisions/open questions, and invite discussion. Then stop. **Do not start coding or testing under any circumstance until the user explicitly says so.**
