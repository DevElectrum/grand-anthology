# Pokémon Mansion: Mew's Nine Masks

**Scope:** Mew's existing Pokémon Mansion 1F overworld object. No other legendary encounters are changed.

## Encounter flow

- Interact with Mew to roll exactly one random form from each tier.
- Round 1 (Lv. 30): Tapu Bulu, Regirock, or Raikou.
- Round 2 (Lv. 31–32): Lunala, Volcanion, Genesect, or Single Strike Urshifu.
- Round 3 (Lv. 34): Rayquaza or Groudon.
- Round 4 (Lv. 35): genuine Mew, the **only** catchable opponent.
- Each roll produces one of 3 × 4 × 2 = **24** possible ordered encounter sequences.
- Every round is an independent scripted legendary battle with its own scripted four-move set, ordinary species abilities, a neutral Hardy nature, no held item, and no EV training.
- All nine disguise species are preserved literally, even if the global static-species randomizer is enabled. They are not awarded or recorded as catches.
- The player keeps HP, PP, fainted members and status across every battle. No automatic heals or recovery pauses are added. Battle items remain usable.
- In disguise rounds, a dedicated battle-type flag prevents *any* Poke Ball use through the normal bag legality check. Round 4 has no restriction and uses ordinary capture rules.
- The scripted encounter advances **only** when each disguise is defeated (B_OUTCOME_WON). Fleeing, losing, teleporting, or any other result aborts. The last Mew advances **only** when caught (B_OUTCOME_CAUGHT).
- Mew stays visible for another fresh attempt after defeat, failure, or retreat. The original Pokémon Mansion on-transition getcaughtmon check still removes it permanently if Mew has been caught.
- The random choices are transient EWRAM data; progress never saves between rounds. Selection resets with each new interaction.

## Custom movesets

| Tier | Form (level) | Moves |
| --- | --- | --- |
| Weak | Tapu Bulu (30) | Horn Leech, Dazzling Gleam, Brick Break, Scary Face |
| Weak | Regirock (30) | Rock Slide, Bulldoze, Stomp, Charge Beam |
| Weak | Raikou (30) | Spark, Bite, Quick Attack, Reflect |
| Medium | Lunala (31) | Shadow Ball, Air Slash, Confusion, Night Shade |
| Medium | Volcanion (32) | Water Pulse, Flame Charge, Stomp, Haze |
| Medium | Genesect (32) | X-Scissor, Metal Claw, Flame Charge, Quick Attack |
| Medium | Urshifu Single Strike (32) | Wicked Blow, Brick Break, Aerial Ace, Headbutt |
| Strong | Rayquaza (34) | Dragon Claw, Air Slash, Crunch, Ancient Power |
| Strong | Groudon (34) | Earthquake, Lava Plume, Ancient Power, Slash |
| Capture | Mew (35) | Psychic, Swift, Ancient Power, Light Screen |

## Implementation locations

- `src/script_pokemon_util.c`: tier selection, static roster, battle-party preparation and move assignment.
- `src/battle_setup.c`: scripted legendary battle launcher with disguise-only flag.
- `src/item_use.c`: refuses Poke Balls during disguised battles, without restricting healing items.
- `include/constants/battle.h`: named flag alias for otherwise unused battle-type bit 14.
- `data/maps/PokemonMansion_1F_Frlg/scripts.inc`: four-round script and catch-only completion.
- `data/specials.inc`: registers the new special functions, appended to avoid renumbering existing specials.

**Implementation-only handoff:** The user explicitly prohibited testing in this pass. No build, emulator run, or validation was requested or performed. The commit is source implementation, not independently verified gameplay acceptance.
