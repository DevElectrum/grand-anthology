#include "xgba_a2_transaction.h"

#include <stddef.h>
#include <string.h>

static void Write32(uint8_t *p, uint32_t number)
{
    p[0] = (uint8_t)number;
    p[1] = (uint8_t)(number >> 8);
    p[2] = (uint8_t)(number >> 16);
    p[3] = (uint8_t)(number >> 24);
}

int XgbaA2TransactionCommit(struct XgbaA2Pager *pager,
                            const struct XgbaA2Cohort *committed,
                            XgbaA2ReadNativeSector reader,
                            XgbaA2WriteNativeSlot writer,
                            void *nativeContext, uint8_t *scratch4096,
                            struct XgbaA2Cohort *confirmed)
{
    struct XgbaA2Cohort before, after;
    struct XgbaA2JournalInfo metadata;
    uint32_t generations[XGBA_A2_PC_PAGES];
    uint8_t payload[XGBA_A2_META_PAYLOAD_BYTES] = {0};
    uint8_t nextSlot;
    uint32_t nextCounter;
    uint32_t i;
    int writeStatus, selectStatus;

    if (pager == NULL || committed == NULL || reader == NULL || writer == NULL
        || scratch4096 == NULL || confirmed == NULL
        || !pager->ready || pager->fatal || pager->resident == NULL
        || committed->nativeSlot > 1 || committed->nativeCounter == UINT32_MAX
        || committed->metadataGeneration == 0
        || pager->activePage >= XGBA_A2_PC_PAGES)
        return XGBA_A2_JOURNAL_INVALID_ARG;

    // Do not write if the physical disk no longer matches the caller's
    // prior globally committed native/extended save generation.
    if (XgbaA2CohortSelect(&pager->io, reader, nativeContext,
                           scratch4096, &before) != XGBA_A2_JOURNAL_OK
        || before.metadataGeneration != committed->metadataGeneration
        || before.nativeSlot != committed->nativeSlot
        || before.nativeCounter != committed->nativeCounter
        || memcmp(before.pages, committed->pages, sizeof(before.pages)) != 0)
        return XGBA_A2_JOURNAL_MISSING;

    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
    {
        if (pager->committed[i] != committed->pages[i])
            return XGBA_A2_JOURNAL_INVALID_ARG;
    }

    if (XgbaA2PagerPrepareSave(pager, generations) != XGBA_A2_JOURNAL_OK)
        return XGBA_A2_JOURNAL_IO_ERROR;

    nextCounter = committed->nativeCounter + 1;
    nextSlot = (uint8_t)(1 - committed->nativeSlot);

    // Native Flash writes are constrained to the OTHER physical save slot.
    // The 14 sectors must have consistent checksums and new counter before
    // an extended metadata record is even considered.
    if (!writer(nativeContext, nextSlot, nextCounter, pager->resident,
                XGBA_A2_PC_BYTES))
        return XGBA_A2_JOURNAL_IO_ERROR;
    if (!XgbaA2CohortVerifyNativeSlot(reader, nativeContext,
                                      scratch4096, nextSlot, nextCounter))
        return XGBA_A2_JOURNAL_IO_ERROR;
    if (!XgbaA2FingerprintNativeSlot(reader, nativeContext, nextSlot,
                                     scratch4096, payload))
        return XGBA_A2_JOURNAL_IO_ERROR;

    Write32(payload + 16, nextCounter);
    payload[20] = pager->activePage;
    payload[21] = nextSlot;
    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
        Write32(payload + 24 + i * 4, generations[i]);

    // The single-byte A/B metadata commit is LAST. A failed commit leaves
    // an inspectable previous generation, even if the new native slot is
    // fully written.
    writeStatus = XgbaA2JournalStageMeta(
        &pager->io, committed->metadataGeneration, payload,
        sizeof(payload), &metadata);

    // A power loss after the final commit byte but before readback can make
    // a write report failure even if the new global state is now committed.
    // A re-select is the authoritative test, never the API return alone.
    selectStatus = XgbaA2CohortSelect(&pager->io, reader, nativeContext,
                                      scratch4096, &after);
    if (selectStatus != XGBA_A2_JOURNAL_OK
        || after.nativeSlot != nextSlot || after.nativeCounter != nextCounter
        || after.activePage != pager->activePage
        || memcmp(after.pages, generations, sizeof(generations)) != 0)
        return XGBA_A2_JOURNAL_IO_ERROR;

    if (writeStatus != XGBA_A2_JOURNAL_OK
        && writeStatus != XGBA_A2_JOURNAL_IO_ERROR)
        return XGBA_A2_JOURNAL_IO_ERROR;

    if (XgbaA2PagerCommit(pager, generations) != XGBA_A2_JOURNAL_OK)
        return XGBA_A2_JOURNAL_IO_ERROR;

    *confirmed = after;
    return XGBA_A2_JOURNAL_OK;
}
