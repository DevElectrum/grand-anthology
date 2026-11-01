#include "xgba_a2_journal.h"

#include <stddef.h>
#include <string.h>

#define HEADER_BYTES 32
#define TRANSFER_BYTES 128
#define META_BYTES 24

static const uint8_t sPageMagic[8] = { 'X', 'G', 'B', 'A', 'A', '2', 'P', 0 };
static const uint8_t sMetaMagic[8] = { 'X', 'G', 'B', 'A', 'A', '2', 'M', 0 };

struct Candidate
{
    uint32_t generation;
    uint32_t dataCrc;
    uint8_t slot;
    uint8_t valid;
};

static uint16_t Read16(const uint8_t *p)
{
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t Read32(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8)
        | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void Write16(uint8_t *p, uint16_t value)
{
    p[0] = (uint8_t)value;
    p[1] = (uint8_t)(value >> 8);
}

static void Write32(uint8_t *p, uint32_t value)
{
    p[0] = (uint8_t)value;
    p[1] = (uint8_t)(value >> 8);
    p[2] = (uint8_t)(value >> 16);
    p[3] = (uint8_t)(value >> 24);
}

// CRC32, reflected polynomial 0xEDB88320; matches Python zlib.crc32.
// The per-byte bit loop makes an infrequent page commit slower, but avoids
// a full 1-KiB lookup table and its unnecessary ROM residency during A2.
static uint32_t CrcUpdate(uint32_t crc, const uint8_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        uint32_t bit;
        crc ^= buffer[i];
        for (bit = 0; bit < 8; bit++)
            crc = (crc >> 1) ^ (0xEDB88320U & (0U - (crc & 1U)));
    }
    return crc;
}

static uint32_t CrcRam(const uint8_t *data, uint32_t bytes)
{
    return ~CrcUpdate(0xFFFFFFFFU, data, bytes);
}

static int IoReady(const struct XgbaA2JournalIo *io)
{
    return io != NULL && io->read != NULL && io->write != NULL;
}

static uint32_t RecordOffset(uint8_t isMeta, uint8_t page, uint8_t slot)
{
    if (isMeta)
        return XGBA_A2_PC_META_BASE + (uint32_t)slot * XGBA_A2_PC_META_STRIDE;
    return XGBA_A2_PC_BASE + ((uint32_t)page * 2 + slot) * XGBA_A2_PC_STRIDE;
}

static int ReadPayloadCrc(const struct XgbaA2JournalIo *io, uint32_t offset,
                          uint32_t bytes, uint32_t *result)
{
    uint8_t buffer[TRANSFER_BYTES];
    uint32_t remaining = bytes;
    uint32_t crc = 0xFFFFFFFFU;
    offset += HEADER_BYTES;

    while (remaining != 0)
    {
        uint32_t amount = remaining > TRANSFER_BYTES ? TRANSFER_BYTES : remaining;
        if (!io->read(io->context, offset, buffer, amount))
            return 0;
        crc = CrcUpdate(crc, buffer, amount);
        offset += amount;
        remaining -= amount;
    }
    *result = ~crc;
    return 1;
}

static int CheckCandidate(const struct XgbaA2JournalIo *io, uint8_t isMeta,
                          uint8_t page, uint8_t slot, uint32_t bytes,
                          struct Candidate *out)
{
    const uint8_t *magic = isMeta ? sMetaMagic : sPageMagic;
    const uint32_t offset = RecordOffset(isMeta, page, slot);
    uint8_t header[HEADER_BYTES];
    uint32_t dataCrc;

    memset(out, 0, sizeof(*out));
    out->slot = slot;
    if (!io->read(io->context, offset, header, HEADER_BYTES))
        return XGBA_A2_JOURNAL_IO_ERROR;

    if (memcmp(header, magic, 8) != 0
        || Read16(header + 8) != XGBA_A2_RECORD_VERSION
        || Read16(header + 10) != 0
        || header[12] != page || header[13] != 0
        || Read16(header + 14) != 0
        || Read32(header + 16) == 0
        || Read32(header + 20) != bytes
        || Read32(header + 28) != CrcRam(header, 28))
        return XGBA_A2_JOURNAL_OK;

