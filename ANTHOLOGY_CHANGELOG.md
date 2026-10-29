# Pokémon Grand Anthology: development changelog

**No public Grand Anthology release has been declared.** This log describes
experimental source development and its evidence.
For the preserved original Pokémon World release history,
see [CHANGELOG.md](CHANGELOG.md).

## Unreleased development: October 2026

### Landorus special Flying coverage and TM51 (29 October 2026)

- Landorus (Incarnate and Therian) now learns **Air Slash** at Lv37,
  **Nasty Plot** at Lv43, and **Hurricane** at Lv55 through the existing
  shared level-up moveset. Already-caught Landorus can recover eligible
  previous-level moves with the Move Relearner.
- Previously inactive **TM51** is now reusable **Nasty Plot** (same item
  ID 632), priced at 3,000, obtainable from the TM clerks in Celadon
  2F, Goldenrod 5F, and Lilycove 4F. Goldenrod's clerk intentionally
  shares Lilycove's TM shop list.
- The teachable-learnset generator uses a single explicit Grand Anthology
  exception so Landorus can use TM51 regardless of upstream learnability
  data. Other Pokémon keep the existing cross-game teaching policy.
- No save or item ID migrations. Existing ROMs must be rebuilt; code
  validation and real gameplay acceptance are separate.

### Johto field-move badge correction (28 October 2026)

- Fixed four Johto HM field-use gates that incorrectly reused Hoenn badge order:
  Flash now requires Falkner's Zephyr Badge (1), Cut Bugsy's Hive Badge (2),
  Strength Whitney's Plain Badge (3), and Fly Chuck's Storm Badge (5).
  The other regions' field-move requirements remain unchanged.
- Added a source regression check grounded in each Johto Gym's actual badge
  award, wired into the P0 gameplay build CI. Existing saves do not need
  a new flag, item, or migration; a newly compiled ROM is required.

### Research retirement (28 October 2026)

- Removed the post-Champion Research species-substitution mechanism from
  grass, walking overworld Pokémon, Surf and fishing. Finishing a regional
  League no longer changes unrelated route wild encounter species.
- Removed Research from the legacy text Start menu and deleted the
  target/rotation debug UI. The graphical menu never implemented it.
- Kept the optional seeded Wild Randomizer and all authored native, rare,
  supplemental and aquatic habitats, statics, eggs, gifts and evolutions.
- Reserved the now-unused game-stat index 53 without altering save layouts
  or clearing existing saves. The optimistic 1,025-species acquisition
  catalog has no Research-only sources; runtime accessibility remains
  a separate gameplay acceptance matter.

### Gameplay engineering in progress

- Expanded the source roster to all **1,025 National Pokédex IDs** through
  Generation 9, divided into Kanto (346), Johto (352) and Hoenn (327)
  regional generation groups. The October 23 source-reachability validator
  reported **1,025/1,025 optimistic acquisition paths**.
- Added many contextual habitats, fossil and scripted encounter sources;
  implemented 10 Kanto / 9 Johto / 9 Hoenn starter-choice source paths
  and expanded offline evolution, forms and player options.
- Continued work on one trainer/party/PC/bag/save across three inherited
  campaigns, while preserving the 420-slot PC for the initial playable scope.

### Verified versus pending

- Optimized GBA build/source validation **passed** in private development CI
  at gameplay commit `45dc98d9`.
- mGBA release smoke **failed** at that milestone; the missing Lua release
  symbol was subsequently fixed, but mGBA at `5667f5f` still failed the real
  Diancie field visibility test. The full encounter suite is not accepted
  until a passing exact-commit emulator run.
- Real-world pre-Champion acquisition of all species, meaningful forms,
  full story completion, save migration and every configured mode
  remain **unverified or incomplete** as documented by
  [public development status](docs/PUBLIC_STATUS.md).

### Documentation preparation

- Introduced a distinct README and atlas-themed fan-project banner,
  retained the upstream credits, clarified build instructions and
  documented publication gates. These changes are **not** a gameplay
  readiness or public-release milestone.

