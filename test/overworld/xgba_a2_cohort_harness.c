#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xgba_a2_cohort.h"
#include "xgba_a2_transaction.h"
#include "xgba_a2_bootstrap.h"

#define DISK_BYTES 0x1000000U
#define SECTOR_BYTES 4096U
#define DATA_BYTES 3968U
#define FOOTER 4084U

static uint8_t *sDisk;
static uint32_t sWriterSectors = 14;
static uint32_t sExtensionBytesRead = 0;
static int64_t sWriteBudget = -1;

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
    sExtensionBytesRead += length;
    return 1;
}

static int IoWrite(void *context, uint32_t offset, const uint8_t *src, uint32_t length)
{
    (void)context;
    if ((uint64_t)offset + length > DISK_BYTES)
        return 0;
    if (sWriteBudget >= 0)
    {
        uint32_t count = (uint32_t)(sWriteBudget < length ? sWriteBudget : length);
        if (count)
            memcpy(sDisk + offset, src, count);
        sWriteBudget -= count;
        return count == length;
    }
    memcpy(sDisk + offset, src, length);
    return 1;
}

static int NativeRead(void *context, uint8_t physical, uint8_t *buffer)
{
    (void)context;
    if (physical >= 32)
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
    for (id = 0; id < sWriterSectors; id++)
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

static int NativeWriter(void *context, uint8_t slot, uint32_t counter,
                        const void *pc, uint32_t bytes)
{
    (void)context;
    if (slot > 1 || bytes != XGBA_A2_PC_BYTES)
        return 0;
    BuildNative(slot, counter, pc);
    return sWriterSectors == 14;
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

static void BlankSeed(void)
{
    static const uint8_t superblock[24] =
    {
        'X','G','B','A','E','6','S',0,
        2,0,0,0,0,0,2,0,0,0,0,1,
        0xB0,0x2B,0x5B,0x12
    };
    memset(sDisk, 0xFF, DISK_BYTES);
    memcpy(sDisk + 0x20000, superblock, sizeof(superblock));
    memcpy(sDisk + 0xFFF000, superblock, sizeof(superblock));
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

    // New-game protocol cannot run unless BOTH native slots and all reserved
    // extension records are genuinely blank. Test bad legacy data first.
    BlankSeed();
    sDisk[5 * SECTOR_BYTES + 42] = 0;
    assert(XgbaA2BootstrapCommit(&io, NativeRead, NativeWriter, NULL,
                                  scratch, old, &selected)
           == XGBA_A2_JOURNAL_MISSING);
    BlankSeed();
    sDisk[XGBA_A2_PC_BASE + 140] = 0;
    assert(XgbaA2BootstrapCommit(&io, NativeRead, NativeWriter, NULL,
                                  scratch, old, &selected)
           == XGBA_A2_JOURNAL_MISSING);

    // A failed initial native write must NOT turn staged pages into a save.
    BlankSeed();
    sWriterSectors = 7;
    assert(XgbaA2BootstrapCommit(&io, NativeRead, NativeWriter, NULL,
                                  scratch, old, &selected)
           == XGBA_A2_JOURNAL_IO_ERROR);
    assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected)
           == XGBA_A2_JOURNAL_MISSING);
    assert(XgbaA2BootstrapCommit(&io, NativeRead, NativeWriter, NULL,
                                  scratch, old, &selected)
           == XGBA_A2_JOURNAL_MISSING);

    // Crash at each stage. There is only either a complete cohort or NO
    // selectable cohort; never a half-initialized global Pokemon collection.
    sWriterSectors = 14;
    {
        static const int64_t budgets[] =
        {
            0, 1, 7, 31, 32, 1024, 32768, 100000,
            250000, 500000, 550000, 552000, 557000,
            559000, 600000
        };
        uint32_t trial;
        for (trial = 0; trial < sizeof(budgets) / sizeof(budgets[0]); trial++)
        {
            int result, recovered;
            BlankSeed();
            sWriteBudget = budgets[trial];
            result = XgbaA2BootstrapCommit(&io, NativeRead, NativeWriter,
                                            NULL, scratch, old, &selected);
            sWriteBudget = -1;
            recovered = XgbaA2CohortSelect(&io, NativeRead, NULL,
                                            scratch, &selected);
            if (recovered == XGBA_A2_JOURNAL_OK)
            {
                assert(selected.nativeSlot == 1 && selected.nativeCounter == 1);
                assert(selected.metadataGeneration == 1 && selected.activePage == 0);
                assert(result == XGBA_A2_JOURNAL_OK
                       || result == XGBA_A2_JOURNAL_IO_ERROR);
            }
            else
            {
                assert(recovered == XGBA_A2_JOURNAL_MISSING);
                assert(result != XGBA_A2_JOURNAL_OK);
                // Zero bytes written leaves the pristine disk available
                // for a fresh explicitly authorized game. Any actual partial
                // write makes the disk non-pristine and blocks re-bootstrap.
                if (budgets[trial] > 0)
                    assert(XgbaA2BootstrapCommit(&io, NativeRead, NativeWriter,
                                                  NULL, scratch, old, &selected)
                           == XGBA_A2_JOURNAL_MISSING);
            }
        }
    }

    BlankSeed();
    assert(XgbaA2BootstrapCommit(&io, NativeRead, NativeWriter, NULL,
                                  scratch, old, &selected) == 0);
    assert(selected.nativeCounter == 1 && selected.nativeSlot == 1);
    assert(selected.metadataGeneration == 1);
    for (page = 0; page < XGBA_A2_PC_PAGES; page++)
        assert(selected.pages[page] == 2);
    assert(memcmp(sDisk + XGBA_A2_PC_BASE + XGBA_A2_PC_STRIDE + 32,
                  old, XGBA_A2_PC_BYTES) == 0);
    {
        uint32_t empty = XGBA_A2_PC_BASE + 3 * XGBA_A2_PC_STRIDE + 32;
        assert(sDisk[empty] == 0);
        assert(sDisk[empty + 4] == 0);
        assert(sDisk[empty + 0x8344] == old[0x8344]);
        assert(sDisk[empty + 0x83D0] == 0);
    }
    assert(XgbaA2BootstrapCommit(&io, NativeRead, NativeWriter, NULL,
                                  scratch, old, &selected)
           == XGBA_A2_JOURNAL_MISSING);

    // Hall of Fame history occupies native Flash sectors 28/29, outside the
    // fourteen-sector alternating game saves. Its independent archive write
    // (or a crash partway through it) cannot invalidate the committed PC.
    memset(sDisk + 28 * SECTOR_BYTES, 0xA7, SECTOR_BYTES);
    memset(sDisk + 29 * SECTOR_BYTES, 0x3C, SECTOR_BYTES / 2);
    assert(XgbaA2CohortSelect(&io, NativeRead, NULL,
                              scratch, &selected) == XGBA_A2_JOURNAL_OK);
    assert(selected.nativeCounter == 1 && selected.nativeSlot == 1);
    assert(selected.activePage == 0 && selected.metadataGeneration == 1);
    puts("A2 fresh E6 bootstrap, HOF independence and crash budget matrix PASS");

    // Reset for existing independent multi-generation transaction tests.
    memset(sDisk, 0xFF, DISK_BYTES);
    BuildNative(0, 2, old);
    for (page = 0; page < XGBA_A2_PC_PAGES; page++)
    {
        const uint8_t *payload = page == 0 ? old : next;
        MakePage((uint8_t)page, 0, 1, payload);
        MakePage((uint8_t)page, 1, 2, payload);
    }
    MakeMeta(0, 1, 0, 2, 1);
    MakeMeta(1, 2, 0, 2, 2);

    sExtensionBytesRead = 0;
    assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
    // A clean boot must not hash both 8-page cohorts. The metadata-pinned
    // generation selector scans one 34 KiB copy per page, not both.
    assert(sExtensionBytesRead < 400000);
    assert(selected.metadataGeneration == 2 && selected.nativeSlot == 0);
    assert(selected.nativeCounter == 2 && selected.pages[0] == 2);

    // Simulate the exact boot-order hazard: the alternate native Flash slot
    // has a completely valid, NEWER save counter and a different trainer-id
    // byte, but the A2 metadata was never committed. Boot relocation and the
    // main loader MUST both keep using the metadata-pinned older native slot.
    BuildNative(1, 3, next);
    {
        uint8_t *newTrainer = sDisk + 14U * SECTOR_BYTES;
        newTrainer[0] = 0xD7;
        newTrainer[1] = 0x31;
        U16(newTrainer + FOOTER + 2, LegacyChecksum(newTrainer, 3884));
    }
    assert(XgbaA2CohortVerifyNativeSlot(NativeRead, NULL, scratch, 1, 3));
    assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected)
           == XGBA_A2_JOURNAL_OK);
    assert(selected.nativeSlot == 0 && selected.nativeCounter == 2);
    assert(selected.metadataGeneration == 2 && !selected.recovered);
    assert(sDisk[14U * SECTOR_BYTES] != sDisk[0]);

    // Start from the proven coherent v2 collection, then simulate a real
    // game save with a direct BoxPokemon edit and an interrupted native slot.
    {
        struct XgbaA2Pager pager;
        struct XgbaA2Cohort committed = selected;
        struct XgbaA2Cohort result;
        assert(XgbaA2PagerInit(&pager, &io, old, 0, selected.pages) == 0);
        memcpy(old, next, XGBA_A2_PC_BYTES);

        sWriterSectors = 7;
        assert(XgbaA2TransactionCommit(&pager, &committed, NativeRead,
                                       NativeWriter, NULL, scratch, &result)
               == XGBA_A2_JOURNAL_IO_ERROR);
        // Corrupt/partial new native Flash cannot become a successful save.
        assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
        assert(selected.metadataGeneration == 2 && selected.nativeSlot == 0);
        assert(pager.committed[0] == 2 && pager.staged[0] == 3);

        sWriterSectors = 14;
        assert(XgbaA2TransactionCommit(&pager, &committed, NativeRead,
                                       NativeWriter, NULL, scratch, &result) == 0);
        assert(result.metadataGeneration == 3 && result.nativeSlot == 1);
        assert(result.nativeCounter == 3 && result.pages[0] == 3);
        assert(pager.committed[0] == 3 && pager.staged[0] == 0);
        assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
        assert(selected.metadataGeneration == 3 && selected.nativeSlot == 1);

        // Make a second committed save after switching to another PC page.
        // The alternate native slot is recycled; the previous GLOBAL
        // metadata commit must still point to an intact native+PC pair.
        assert(XgbaA2PagerSwitch(&pager, 1) == XGBA_A2_JOURNAL_OK);
        old[47] ^= 0x39;
        committed = result;
        assert(XgbaA2TransactionCommit(&pager, &committed, NativeRead,
                                       NativeWriter, NULL, scratch, &result) == 0);
        assert(result.metadataGeneration == 4 && result.nativeSlot == 0);
        assert(result.nativeCounter == 4 && result.activePage == 1);
        assert(result.pages[1] == 3);
        assert(result.pages[0] == 3);
        assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
        assert(selected.metadataGeneration == 4 && selected.activePage == 1);
    }

    // Latest active page is page 1; corrupt its new generation, not the
    // prior save's still-committed page-0 record.
    sDisk[XGBA_A2_PC_BASE + 2 * XGBA_A2_PC_STRIDE + 32 + 11] ^= 0x80;
    assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
    assert(selected.metadataGeneration == 3 && selected.nativeSlot == 1);
    assert(selected.nativeCounter == 3 && selected.activePage == 0);
    assert(selected.recovered);

    // Also corrupt the previously committed native slot: fail closed.
    sDisk[(14 + 5) * SECTOR_BYTES + 100] ^= 0x40;
    assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected)
           == XGBA_A2_JOURNAL_MISSING);

    // Independent 112-box far-page save/restore scenario. Reinitialize the
    // disposable disk, then stage a change on page 7 while page 0 stays
    // committed. A true page switch must preserve that staged data; only the
    // subsequent native+metadata transaction may publish it globally.
    {
        struct XgbaA2Pager pager, rebooted;
        struct XgbaA2Cohort first, saved, reloaded;
        uint32_t j;

        for (j = 0; j < XGBA_A2_PC_BYTES; j++)
            old[j] = (uint8_t)((j * 7U + 3U) & 255U);
        BlankSeed();
        sWriterSectors = 14;
        assert(XgbaA2BootstrapCommit(&io, NativeRead, NativeWriter, NULL,
                                      scratch, old, &first) == XGBA_A2_JOURNAL_OK);
        assert(XgbaA2PagerInit(&pager, &io, old, 0, first.pages)
               == XGBA_A2_JOURNAL_OK);
        assert(XgbaA2PagerSwitch(&pager, 7) == XGBA_A2_JOURNAL_OK);
        old[47] = 0xA5;
        assert(XgbaA2PagerSwitch(&pager, 0) == XGBA_A2_JOURNAL_OK);
        assert(pager.staged[7] == 3 && pager.committed[7] == 2);
        assert(XgbaA2PagerSwitch(&pager, 7) == XGBA_A2_JOURNAL_OK);
        assert(old[47] == 0xA5);
        assert(XgbaA2TransactionCommit(&pager, &first, NativeRead,
                                        NativeWriter, NULL, scratch, &saved)
               == XGBA_A2_JOURNAL_OK);
        assert(saved.nativeSlot == 0 && saved.nativeCounter == 2);
        assert(saved.metadataGeneration == 2 && saved.activePage == 7);
        assert(saved.pages[0] == 2 && saved.pages[7] == 3);
        assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &reloaded)
               == XGBA_A2_JOURNAL_OK);
        assert(reloaded.nativeCounter == 2 && reloaded.activePage == 7);
        assert(reloaded.pages[0] == 2 && reloaded.pages[7] == 3);

        // Simulate cold reloading the metadata-pinned far page into the PC.
        memset(old, 0, XGBA_A2_PC_BYTES);
        assert(XgbaA2JournalReadPageGeneration(&io, 7, reloaded.pages[7],
                                               old, XGBA_A2_PC_BYTES, NULL)
               == XGBA_A2_JOURNAL_OK);
        assert(old[47] == 0xA5);
        assert(XgbaA2PagerInit(&rebooted, &io, old, 7, reloaded.pages)
               == XGBA_A2_JOURNAL_OK);
        assert(XgbaA2PagerSwitch(&rebooted, 0) == XGBA_A2_JOURNAL_OK);
        assert(XgbaA2PagerSwitch(&rebooted, 7) == XGBA_A2_JOURNAL_OK);
        assert(old[47] == 0xA5);
    }

    // R8 RAM: mutate two distant groups and complete two native+E6 transactions.
    // This tests metadata-selected cold rehydration, not just staging.
    {
        struct XgbaA2Pager live, coldPager, secondCold;
        struct XgbaA2Cohort initial, saved, savedAgain, selected;
        uint8_t *ram = malloc(XGBA_A2_PC_PAGES * XGBA_A2_PC_BYTES);
        uint8_t *cold = malloc(XGBA_A2_PC_PAGES * XGBA_A2_PC_BYTES);
        uint8_t *cold2 = malloc(XGBA_A2_PC_PAGES * XGBA_A2_PC_BYTES);
        uint32_t i;
        assert(ram != NULL && cold != NULL && cold2 != NULL);
        for (i = 0; i < XGBA_A2_PC_BYTES; i++)
            old[i] = (uint8_t)((i * 7U + 3U) & 255U);
        BlankSeed();
        sWriterSectors = 14;
        sWriteBudget = -1;
        assert(XgbaA2BootstrapCommit(&io, NativeRead, NativeWriter, NULL,
                                    scratch, old, &initial) == XGBA_A2_JOURNAL_OK);
        assert(XgbaA2PagerInit(&live, &io, old, 0, initial.pages) == 0);
        assert(XgbaA2PagerEnableRam(&live, ram, 8 * XGBA_A2_PC_BYTES) == 0);

        old[47] = 0x91;
        assert(XgbaA2PagerSwitch(&live, 7) == 0);
        old[49] = 0xA2;
        assert(XgbaA2PagerSwitch(&live, 0) == 0);
        assert(old[47] == 0x91);
        assert(XgbaA2TransactionCommit(&live, &initial, NativeRead, NativeWriter,
                                       NULL, scratch, &saved) == 0);
        assert(saved.pages[0] == 3 && saved.pages[7] == 3);
        assert(saved.nativeCounter == 2 && saved.metadataGeneration == 2);
        assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
        assert(selected.nativeCounter == 2 && selected.activePage == 0);

        memset(old, 0, XGBA_A2_PC_BYTES);
        assert(XgbaA2JournalReadPageGeneration(&io, 0, selected.pages[0], old,
                                               XGBA_A2_PC_BYTES, NULL) == 0);
        assert(XgbaA2PagerInit(&coldPager, &io, old, 0, selected.pages) == 0);
        assert(XgbaA2PagerEnableRam(&coldPager, cold, 8 * XGBA_A2_PC_BYTES) == 0);
        assert(old[47] == 0x91);
        assert(XgbaA2PagerSwitch(&coldPager, 7) == 0);
        assert(old[49] == 0xA2);
        old[53] = 0xB3;
        assert(XgbaA2PagerSwitch(&coldPager, 0) == 0);
        old[51] = 0xC4;
        assert(XgbaA2TransactionCommit(&coldPager, &saved, NativeRead, NativeWriter,
                                       NULL, scratch, &savedAgain) == 0);
        assert(savedAgain.pages[0] == 4 && savedAgain.pages[7] == 4);
        assert(savedAgain.nativeCounter == 3 && savedAgain.metadataGeneration == 3);
        assert(XgbaA2CohortSelect(&io, NativeRead, NULL, scratch, &selected) == 0);
        assert(selected.nativeCounter == 3 && selected.activePage == 0);
        memset(old, 0, XGBA_A2_PC_BYTES);
        assert(XgbaA2JournalReadPageGeneration(&io, 0, selected.pages[0], old,
                                               XGBA_A2_PC_BYTES, NULL) == 0);
        assert(XgbaA2PagerInit(&secondCold, &io, old, 0, selected.pages) == 0);
        assert(XgbaA2PagerEnableRam(&secondCold, cold2, 8 * XGBA_A2_PC_BYTES) == 0);
        assert(old[47] == 0x91 && old[51] == 0xC4);
        assert(XgbaA2PagerSwitch(&secondCold, 7) == 0);
        assert(old[49] == 0xA2 && old[53] == 0xB3);
        free(cold2);
        free(cold);
        free(ram);
    }

    puts("A2 cohort: native sector/checksum/SHA, page 7 staged/cold reload, global commit and rollback PASS");
    free(next);
    free(old);
    free(sDisk);
    return 0;
}
