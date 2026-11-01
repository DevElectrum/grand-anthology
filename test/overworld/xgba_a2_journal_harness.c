#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xgba_a2_journal.h"
#include "xgba_a2_guest_bus.h"

#define MEMORY_SIZE 0x2A0000
#define PAGE_BYTES XGBA_A2_PC_BYTES

struct FakeDisk
{
    uint8_t *bytes;
    int64_t remainingWrites;
    int failed;
    int reads;
    int writes;
};

static uint32_t Crc(const uint8_t *data, uint32_t size)
{
    uint32_t crc = 0xFFFFFFFFU;
    uint32_t i;
    for (i = 0; i < size; i++)
    {
        uint32_t b;
        crc ^= data[i];
        for (b = 0; b < 8; b++)
            crc = (crc >> 1) ^ (0xEDB88320U & (0U - (crc & 1U)));
    }
    return ~crc;
}

static void Write16(uint8_t *ptr, uint16_t value)
{
    ptr[0] = (uint8_t)value;
    ptr[1] = (uint8_t)(value >> 8);
}

static void Write32(uint8_t *ptr, uint32_t value)
{
    ptr[0] = (uint8_t)value;
    ptr[1] = (uint8_t)(value >> 8);
    ptr[2] = (uint8_t)(value >> 16);
    ptr[3] = (uint8_t)(value >> 24);
}

static int Read(void *context, uint32_t offset, uint8_t *dst, uint32_t count)
{
    struct FakeDisk *d = context;
    if ((uint64_t)offset + count > MEMORY_SIZE)
        return 0;
    memcpy(dst, d->bytes + offset, count);
    d->reads++;
    return 1;
}

static int Write(void *context, uint32_t offset, const uint8_t *src, uint32_t count)
{
    struct FakeDisk *d = context;
    uint32_t actual = count;
    if ((uint64_t)offset + count > MEMORY_SIZE)
        return 0;
    if (d->remainingWrites >= 0 && d->remainingWrites < (int64_t)count)
        actual = (uint32_t)d->remainingWrites;
    if (actual)
        memcpy(d->bytes + offset, src, actual);
    if (d->remainingWrites >= 0)
        d->remainingWrites -= actual;
    d->writes++;
    if (actual != count)
    {
        d->failed = 1;
        return 0;
    }
    return 1;
}

static uint32_t PageOffset(int page, int slot)
{
    return XGBA_A2_PC_BASE + (uint32_t)(page * 2 + slot) * XGBA_A2_PC_STRIDE;
}

static void Fixture(struct FakeDisk *d, int page, int slot, uint32_t generation,
                    const uint8_t *payload, uint32_t length)
{
    uint8_t h[32] = {0};
    uint8_t *dst = d->bytes + PageOffset(page, slot);
    static const uint8_t magic[8] = {'X','G','B','A','A','2','P',0};
    memcpy(h, magic, 8);
    Write16(h + 8, 2);
    h[12] = (uint8_t)page;
    Write32(h + 16, generation);
    Write32(h + 20, length);
    Write32(h + 24, Crc(payload, length));
    Write32(h + 28, Crc(h, 28));
    memcpy(dst, h, 32);
    memcpy(dst + 32, payload, length);
}

static void Init(struct FakeDisk *d)
{
    d->bytes = malloc(MEMORY_SIZE);
    assert(d->bytes != NULL);
    memset(d->bytes, 0xFF, MEMORY_SIZE);
    d->remainingWrites = -1;
    d->failed = 0;
    d->reads = 0;
    d->writes = 0;
}

static void Fill(uint8_t *p, uint8_t seed)
{
    uint32_t i;
    for (i = 0; i < PAGE_BYTES; i++)
        p[i] = (uint8_t)((i * 19U + seed) & 255U);
}

