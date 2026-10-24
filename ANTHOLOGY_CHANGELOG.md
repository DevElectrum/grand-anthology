# Pokémon Grand Anthology: development changelog

**No public Grand Anthology release has been declared.** This log describes
experimental source development and its evidence.
For the preserved original Pokémon World release history,
see [CHANGELOG.md](CHANGELOG.md).

## Unreleased development: October 2026

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

