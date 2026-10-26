# New Game & Randomizer v2: developer and Nyx acceptance handoff
**25 October 2026 (America/Chicago)**

## Scope and authority
Canonical repository: `DevElectrum/grand-anthology`, public `main`.
Only this project is in scope. Old private repository remains untouched.

This pass implements the pre-intro Adventure Settings page, a save-locked seed and category configuration, the original Oak/character/outfit introduction afterwards, then the World Transit Hub. The existing configuration displays read-only in Options after confirming, and non-randomizer gameplay options can still be adjusted. The previously editable player Cheats randomizer menu entry was removed.

The deterministic categories are Wild, Trainers, Starters, Gifts and Statics. There are three static policies: Ordinary, Legendary Shuffle, and Full Chaos. All 28 starter choices have seeded previews. DexNav and regional Pokédex area discovery read the seeded wild species.

Static graphics are resolved from the same seed/identity as the battle for supported object events. Legacy and modern Pokémon graphics IDs are handled; the two Route 27 guardians use different slots. Their double battle awards the very opponents faced, and their Rusted Sword/Shield follows the actual Zacian/Zamazenta species. Scripted single/double static constructors strip these relics from replacement species. Full Chaos supports Snorlax, Sudowoodo and Kecleon replacements while keeping their location-based gates. Modern statics exclude temporary battle-only forms; legacy v12 mappings are left unchanged.

The Mansion's three-disguise Mew gauntlet runs only when Mew remains in that location; otherwise there is a single static battle. Completing this location does not substitute for actually catching Mew elsewhere to satisfy Mewtwo's condition.

## Tested without a human
- Source suite and optimized ROM were last **fully green** on commit `1553b921`, before the final focused changes to static-form eligibility, item ownership and normal-option editability.
- A real mGBA regression successfully completed original title, fresh game-to-Hub, starter, save/reload and other baseline suites. The new `WorldNewGameRandomizer.lua` menu-navigation test failed at a title callback it was observing, before exercising randomizer controls. It is retained for optional debugging but no longer gates the reliable mGBA baseline suite.
- Latest source-level static checks now require sprite resolver parity, correct relic ownership, and transient-form exclusions. **Do not represent these as a passed ROM or gameplay test until run.**
- The source-indexed 1,025-species acquisition audit proves optimistic unrandomized candidate paths, **not** full coverage across arbitrary randomized seeds.
- No manual testing on Nyx has yet happened for this v2 flow.

## Short Nyx acceptance sequence
Use a separate test save, not the regular playthrough.

1. On Nyx, update the canonical public checkout: `cd ~/Projects/grand-anthology && git pull --ff-only`; build with the project's GBA toolchain, typically `make release -j4`, then launch `pokemonworld-release.gba` in mGBA.
2. Start **New Game**. Verify Adventure Settings appears *before* the name/gender/outfit introduction. Change the seed from 2026 to 2027 and select **Full**; confirm with START or the on-screen Confirm & Lock entry.
3. Complete character customization. Confirm the World Transit Hub appears, normal controls work, and Options displays seed 02027, all five categories on, Static Mode Chaos.
4. Try left/right on the seed and category fields from Options. Neither must change. Edit a normal EXP or Hard Mode option to verify only the randomizer is locked.
5. Save, restart mGBA, Continue and confirm the seed, categories and locked status remain identical.
6. With a separate disposable save, start New Game, modify a seed and then **press B** before confirmation. Returning to the main menu must not overwrite the original save. Continue should retain the original settings.
7. If practical, visit one easy ordinary scripted static encounter. Its visible overworld Pokémon must match the battle. Its catch/faint/revisit behavior must work. A Full Chaos run should change the species deterministically for the selected seed.
8. Mew Mansion and Royal Duo require later targeted progression/warp fixtures. Do not claim gameplay acceptance from source checks alone.

Report only the smallest failure reproduction: menu selection, seed, button sequence, screen or screenshot, and relevant save outcome. Testing must avoid the main playthrough's only save.

## Important caveats and deferred work
- The seed/location Atlas and spoiler-log website remain **out of scope** and belong to another Astra.
- All statics must *eventually* display their randomized species. The source hooks cover supported object events, but custom special events and every legacy graphics layout have not received exhaustive emulator acceptance yet.
- A global 1,025-species obtainable guarantee for every Full Chaos seed is **not established**. Deterministic Legendary Shuffle is a one-to-one mapping over eligible legendary species; this alone does not prove that every original acquisition chain is satisfiable after swapping.
- The owner asked to avoid repeated entire-ROM CI builds just to debug the automation. Prioritize a single short manual New Game acceptance pass, with specific source repairs only for observed failures.
