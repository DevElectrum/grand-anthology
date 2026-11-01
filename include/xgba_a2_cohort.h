#ifndef GUARD_XGBA_A2_COHORT_H
#define GUARD_XGBA_A2_COHORT_H

#include <stdint.h>
#include "xgba_a2_journal.h"
#include "xgba_a2_fingerprint.h"

// Complete native Flash and extended-PC cohort. No game globals modified.
struct XgbaA2Cohort
{
    uint32_t metadataGeneration;
    uint32_t nativeCounter;
    uint32_t pages[XGBA_A2_PC_PAGES];
    uint8_t nativeSlot;
    uint8_t activePage;
    uint8_t metadataSlot;
    uint8_t recovered;
};

// Requires a reusable 4 KiB native-sector scratch buffer. Validates all 14
// uniquely numbered native sectors, their counters/legacy checksums, the slot
// SHA-256 prefix, every pinned PC generation and native-PC byte equality.
// Fails closed; caller must not enable gameplay if no cohort is coherent.
int XgbaA2CohortSelect(const struct XgbaA2JournalIo *io,
                       XgbaA2ReadNativeSector reader,
                       void *nativeContext, uint8_t *scratch,
                       struct XgbaA2Cohort *selected);

int XgbaA2CohortVerifyNativeSlot(XgbaA2ReadNativeSector reader,
                                 void *nativeContext, uint8_t *scratch,
                                 uint8_t slot, uint32_t counter);
#endif
