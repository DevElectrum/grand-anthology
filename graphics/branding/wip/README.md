# Grand Anthology native GBA title candidate, first visual pass

This is a working **240 x 160 indexed PNG** title-screen candidate, *not* the currently shipped title screen.

- `grand_anthology_title_gba_candidate_240x160.png`: native-size, no scaling required, 222 used palette colors (indices 1..224; index 0 reserved).
- `tools/build_gba_title_candidate.py`: reproducible composition generator using Pillow (`python3 -m pip install Pillow`) and the archived transparent logo master at `graphics/branding/concepts/2026-10-26/grand-anthology-logo-master.png`.
- Generator also exports separate 4x nearest-neighbor visual previews under this WIP directory.

Composition: clear shared Pokémon GRAND ANTHOLOGY logo, dark twilight sky, sparse stars, stylized Kanto village (left), Johto tower (middle), Hoenn volcanic/mountain ridge (right), and a centered PRESS START inset.

**Native title-asset compatibility spot check:** The candidate is indexed P-mode, exactly 240 x 160, palette indices avoid zero and stay below 240, and currently deduplicates to 319 native 8x8 tiles = 20,416 raw tile bytes. The current live title code in `src/title_screen_world.c` asserts an older 601-tile size; when promoting this candidate, that assertion must be updated to the generated asset rather than kept hard-coded.

Do not replace `graphics/title_screen/world/grand_anthology_title.png` or its currently working ROM output until the maintainer visually approves this candidate. The source concept and master logo remain separately archived. This pass is a mockup only, not yet an emulator test.
