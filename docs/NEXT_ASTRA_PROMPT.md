Astra, continue **Pokémon Grand Anthology**! 🎮

**Repository:** `DevElectrum/grand-anthology`, public `main`. Do not touch the old private archive `DevElectrum/pokemon-grand-anthology` or import its history.

**First read directly from GitHub:**
1. `AGENTS.md`
2. **`docs/NEXT_ASTRA_HANDOFF_2026-10-24_ACTIVE.md`** (our current full project/test/design handoff)
3. `docs/NEXT_ASTRA_HANDOFF.md`
4. **ALL** of `docs/LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md`, especially the final delegated location section, not just older tentative design tables
5. `docs/NYX_MANUAL_QA_2026-10-24.md`, `docs/PUBLIC_STATUS.md`, `docs/PREIMPLEMENTATION_DESIGN_AUDIT_2026-10-24.md`, `docs/LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md`
6. `README.md`, `FEATURES.md`, current map/scripts/flags/layouts and CI results.

**Latest completed gameplay baseline:** `6621cd852e09038377654611d8bd7a0a36a22db7` (Johto Swords linker repair), with later docs-only handoff commits. Refresh current `main` before editing. [Optimized build + validations #40151749582](https://github.com/DevElectrum/grand-anthology/actions/runs/40151749582) **PASSED**. [Same-SHA mGBA #40151749580](https://github.com/DevElectrum/grand-anthology/actions/runs/40151749580) passed nine normal suites and stayed red only on the old synthetic Diancie/daycare door fixtures. Both doors were independently manually verified in actual gameplay on Nyx, with Diancie lifecycle and daycare Manaphy+Ditto→Phione hatch both tested. **Do not mistake a bad fixture for broken gameplay or claim CI fully green.**

**Implemented already:** Giratina to Mt Moon B2F with caught Dialga+Palkia; Darkrai to Lavender Tower7F after caught Cresselia+Fuji rescue; Tapu Lele Celadon garden; four independent early/mid Johto Swords Keldeo Route32 Lv18, Terrakion UnionCave1F Lv20, Virizion National Park Lv23, Cobalion Mt Mortar 1F South Lv28. Linker correction finished and ROM build PASS. No need to redo them. New static battles remain to be accepted with gameplay.

**Next code batch:** Johto **Tao trio**. Reshiram independently lower Bell/Tin Tower after natural late-Johto tower admission (not Ho-Oh caught-beasts roof gate); Zekrom independently Dark Cave Blackthorn/Route45-side; Kyurem Ice Path B1F after *both Reshiram and Zekrom dex registered caught*, **before Johto Champion**. No old Route45/Route44 duplicate statics. Reward DNA Splicers on Kyurem catch and preserve reversible White/Black Kyurem and constituents. Verify actual access, original stories, puzzles, source/object flags and item mechanics before committing. Use substantive grouped implementation, validation, optimized builds and emulator tests.

**Project contract:** one persistent trainer/team/PC through original Kanto/Johto/Hoenn, casual/no forced scaling/resets, 1,025 species obtainable eventually including postgame (not necessarily pre-first-Champion); Legendaries/Mythicals usually deliberate statics; rare existing wild species may remain if actual lore justifies multiple individuals (e.g., Zarude, whose special static still moves to Ilex Forest). Ultra Beasts/Paradox ordinary rare wild allowed; **optional randomizer exempt from lore, automatic Research/discovery is NOT**. Distinct catch flags for multiple copies/forms, caught-dex prerequisites, faint/revisit rematch, caught/save/reload permanent hide. Honor full ledger, preserve story and puzzles. Three especially complex final implementation tasks are Zacian+Zamazenta paired battle, permanent postgame Complete Zygarde, permanent postgame Stellar Terapagos.

**QA agreement:** Automate as much as possible. The user can happily handle an occasional **2–5 minute Nyx manual test** with prepared local-only teleport flags, quick Pokémon and protected saves; do not make them grind or spend their evening fighting a broken test fixture. Never commit local-only sign or Egg developer cheats. Distinguish source pass, build pass, emulator pass, and manual pass.

The user already said **“Begin”** and **“Continue”**. The old design-only “wait” directives in archived docs have been superseded. Continue implementing without reopening settled discussions, but ask if a substantial new decision is genuinely unclear. Keep the user informed when working. **Big steps, better mistakes, even better solutions.**
