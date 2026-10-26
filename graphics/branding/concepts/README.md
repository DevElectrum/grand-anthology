# Grand Anthology branding concepts (26 October 2026)

These two source PNGs were approved for **retention as design references**, not yet selected as the production title screen.

| Asset | Resolution | Purpose |
|---|---|---|
| [Grand Anthology logo](grand-anthology-logo-concept-2026-10-26.png) | 2172 × 724, RGBA PNG | Standalone project-wide Pokémon Grand Anthology wordmark concept; candidate for the title screen and future README banner or separate README logo. |
| [Twilight regional panorama title screen](grand-anthology-twilight-title-concept-2026-10-26.png) | 1536 × 1024, RGB PNG | Clean, character-free GBA-inspired title-screen composition with regional silhouettes, sunset, wordmark and PRESS START. |

Both files are the **original full-resolution generated source images**, committed byte-for-byte. They have not been downsampled, color-indexed, or integrated into the GBA ROM.

## Follow-up before integration

- Preserve one shared project-wide logo identity. Check the standalone master at actual GBA display size; simplify fine details by hand where necessary.
- Build a true **240 × 160, indexed <= 256-color** GBA version of the title concept and inspect it at native size. Merely shrinking the 1536 × 1024 mockup may cause illegible lettering or mushy silhouettes.
- Existing title rendering uses `src/title_screen_world.c` and `tools/build_anthology_title.py` from `graphics/title_screen/world/grand_anthology_title.png`. **Do not replace that production image solely because these concepts have been archived.**
- Future README changes (replace banner or add a separate logo) are intentionally deferred.

These are visual design drafts, not a claim of pixel-perfect finalized GBA artwork or cleared rights to Pokémon marks.
