# Eternatus: Johto Victory Road side chamber

**Approved 27 October 2026.** The user authorized relocating Eternatus from Johto Route 44 to a dedicated side room off Victory Road 1F, near the Indigo Plateau exit.

## Implementation

- **Entry:** Interact with a meteorite at Johto Victory Road 1F (35,4), on the accessible northern path close to exit warp (39,5). Before the Royal Duo victory, the seal refuses entry.
- **Prerequisite:** `FLAG_ANTHOLOGY_ROYAL_DUO_CLAIMED`, set by completing the Route 27 Zacian+Zamazenta double battle after eight Johto badges. The encounter is available before the Johto Champion.
- **Chamber:** `data/maps/JohtoEternatusChamber/` appended to the JohtoLeague map group, using the existing small Granite Cave Steven room layout. The entrance script warps the player to (7,6), and the room's original-style exit at (7,3) returns to the Victory Road 1F Indigo Plateau exit warp (ID 1). Existing road geometry, other warps and trainers are untouched.
- **Eternatus:** Visible at room (10,9), level 55. Flee or teleport leaves it available; defeating temporarily hides the encounter until the next room entry; catching permanently hides it. Both the entry and the battle independently check the Royal Duo flag.
- **Seeded static randomizer:** Reuse the historical `FLAG_CAUGHT_RANDOM_ROUTE44_ETERNATUS` at its original saved numeric position so earlier seeded saves are not renumbered. The flag now tracks the chamber's source. The randomizer replacement may differ after moving because the map hash changes. In normal mode the caught-Pokédex state is checked.
- **Route 44:** Remove only Eternatus's overworld object and associated script. Preserve Tornadus and ordinary encounter content.

## Verification

`test/overworld/ValidateJohtoEternatusChamber.py` verifies object uniqueness, flags, gating, chamber walkability, source registration and exit, preserving unrelated Route 44 content and pre-existing map indices. The Gameplay-P0 workflow includes it; the existing acquisition and static-randomizer validators have their location targets updated.

**Source code is not a gameplay pass.** A short disposable-save mGBA acceptance still needs to check the seal before and after Royal Duo, chamber entry and exit, level/species, catch-only saved completion, KO and flee retries, and ordinary and seeded encounters. Also check the original Indigo Plateau entrance and save/reload persistence. Do not treat a green static validator or ROM compile as proving all of those.
