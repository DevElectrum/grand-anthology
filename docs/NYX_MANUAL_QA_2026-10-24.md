# Nyx Manual Gameplay Acceptance: Diancie and Cave Encounters

**Observed:** 24 October 2026, Pop!_OS, graphical mGBA 0.10.2 (Nyx).
**Source under manual test:** public `main` at approximately `bd135e1`, plus an **uncommitted, local-only** test branch editing the two Littleroot Town sign scripts. This is not a normal story run and is not a separately published ROM.
**Status:** Diancie scripted encounter manually verified with prerequisite flags supplied through the local test shortcut. Automatic cave-wild-spawn restriction is a subsequent change and has not been manually retested here.

## Manually passed

- Player with a normal Hoenn starter and saved progress interacted with the **Littleroot Town sign**, which set `FLAG_DELIVERED_STEVEN_LETTER`, `FLAG_BADGE01_GET`, `FLAG_BADGE02_GET`, and `FLAG_HIDE_GRANITE_CAVE_STEVEN`, then warped to Granite Cave 1F at `(5,9)`.
- Player **entered Steven's room through its real north-arrow doorway**. This is significantly stronger navigation evidence than the existing synthetic Save/Continue mGBA fixture.
- Diancie stood at its original scripted position; interacting began a wild **Diancie level 18** battle.
- Fainting Diancie temporarily removed its overworld sprite. Leaving and reentering Steven's room restored it.
- Catching Diancie removed the static. Leaving and reentering kept it absent.
- Saving normally, closing/restarting mGBA, loading the save, and returning to Steven's room kept Diancie absent.

These observations validate door traversal, conditional spawn with flags supplied, correct species/level, faint/revisit, caught/revisit, and caught/save/restart persistence. The user did **not** report checking Diancie's party/PC slot after reload.

## Not yet tested

- Getting Roxanne's and Brawly's badges and actually delivering the Letter to Steven through normal story progression, without the shortcut.
- Whether a noneligible trainer can see or battle Diancie before meeting all three prerequisites.
- Full-party/PC capture fallback, flee/teleport, and longer cross-region persistence.
- Catch/retry from a fresh gameplay save with no local test flags.
- Broader overworld-wild ecological behavior across all cave and non-cave maps.

## Additional player-reported cave defects

During this manual test, the user observed **moving ordinary wild Pokémon sprites** in the cave, sometimes approaching the stationary trainer and initiating battles, and reported that remaining in Steven's room for an extended period produced an unpleasantly crowded scene.

Code inspection confirmed `WE_VANILLA_RANDOM = TRUE` and `WE_OW_ENCOUNTERS = TRUE`, with a nominal maximum of **four generated OWE slots** (`OWE_SPAWNS_MAX = 4`). The report does not prove more than four generated OWEs were simultaneously active: other object sprites, transient effects, and encounter replacement would require a separate count to diagnose.

**Follow-up implementation:** the cave-only spawn guard in `src/wild_encounter_ow.c` blocks generated OWEs on `MAP_TYPE_UNDERGROUND` maps and clears any active generated OWEs during update. Standard step-based encounters remain enabled globally. Scripted static encounters are not targeted. This is an intentionally scoped fix and must be verified in play.

## Existing automated test remains blocked

The [mGBA release run #40142464744](https://github.com/DevElectrum/grand-anthology/actions/runs/40142464744) on `bd135e1` recorded a **7/8 Diancie fixture failure**. Its synthetic save setup had a valid map header with Steven's room warp, player elevation 3, and north facing, but the object-event `currentMetatileBehavior` read was 0. It did not enter the room or test Diancie itself. Nyx's real doorway traversal and scripted battle directly contradict any interpretation that the normal game doorway/Diancie is broken.

Do not convert this red fixture to a false PASS or alter Steven's original story to appease it. Replace the fixture with an in-engine progression/navigation method when available. Continue to report the automatic test separately from the manually observed PASS.

## Local playtest cleanup

The local Littleroot signs are **not** intended for `main`. Before restoring Nyx's normal build, preserve the current `.sav` separately, switch to clean public `main`, rebuild, and use the original pre-test clean-save backup if the user wants untouched story progress. Do not overwrite either save without a backup.