    if (!ReadPayloadCrc(io, offset, bytes, &dataCrc))
        return XGBA_A2_JOURNAL_IO_ERROR;

    if (dataCrc == Read32(header + 24))
    {
        out->valid = 1;
        out->generation = Read32(header + 16);
        out->dataCrc = dataCrc;
    }
    return XGBA_A2_JOURNAL_OK;
}

static int FindRecord(const struct XgbaA2JournalIo *io, uint8_t isMeta,
                      uint8_t page, uint32_t bytes,
                      struct Candidate *best, uint8_t *count)
{
    struct Candidate left, right;
    int status = CheckCandidate(io, isMeta, page, 0, bytes, &left);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;
    status = CheckCandidate(io, isMeta, page, 1, bytes, &right);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;

    *count = (uint8_t)(left.valid + right.valid);
    if (*count == 0)
        return XGBA_A2_JOURNAL_MISSING;

    if (right.valid && (!left.valid || right.generation > left.generation))
        *best = right;
    else
        *best = left;
    return XGBA_A2_JOURNAL_OK;
}

static int ReadRecord(const struct XgbaA2JournalIo *io, uint8_t isMeta,
                      uint8_t page, void *destination, uint32_t bytes,
                      struct XgbaA2JournalInfo *info)
{
    struct Candidate best;
    uint8_t count;
    uint8_t *result = destination;
    uint32_t offset, remaining;
    int status;

    if (!IoReady(io) || destination == NULL
        || page >= (isMeta ? 1 : XGBA_A2_PC_PAGES)
        || bytes != (isMeta ? META_BYTES : XGBA_A2_PC_BYTES))
        return XGBA_A2_JOURNAL_INVALID_ARG;

    // CRC validate the record BEFORE changing the caller's live page buffer.
    status = FindRecord(io, isMeta, page, bytes, &best, &count);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;

    offset = RecordOffset(isMeta, page, best.slot) + HEADER_BYTES;
    remaining = bytes;
    while (remaining != 0)
    {
        uint32_t amount = remaining > TRANSFER_BYTES ? TRANSFER_BYTES : remaining;
        if (!io->read(io->context, offset, result, amount))
            return XGBA_A2_JOURNAL_IO_ERROR;
        offset += amount;
        result += amount;
        remaining -= amount;
    }

    if (info != NULL)
    {
        info->generation = best.generation;
        info->slot = best.slot;
        info->validCopies = count;
    }
    return XGBA_A2_JOURNAL_OK;
}

static int SameAsRecord(const struct XgbaA2JournalIo *io, uint32_t offset,
                        const uint8_t *source, uint32_t bytes)
{
    uint8_t data[TRANSFER_BYTES];
    uint32_t remaining = bytes;
    offset += HEADER_BYTES;

    while (remaining != 0)
    {
        uint32_t amount = remaining > TRANSFER_BYTES ? TRANSFER_BYTES : remaining;
        if (!io->read(io->context, offset, data, amount))
            return -1;
        if (memcmp(data, source, amount) != 0)
            return 0;
        source += amount;
        offset += amount;
        remaining -= amount;
    }
    return 1;
}

static int WriteRecord(const struct XgbaA2JournalIo *io, uint8_t isMeta,
                       uint8_t page, const void *source, uint32_t bytes,
                       struct XgbaA2JournalInfo *info)
{
    struct Candidate best;
    const uint8_t *payload = source;
    const uint8_t *magic = isMeta ? sMetaMagic : sPageMagic;
    uint8_t header[HEADER_BYTES] = {0};
    uint8_t invalidate[8] = {0};
    uint8_t targetSlot;
    uint8_t count;
    uint32_t offset;
    uint32_t remaining;
    uint32_t actualCrc;
    int compareResult, status;

    if (!IoReady(io) || source == NULL
        || page >= (isMeta ? 1 : XGBA_A2_PC_PAGES)
        || bytes != (isMeta ? META_BYTES : XGBA_A2_PC_BYTES))
        return XGBA_A2_JOURNAL_INVALID_ARG;

