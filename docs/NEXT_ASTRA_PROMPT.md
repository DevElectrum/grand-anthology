# Next Astra: Pokémon Grand Anthology, design-complete handoff (24 October 2026)

Astra, we're continuing **Pokémon Grand Anthology**. The canonical public repository is **DevElectrum/grand-anthology**, branch **main**. Do not access or modify the archived private DevElectrum/pokemon-grand-anthology repository or import private history.

## First: read the repository and *our full discussion decisions* before suggesting implementation

Read these **directly from GitHub**, in this order:

1. AGENTS.md
2. docs/NEXT_ASTRA_HANDOFF.md
3. docs/NEXT_ASTRA_HANDOFF_2026-10-24_DESIGN_READY.md
4. **docs/LEGENDARY_MYTHICAL_DESIGN_LEDGER_2026-10-24.md**: **read ALL sections, especially the later superseding decisions and delegated location closeout**. It records our extended Kanto → Johto → Hoenn discussion. Do not base decisions only on the ledger's earliest tentative tables.
5. docs/LEGENDARY_MYTHICAL_LOCATION_AUDIT_2026-10-24.md: previous authored-source location inventory, NOT proof that every encounter is playable.
6. docs/PREIMPLEMENTATION_DESIGN_AUDIT_2026-10-24.md: issues and reachability traps.
7. README.md, FEATURES.md, docs/PUBLIC_STATUS.md, and the historical docs/P0_PRECHAMPION_ACCESS.md.

Then perform a **read-only review of the current public main source** to compare real scripts, maps, flags, wild/discovery/roamer mechanics, items, evolutions, abilities, form support and CI evidence with our decisions. Be precise about the gap between source-level placement, valid map traversal, actual gameplay and game-breaking conflicts. Do not claim testing not performed.

## Non-negotiable design

- Three original-region adventures with one persistent trainer/team/PC. Casual progression; overpowered carryover accepted. All **1,025 species eventually obtainable including postgame**, not necessarily before one Champion.
- Legendary/Mythical Pokémon primarily deliberately scripted static encounters, not random grass. Ultra Beasts and Paradox Pokémon remain rare natural wild Pokémon. Kartana in Viridian Forest and Poipole early are intentional. Preserve original story events and classic puzzles. Roaming beasts should become fixed statics; caught/defeated/fled event persistence must be safe.
- **Our detailed locations/levels/dependencies have already been chosen** across Kanto Gen 1/4/7; Johto Gen 2/5/8; Hoenn Gen 3/6/9. See full ledger, especially the latest delegated choices for Kubfu, Tao Trio, box dragons, Hoopa permanent chamber, Deoxys ferry, and the Ogerpon hunt. Do not reopen settled questions without a concrete source conflict.
- **Special mechanisms deferred until the end of Legendary/Mythical fixes:** Zacian+Zamazenta double battle with both awarded only after defeating both; separately catchable permanently Complete Zygarde near Littleroot post-Hoenn Champion; separately catchable permanently Stellar Terapagos in postgame Cave of Origin. Regular 10%/50% Zygarde and regular Gym-4 Terapagos are normal earlier implementation work. 50% Zygarde has pre-Champion usable Power Construct as a *temporary glimpse* of Complete; 10% never gets it.
- Review/remove five Legendary wild supplemental habitats and fix the post-League discovery/Research bypass *without deleting intended Ultra Beast and Paradox wilds*. Keep original Regis puzzles, Rayquaza pre-Champion capture, Groudon/Kyogre after Rayquaza, Mewtwo only after Mew + Champion, Celebi and Zarude in Ilex Forest, Jirachi very early at Route 102 pond, and Ogerpon→Loyal Three→Pecharunt quest.
- Implementation must be grouped by region/location with genuine source and representative runtime checks. Do not over-focus on synthetic doorway fixtures. No ROMs, save files or private predecessor history in public repo.

## Scope of this handoff

The maintainer authorized publishing the design ledger, preimplementation audit and handoff to GitHub. **That is documentation permission, NOT permission to begin implementing, editing game code or maps, running builds/CI/tests, opening issues/PRs or making unrelated commits.**

**Your first task:** Read and understand the whole repository and ledger; give a concise, actionable region-by-region summary of code-vs-design discrepancies and proposed implementation batches. Explain any uncertainty. **Then stop and wait for explicit implementation approval.** Ask only if a real design conflict remains after reading.
