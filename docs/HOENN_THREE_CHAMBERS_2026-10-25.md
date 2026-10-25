# Hoenn permanent side chambers (25 October 2026)

Canonical repository: public `DevElectrum/grand-anthology` `main`. This is a source implementation note, **not** proof of real emulator acceptance.

Three approved side chambers are implemented as independent maps reusing existing game layouts, without modifying the original binary tile layouts or their puzzles:

| Chamber | Entry point / unlock | Encounter and return |
|---|---|---|
| Hoopa Ring Chamber | Permanent Route 111 desert relic (30,56), after badge 4 | Independent Mirage Tower 4F-themed layout; Hoopa Lv30, catches hide permanently, defeat/revisit retries; room warp returns beside relic. Exists regardless of Mirage Tower collapse or fossil choice. |
| Koraidon Ancient Grotto | Underwater Route 128 relic (42,27), after badge 7 and Space Center Magma victory | Cave chamber with Koraidon Lv45; independently exits to its underwater approach, not Seafloor Cavern. |
| Miraidon Generator Annex | New Mauville Inside service terminal (34,24), after badge 7, Space Center Magma victory, and Wattson's generator shutdown | Separate New Mauville-themed generator annex with Miraidon Lv45; exits to original power plant without altering button puzzle. |

Entry objects remain after the Pokémon is caught; existing caught-dex hide, faint/escape retry and progression scripts moved into the dedicated rooms. New maps are **appended to the existing dungeon group** rather than inserted among old maps, preserving pre-existing map indices. The extra parent warp entries are landing anchors for return paths; player enters by interacting with the themed entry objects. The art is reused from existing Mirage Tower, Granite Cave, and New Mauville rooms, not newly drawn. Test updates verify source-level object and exit contracts. **Check exact P0 source+optimized ROM CI before claiming compile success; then require emulator/physical gameplay for real capture and return acceptance.**

Remaining Hoenn-late milestones are not claimed by this batch: special Zacian+Zamazenta paired double battle, separate permanent Complete Zygarde and Stellar Terapagos mechanics, and comprehensive physical three-region story acceptance. Hoenn is the final region in the current Kanto→Johto→Hoenn scope.
