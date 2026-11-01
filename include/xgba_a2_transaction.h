#ifndef GUARD_XGBA_A2_TRANSACTION_H
#define GUARD_XGBA_A2_TRANSACTION_H

#include "xgba_a2_pager.h"
#include "xgba_a2_cohort.h"

// Must write a COMPLETE new native Flash slot and no other slot. The
// caller supplies prepared SaveBlock1, SaveBlock2, SaveBlock3 and the live
// PokemonStorage page. Return false on any incomplete physical write.
typedef int (*XgbaA2WriteNativeSlot)(void *context, uint8_t targetSlot,
                                     uint32_t newCounter,
                                     const void *residentPC,
                                     uint32_t residentBytes);

// Only the metadata commit publishes a new native + extended collection.
// On any failure the old metadata and old native slot remain protected.
// The game must not report a successful save unless this returns OK.
int XgbaA2TransactionCommit(struct XgbaA2Pager *pager,
                            const struct XgbaA2Cohort *committed,
                            XgbaA2ReadNativeSector reader,
                            XgbaA2WriteNativeSlot writer,
                            void *nativeContext, uint8_t *scratch4096,
                            struct XgbaA2Cohort *confirmed);

#endif
