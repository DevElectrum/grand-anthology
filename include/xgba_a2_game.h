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
// Pointer is borrowed and valid until the next resident-group switch.
// Counts return -1 on invalid/unavailable storage rather than false zero.
u8 *XgbaA2GameGlobalBoxName(u16 globalBox);
s16 XgbaA2GameCountGlobalBoxMons(u16 globalBox);
// Checked, in-memory, exact-record transfers. Return FALSE without mutations
// for invalid IDs, empty source, occupied move target or identical positions.
// The caller must not retain pointers across a subsequent group switch.
bool8 XgbaA2GameMoveGlobalBoxMon(u16 sourceBox, u8 sourcePos,
                                 u16 destinationBox, u8 destinationPos);
bool8 XgbaA2GameSwapGlobalBoxMons(u16 firstBox, u8 firstPos,
                                  u16 secondBox, u8 secondPos);
// Non-destructive R8 cross-group move/swap, independent of box UI pointers.
bool8 XgbaA2GameBeginGlobalTransfer(u16 sourceBox, u8 sourcePosition);
bool8 XgbaA2GameGlobalTransferActive(void);
void XgbaA2GameCancelGlobalTransfer(void);
bool8 XgbaA2GameGlobalTransferTargetOccupied(u16 destinationBox, u8 position);
bool8 XgbaA2GameCommitGlobalTransfer(u16 destinationBox, u8 position, bool8 allowSwap);
// Shared C4 release transaction. Scope is explicitly chosen by the player;
// a 3360-bit plan never changes Pokemon or the bag until checked commit.
#define XGBA_R8_RELEASE_BOX 0
#define XGBA_R8_RELEASE_GROUP 1
#define XGBA_R8_RELEASE_ALL 2
#define XGBA_R8_RELEASE_LOCAL_MASK 3
#define XGBA_R8_RELEASE_SINGLE 4
u16 XgbaA2GamePlanGlobalRelease(u8 scope, u16 globalBox, u32 localMask);
u16 XgbaA2GamePlannedReleaseCount(void);
void XgbaA2GameCancelGlobalRelease(void);
bool8 XgbaA2GameCommitGlobalRelease(void);
bool8 XgbaA2GameCanReleaseGlobalSingle(u16 globalBox, u8 slot);
bool8 XgbaA2GameReleaseGlobalSingle(u16 globalBox, u8 slot);
// Move or swap the held items of two boxed Pokemon with no bag involvement.
bool8 XgbaA2GameTransferGlobalHeldItem(u16 fromBox, u8 fromPos,
                                      u16 toBox, u8 toPos, bool8 allowSwap);
// Checked party-to-global deposit. Returns FALSE without mutation for
// invalid/full destination, mail, empty slot or last usable party Pokémon.
bool8 XgbaA2GameDepositPartyMonGlobal(u8 partyPosition, u16 globalBox,
                                     u8 *placedPosition);
// C3B: checked global box-to-party withdrawal. Never changes active PC group.
// Both ends remain unmodified when storage is unavailable, source invalid,
// party full, or the party layout is inconsistent.
bool8 XgbaA2GameWithdrawGlobalBoxMon(u16 globalBox, u8 sourcePosition,
                                     u8 *placedPartyPosition);
#endif
#endif

#endif
