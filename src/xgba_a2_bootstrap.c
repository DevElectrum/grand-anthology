#include "xgba_a2_bootstrap.h"

#include <stddef.h>
#include <string.h>

#define TRANSFER_BYTES 128U
#define PC_BOX_START 4U
#define PC_BOX_END 0x8344U
#define PC_FUSIONS_START 0x83D0U
#define NATIVE_ERASE_SECTORS 32U

static const uint8_t sSuperblock[24] =
{
    'X','G','B','A','E','6','S',0,
    2,0,0,0,0,0,2,0,0,0,0,1,
    0xB0,0x2B,0x5B,0x12
};
static const uint8_t sPageMagic[8] = {'X','G','B','A','A','2','P',0};
static const uint8_t sMetaMagic[8] = {'X','G','B','A','A','2','M',0};

static void Put16(uint8_t *p, uint16_t value)
{
    p[0] = (uint8_t)value;
    p[1] = (uint8_t)(value >> 8);
}

static void Put32(uint8_t *p, uint32_t value)
{
    p[0] = (uint8_t)value;
    p[1] = (uint8_t)(value >> 8);
    p[2] = (uint8_t)(value >> 16);
    p[3] = (uint8_t)(value >> 24);
}

static uint32_t Crc(const uint8_t *p, uint32_t length)
{
    return ~XgbaA2JournalCrcExtend(0xFFFFFFFFU, p, length);
}

static uint32_t RecordOffset(uint8_t meta, uint8_t page, uint8_t slot)
{
    if (meta)
        return XGBA_A2_PC_META_BASE + (uint32_t)slot * XGBA_A2_PC_META_STRIDE;
    return XGBA_A2_PC_BASE
        + ((uint32_t)page * 2U + slot) * XGBA_A2_PC_STRIDE;
}

static void ComposeBlock(uint8_t *out, uint32_t start, uint32_t n,
                         uint8_t meta, uint8_t page, const uint8_t *payload)
{
    uint32_t i;
    for (i = 0; i < n; i++)
    {
        uint32_t pos = start + i;
        if (meta || page == 0)
            out[i] = payload[pos];
        else if (pos == 0
                 || (pos >= PC_BOX_START && pos < PC_BOX_END)
                 || pos >= PC_FUSIONS_START)
            out[i] = 0;
        else
            out[i] = payload[pos];
    }
}

static int AllErased(const struct XgbaA2JournalIo *io,
                     uint32_t offset, uint32_t length)
{
    uint8_t data[TRANSFER_BYTES];
    while (length != 0)
    {
        uint32_t n = length > sizeof(data) ? sizeof(data) : length;
        uint32_t i;
        if (!io->read(io->context, offset, data, n))
            return XGBA_A2_JOURNAL_IO_ERROR;
        for (i = 0; i < n; i++)
        {
            if (data[i] != 0xFF)
                return XGBA_A2_JOURNAL_MISSING;
        }
        offset += n;
        length -= n;
    }
    return XGBA_A2_JOURNAL_OK;
}

static int Pristine(const struct XgbaA2JournalIo *io,
                    XgbaA2ReadNativeSector reader, void *context,
                    uint8_t *scratch)
{
    uint8_t signature[24];
    uint32_t i, j;
    if (!io->read(io->context, XGBA_A2_NATIVE_BYTES,
                  signature, sizeof(signature))
        || !io->read(io->context, 0xFFF000, signature, sizeof(signature)))
        return XGBA_A2_JOURNAL_IO_ERROR;
    if (memcmp(signature, sSuperblock, sizeof(signature)) != 0)
        return XGBA_A2_JOURNAL_MISSING;
    if (!io->read(io->context, XGBA_A2_NATIVE_BYTES,
                  signature, sizeof(signature)))
        return XGBA_A2_JOURNAL_IO_ERROR;
    if (memcmp(signature, sSuperblock, sizeof(signature)) != 0)
        return XGBA_A2_JOURNAL_MISSING;

    for (i = 0; i < NATIVE_ERASE_SECTORS; i++)
    {
        if (!reader(context, (uint8_t)i, scratch))
            return XGBA_A2_JOURNAL_IO_ERROR;
        for (j = 0; j < 4096; j++)
        {
            if (scratch[j] != 0xFF)
                return XGBA_A2_JOURNAL_MISSING;
        }
    }
    return AllErased(io, XGBA_A2_PC_BASE,
                     XGBA_A2_PC_META_BASE + 2 * XGBA_A2_PC_META_STRIDE
                     - XGBA_A2_PC_BASE);
}

