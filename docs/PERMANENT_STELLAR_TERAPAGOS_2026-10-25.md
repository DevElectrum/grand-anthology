# Permanent Stellar Terapagos: Cave of Origin

**Status:** Source implementation on public `main`, subject to exact-head optimized build and gameplay acceptance. No emulator save/ROM files published.

## Intent

This is **not** the standard Terastallization mechanic and requires neither a Tera Orb nor any activation input. In addition to the ordinary level-31 Terapagos on Route 114, the Hoenn Champion can encounter a separate level-75 `SPECIES_TERAPAGOS_STELLAR` in Cave of Origin B1F. Both are independently catchable within one save and share only their National Dex species, not encounter-caught flags.

- Native Stellar species: 160 HP / 105 Attack / 110 Defense / 130 Special Attack / 110 Special Defense / 85 Speed (700 BST), native Stellar battle art and species ability Teraform Zero.
- New form table for the **permanent Stellar instance only** contains no faint/battle-end return-to-Normal transition and no Terastallization activation. Its native stored species is Stellar, including party, PC, and reload; verify this end-to-end in emulator. Original Normal and temporary Terastal/Tera Shift form data remain unchanged.
- Teraform Zero is now checked on **switch-in** for permanent Stellar species, clearing applicable active weather/terrain without requiring form-change activation. Tera Starstorm already checks `SPECIES_TERAPAGOS_STELLAR` for Stellar move typing/spread behavior. Full battle damage and Tera interactions still require gameplay verification.
- The overworld display borrows the existing Terapagos Terastal-form follower sprite; the catch/battle uses actual Stellar species and existing native in-battle Stellar sprites. Adding bespoke Stellar overworld art is optional cosmetic work, not required for the form to stay permanent.

## Encounter

- Cave of Origin **B1F**, object at **(9,14), elevation 3**, after `FLAG_HOENN_CHAMPION`; accessible from the original 1F warp and outside cave entrance. The Cave of Origin expert steps aside at Sootopolis **(30,18)** post-Champion, keeping the original crisis/Wallace scene unchanged before Champion.
- `FLAG_HIDE_ANTHOLOGY_TERAPAGOS_STELLAR` is a transient encounter visibility bit and `FLAG_CAUGHT_ANTHOLOGY_TERAPAGOS_STELLAR` is permanent caught state. Faint = hide until exit/revisit; flee/teleport = retry; catch = permanently hidden on future transitions.
- The Route 114 Normal encounter now uses `FLAG_CAUGHT_ANTHOLOGY_TERAPAGOS_NORMAL` rather than the **species-wide caught bit**, so catching Stellar first does not remove Normal. Likewise catching Normal does not remove Stellar. Earlier saves whose Normal catch predates this new flag should be checked; they may see a second Normal encounter rather than permanently losing the chance to catch it.
- No other caught Legendary prerequisites and no changes to general P5 Terastallization settings.

## Verification policy

`test/overworld/ValidatePermanentStellarTerapagos.py` checks the two distinct form-change paths, battle ability hook, Champion gate, independent catches, script references, native B1F walkable geometry and map entrance flood-fill. It runs in P0 Gameplay Build. This is **source-level** evidence, not proof that PC storage/save/return-to-battle preserve the species or that all Stellar offensive effects work.

Future mGBA/Nyx acceptance (separate test session): before Champion, expert blocks original cave / Stellar absent; after Champion, expert yields and object appears. Catch Normal then Stellar, repeat with reversed order on disposable save. Verify native Stellar 700 stats and species persist through faint/heal, party/PC transfer, battle exit, save/close/reopen. Test no Tera Orb, Teraform Zero weather/terrain reset and Tera Starstorm damage/type, plus catch/faint/revisit and bag/PC full behavior. Preserve original Hoenn storyline.
