# Route 101: Permanent Zygarde Complete Forme

## Contract

- Three independent catches coexist: Zygarde 10% in Petalburg Woods (Aura Break), 50% in Meteor Falls (Power Construct), and **permanent Complete Forme** near Littleroot.
- Complete appears on Route 101 at **(5, 16), elevation 3**, a walkable west-side clearing close to the Littleroot connection, **only after FLAG_HOENN_CHAMPION**.
- Overworld object `LOCALID_ANTHOLOGY_ZYGARDE_COMPLETE` starts a **level-70 SPECIES_ZYGARDE_COMPLETE** scripted Legendary battle.
- **Independent captured flag** `FLAG_CAUGHT_ANTHOLOGY_ZYGARDE_COMPLETE` avoids hiding the object merely because the shared Zygarde Pokédex number was registered by catching Doggo or 50%.
- Catch => set dedicated flag, remove object permanently (including revisit, save/load). Faint => remove until next transition, then reappear. Flee/teleport => retry available. No 100-cell collection, no Greninja-in-party prerequisite, no dependence on catching 10% or 50%.
- The encounter uses the engine's **actual SPECIES_ZYGARDE_COMPLETE**, not an in-battle transformation from 50%. Form data has its own graphics, 708-BST stats, and form change table; the existing battle-party species tracking restores a temporarily transformed 50% Zygarde after battle, while a monster that started as Complete should remain Complete on its own. **Persistence still requires gameplay verification**.
- The existing `P_MEGA_EVOLUTIONS=TRUE` and `P_GEN_9_MEGA_EVOLUTIONS=P_MEGA_EVOLUTIONS` enable the Pokémon Legends: Z-A Mega catalog; Complete's form table already supports `SPECIES_ZYGARDE_MEGA` with `ITEM_ZYGARDITE`. This implementation **does not distribute Zygardite or the Mega Ring** and does not alter their other acquisition gates.

## Source changes

- `data/maps/Route101/map.json`: static object's walkable clearing.
- `data/maps/Route101/scripts.inc`: champion-gated visibility, challenge, catch/faint/retry, concise guardian dialogue.
- `include/constants/flags.h`: new independent Complete-specific hide/caught bits in the existing world-map allocation.

## Next Astra: deferred testing, not performed in implementation pass

The player explicitly requested **no testing this pass**. The next Astra should verify source/build, then emulator:
1. Before Hoenn Champion, object absent; afterward, object visible even if the player previously caught either/both Zygarde 10% and 50%.
2. Beat Complete: temporarily disappears and returns on re-entering Route 101. Flee or teleport: no permanently caught flag.
3. Catch Complete: inspect actual species and Complete sprite/stats in party and PC; verify after entering/leaving a battle, depositing/withdrawing and save/reload.
4. Re-enter Route 101 and save/reload: captured Complete object stays absent, while other two forms' own flags are independent.
5. Hold Zygardite + have usable Mega evolution access, if desired, to separately check Mega Zygarde and its postbattle reversion. This is not a new acquisition requirement.
6. Review preexisting **Zygarde Cube ability-change route**, which can change Doggo into a Power Construct form and may bypass the intended 10%-only Aura Break policy. Not changed in this focused Complete encounter implementation.

The map spot was chosen after inspecting the Route 101 binary walkable-tile grid. **No game build, automated suite, or emulator run was performed in this pass.**