    // Never initialize or reconstruct a page that has lost both valid copies.
    // Doing so would transform corruption into a silently empty collection.
    status = FindRecord(io, isMeta, page, bytes, &best, &count);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;

    offset = RecordOffset(isMeta, page, best.slot);
    if (best.dataCrc == CrcRam(payload, bytes))
    {
        compareResult = SameAsRecord(io, offset, payload, bytes);
        if (compareResult < 0)
            return XGBA_A2_JOURNAL_IO_ERROR;
        if (compareResult > 0)
        {
            if (info != NULL)
            {
                info->generation = best.generation;
                info->slot = best.slot;
                info->validCopies = count;
            }
            return XGBA_A2_JOURNAL_UNCHANGED;
        }
    }

    if (best.generation == UINT32_MAX)
        return XGBA_A2_JOURNAL_GENERATION_FULL;

    targetSlot = (uint8_t)(1 - best.slot);
    offset = RecordOffset(isMeta, page, targetSlot);

    // Start by destroying the non-active copy's magic. The last 8-byte magic
    // write is the commit marker. Any incomplete payload/header is ignored.
    if (!io->write(io->context, offset, invalidate, sizeof(invalidate)))
        return XGBA_A2_JOURNAL_IO_ERROR;

    remaining = bytes;
    while (remaining != 0)
    {
        uint32_t done = bytes - remaining;
        uint32_t amount = remaining > TRANSFER_BYTES ? TRANSFER_BYTES : remaining;
        if (!io->write(io->context, offset + HEADER_BYTES + done,
                       payload + done, amount))
            return XGBA_A2_JOURNAL_IO_ERROR;
        remaining -= amount;
    }

    if (!ReadPayloadCrc(io, offset, bytes, &actualCrc)
        || actualCrc != CrcRam(payload, bytes))
        return XGBA_A2_JOURNAL_IO_ERROR;

    memcpy(header, magic, 8);
    Write16(header + 8, XGBA_A2_RECORD_VERSION);
    Write16(header + 10, 0);
    header[12] = page;
    Write32(header + 16, best.generation + 1);
    Write32(header + 20, bytes);
    Write32(header + 24, actualCrc);
    Write32(header + 28, CrcRam(header, 28));

    if (!io->write(io->context, offset + 8, header + 8, HEADER_BYTES - 8))
        return XGBA_A2_JOURNAL_IO_ERROR;
    if (!io->write(io->context, offset, header, 8))
        return XGBA_A2_JOURNAL_IO_ERROR;

    // Verify the committed record from physical storage, not assumptions.
    struct Candidate verified;
    status = CheckCandidate(io, isMeta, page, targetSlot, bytes, &verified);
    if (status != XGBA_A2_JOURNAL_OK || !verified.valid
        || verified.generation != best.generation + 1)
        return XGBA_A2_JOURNAL_IO_ERROR;

    if (info != NULL)
    {
        info->generation = verified.generation;
        info->slot = targetSlot;
        info->validCopies = (uint8_t)(count == 2 ? 2 : 2);
    }
    return XGBA_A2_JOURNAL_OK;
}

int XgbaA2JournalReadPage(const struct XgbaA2JournalIo *io, uint8_t page,
                          void *destination, uint32_t bytes,
                          struct XgbaA2JournalInfo *info)
{
    return ReadRecord(io, 0, page, destination, bytes, info);
}

int XgbaA2JournalWritePage(const struct XgbaA2JournalIo *io, uint8_t page,
                           const void *source, uint32_t bytes,
                           struct XgbaA2JournalInfo *info)
{
    return WriteRecord(io, 0, page, source, bytes, info);
}

int XgbaA2JournalReadMeta(const struct XgbaA2JournalIo *io,
                          void *destination, uint32_t bytes,
                          struct XgbaA2JournalInfo *info)
{
    return ReadRecord(io, 1, 0, destination, bytes, info);
}

int XgbaA2JournalWriteMeta(const struct XgbaA2JournalIo *io,
                           const void *source, uint32_t bytes,
                           struct XgbaA2JournalInfo *info)
{
    return WriteRecord(io, 1, 0, source, bytes, info);
}