int main(void)
{
    struct FakeDisk disk;
    struct XgbaA2JournalIo io;
    struct XgbaA2JournalInfo result;
    uint8_t *old = malloc(PAGE_BYTES);
    uint8_t *newPage = malloc(PAGE_BYTES);
    uint8_t *readback = malloc(PAGE_BYTES);
    int64_t failPoints[] = {0, 1, 8, 16, 64, 256, 1024, 16384, 34140, 34183};
    unsigned i;

    assert(old != NULL && newPage != NULL && readback != NULL);
    Init(&disk);
    io.context = &disk;
    io.read = Read;
    io.write = Write;
    Fill(old, 11);
    Fill(newPage, 37);
    Fixture(&disk, 0, 0, 1, old, PAGE_BYTES);
    Fixture(&disk, 0, 1, 2, old, PAGE_BYTES);

    assert(XgbaA2JournalReadPage(&io, 0, readback, PAGE_BYTES, &result) == 0);
    assert(memcmp(readback, old, PAGE_BYTES) == 0);
    assert(result.generation == 2 && result.slot == 1);

    // A clean page is never rewritten just because a save was requested.
    disk.writes = 0;
    assert(XgbaA2JournalWritePage(&io, 0, old, PAGE_BYTES, &result) == XGBA_A2_JOURNAL_UNCHANGED);
    assert(disk.writes == 0);

    // A successful update commits to the older copy and reads back identically.
    assert(XgbaA2JournalWritePage(&io, 0, newPage, PAGE_BYTES, &result) == 0);
    assert(result.generation == 3 && result.slot == 0);
    assert(XgbaA2JournalReadPage(&io, 0, readback, PAGE_BYTES, &result) == 0);
    assert(memcmp(readback, newPage, PAGE_BYTES) == 0);

    // Lose the latest generation's payload: older generation wins.
    disk.bytes[PageOffset(0, 0) + 32 + 101] ^= 0xFF;
    assert(XgbaA2JournalReadPage(&io, 0, readback, PAGE_BYTES, &result) == 0);
    assert(result.generation == 2 && result.slot == 1);
    assert(memcmp(readback, old, PAGE_BYTES) == 0);

    // Both copies damaged: never manufacture empty Pokémon boxes.
    disk.bytes[PageOffset(0, 1) + 32 + 101] ^= 0xFF;
    assert(XgbaA2JournalReadPage(&io, 0, readback, PAGE_BYTES, &result) == XGBA_A2_JOURNAL_MISSING);
    assert(XgbaA2JournalWritePage(&io, 0, newPage, PAGE_BYTES, &result) == XGBA_A2_JOURNAL_MISSING);

    // Interrupted writes at distinct payload and commit boundaries must
    // preserve the previous valid page and never expose uncommitted data.
    for (i = 0; i < sizeof(failPoints) / sizeof(failPoints[0]); i++)
    {
        memset(disk.bytes, 0xFF, MEMORY_SIZE);
        Fixture(&disk, 3, 0, 11, old, PAGE_BYTES);
        Fixture(&disk, 3, 1, 12, old, PAGE_BYTES);
        disk.remainingWrites = failPoints[i];
        assert(XgbaA2JournalWritePage(&io, 3, newPage, PAGE_BYTES, &result)
               == XGBA_A2_JOURNAL_IO_ERROR);
        disk.remainingWrites = -1;
        assert(XgbaA2JournalReadPage(&io, 3, readback, PAGE_BYTES, &result) == 0);
        assert(result.generation == 12);
        assert(memcmp(readback, old, PAGE_BYTES) == 0);
    }

    // A page with no initialized records is never silently created.
    assert(XgbaA2JournalReadPage(&io, 7, readback, PAGE_BYTES, &result) == XGBA_A2_JOURNAL_MISSING);
    assert(XgbaA2JournalReadPage(&io, 8, readback, PAGE_BYTES, &result) == XGBA_A2_JOURNAL_INVALID_ARG);

    // The metadata journal has the exact same A/B safety contract.
    {
        uint8_t h[32] = {0};
        uint8_t meta[56] = {0};
        uint8_t updated[56] = {0};
        uint8_t output[56] = {0};
        uint8_t *p = disk.bytes + XGBA_A2_PC_META_BASE;
        const uint8_t magic[8] = {'X','G','B','A','A','2','M',0};
        memcpy(h, magic, 8);
        Write16(h + 8, 2);
        Write32(h + 16, 2);
        Write32(h + 20, 56);
        meta[20] = 0;
        Write32(h + 24, Crc(meta, 56));
        Write32(h + 28, Crc(h, 28));
        memcpy(p, h, 32);
        memcpy(p + 32, meta, 24);
        updated[20] = 3;
        assert(XgbaA2JournalWriteMeta(&io, updated, 56, &result) == 0);
        assert(XgbaA2JournalReadMeta(&io, output, 56, &result) == 0);
        assert(memcmp(output, updated, 56) == 0);
    }

    // The 0x0F adapter must not mistake native Flash offsets for extended
    // storage, and must reject an absent E6 superblock without writes.
    {
        struct XgbaA2GuestBus guest;
        struct XgbaA2JournalIo guestIo;
        uint8_t signature[24] = {
            'X','G','B','A','E','6','S',0,
            2,0,0,0,0,0,2,0,0,0,0,1,
            0xB0,0x2B,0x5B,0x12
        };
        uint8_t testByte = 0;
        uint8_t value = 0x4B;
        disk.bytes[XGBA_A2_NATIVE_BYTES] = 0xFF;
        assert(!XgbaA2GuestBusBind(&guest, disk.bytes + XGBA_A2_NATIVE_BYTES));
        XgbaA2GuestBusCreateIo(&guest, &guestIo);
        assert(!guestIo.write(guestIo.context, XGBA_A2_PC_BASE, &value, 1));
        memcpy(disk.bytes + XGBA_A2_NATIVE_BYTES, signature, sizeof(signature));
        assert(XgbaA2GuestBusBind(&guest, disk.bytes + XGBA_A2_NATIVE_BYTES));
        assert(!guestIo.read(guestIo.context, XGBA_A2_NATIVE_BYTES - 1, &testByte, 1));
        assert(!guestIo.read(guestIo.context, XGBA_A2_SAVE_BYTES, &testByte, 1));
        assert(guestIo.write(guestIo.context, XGBA_A2_PC_BASE + 17, &value, 1));
        assert(guestIo.read(guestIo.context, XGBA_A2_PC_BASE + 17, &testByte, 1));
        assert(testByte == value);
        assert(disk.bytes[XGBA_A2_PC_BASE + 17] == value);
    }

    printf("A2 C journal: A/B writes, no-op, corruption recovery, page bounds, metadata,");
    printf(" and %u interrupted-write boundaries PASS\n",
           (unsigned)(sizeof(failPoints) / sizeof(failPoints[0])));
    free(disk.bytes);
    free(old);
    free(newPage);
    free(readback);
    return 0;
}
