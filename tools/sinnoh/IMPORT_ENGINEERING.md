# Sinnoh full-import engineering worktree

This branch contains the *actual import machinery*, not a claim that Sinnoh is
playable. The pinned 93-map/64-layout source inventory lives in
\`pinned_source_inventory.json\`; asset candidates are tracked by exact source
Git blob SHA.

## Invocation

From a **local checkout** of Grand Anthology, at the root:

\`\`\`bash
python3 tools/sinnoh/test_import_source.py
python3 tools/sinnoh/import_source.py --upstream /path/to/pokeemerald-platinum --mode audit
python3 tools/sinnoh/import_source.py --upstream /path/to/pokeemerald-platinum --mode stage --assets
\`\`\`

The script refuses an upstream HEAD other than
\`c79120fbab7d62c66785c90b491e6a370249d5db\`. It validates the exact bytes
before touching a target. Stage copies **all 93 map JSON files, all 93 map
scripts, all 128 paired layout binaries and, with \`--assets\`, the 2,444
source-only asset candidates** into an ignored build directory, with proposed
append-only map/layout registries. Staging leaves the live game unchanged.

\`\`\`bash
python3 tools/sinnoh/import_source.py --upstream /path/to/pokeemerald-platinum --mode install --assets
\`\`\`

Install is an **opt-in working-tree operation for the experimental branch**:
it copies only missing files and refuses any different-content shared-path
overwrite. **It does not activate maps.** Scripts still need flag/var/trainer
translation, missing helpers/registrations, soundbanks, palette and map
section handling, plus independent Sinnoh save state. Installing this source
set does not imply a successful build.

## Hard boundaries

- Never rebase the existing 101 map-group IDs or 1,194 map IDs.
- Do not import upstream battle, save, PC or species engine files.
- Do not overwrite any existing assets or native saves.
- No executable region activation until independent Sinnoh progression and
  old-save migration pass their acceptance gates.
- **Rights review is unresolved** for the upstream images/audio/text.
  Do not publish unreviewed copied assets or resulting ROM binaries.
- The original Platinum \`.nds\` is a *later-stage reference*, not a source
  to import into this first pass.
