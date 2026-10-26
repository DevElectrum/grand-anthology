# New Game randomizer repair progress (26 October 2026)

Canonical repository: `DevElectrum/grand-anthology` public `main`. Do not touch the archived private repository, or overwrite Nyx's unrelated local worktrees/saves.

## Manual test evidence before repair

- New Game settings, locked seed 02026, save/continue, starter award, Route 1 wild, ordinary trainer and Brock randomized teams were manually verified by the maintainer on Nyx. Original trainer levels were retained.
- Viridian Forest DexNav still showed original Weedle instead of caught/randomized Ceruledge, Electrike and Pyroar.
- Master Ball broke open against an Ultra Beast.
- Gary's opening Oak-lab team still had scripted Charmander rather than the randomized counterpart.
- Mt. Moon lower floor: randomized green overworld sprite led to **Palkia level 45**. This is Palkia's original encounter level, not 95. The same constructor discrepancy affects `seteventmon` static scripts elsewhere.
- Static sprite-to-battle parity and persistence were **not** universally demonstrated. The earlier generalization that all statics worked was overturned by Mt. Moon.

## Changes already on main

- `480d6a0de5`: Master Ball guarantee before Ultra Beast modifier; seeded DexNav displayed encounter list; shared `CreateEnemyEventMon` static resolver.
- `fa2c7359a1`: Kanto Blue seeded counterpart starter across opening and later trainer appearances.
- `87fa204888`, `59f8e2e3b1`, `d285de4153`: direct five-digit seed editor and random-seed shortcut, arithmetic and link fixes.
- `f9033b1d3d`: randomized static encounter cries use the common resolver, with Route 27 Zamazenta using its second slot.
- `756672ab9a`: exclude Ilex Forest's nonbattle Farfetch'd puzzle actors from overworld static sprite shuffle.
- `c82a7347df`: Mt. Moon Cresselia/Dialga/Palkia/Giratina get independent source-location capture flags in randomized games; B2F Giratina gates on the first two source encounters instead of ownership of original species. Normal-mode species gates retained.

A successful optimized gameplay build and mGBA release smoke were observed on `d285de4153`; verify CI anew for later commits, not assume they pass.

## Known unresolved work, not to be forgotten

1. **System-wide static persistence**: across the 81 script files with `seteventmon` calls (including shared scripted encounters), many transition checks still depend on `getcaughtmon SPECIES_ORIGINAL`. A replacement catch may not permanently clear its source location, and catching the original elsewhere may hide it prematurely. Mt. Moon has only a *targeted* correction. Implement a robust source-location ledger or explicit per-location caught flags with compatibility-safe migration and no hash collisions. Audit multiple same-species sites (e.g. two Calyrex or Celebi slots), faint/run/reload semantics, Mew/Mewtwo and the Royal Duo.
2. **Title screen still Pokémon World**: approved 240×160 indexed Grand Anthology artwork and an installer package exist in conversation attachments as `grand_anthology_title_integration_package.zip`. Package contains `graphics/title_screen/world/grand_anthology_title.png`, `src/title_screen_world.c`, `tools/build_anthology_title.py`, installer and README. It has **NOT** been integrated into public main. Use approved PNG, preserve title music, Press Start, save clear/RTC shortcuts and existing creator credits elsewhere. Do not substitute the earlier preview banner or an invented title.
3. Validate source-level ROM build, loaded in mGBA; normal mode Off, seeded Full Chaos on, DexNav search, Master Ball/Ultra Beast, rival continuation and representative static persistence across regions, especially scripted forms.
4. Optional spoiler/Atlas website is a separate phase. Do not expand this repair scope to that website.

## Testing approach

Prefer automated source/CI checks first; after successful builds, one brief human gameplay acceptance session is enough. Do not ask the maintainer to replay an entire campaign or do repeated full rebuilds just to debug menu automation.
