#include "xgba_a2_transaction.h"

#include <stddef.h>
#include <string.h>

#if defined(XGBA_A2) && XGBA_A2
extern volatile uint32_t gXgbaA2TransactionTrace;
#define TX_TRACE(stage, detail) \
    (gXgbaA2TransactionTrace = ((uint32_t)(stage) << 24) | ((uint32_t)(detail) & 0xFFFFFFU))
#else
#define TX_TRACE(stage, detail) ((void)0)
#endif

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
    int writeStatus, selectStatus, prepareStatus;

    TX_TRACE(1, committed != NULL ? committed->nativeCounter : 0);

    if (pager == NULL || committed == NULL || reader == NULL || writer == NULL
        || scratch4096 == NULL || confirmed == NULL
        || !pager->ready || pager->fatal || pager->resident == NULL
        || committed->nativeSlot > 1 || committed->nativeCounter == UINT32_MAX
        || committed->metadataGeneration == 0
        || pager->activePage >= XGBA_A2_PC_PAGES)
    {
        TX_TRACE(0xE0, 0);
        return XGBA_A2_JOURNAL_INVALID_ARG;
    }

    // Do not write if the physical disk no longer matches the caller's
    // prior globally committed native/extended save generation.
    if (XgbaA2CohortSelect(&pager->io, reader, nativeContext,
                           scratch4096, &before) != XGBA_A2_JOURNAL_OK
        || before.metadataGeneration != committed->metadataGeneration
        || before.nativeSlot != committed->nativeSlot
        || before.nativeCounter != committed->nativeCounter
        || memcmp(before.pages, committed->pages, sizeof(before.pages)) != 0)
    {
        TX_TRACE(0xE1, 0);
        return XGBA_A2_JOURNAL_MISSING;
    }
    TX_TRACE(2, before.metadataGeneration);

    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
    {
        if (pager->committed[i] != committed->pages[i])
        {
            TX_TRACE(0xE2, i);
            return XGBA_A2_JOURNAL_INVALID_ARG;
        }
    }

    prepareStatus = XgbaA2PagerPrepareSave(pager, generations);
    if (prepareStatus != XGBA_A2_JOURNAL_OK)
    {
        TX_TRACE(0xE3, prepareStatus);
        return XGBA_A2_JOURNAL_IO_ERROR;
    }
    TX_TRACE(3, generations[pager->activePage]);

    nextCounter = committed->nativeCounter + 1;
    nextSlot = (uint8_t)(1 - committed->nativeSlot);

    // Native Flash writes are constrained to the OTHER physical save slot.
    // The 14 sectors must have consistent checksums and new counter before
    // an extended metadata record is even considered.
    TX_TRACE(4, (nextSlot << 16) | (nextCounter & 0xFFFF));
    if (!writer(nativeContext, nextSlot, nextCounter, pager->resident,
                XGBA_A2_PC_BYTES))
    {
        TX_TRACE(0xE4, nextCounter);
        return XGBA_A2_JOURNAL_IO_ERROR;
    }
    TX_TRACE(5, nextCounter);
    if (!XgbaA2CohortVerifyNativeSlot(reader, nativeContext,
                                      scratch4096, nextSlot, nextCounter))
    {
        TX_TRACE(0xE5, nextCounter);
        return XGBA_A2_JOURNAL_IO_ERROR;
    }
    TX_TRACE(6, nextCounter);
    if (!XgbaA2FingerprintNativeSlot(reader, nativeContext, nextSlot,
                                     scratch4096, payload))
    {
        TX_TRACE(0xE6, nextCounter);
        return XGBA_A2_JOURNAL_IO_ERROR;
    }
    TX_TRACE(7, nextCounter);

    Write32(payload + 16, nextCounter);
    payload[20] = pager->activePage;
    payload[21] = nextSlot;
    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
        Write32(payload + 24 + i * 4, generations[i]);

    // The single-byte A/B metadata commit is LAST. A failed commit leaves
    // an inspectable previous generation, even if the new native slot is
    // fully written.
    TX_TRACE(8, committed->metadataGeneration);
    writeStatus = XgbaA2JournalStageMeta(
        &pager->io, committed->metadataGeneration, payload,
        sizeof(payload), &metadata);
    TX_TRACE(9, writeStatus);

    // A power loss after the final commit byte but before readback can make
    // a write report failure even if the new global state is now committed.
    // A re-select is the authoritative test, never the API return alone.
    selectStatus = XgbaA2CohortSelect(&pager->io, reader, nativeContext,
                                      scratch4096, &after);
    if (selectStatus != XGBA_A2_JOURNAL_OK
        || after.nativeSlot != nextSlot || after.nativeCounter != nextCounter
        || after.activePage != pager->activePage
        || memcmp(after.pages, generations, sizeof(generations)) != 0)
    {
        TX_TRACE(0xE7, selectStatus);
        return XGBA_A2_JOURNAL_IO_ERROR;
    }
    TX_TRACE(10, after.metadataGeneration);

    if (writeStatus != XGBA_A2_JOURNAL_OK
        && writeStatus != XGBA_A2_JOURNAL_IO_ERROR)
    {
        TX_TRACE(0xE8, writeStatus);
        return XGBA_A2_JOURNAL_IO_ERROR;
    }

    if (XgbaA2PagerCommit(pager, generations) != XGBA_A2_JOURNAL_OK)
    {
        TX_TRACE(0xE9, 0);
        return XGBA_A2_JOURNAL_IO_ERROR;
    }
    TX_TRACE(11, after.metadataGeneration);

    *confirmed = after;
    return XGBA_A2_JOURNAL_OK;
}
