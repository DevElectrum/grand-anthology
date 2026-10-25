# Nyx Manual Gameplay Acceptance: Diancie and Cave Encounters

## QA acceptance policy update (October 25, 2026)

The two legacy synthetic doorway probes `WorldKantoDaycare.lua` and `WorldDiancieStatic.lua` are **retired from required mGBA CI**. They were repeatedly failing to traverse normal doors from artificially relocated flash saves. They did not demonstrate a gameplay defect. Nyx manually confirmed the actual Route 5 daycare door, three NPC interactions, the Manaphy+Ditto breeding/Phione hatch flow, and Diancie's normal doorway, level-18 encounter, faint/revisit respawn and caught/save/restart persistence.

The Lua scripts remain in the repository as optional historical diagnostics; their verdicts are **not** included in automatic release pass/fail. Manual verification is recorded as manual, not repackaged into a fabricated emulator PASS. Normal optimized ROM compilation, source contracts and the nine other independent mGBA suites remain active. Re-enable a doorway smoke only once its fixture accurately follows real gameplay, or if a new reproducible gameplay defect actually arises.


**Observed:** 24 October 2026, Pop!_OS, graphical mGBA 0.10.2 (Nyx).
**Source under manual test:** public `main` at approximately `bd135e1`, plus an **uncommitted, local-only** test branch editing the two Littleroot Town sign scripts. This is not a normal story run and is not a separately published ROM.
**Status:** Diancie scripted encounter manually verified with prerequisite flags supplied through the local test shortcut. The later cave-wild-spawn restriction and the Kanto Route 5 daycare entrance/NPCs have now also been manually checked; detailed limits are recorded below.

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

## Further Nyx manual verification (24 October 2026 local session)

**Cave ecology fix on updated public `main`, commit `919cbfd`, gameplay mGBA 0.10.2:**

- Ordinary wild encounters **still occur while walking** on eligible cave encounter tiles.
- Generated roaming wild Pokémon sprites no longer appear/crowd cave rooms.
- Standing still did not trigger a wild battle during the user's observation period. This is observed no-battle behavior, not a mathematical guarantee for indefinite time.
- An authored Legendary **Ting-Lu overworld sprite remains visible** at Granite Cave B2F (player screenshot), independently confirming that the generated-OWE suppression did not indiscriminately remove authored statics. **Ting-Lu's battle itself was not tested.**

**Kanto Route 5 daycare, local-only Littleroot sign shortcut (same graphical mGBA):**

- Town sign warped to Route 5 exterior just south of the daycare entrance at `(23,26)`.
- The user **entered the daycare building using its actual door**, rather than the direct-to-interior fallback shortcut.
- The **original caretaker plus both additional NPCs** were visible, and **all three responded to interaction**.
- Two NPCs offered Pokémon deposit dialogue, matching the original one-Pokémon caretaker and the newly added two-parent deposit service.
- This confirms real in-game doorway access, three NPC presence, and interaction/dialogue availability. **Later Manaphy + Ditto testing additionally verified deposit, compatible parent recognition, developer-triggered Egg generation, daycare pickup, and successful Phione hatching; see below.**
- The mGBA synthetic Route 5 Save/Continue navigation fixture still fails **3/4 at its door step**, despite this normal gameplay PASS. Keep it flagged as a test-fixture problem, not a daycare gameplay regression.

**Manaphy + Ditto → Phione, further short Nyx manual test (same local daycare build):**

- User added Manaphy and Ditto to the party with **CHEATS → Quick Pokémon**, then deposited both using the **new breeding lady** at Kanto Route 5.
- Daycare man reported the parents **"don't seem to like each other much"**: low yet nonzero compatibility. Both test parents have the same Original Trainer ID.
- To avoid an unnecessary 256-step breeding roll, the user exposed the *existing developer-only* `DebugAction_Give_DayCareEgg` through the local CHEATS → Pokémon Tools submenu, invoked it once with the two compatible parents already deposited, and collected an Egg from the daycare man.
- The user likewise exposed and invoked the existing `Debug_HatchAnEgg` action and confirmed the on-screen message **"Phione hatched from the EGG!"** (Nyx screenshot).
- **Verified:** shared two-parent deposit flow and compatible pairing, pending-Egg generation from the deposited parents using the game daycare routine, Egg handoff, egg/species construction, and Phione hatch in real graphical mGBA gameplay.
- **Not verified:** unassisted random 20% / 256-step Egg production, ordinary incubation/hatch timing, long-term daycare state after cross-region travel/save-reload, or depositing species from normal acquisition rather than cheats. The egg and hatch were accelerated via *local-only* developer tools, which do not belong on public release `main`.

**Reproduction hygiene:** These checks used local-only sign shortcuts and disposable/backup saves; they are not normal story progression proof. Do not put the altered Littleroot sign scripts on public `main`. Keep the user's starter save and test saves separately.

## Existing automated test remains blocked

The [mGBA release run #40142464744](https://github.com/DevElectrum/grand-anthology/actions/runs/40142464744) on `bd135e1` recorded a **7/8 Diancie fixture failure**. Its synthetic save setup had a valid map header with Steven's room warp, player elevation 3, and north facing, but the object-event `currentMetatileBehavior` read was 0. It did not enter the room or test Diancie itself. Nyx's real doorway traversal and scripted battle directly contradict any interpretation that the normal game doorway/Diancie is broken.

Do not convert this red fixture to a false PASS or alter Steven's original story to appease it. Replace the fixture with an in-engine progression/navigation method when available. Continue to report the automatic test separately from the manually observed PASS.

## Local playtest cleanup

The local Littleroot signs are **not** intended for `main`. Before restoring Nyx's normal build, preserve the current `.sav` separately, switch to clean public `main`, rebuild, and use the original pre-test clean-save backup if the user wants untouched story progress. Do not overwrite either save without a backup.
