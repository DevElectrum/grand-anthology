#ifndef GUARD_XGBA_A2_BOOTSTRAP_H
#define GUARD_XGBA_A2_BOOTSTRAP_H

#include "xgba_a2_transaction.h"

// Only for an EXPLICIT new game with a verified, completely erased E6 seed.
// No recovery or migration is attempted when any byte is unexpected.
// All input/output callbacks are borrowed; no 34 KiB temporary RAM page.
int XgbaA2BootstrapCommit(const struct XgbaA2JournalIo *io,
                          XgbaA2ReadNativeSector reader,
                          XgbaA2WriteNativeSlot writer, void *nativeContext,
                          uint8_t *scratch4096, const void *residentPC,
                          struct XgbaA2Cohort *confirmed);

#endif