static int SeedRecord(const struct XgbaA2JournalIo *io,
                      uint8_t meta, uint8_t page, uint8_t slot,
                      uint32_t generation, const uint8_t *payload)
{
    uint8_t header[32] = {0};
    uint8_t data[TRANSFER_BYTES];
    uint8_t check[TRANSFER_BYTES];
    uint32_t length = meta ? XGBA_A2_META_PAYLOAD_BYTES : XGBA_A2_PC_BYTES;
    uint32_t base = RecordOffset(meta, page, slot);
    uint32_t offset;
    uint32_t crc = 0xFFFFFFFFU;
    uint32_t verifyCrc = 0xFFFFFFFFU;
    const uint8_t *magic = meta ? sMetaMagic : sPageMagic;

    // Only called after a pristine full-image check. Re-running initialization
    // after any partial write is forbidden by the caller's pristine guard.
    for (offset = 0; offset < length; offset += TRANSFER_BYTES)
    {
        uint32_t count = length - offset;
        if (count > TRANSFER_BYTES)
            count = TRANSFER_BYTES;
        ComposeBlock(data, offset, count, meta, page, payload);
        if (!io->write(io->context, base + 32 + offset, data, count))
            return XGBA_A2_JOURNAL_IO_ERROR;
        crc = XgbaA2JournalCrcExtend(crc, data, count);
    }

    for (offset = 0; offset < length; offset += TRANSFER_BYTES)
    {
        uint32_t count = length - offset;
        if (count > TRANSFER_BYTES)
            count = TRANSFER_BYTES;
        if (!io->read(io->context, base + 32 + offset, check, count))
            return XGBA_A2_JOURNAL_IO_ERROR;
        ComposeBlock(data, offset, count, meta, page, payload);
        if (memcmp(data, check, count) != 0)
            return XGBA_A2_JOURNAL_IO_ERROR;
        verifyCrc = XgbaA2JournalCrcExtend(verifyCrc, check, count);
    }
    if (verifyCrc != crc)
        return XGBA_A2_JOURNAL_IO_ERROR;

    memcpy(header, magic, 8);
    Put16(header + 8, XGBA_A2_RECORD_VERSION);
    header[12] = page;
    Put32(header + 16, generation);
    Put32(header + 20, length);
    Put32(header + 24, ~crc);
    Put32(header + 28, Crc(header, 28));

    // One-byte LAST activation marker. No metadata can be exposed until
    // all eight pages AND a complete native slot have been verified.
    if (!io->write(io->context, base + 8, header + 8, 24)
        || !io->write(io->context, base + 1, header + 1, 7)
        || !io->write(io->context, base, header, 1))
        return XGBA_A2_JOURNAL_IO_ERROR;

    if (meta)
    {
        uint8_t readback[XGBA_A2_META_PAYLOAD_BYTES];
        struct XgbaA2JournalInfo found;
        if (XgbaA2JournalReadMetaSlot(io, slot, readback, &found)
            != XGBA_A2_JOURNAL_OK
            || found.generation != generation
            || memcmp(readback, payload, sizeof(readback)) != 0)
            return XGBA_A2_JOURNAL_IO_ERROR;
    }
    else
    {
        struct XgbaA2JournalInfo found;
        if (XgbaA2JournalFindPageGeneration(io, page, generation, &found)
            != XGBA_A2_JOURNAL_OK || found.slot != slot)
            return XGBA_A2_JOURNAL_IO_ERROR;
    }
    return XGBA_A2_JOURNAL_OK;
}

int XgbaA2BootstrapCommit(const struct XgbaA2JournalIo *io,
                          XgbaA2ReadNativeSector reader,
                          XgbaA2WriteNativeSlot writer, void *nativeContext,
                          uint8_t *scratch4096, const void *residentPC,
                          struct XgbaA2Cohort *confirmed)
{
    uint8_t payload[XGBA_A2_META_PAYLOAD_BYTES] = {0};
    struct XgbaA2Cohort result;
    uint32_t page;
    int status;

    if (io == NULL || io->read == NULL || io->write == NULL
        || reader == NULL || writer == NULL || scratch4096 == NULL
        || residentPC == NULL || confirmed == NULL)
        return XGBA_A2_JOURNAL_INVALID_ARG;

    status = Pristine(io, reader, nativeContext, scratch4096);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;

    for (page = 0; page < XGBA_A2_PC_PAGES; page++)
    {
        status = SeedRecord(io, 0, (uint8_t)page, 0, 1, residentPC);
        if (status != XGBA_A2_JOURNAL_OK)
            return status;
        status = SeedRecord(io, 0, (uint8_t)page, 1, 2, residentPC);
        if (status != XGBA_A2_JOURNAL_OK)
            return status;
    }

    // The first native save uses counter 1, physical Flash slot 1.
    if (!writer(nativeContext, 1, 1, residentPC, XGBA_A2_PC_BYTES)
        || !XgbaA2CohortVerifyNativeSlot(reader, nativeContext,
                                          scratch4096, 1, 1)
        || !XgbaA2FingerprintNativeSlot(reader, nativeContext, 1,
                                        scratch4096, payload))
        return XGBA_A2_JOURNAL_IO_ERROR;

    Put32(payload + 16, 1);
    payload[20] = 0;
    payload[21] = 1;
    for (page = 0; page < XGBA_A2_PC_PAGES; page++)
        Put32(payload + 24 + page * 4, 2);

    // First and ONLY global publication boundary for a new save.
    status = SeedRecord(io, 1, 0, 0, 1, payload);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;
    status = XgbaA2CohortSelect(io, reader, nativeContext,
                                scratch4096, &result);
    if (status != XGBA_A2_JOURNAL_OK
        || result.nativeCounter != 1 || result.nativeSlot != 1
        || result.metadataGeneration != 1 || result.activePage != 0)
        return XGBA_A2_JOURNAL_IO_ERROR;
    *confirmed = result;
    return XGBA_A2_JOURNAL_OK;
}
