#include "xgba_a2_cohort.h"

#include <stddef.h>
#include <string.h>

#define DATA_BYTES 3968U
#define FOOTER 4084U
#define NATIVE_SECTORS 14U
#define SAVE_MAGIC 0x08012025U

static uint16_t Read16(const uint8_t *p)
{
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t Read32(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8)
        | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static uint32_t SectorLength(uint32_t logical)
{
    uint32_t remain;
    if (logical == 0)
        return 3884;
    if (logical < 5)
        remain = 14752U - (logical - 1U) * DATA_BYTES;
    else
        remain = XGBA_A2_PC_BYTES - (logical - 5U) * DATA_BYTES;
    return remain < DATA_BYTES ? remain : DATA_BYTES;
}

static uint16_t Checksum(const uint8_t *p, uint32_t length)
{
    uint32_t sum = 0, i;
    for (i = 0; i + 4 <= length; i += 4)
        sum += Read32(p + i);
    return (uint16_t)(sum + (sum >> 16));
}

static int VerifyNative(XgbaA2ReadNativeSector reader, void *context,
                        uint8_t *scratch, uint8_t slot, uint32_t counter,
                        uint8_t positions[14])
{
    uint32_t seen = 0, i;
    if (slot > 1 || (counter & 1U) != slot)
        return 0;
    for (i = 0; i < NATIVE_SECTORS; i++)
    {
        const uint8_t *f;
        uint32_t logical;
        if (!reader(context, (uint8_t)(slot * NATIVE_SECTORS + i), scratch))
            return 0;
        f = scratch + FOOTER;
        logical = Read16(f);
        if (logical >= NATIVE_SECTORS || (seen & (1U << logical))
            || Read32(f + 4) != SAVE_MAGIC || Read32(f + 8) != counter
            || Read16(f + 2) != Checksum(scratch, SectorLength(logical)))
            return 0;
        seen |= 1U << logical;
        positions[logical] = (uint8_t)(slot * NATIVE_SECTORS + i);
    }
    return seen == 0x3FFFU;
}

static int PcMatches(const struct XgbaA2JournalIo *io,
                     XgbaA2ReadNativeSector reader, void *context,
                     uint8_t *scratch, const uint8_t positions[14],
                     uint8_t page, uint8_t copy)
{
    uint8_t chunk[128];
    uint32_t logical;
    for (logical = 5; logical < NATIVE_SECTORS; logical++)
    {
        uint32_t i = 0;
        uint32_t size = SectorLength(logical);
        uint32_t pcPosition = (logical - 5U) * DATA_BYTES;
        if (!reader(context, positions[logical], scratch))
            return 0;
        while (i < size)
        {
            uint32_t n = size - i;
            uint32_t address;
            if (n > sizeof(chunk))
                n = sizeof(chunk);
            address = XGBA_A2_PC_BASE
                + ((uint32_t)page * 2 + copy) * XGBA_A2_PC_STRIDE
                + XGBA_A2_RECORD_HEADER_BYTES + pcPosition + i;
            if (!io->read(io->context, address, chunk, n)
                || memcmp(scratch + i, chunk, n) != 0)
                return 0;
            i += n;
        }
    }
    return 1;
}

static int TrySlot(const struct XgbaA2JournalIo *io,
                   XgbaA2ReadNativeSector reader, void *nativeContext,
                   uint8_t *scratch, uint8_t metaSlot,
                   struct XgbaA2Cohort *out)
{
    uint8_t payload[XGBA_A2_META_PAYLOAD_BYTES];
    uint8_t positions[14];
    uint8_t fingerprint[16];
    uint8_t page, slot;
    uint32_t counter, i;
    struct XgbaA2JournalInfo meta, pageRecord;
    int status = XgbaA2JournalReadMetaSlot(io, metaSlot, payload, &meta);

    if (status != XGBA_A2_JOURNAL_OK)
        return status;
    counter = Read32(payload + 16);
    page = payload[20];
    slot = payload[21];
    if (page >= XGBA_A2_PC_PAGES || slot > 1
        || payload[22] || payload[23])
        return XGBA_A2_JOURNAL_MISSING;
    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
    {
        out->pages[i] = Read32(payload + 24 + i * 4);
        if (!out->pages[i])
            return XGBA_A2_JOURNAL_MISSING;
    }
    if (!VerifyNative(reader, nativeContext, scratch, slot, counter, positions))
        return XGBA_A2_JOURNAL_MISSING;
    if (!XgbaA2FingerprintNativeSlot(reader, nativeContext, slot, scratch,
                                     fingerprint) || memcmp(payload, fingerprint, 16))
        return XGBA_A2_JOURNAL_MISSING;

    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
    {
        status = XgbaA2JournalFindPageGeneration(io, (uint8_t)i,
                                                  out->pages[i], &pageRecord);
        if (status != XGBA_A2_JOURNAL_OK)
            return status;
        if (i == page && !PcMatches(io, reader, nativeContext, scratch,
                                    positions, page, pageRecord.slot))
            return XGBA_A2_JOURNAL_MISSING;
    }
    out->metadataGeneration = meta.generation;
    out->nativeCounter = counter;
    out->metadataSlot = metaSlot;
    out->nativeSlot = slot;
    out->activePage = page;
    out->recovered = 0;
    return XGBA_A2_JOURNAL_OK;
}

int XgbaA2CohortSelect(const struct XgbaA2JournalIo *io,
                       XgbaA2ReadNativeSector reader, void *nativeContext,
                       uint8_t *scratch, struct XgbaA2Cohort *selected)
{
    uint8_t metaPayload[2][XGBA_A2_META_PAYLOAD_BYTES];
    struct XgbaA2JournalInfo record[2];
    struct XgbaA2Cohort candidate;
    uint8_t valid[2] = {0, 0};
    uint8_t first, second;
    uint8_t slot;

    if (io == NULL || io->read == NULL || io->write == NULL
        || reader == NULL || scratch == NULL || selected == NULL)
        return XGBA_A2_JOURNAL_INVALID_ARG;

    // First read only 2 tiny metadata records. Verify the newest COMPLETE
    // cohort; only scan the older 8-page cohort when recovery is needed.
    for (slot = 0; slot < 2; slot++)
    {
        int status = XgbaA2JournalReadMetaSlot(io, slot,
                                               metaPayload[slot], &record[slot]);
        if (status == XGBA_A2_JOURNAL_OK)
            valid[slot] = 1;
        else if (status == XGBA_A2_JOURNAL_IO_ERROR)
            return status;
    }
    if (!valid[0] && !valid[1])
        return XGBA_A2_JOURNAL_MISSING;

    if (valid[0] && valid[1]
        && record[0].generation == record[1].generation
        && memcmp(metaPayload[0], metaPayload[1],
                  XGBA_A2_META_PAYLOAD_BYTES) != 0)
        return XGBA_A2_JOURNAL_MISSING;

    first = valid[1] && (!valid[0]
        || record[1].generation > record[0].generation) ? 1 : 0;
    second = (uint8_t)(1 - first);

    if (TrySlot(io, reader, nativeContext, scratch, first, &candidate)
        == XGBA_A2_JOURNAL_OK)
    {
        *selected = candidate;
        selected->recovered = 0;
        return XGBA_A2_JOURNAL_OK;
    }

    if (valid[second] && TrySlot(io, reader, nativeContext,
                                 scratch, second, &candidate)
        == XGBA_A2_JOURNAL_OK)
    {
        *selected = candidate;
        selected->recovered = 1;
        return XGBA_A2_JOURNAL_OK;
    }
    return XGBA_A2_JOURNAL_MISSING;
}

int XgbaA2CohortVerifyNativeSlot(XgbaA2ReadNativeSector reader,
                                 void *nativeContext, uint8_t *scratch,
                                 uint8_t slot, uint32_t counter)
{
    uint8_t positions[14];
    if (reader == NULL || scratch == NULL)
        return 0;
    return VerifyNative(reader, nativeContext, scratch, slot, counter, positions);
}
