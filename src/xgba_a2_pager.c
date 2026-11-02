#include "xgba_a2_pager.h"

#include <stddef.h>
#include <string.h>

#if defined(XGBA_A2) && XGBA_A2
// A2-only volatile diagnostics, owned by src/save.c; never persisted.
extern volatile uint32_t gXgbaA2PagerTrace;
#define PAGER_TRACE(stage, result) \
    (gXgbaA2PagerTrace = ((uint32_t)(stage) << 24) | ((uint32_t)(result) & 0xFFFFFFU))
#else
#define PAGER_TRACE(stage, result) ((void)0)
#endif

static int Ready(const struct XgbaA2Pager *pager)
{
    return pager != NULL && pager->ready && !pager->fatal
        && pager->resident != NULL;
}

// Compare the live page with the exact current staged or committed record.
// No source mutations need to manually set a dirty flag: a change is found
// even if it was made through an old direct BoxPokemon pointer.
static int FlushCurrent(struct XgbaA2Pager *pager)
{
    uint8_t page = pager->activePage;
    uint32_t sourceGeneration = pager->staged[page] != 0
        ? pager->staged[page] : pager->committed[page];
    struct XgbaA2JournalInfo info;
    int status;

    PAGER_TRACE(1, ((uint32_t)page << 16) | (sourceGeneration & 0xFFFFU));
    status = XgbaA2JournalEqualsPageGeneration(
        &pager->io, page, sourceGeneration,
        pager->resident, XGBA_A2_PC_BYTES);

    PAGER_TRACE(2, status);
    if (status == XGBA_A2_JOURNAL_OK)
        return XGBA_A2_JOURNAL_OK;
    if (status != XGBA_A2_JOURNAL_DIFFERENT)
        return status;

    // Even if a prior attempt partially staged a newer record, ALWAYS
    // overwrite the slot opposite the committed generation.
    PAGER_TRACE(3, page);
    status = XgbaA2JournalStagePage(
        &pager->io, page, pager->committed[page], pager->resident,
        XGBA_A2_PC_BYTES, &info);
    PAGER_TRACE(4, status);
    if (status == XGBA_A2_JOURNAL_OK)
    {
        pager->staged[page] = info.generation;
        return XGBA_A2_JOURNAL_OK;
    }

    if (status == XGBA_A2_JOURNAL_UNCHANGED)
    {
        pager->staged[page] = 0;
        return XGBA_A2_JOURNAL_OK;
    }
    return status;
}

int XgbaA2PagerInit(struct XgbaA2Pager *pager,
                    const struct XgbaA2JournalIo *io,
                    void *currentNativePc, uint8_t selectedPage,
                    const uint32_t committedGenerations[XGBA_A2_PC_PAGES])
{
    uint32_t i;
    int status;

    if (pager == NULL || io == NULL || io->read == NULL || io->write == NULL
        || currentNativePc == NULL || committedGenerations == NULL
        || selectedPage >= XGBA_A2_PC_PAGES)
        return XGBA_A2_JOURNAL_INVALID_ARG;

    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
    {
        if (committedGenerations[i] == 0)
            return XGBA_A2_JOURNAL_INVALID_ARG;
    }

    memset(pager, 0, sizeof(*pager));
    pager->io = *io;
    pager->resident = currentNativePc;
    pager->activePage = selectedPage;
    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
        pager->committed[i] = committedGenerations[i];

    status = XgbaA2JournalEqualsPageGeneration(
        &pager->io, selectedPage, pager->committed[selectedPage],
        pager->resident, XGBA_A2_PC_BYTES);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;

    pager->ready = 1;
    return XGBA_A2_JOURNAL_OK;
}

int XgbaA2PagerSwitch(struct XgbaA2Pager *pager, uint8_t destination)
{
    uint32_t generation;
    int status;

    if (!Ready(pager) || destination >= XGBA_A2_PC_PAGES)
        return XGBA_A2_JOURNAL_INVALID_ARG;
    if (destination == pager->activePage)
        return XGBA_A2_JOURNAL_OK;

    status = FlushCurrent(pager);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;

    generation = pager->staged[destination] != 0
        ? pager->staged[destination] : pager->committed[destination];
    status = XgbaA2JournalReadPageGeneration(
        &pager->io, destination, generation, pager->resident,
        XGBA_A2_PC_BYTES, NULL);
    if (status != XGBA_A2_JOURNAL_OK)
    {
        // Some backends may fail during the final copy after CRC validation.
        // Resident memory may be partial. Never continue or save in this state.
        pager->fatal = 1;
        return status;
    }

    pager->activePage = destination;
    return XGBA_A2_JOURNAL_OK;
}

int XgbaA2PagerPrepareSave(struct XgbaA2Pager *pager,
                           uint32_t pageGenerations[XGBA_A2_PC_PAGES])
{
    uint32_t i;
    int status;

    if (!Ready(pager) || pageGenerations == NULL)
        return XGBA_A2_JOURNAL_INVALID_ARG;

    PAGER_TRACE(5, pager->activePage);
    status = FlushCurrent(pager);
    if (status != XGBA_A2_JOURNAL_OK)
    {
        PAGER_TRACE(6, status);
        return status;
    }
    PAGER_TRACE(7, pager->activePage);

    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
        pageGenerations[i] = pager->staged[i] != 0
            ? pager->staged[i] : pager->committed[i];

    return XGBA_A2_JOURNAL_OK;
}

int XgbaA2PagerCommit(struct XgbaA2Pager *pager,
                      const uint32_t committedGenerations[XGBA_A2_PC_PAGES])
{
    uint32_t i;

    if (!Ready(pager) || committedGenerations == NULL)
        return XGBA_A2_JOURNAL_INVALID_ARG;

    // The caller MUST have durably written the matching native save slot
    // and atomically committed the metadata record before calling this.
    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
    {
        uint32_t planned = pager->staged[i] != 0
            ? pager->staged[i] : pager->committed[i];
        if (committedGenerations[i] != planned)
            return XGBA_A2_JOURNAL_INVALID_ARG;
    }
    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
    {
        pager->committed[i] = committedGenerations[i];
        pager->staged[i] = 0;
    }
    return XGBA_A2_JOURNAL_OK;
}
