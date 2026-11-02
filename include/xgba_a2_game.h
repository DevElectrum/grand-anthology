#ifndef GUARD_XGBA_A2_GAME_H
#define GUARD_XGBA_A2_GAME_H

#include "global.h"
#include "xgba_a2_storage_format.h"

// Available ONLY in a separately qualified A2 experimental build.
// Page index is zero-based, with 14 local boxes per page.
// A failed switch locks out saves until reloading the last valid cohort.
#if defined(XGBA_A2) && XGBA_A2
// Grants exactly one pristine-E6 first-save attempt after NewGameInitData.
void XgbaA2GameArmFreshSave(void);
u8 XgbaA2GamePage(void);
bool8 XgbaA2GameSwitchPage(u8 page);
// Rebinds the resident 14-box page ONLY after the game's ASLR move has
// copied PokemonStorage into its new address. No journal writes or resets.
void XgbaA2GameRebindRelocatedPc(void);
// Reports another page with free slots without changing the live PC.
bool8 XgbaA2GameFindFreePage(u8 *page);
#if defined(XGBA_R8) && XGBA_R8
// 0..111 global boxes; local box 14 remains the legacy party sentinel.
bool8 XgbaA2GameCollectionReady(void);
// Before the first Save only the native 14-box structure is initialized.
bool8 XgbaA2GameFreshUncommitted(void);
bool8 XgbaA2GameSelectGlobalBox(u16 globalBox);
struct BoxPokemon *XgbaA2GameGlobalBoxMon(u16 globalBox, u8 position);
// Checked, in-memory, exact-record transfers. Return FALSE without mutations
// for invalid IDs, empty source, occupied move target or identical positions.
// The caller must not retain pointers across a subsequent group switch.
bool8 XgbaA2GameMoveGlobalBoxMon(u16 sourceBox, u8 sourcePos,
                                 u16 destinationBox, u8 destinationPos);
bool8 XgbaA2GameSwapGlobalBoxMons(u16 firstBox, u8 firstPos,
                                  u16 secondBox, u8 secondPos);
#endif
#endif

#endif
