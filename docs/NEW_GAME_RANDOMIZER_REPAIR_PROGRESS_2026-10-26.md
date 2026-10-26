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
2. **Title screen fixed and verified**: approved 240×160 Grand Anthology artwork integrated in `586b403e8a63`; real mGBA 25/25 title test PASS after raw 8bpp VRAM fix in `d688587ba583`. Title music, main menu, A/Start input, title loop, save-clear/RTC behavior and absence of duplicate title prompt were tested. See the verified-run section below. The artwork's embedded PRESS START remains static.
3. Validate source-level ROM build, loaded in mGBA; normal mode Off, seeded Full Chaos on, DexNav search, Master Ball/Ultra Beast, rival continuation and representative static persistence across regions, especially scripted forms.
4. Optional spoiler/Atlas website is a separate phase. Do not expand this repair scope to that website.

## Testing approach

Prefer automated source/CI checks first; after successful builds, one brief human gameplay acceptance session is enough. Do not ask the maintainer to replay an entire campaign or do repeated full rebuilds just to debug menu automation.

## Additional static persistence batch (26 October)

Source-only repair for **nine encounter sites** across eight map files: Reshiram (Tin Tower 2F), Zekrom (Dark Cave North), Kyurem (Ice Path B1F), Glastrier (Ice Path B2F), Spectrier (Cliff Edge Cave), Cobalion (Mt. Mortar south), Tapu Bulu (Route 5), Tapu Koko and Zeraora (Power Plant). Each site gains an independent saved caught flag in the verified-free world-map bank. Only a **caught** battle sets the site flag; winning without catching retains the existing re-entry retry, and running/teleporting does not consume the site. The original species-Dex branch remains unchanged when Statics randomization is Off.

In seeded mode, Ice Path Kyurem's prerequisite now checks **both source-site capture flags** from Reshiram and Zekrom; the DNA Splicers plinth checks Kyurem's source-site flag rather than original species ownership. The Power Plant's Zeraorite check similarly uses the Zeraora encounter-site flag. Mt. Moon's four previously fixed slots are unchanged.

This batch is **not repository-wide closeout**. Other `getcaughtmon` scripted statics and dependencies remain to be audited and repaired. Source changes require optimized build, mGBA checks, and manual site acceptance; no gameplay PASS is claimed from the edits alone.

## Additional Kanto/Johto/Hoenn location sweep (26 October)

Twelve further seed-randomized caught-site flags were integrated for Yveltal, Necrozma, Manaphy, Tapu Fini, Jirachi, Chien-Pao, Walking Wake, Iron Crown, Landorus, Thundurus, Eternatus and Tornadus. On every affected map, the original species-Dex test is retained when static randomization is Off. Catching a replacement sets only the local completion flag; KO and escape do not. Duplicate native Tornadus sources in other maps are not automatically retired by this Johto site's capture when seeded randomness is on. This is source repair pending game-level acceptance.

## Johto Sword/Forces dependencies and Viridian Forest (26 October)

Five more independent seed-mode completion flags cover Union Cave Terrakion, National Park Virizion and Victini, Viridian Forest Shaymin, and Route 39 Enamorus. The original source checks remain when randomization is Off. In seed mode, Enamorus requires the **Route 44 Tornadus, Route 41 Thundurus, and Route 43 Landorus source sites** to be caught, not just the Pokédex registration of their original species. Victini still requires FLAG_SYS_GAME_CLEAR first. No site marks complete on win/run.

Cumulative newly repaired source-level sites in these grouped passes: **26** (Mt. Moon 4 + batch 1 9 + batch 2 12 + batch 3 5 = 30, including Mt. Moon; grouped changes alone 26). Remaining special and native encounters still require a location-ledger audit and physical verification.


## 26 October static source-site persistence closeout batch (14 more sites)

In randomized games, Hoenn's native Regirock/Regice/Registeel rooms, Rock Tunnel B1F's Regigigas/Uxie/Mesprit/Azelf/Heatran, the Route 117→112/113/114 Ogerpon/Loyal Three/Pecharunt sequence, and Lavender Pokémon Tower 7F Darkrai now track independent source-site catches (14 new flags, indices 0x14B–0x158). Source rewards for Heatranite and Darkranite use the matching site flags in randomized mode. Original species-based checks remain active with randomization Off. Knockouts and escapes must not set caught flags. Source checks/CI and emulator gameplay are separate evidence; validate against the exact commit.


## Grand Anthology title-screen artwork integration (26 October 2026)

Integrated the **approved** 240 × 160, 239-color indexed title image with Oak, Elm, Birch, seated trainer, Arcanine, Eevee, Pikachu, Mudkip and Rowlet. Its precise PNG Git blob is `fa073c5748d3602411fa2e8c16aa97eaa9439727` (SHA-256 `b23ffce8b56cda032dc88e46e8d7fabec5207b6021fea00a077af7a01f9a9967`). `tools/build_anthology_title.py` converts it into native BG0 tiles, palette and map before `src/title_screen_world.c` is compiled, using a Makefile stamp. Original World scenery/fog render paths and duplicate prompt sprites are no longer loaded; credits remain in their existing flow. A, Start, save clear, RTC, SELECT quickstart and title music paths remain in source. The image's PRESS START is static. **Build and actual mGBA screen acceptance must be verified from the exact integration commit; source review is not runtime proof.**


## Final title-screen and CI verification (26 October 2026)

- Approved PNG SHA-256: `b23ffce8b56cda032dc88e46e8d7fabec5207b6021fea00a077af7a01f9a9967`.
- `586b403e8a63` integrated the approved indexed artwork and build converter, `206815310acc` added actual mGBA title/VRAM/input tests, and `d688587ba583` changed tile loading to aligned DMA of exact 601 raw 8bpp tiles after the first emulated check found a mismatch at byte 697 with BIOS VRAM LZ10 loading.
- **PASS**: optimized full GBA ROM build and source validators on `d688587ba583`: https://github.com/DevElectrum/grand-anthology/actions/runs/40285984737
- **PASS**: real mGBA release smoke on `d688587ba583`: https://github.com/DevElectrum/grand-anthology/actions/runs/40285984781 . `WorldTitle: 25/25`, cheats `8/8`, full-party gift `15/15`, Johto/Kanto starter scenes `34/34` each, Birch full-PC refusal `26/26`, shiny odds `26/26`, first boot `4/4`, region travel/save continuity `27/27`.
- **Not yet finished**: source-level per-location catch flags currently cover **44 targeted encounter sites** (including 4 Mt. Moon, plus 9+12+5+14 grouped repairs). This does not exhaust all scripts using `getcaughtmon SPECIES_ORIGINAL`, including Mew/Mewtwo and duplicate Celebi/Calyrex special cases. Capture/KO/run/revisit/save on representative randomized statics must still be accepted in-game. Do not mistake passing general mGBA tests for full static-persistence acceptance.
