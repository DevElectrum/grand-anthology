# Johto Tao Trio: Implementation checkpoint, 24 October 2026

**Canonical source:** `DevElectrum/grand-anthology` public `main`. This documents implementation, **not** real-gameplay PASS.

## Changes

- Reshiram moved from Route45 to **TinTower_2F (9,8), Lv44**, behind native Clear Bell/tower-admission story and not Ho-Oh's rooftop beast-capture prerequisite.
- Zekrom moved from Route45 to **DarkCave_NorthSide (40,13), Lv44**, in the Blackthorn/Route45-side chamber. Independently obtainable.
- Kyurem moved from Route44 to **IcePath_B1F (8,10), Lv49**, near the passage reached through IcePath 1F stair (9,20). Only shown after **both Reshiram and Zekrom Pokédex caught bits** are true. No Johto Champion gate.
- Capture Kyurem to get the **DNA Splicers Key Item**. If the bag is full, return to the Ice Path plinth at (8,9) to claim it. KO and escape do not award it.
- New source scripts use species-specific hide flags, current caught dex for persistent disappearance, and revisit restoration after faint. Native puzzles, cutscenes, map connections, Ho-Oh's rooftop script, and Clear Bell reward unchanged.
- Old Route45/Route44 duplicate statics and scripts removed. `ValidateAnthologyHabitatRare.py` inventory revised, new `ValidateAnthologyTaoTrio.py` checks geometry/gates, and mGBA smoke workflow now listens for changes in the new maps.

## Verification boundaries

Map binary collision checks used 3-elevation walkable tiles and flood fill from actual existing entrance warps. This does not establish navigation through every puzzle, cutscene, badge restriction, or untested battle. Optimized GBA compilation, emulator capture/faint/revisit and the DNA Splicers' **fusion, defusion, storage and save/load** must be verified separately. Existing fusion recipes and item-use callback were read and left unchanged. The two prior Diancie/daycare synthetic doorway fixture failures are unrelated to the real-door Nyx manual passes.

No ROMs, saves, debug warps or cheat scripts in public GitHub.
