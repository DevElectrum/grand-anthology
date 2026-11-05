# Sinnoh import: pinned machine inventory (I0 in progress)

This directory now contains a reproducible **metadata-only** snapshot from the two Git trees pinned in `docs/SINNOH_FULL_IMPORT_EXECUTION_PLAN.md`. It is not a playable import. No upstream art, music or dialogue is republished here.

- Grand Anthology baseline: `c78d81264c211882dcd0ecbe34102ba3018e8501`
- Platinum GBA upstream: `c79120fbab7d62c66785c90b491e6a370249d5db`
- Exact map groups: 13, maps: 93, new layouts: 64
- Selected/map candidate file records: 2763; breakdown: {"layout":128,"map":186,"script-dependency":5,"asset":2444}
- Baseline map groups: 101, maps: 1194

## What is still required before bulk linking

The names/paths, Git blob hashes and sizes are recorded, but sources are not yet normalized into noncolliding ID spaces or dependency-closed. The next implementation steps are to resolve the map/flag/trainer/music/tileset namespace and map building, integrate independent region/save persistence, then bring source assets into the working build only with an established permission basis. The upstream pinned tree has a credits file but no top-level LICENSE. Public availability alone does not authorize publishing third-party art/music/dialogue.

`python3 tools/sinnoh/check_inventory.py --upstream /path/to/pokeemerald-platinum --target /path/to/grand-anthology` checks the pinned local source blob identities and guards the legacy map layout before copying any material.

No commit or work in this directory authorizes importing proprietary NDS ROM assets, modifying Mosaic/XGBA, changing saves, publishing ROMs or merging the branch.
