# A2 special save-entry audit (2026-11-01; read-only investigation)

## Baseline and evidence

- Branch `xgba/a2-collection-lab`, reviewed against code head `e8f5f02a9d2f53ee4aebff9d1085adca2ddf514d` and docs checkpoint `ea589362`.
- Both jobs in `41297349236` passed. ARM compilation and host cohort tests **do not validate the link UI or trade synchronization**.
- Major impact: a missing global native+extended transaction can silently roll back party/PC/progression while an old caller believes it saved.

## Save API coverage

| Entry point | A2 implementation | Call sites | Risk / next design action |
|---|---|---|---|
| `TrySavingData(SAVE_NORMAL/LINK/EREADER)` | Calls `A2Save` and verified transaction; propagates failure screen | save dialog, link, Frontier and other modules | Test in emulator and inspect caller error handling |
| `TrySavingData(SAVE_HALL_OF_FAME)` | Global commit, then auxiliary 28/29 archive sectors; two HOF scene handlers test return | Emerald and FRLG Hall of Fame | Verify runtime ceremony, auxiliary-failure independence |
| `SAVE_OVERWRITE_DIFFERENT_FILE` | Allowed only on one-shot new-game bootstrap permission | New Game | Test pristine vs partially written E6 and recovery |
| `SAVE_HALL_OF_FAME_ERASE_BEFORE` | Refused in A2; legacy marks it unused | save.c only | Keep denied unless new callsite appears |
| `LinkFullSave_Init/WriteSector/ReplaceLastSector/SetLastSectorSignature` | A2 branches return TRUE with **no save** | `src/trade.c` two trade state machines | **Critical open defect.** Trade code advances on TRUE as if a full save finished. A direct full commit at first WriteSector would violate the legacy deferred-final-signature handshake. Do not naive-patch before protocol-specific failure and synchronization tests |
| `WriteSaveBlock2/WriteSaveBlock1Sector` | A2 branches return TRUE with **no save** | `src/start_menu.c`, `src/record_mixing.c`, `src/berry_blender.c` | **Critical open defect.** First function's return is ignored; second TRUE advances state. Options: guarded one-time full SAVE_LINK with explicit caller error handling, or disable unsupported A2 link flows before transactions |
| `HandleSavingData` direct | A2 calls `A2Save` | `src/save_failed_screen.c` | Inspect retry/return logic, ensure unknown final-marker state triggers clean reselect rather than an unsafe retry |
| Native `ProgramFlashSectorAndVerify`, `ProgramFlashByte`, `EraseFlashSector` | Core callers mainly `src/save.c`; `src/save_failed_screen.c` writes a byte to identify damaged sectors | save and failure paths | Verify special diagnostic writes do not mutate currently committed save sectors or masquerade as a successful save |
| `ClearSaveData` | A2 refuses native erase | New Game / clear data flow | Data-preserving but must not display false deletion success; test screen behavior |

## Trade protocol reason naive replacement is unsafe

Original trade flow in `src/trade.c`:

1. Transferred Pokémon and trade statistic are applied.
2. `LinkFullSave_Init` starts an incremental save.
3. `LinkFullSave_WriteSector` returns TRUE only when sectors are complete.
4. `LinkFullSave_ReplaceLastSector` performs additional verification.
5. Following link standby/partner sync, `LinkFullSave_SetLastSectorSignature` publishes the final native save signature, then trade exits.

A2's single metadata marker is the *global* publication point. Publishing it during step 3 instead of after partner synchronization (step 5) changes bilateral trade crash semantics. A durable solution must stage the alternate slot and changed PC pages, then publish metadata only at the last signature/handshake boundary, or explicitly block unsupported trading before irreversible mutation.

## Safe implementation decision

- **First choice:** Implement phase-split A2 transaction with an independent host harness: prepare/stage without committing, verify alternate native slot and metadata payload, publish marker at final sync, then reselect exact cohort. Any error triggers a visible failed-save path, locks saving until reload, and trade callers cannot progress as if saved. Ensure interrupted prepare never invalidates prior cohort.
- **Secondary choice:** If real XGBA multiplayer cannot be tested/reliably supported, gate trade/link feature *before* entering state machines in A2, with a clear unsupported message; do not silently drop saves.
- **Other incremental callers:** For start menu, mixing and berry-blender paths, replace the two-phase native-only writes with a single full A2 transaction at the correct protocol point and explicit caller error handling; where synchronized gameplay cannot be proved, fail closed before committing external changes.
- **Do not merge or publish A2** until both categories are resolved or explicitly prevented from being entered, with automated source/host tests and a traceable limitation.

## Autonomous next action

Proceed with the disposable XGBA boot probe while planning the write-stage/publish-stage refactor and evaluating which link flows are actually reachable in the XGBA-only build. No player input or Nyx tests are needed for this audit.
