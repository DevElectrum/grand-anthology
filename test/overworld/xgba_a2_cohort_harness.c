#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xgba_a2_cohort.h"

#define DISK_BYTES 0x2A0000U
#define SECTOR_BYTES 4096U
#define DATA_BYTES 3968U
#define FOOTER 4084U

static uint8_t *sDisk;

static void U16(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
}

static void U32(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

static uint32_t Word(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8)
        | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static uint32_t Crc(const uint8_t *p, uint32_t n)
{
    uint32_t crc = 0xFFFFFFFFU;
    uint32_t i;
    for (i = 0; i < n; i++)
    {
        uint32_t j;
        crc ^= p[i];
        for (j = 0; j < 8; j++)
            crc = (crc >> 1) ^ (0xEDB88320U & (0U - (crc & 1U)));
    }
    return ~crc;
}

static uint16_t LegacyChecksum(const uint8_t *p, uint32_t size)
{
    uint32_t i, sum = 0;
    for (i = 0; i + 4 <= size; i += 4)
        sum += Word(p + i);
    return (uint16_t)(sum + (sum >> 16));
}

static int IoRead(void *context, uint32_t offset, uint8_t *dst, uint32_t length)
{
    (void)context;
    if ((uint64_t)offset + length > DISK_BYTES)
        return 0;
    memcpy(dst, sDisk + offset, length);
    return 1;
}

static int IoWrite(void *context, uint32_t offset, const uint8_t *src, uint32_t length)
{
    (void)context;
    if ((uint64_t)offset + length > DISK_BYTES)
        return 0;
    memcpy(sDisk + offset, src, length);
    return 1;
}

static int NativeRead(void *context, uint8_t physical, uint8_t *buffer)
{
    (void)context;
    if (physical >= 28)
        return 0;
    memcpy(buffer, sDisk + (uint32_t)physical * SECTOR_BYTES, SECTOR_BYTES);
    return 1;
}

static uint32_t Length(uint32_t logical)
{
    uint32_t remainder;
    if (logical == 0)
        return 3884;
    if (logical <= 4)
        remainder = 14752U - (logical - 1U) * DATA_BYTES;
    else
        remainder = XGBA_A2_PC_BYTES - (logical - 5U) * DATA_BYTES;
    return remainder < DATA_BYTES ? remainder : DATA_BYTES;
}

static void BuildNative(uint8_t slot, uint32_t counter, const uint8_t *pc)
{
    uint32_t id;
    for (id = 0; id < 14; id++)
    {
        uint32_t size = Length(id);
        uint8_t *physical = sDisk + (slot * 14U + id) * SECTOR_BYTES;
        memset(physical, 0, SECTOR_BYTES);
        if (id >= 5)
            memcpy(physical, pc + (id - 5U) * DATA_BYTES, size);
        U16(physical + FOOTER, (uint16_t)id);
        U16(physical + FOOTER + 2, LegacyChecksum(physical, size));
        U32(physical + FOOTER + 4, 0x08012025U);
        U32(physical + FOOTER + 8, counter);
    }
}

static void MakeRecord(uint8_t *at, const uint8_t magic[8],
                       uint8_t page, uint32_t generation,
                       const uint8_t *payload, uint32_t bytes)
{
    uint8_t header[32] = {0};
    memcpy(header, magic, 8);
    U16(header + 8, 2);
    header[12] = page;
    U32(header + 16, generation);
    U32(header + 20, bytes);
    U32(header + 24, Crc(payload, bytes));
    U32(header + 28, Crc(header, 28));
    memcpy(at, header, 32);
    memcpy(at + 32, payload, bytes);
}

static void MakePage(uint8_t page, uint8_t copy, uint32_t generation,
                     const uint8_t *payload)
{
    static const uint8_t magic[8] = {'X','G','B','A','A','2','P',0};
    uint32_t offset = XGBA_A2_PC_BASE
        + ((uint32_t)page * 2 + copy) * XGBA_A2_PC_STRIDE;
    MakeRecord(sDisk + offset, magic, page, generation,
               payload, XGBA_A2_PC_BYTES);
}

static void MakeMeta(uint8_t copy, uint32_t generation,
                     uint8_t nativeSlot, uint32_t counter,
                     uint32_t pageZeroGeneration)
{
    static const uint8_t magic[8] = {'X','G','B','A','A','2','M',0};
    uint8_t scratch[SECTOR_BYTES];
    uint8_t payload[56] = {0};
    uint8_t hash[16];
    uint32_t i;
    assert(XgbaA2FingerprintNativeSlot(NativeRead, NULL, nativeSlot,
                                        scratch, hash));
    memcpy(payload, hash, 16);
    U32(payload + 16, counter);
    payload[20] = 0;
    payload[21] = nativeSlot;
    for (i = 0; i < XGBA_A2_PC_PAGES; i++)
        U32(payload + 24 + i * 4, i == 0 ? pageZeroGeneration : 2);
    MakeRecord(sDisk + XGBA_A2_PC_META_BASE
               + copy * XGBA_A2_PC_META_STRIDE, magic, 0,
               generation, payload, 56);
}

int main(void)
{
    struct XgbaA2JournalIo io;
    struct XgbaA2Cohort selected;
    uint8_t *old = malloc(XGBA_A2_PC_BYTES);
    uint8_t *next = malloc(XGBA_A2_PC_BYTES);
    uint8_t scratch[SECTOR_BYTES];
    uint32_t page, i;

    sDisk = malloc(DISK_BYTES);
    assert(sDisk != NULL && old != NULL && next != NULL);
    memset(sDisk, 0xFF, DISK_BYTES);
    io.context = NULL;
    io.read = IoRead;
    io.write = IoWrite;

    for (i = 0; i < XGBA_A2_PC_BYTES; i++)
        old[i] = (uint8_t)((i * 13U + 7U) & 255U);
    memcpy(next, old, XGBA_A2_PC_BYTES);
    next[4] ^= 0x5A;
    next[25000] ^= 0x22;

    BuildNative(0, 2, old);
    for (page = 0; page < XGBA_A2_PC_PAGES; page++)
    {
        const uint8_t *payload = page == 0 ? old : next;
        MakePage((uint8_t)page, 0, 1, payload);
        MakePage((uint8_t)page, 1, 2, payload);
    }
    MakeMeta(0, 1, 0, 2, 1);
    MakeMeta(1, 2, 0, 2, 2);

    assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
    assert(selected.metadataGeneration == 2 && selected.nativeSlot == 0);
    assert(selected.nativeCounter == 2 && selected.pages[0] == 2);

    // A newer complete native slot with no matching metadata isn't committed.
    BuildNative(1, 3, next);
    assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
    assert(selected.nativeSlot == 0 && selected.nativeCounter == 2);

    MakePage(0, 0, 3, next);
    assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
    assert(selected.nativeSlot == 0 && selected.pages[0] == 2);

    // Metadata is the only global commit boundary.
    MakeMeta(0, 3, 1, 3, 3);
    assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
    assert(selected.metadataGeneration == 3 && selected.nativeSlot == 1);
    assert(selected.pages[0] == 3 && selected.nativeCounter == 3);

    // Corrupt latest page -> consistent rollback to old native slot.
    sDisk[XGBA_A2_PC_BASE + 32 + 11] ^= 0x80;
    assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
    assert(selected.metadataGeneration == 2 && selected.nativeSlot == 0);
    assert(selected.recovered);

    // Also corrupt the old native Flash: never silently load mixed progress.
    sDisk[5 * SECTOR_BYTES + 100] ^= 0x40;
    assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected)
           == XGBA_A2_JOURNAL_MISSING);

    puts("A2 cohort: native sector/checksum/SHA, page generations, global commit, rollback and hard refusal PASS");
    free(next);
    free(old);
    free(sDisk);
    return 0;
}
