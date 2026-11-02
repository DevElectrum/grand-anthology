#include "global.h"
#include "agb_flash.h"
#include "gba/flash_internal.h"
#include "fieldmap.h"
#include "save.h"
#include "task.h"
#include "decompress.h"
#include "load_save.h"
#include "overworld.h"
#include "hall_of_fame.h"
#include "pokemon_storage_system.h"
#include "trainer_hill.h"
#include "link.h"
#include "constants/game_stat.h"

#if defined(XGBA_A2) && XGBA_A2
#include "xgba_a2_guest_bus.h"
#include "xgba_a2_transaction.h"
#include "xgba_a2_bootstrap.h"
#include "xgba_a2_game.h"
#if defined(XGBA_R8) && XGBA_R8
#include "xgba_r8_memory.h"
#if defined(XGBA_R8_TEST) && XGBA_R8_TEST
#include "xgba_r8_runtime_test.h"
#endif
#endif
#include "pokemon.h"
#endif

static u16 CalculateChecksum(void *, u16);
static bool8 ReadFlashSector(u8, struct SaveSector *);
static u8 GetSaveValidStatus(const struct SaveSectorLocation *);
static u8 CopySaveSlotData(u16, struct SaveSectorLocation *);
static u8 TryWriteSector(u8, u8 *);
static u8 HandleWriteSector(u16, const struct SaveSectorLocation *);
static u8 HandleReplaceSector(u16, const struct SaveSectorLocation *);
static void CopyToSaveBlock3(u32, struct SaveSector *);
static void CopyFromSaveBlock3(u32, struct SaveSector *);

#if defined(XGBA_A2) && XGBA_A2
static int A2NativeRead(void *context, uint8_t physical, uint8_t *buffer);
static int A2NativeWrite(void *context, uint8_t slot, uint32_t counter,
                         const void *pc, uint32_t bytes);
static bool8 A2ChooseLoad(void);
static bool8 A2FinishLoad(void);
static u8 A2Save(u8 saveType);
#endif

// Divide save blocks into individual chunks to be written to flash sectors

/*
 * Sector Layout:
 *
 * Sectors 0 - 13:      Save Slot 1
 * Sectors 14 - 27:     Save Slot 2
 * Sectors 28 - 29:     Hall of Fame
 * Sector 30:           Trainer Hill
 * Sector 31:           Recorded Battle
 *
 * There are two save slots for saving the player's game data. We alternate between
 * them each time the game is saved, so that if the current save slot is corrupt,
 * we can load the previous one. We also rotate the sectors in each save slot
 * so that the same data is not always being written to the same sector. This
 * might be done to reduce wear on the flash memory, but I'm not sure, since all
 * 14 sectors get written anyway.
 *
 * See SECTOR_ID_* constants in save.h
 */

#define SAVEBLOCK_CHUNK(structure, chunkNum)                                   \
{                                                                              \
    chunkNum * SECTOR_DATA_SIZE,                                               \
    sizeof(structure) >= chunkNum * SECTOR_DATA_SIZE ?                         \
    min(sizeof(structure) - chunkNum * SECTOR_DATA_SIZE, SECTOR_DATA_SIZE) : 0 \
}

struct
{
    u16 offset;
    u16 size;
} static const sSaveSlotLayout[NUM_SECTORS_PER_SLOT] =
{
    SAVEBLOCK_CHUNK(struct SaveBlock2, 0), // SECTOR_ID_SAVEBLOCK2

    SAVEBLOCK_CHUNK(struct SaveBlock1, 0), // SECTOR_ID_SAVEBLOCK1_START
    SAVEBLOCK_CHUNK(struct SaveBlock1, 1),
    SAVEBLOCK_CHUNK(struct SaveBlock1, 2),
    SAVEBLOCK_CHUNK(struct SaveBlock1, 3), // SECTOR_ID_SAVEBLOCK1_END

    SAVEBLOCK_CHUNK(struct PokemonStorage, 0), // SECTOR_ID_PKMN_STORAGE_START
    SAVEBLOCK_CHUNK(struct PokemonStorage, 1),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 2),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 3),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 4),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 5),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 6),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 7),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 8), // SECTOR_ID_PKMN_STORAGE_END
};

// These will produce an error if a save struct is larger than the space
// alloted for it in the flash.
STATIC_ASSERT(sizeof(struct SaveBlock3) <= SAVE_BLOCK_3_CHUNK_SIZE * NUM_SECTORS_PER_SLOT, SaveBlock3FreeSpace);
STATIC_ASSERT(sizeof(struct SaveBlock2) <= SECTOR_DATA_SIZE, SaveBlock2FreeSpace);
STATIC_ASSERT(sizeof(struct SaveBlock1) <= SECTOR_DATA_SIZE * (SECTOR_ID_SAVEBLOCK1_END - SECTOR_ID_SAVEBLOCK1_START + 1), SaveBlock1FreeSpace);
STATIC_ASSERT(sizeof(struct PokemonStorage) <= SECTOR_DATA_SIZE * (SECTOR_ID_PKMN_STORAGE_END - SECTOR_ID_PKMN_STORAGE_START + 1), PokemonStorageFreeSpace);

COMMON_DATA u16 gLastWrittenSector = 0;
COMMON_DATA u32 gLastSaveCounter = 0;
COMMON_DATA u16 gLastKnownGoodSector = 0;
COMMON_DATA u32 gDamagedSaveSectors = 0;
COMMON_DATA u32 gSaveCounter = 0;
COMMON_DATA struct SaveSector *gReadWriteSector = NULL; // Pointer to a buffer for reading/writing a sector
COMMON_DATA u16 gIncrementalSectorId = 0;
COMMON_DATA u16 gSaveFileStatus = 0;
COMMON_DATA MainCallback gGameContinueCallback = NULL;
COMMON_DATA struct SaveSectorLocation gRamSaveSectorLocations[NUM_SECTORS_PER_SLOT] = {0};
COMMON_DATA u16 gSaveAttemptStatus = 0;
COMMON_DATA u8 gSaveVersionRefusal = 0; // SAVE_VERSION_REFUSAL_*; set by LoadGameSave, read by the main menu

EWRAM_DATA struct SaveSector gSaveDataBuffer = {0}; // Buffer used for reading/writing sectors

#if defined(XGBA_A2) && XGBA_A2
// Temporary opt-in, NOT enabled by xgba-a1 or normal releases.
// The existing 4 KiB gSaveDataBuffer is reused for SHA and CRC inspection.
EWRAM_DATA static struct XgbaA2GuestBus sA2Bus;
EWRAM_DATA static struct XgbaA2JournalIo sA2Io;
EWRAM_DATA static struct XgbaA2Cohort sA2Committed;
EWRAM_DATA static struct XgbaA2Pager sA2Pager;
EWRAM_DATA static bool8 sA2Ready = FALSE;
EWRAM_DATA static bool8 sA2FreshSaveArmed = FALSE;
// Read-only disposable E6 runtime diagnostics, never part of any saved block.
// Upper byte is the stage; low 24 bits hold the stage-specific result/flags.
EWRAM_DATA volatile u32 gXgbaA2GameSaveTrace = 0;
EWRAM_DATA volatile u32 gXgbaA2PagerTrace = 0;
EWRAM_DATA volatile u32 gXgbaA2TransactionTrace = 0;
EWRAM_DATA volatile u32 gXgbaA2NativeWriteTrace = 0;
EWRAM_DATA volatile u32 gXgbaA2FlashWriteTrace = 0;
#define A2_NATIVE_TRACE(stage, detail) (gXgbaA2NativeWriteTrace = ((u32)(stage) << 24) | ((u32)(detail) & 0xFFFFFF))
#define A2_FLASH_TRACE(stage, detail) (gXgbaA2FlashWriteTrace = ((u32)(stage) << 24) | ((u32)(detail) & 0xFFFFFF))
#define A2_SAVE_TRACE(stage, detail) (gXgbaA2GameSaveTrace = ((u32)(stage) << 24) | ((u32)(detail) & 0xFFFFFF))
#endif

void ClearSaveData(void)
{
#if defined(XGBA_A2) && XGBA_A2
    // No new-game bootstrap exists yet for the 16 MiB journal. Refuse to
    // destroy the old native slots while metadata still references them.
    return;
#else
    u16 i;

    // Clear the full save two sectors at a time
    for (i = 0; i < SECTORS_COUNT / 2; i++)
    {
        EraseFlashSector(i);
        EraseFlashSector(i + SECTORS_COUNT / 2);
    }
#endif
}

void Save_ResetSaveCounters(void)
{
#if defined(XGBA_A2) && XGBA_A2
    sA2Ready = FALSE;
    sA2FreshSaveArmed = FALSE;
#endif
    gSaveCounter = 0;
    gLastWrittenSector = 0;
    gDamagedSaveSectors = 0;
}

static bool32 SetDamagedSectorBits(u8 op, u8 sectorId)
{
    bool32 retVal = FALSE;

    switch (op)
    {
    case ENABLE:
        gDamagedSaveSectors |= (1 << sectorId);
        break;
    case DISABLE:
        gDamagedSaveSectors &= ~(1 << sectorId);
        break;
    case CHECK: // unused
        if (gDamagedSaveSectors & (1 << sectorId))
            retVal = TRUE;
        break;
    }

    return retVal;
}

static u8 WriteSaveSectorOrSlot(u16 sectorId, const struct SaveSectorLocation *locations)
{
    u32 status;
    u16 i;

    gReadWriteSector = &gSaveDataBuffer;

    if (sectorId != FULL_SAVE_SLOT)
    {
        // A sector was specified, just write that sector.
        // This is never reached, FULL_SAVE_SLOT is always used instead.
        status = HandleWriteSector(sectorId, locations);
    }
    else
    {
        // No sector was specified, write full save slot.
        gLastKnownGoodSector = gLastWrittenSector; // backup the current written sector before attempting to write.
        gLastSaveCounter = gSaveCounter;
        gLastWrittenSector++;
        gLastWrittenSector = gLastWrittenSector % NUM_SECTORS_PER_SLOT;
        gSaveCounter++;
        status = SAVE_STATUS_OK;

        for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
            HandleWriteSector(i, locations);

        if (gDamagedSaveSectors)
        {
            // At least one sector save failed
            status = SAVE_STATUS_ERROR;
            gLastWrittenSector = gLastKnownGoodSector;
            gSaveCounter = gLastSaveCounter;
        }
    }

    return status;
}

static u8 HandleWriteSector(u16 sectorId, const struct SaveSectorLocation *locations)
{
    u16 i;
    u16 sector;
    u8 *data;
    u16 size;

    // Adjust sector id for current save slot
    sector = sectorId + gLastWrittenSector;
    sector %= NUM_SECTORS_PER_SLOT;
    sector += NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    // Get current save data
    data = locations[sectorId].data;
    size = locations[sectorId].size;

    // Clear temp save sector
    for (i = 0; i < SECTOR_SIZE; i++)
        ((u8 *)gReadWriteSector)[i] = 0;

    // Set footer data
    gReadWriteSector->id = sectorId;
    gReadWriteSector->signature = SECTOR_SIGNATURE;
    gReadWriteSector->counter = gSaveCounter;

    // Copy current data to temp buffer for writing
    for (i = 0; i < size; i++)
        gReadWriteSector->data[i] = data[i];

    CopyFromSaveBlock3(sectorId, gReadWriteSector);

    gReadWriteSector->checksum = CalculateChecksum(data, size);

    return TryWriteSector(sector, gReadWriteSector->data);
}

static u8 HandleWriteSectorNBytes(u8 sectorId, u8 *data, u16 size)
{
    u16 i;
    struct SaveSector *sector = &gSaveDataBuffer;

    // Clear temp save sector
    for (i = 0; i < SECTOR_SIZE; i++)
        ((u8 *)sector)[i] = 0;

    sector->signature = SECTOR_SIGNATURE;

    // Copy data to temp buffer for writing
    for (i = 0; i < size; i++)
        sector->data[i] = data[i];

    sector->id = CalculateChecksum(data, size); // though this appears to be incorrect, it might be some sector checksum instead of a whole save checksum and only appears to be relevent to HOF data, if used.
    return TryWriteSector(sectorId, sector->data);
}

static u8 TryWriteSector(u8 sector, u8 *data)
{
    u32 flashStatus = ProgramFlashSectorAndVerify(sector, data);

#if defined(XGBA_A2) && XGBA_A2
    if (flashStatus)
        A2_FLASH_TRACE(0xE0 | (sector / NUM_SECTORS_PER_SLOT),
                       ((u32)sector << 16) | (flashStatus & 0xFFFF));
#endif
    if (flashStatus) // is damaged?
    {
        // Failed
        SetDamagedSectorBits(ENABLE, sector);
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sector);
        return SAVE_STATUS_OK;
    }
}

static u32 RestoreSaveBackupVarsAndIncrement(const struct SaveSectorLocation *locations)
{
    gReadWriteSector = &gSaveDataBuffer;
    gLastKnownGoodSector = gLastWrittenSector;
    gLastSaveCounter = gSaveCounter;
    gLastWrittenSector++;
    gLastWrittenSector %= NUM_SECTORS_PER_SLOT;
    gSaveCounter++;
    gIncrementalSectorId = 0;
    gDamagedSaveSectors = 0;
    return 0;
}

static u32 RestoreSaveBackupVars(const struct SaveSectorLocation *locations)
{
    gReadWriteSector = &gSaveDataBuffer;
    gLastKnownGoodSector = gLastWrittenSector;
    gLastSaveCounter = gSaveCounter;
    gIncrementalSectorId = 0;
    gDamagedSaveSectors = 0;
    return 0;
}

static u8 HandleWriteIncrementalSector(u16 numSectors, const struct SaveSectorLocation *locations)
{
    u8 status;

    if (gIncrementalSectorId < numSectors - 1)
    {
        status = SAVE_STATUS_OK;
        HandleWriteSector(gIncrementalSectorId, locations);
        gIncrementalSectorId++;
        if (gDamagedSaveSectors)
        {
            status = SAVE_STATUS_ERROR;
            gLastWrittenSector = gLastKnownGoodSector;
            gSaveCounter = gLastSaveCounter;
        }
    }
    else
    {
        // Exceeded max sector, finished
        status = SAVE_STATUS_ERROR;
    }

    return status;
}

static u8 HandleReplaceSectorAndVerify(u16 sectorId, const struct SaveSectorLocation *locations)
{
    u8 status = SAVE_STATUS_OK;

    HandleReplaceSector(sectorId - 1, locations);

    if (gDamagedSaveSectors)
    {
        status = SAVE_STATUS_ERROR;
        gLastWrittenSector = gLastKnownGoodSector;
        gSaveCounter = gLastSaveCounter;
    }
    return status;
}

// Similar to HandleWriteSector, but fully erases the sector first, and skips writing the first signature byte
static u8 HandleReplaceSector(u16 sectorId, const struct SaveSectorLocation *locations)
{
    u16 i;
    u16 sector;
    u8 *data;
    u16 size;
    u8 status;

    // Adjust sector id for current save slot
    sector = sectorId + gLastWrittenSector;
    sector %= NUM_SECTORS_PER_SLOT;
    sector += NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    // Get current save data
    data = locations[sectorId].data;
    size = locations[sectorId].size;

    // Clear temp save sector.
    for (i = 0; i < SECTOR_SIZE; i++)
        ((u8 *)gReadWriteSector)[i] = 0;

    // Set footer data
    gReadWriteSector->id = sectorId;
    gReadWriteSector->signature = SECTOR_SIGNATURE;
    gReadWriteSector->counter = gSaveCounter;

    // Copy current data to temp buffer for writing
    for (i = 0; i < size; i++)
        gReadWriteSector->data[i] = data[i];

    CopyFromSaveBlock3(sectorId, gReadWriteSector);

    gReadWriteSector->checksum = CalculateChecksum(data, size);

    // Erase old save data
    EraseFlashSector(sector);

    status = SAVE_STATUS_OK;

    // Write new save data up to signature field
    for (i = 0; i < SECTOR_SIGNATURE_OFFSET; i++)
    {
        if (ProgramFlashByte(sector, i, ((u8 *)gReadWriteSector)[i]))
        {
            status = SAVE_STATUS_ERROR;
            break;
        }
    }

    if (status == SAVE_STATUS_ERROR)
    {
        // Writing save data failed
        SetDamagedSectorBits(ENABLE, sector);
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Writing save data succeeded, write signature and counter
        status = SAVE_STATUS_OK;

        // Write signature (skipping the first byte) and counter fields.
        // The byte of signature that is skipped is instead written by WriteSectorSignatureByte or WriteSectorSignatureByte_NoOffset
        for (i = 0; i < SECTOR_SIZE - (SECTOR_SIGNATURE_OFFSET + 1); i++)
        {
            if (ProgramFlashByte(sector, SECTOR_SIGNATURE_OFFSET + 1 + i, ((u8 *)gReadWriteSector)[SECTOR_SIGNATURE_OFFSET + 1 + i]))
            {
                status = SAVE_STATUS_ERROR;
                break;
            }
        }

        if (status == SAVE_STATUS_ERROR)
        {
            // Writing signature/counter failed
            SetDamagedSectorBits(ENABLE, sector);
            return SAVE_STATUS_ERROR;
        }
        else
        {
            // Succeeded
            SetDamagedSectorBits(DISABLE, sector);
            return SAVE_STATUS_OK;
        }
    }
}

static u8 WriteSectorSignatureByte_NoOffset(u16 sectorId, const struct SaveSectorLocation *locations)
{
    // Adjust sector id for current save slot
    // This first line lacking -1 is the only difference from WriteSectorSignatureByte
    u16 sector = sectorId + gLastWrittenSector;
    sector %= NUM_SECTORS_PER_SLOT;
    sector += NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    // Write just the first byte of the signature field, which was skipped by HandleReplaceSector
    if (ProgramFlashByte(sector, SECTOR_SIGNATURE_OFFSET, SECTOR_SIGNATURE & 0xFF))
    {
        // Sector is damaged, so enable the bit in gDamagedSaveSectors and restore the last written sector and save counter.
        SetDamagedSectorBits(ENABLE, sector);
        gLastWrittenSector = gLastKnownGoodSector;
        gSaveCounter = gLastSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sector);
        return SAVE_STATUS_OK;
    }
}

static u8 CopySectorSignatureByte(u16 sectorId, const struct SaveSectorLocation *locations)
{
    // Adjust sector id for current save slot
    u16 sector = sectorId + gLastWrittenSector - 1;
    sector %= NUM_SECTORS_PER_SLOT;
    sector += NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    // Copy just the first byte of the signature field from the read/write buffer
    if (ProgramFlashByte(sector, SECTOR_SIGNATURE_OFFSET, ((u8 *)gReadWriteSector)[SECTOR_SIGNATURE_OFFSET]))
    {
        // Sector is damaged, so enable the bit in gDamagedSaveSectors and restore the last written sector and save counter.
        SetDamagedSectorBits(ENABLE, sector);
        gLastWrittenSector = gLastKnownGoodSector;
        gSaveCounter = gLastSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sector);
        return SAVE_STATUS_OK;
    }
}

static u8 WriteSectorSignatureByte(u16 sectorId, const struct SaveSectorLocation *locations)
{
    // Adjust sector id for current save slot
    u16 sector = sectorId + gLastWrittenSector - 1;
    sector %= NUM_SECTORS_PER_SLOT;
    sector += NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    // Write just the first byte of the signature field, which was skipped by HandleReplaceSector
    if (ProgramFlashByte(sector, SECTOR_SIGNATURE_OFFSET, SECTOR_SIGNATURE & 0xFF))
    {
        // Sector is damaged, so enable the bit in gDamagedSaveSectors and restore the last written sector and save counter.
        SetDamagedSectorBits(ENABLE, sector);
        gLastWrittenSector = gLastKnownGoodSector;
        gSaveCounter = gLastSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sector);
        return SAVE_STATUS_OK;
    }
}

static u8 TryLoadSaveSlot(u16 sectorId, struct SaveSectorLocation *locations)
{
    u8 status;
    gReadWriteSector = &gSaveDataBuffer;
    if (sectorId != FULL_SAVE_SLOT)
    {
        // This function may not be used with a specific sector id
        status = SAVE_STATUS_ERROR;
    }
    else
    {
#if defined(XGBA_A2) && XGBA_A2
        // Choose the globally committed native+PC cohort FIRST, not the
        // newest physical Flash counter. A newer native slot may be an
        // uncommitted/aborted save despite having valid legacy checksums.
        if (!A2ChooseLoad())
            return SAVE_STATUS_CORRUPT;
        CopySaveSlotData(FULL_SAVE_SLOT, locations);
        if (!A2FinishLoad())
            return SAVE_STATUS_CORRUPT;
        status = SAVE_STATUS_OK;
#else
        status = GetSaveValidStatus(locations);
        CopySaveSlotData(FULL_SAVE_SLOT, locations);
#endif
    }

    return status;
}

// sectorId arg is ignored, this always reads the full save slot
static u8 CopySaveSlotData(u16 sectorId, struct SaveSectorLocation *locations)
{
    u16 i;
    u16 checksum;
    u16 slotOffset = NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);
    u16 id;

    for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
    {
        ReadFlashSector(i + slotOffset, gReadWriteSector);

        id = gReadWriteSector->id;
        if (id >= NUM_SECTORS_PER_SLOT)
            continue;
        if (id == 0)
            gLastWrittenSector = i;

        checksum = CalculateChecksum(gReadWriteSector->data, locations[id].size);

        // Only copy data for sectors whose signature and checksum fields are correct
        if (gReadWriteSector->signature == SECTOR_SIGNATURE && gReadWriteSector->checksum == checksum)
        {
            u16 j;
            for (j = 0; j < locations[id].size; j++)
                ((u8 *)locations[id].data)[j] = gReadWriteSector->data[j];
            CopyToSaveBlock3(id, gReadWriteSector);
        }
    }

    return SAVE_STATUS_OK;
}

static u8 GetSaveValidStatus(const struct SaveSectorLocation *locations)
{
    u16 i;
    u16 checksum;
    u32 saveSlot1Counter = 0;
    u32 saveSlot2Counter = 0;
    u32 validSectorFlags = 0;
    bool8 signatureValid = FALSE;
    u8 saveSlot1Status;
    u8 saveSlot2Status;

    // Check save slot 1
    for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
    {
        ReadFlashSector(i, gReadWriteSector);
        if (gReadWriteSector->signature == SECTOR_SIGNATURE)
        {
            signatureValid = TRUE;
            if (gReadWriteSector->id >= NUM_SECTORS_PER_SLOT)
                continue;
            checksum = CalculateChecksum(gReadWriteSector->data, locations[gReadWriteSector->id].size);
            if (gReadWriteSector->checksum == checksum)
            {
                saveSlot1Counter = gReadWriteSector->counter;
                validSectorFlags |= 1 << gReadWriteSector->id;
            }
        }
    }

    if (signatureValid)
    {
        if (validSectorFlags == (1 << NUM_SECTORS_PER_SLOT) - 1)
            saveSlot1Status = SAVE_STATUS_OK;
        else
            saveSlot1Status = SAVE_STATUS_ERROR;
    }
    else
    {
        // No sectors in slot 1 have the correct signature, treat it as empty
        saveSlot1Status = SAVE_STATUS_EMPTY;
    }

    validSectorFlags = 0;
    signatureValid = FALSE;

    // Check save slot 2
    for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
    {
        ReadFlashSector(i + NUM_SECTORS_PER_SLOT, gReadWriteSector);
        if (gReadWriteSector->signature == SECTOR_SIGNATURE)
        {
            signatureValid = TRUE;
            if (gReadWriteSector->id >= NUM_SECTORS_PER_SLOT)
                continue;
            checksum = CalculateChecksum(gReadWriteSector->data, locations[gReadWriteSector->id].size);
            if (gReadWriteSector->checksum == checksum)
            {
                saveSlot2Counter = gReadWriteSector->counter;
                validSectorFlags |= 1 << gReadWriteSector->id;
            }
        }
    }

    if (signatureValid)
    {
        if (validSectorFlags == (1 << NUM_SECTORS_PER_SLOT) - 1)
            saveSlot2Status = SAVE_STATUS_OK;
        else
            saveSlot2Status = SAVE_STATUS_ERROR;
    }
    else
    {
        // No sectors in slot 2 have the correct signature, treat it as empty.
        saveSlot2Status = SAVE_STATUS_EMPTY;
    }

    if (saveSlot1Status == SAVE_STATUS_OK && saveSlot2Status == SAVE_STATUS_OK)
    {
        if ((saveSlot1Counter == -1 && saveSlot2Counter ==  0)
         || (saveSlot1Counter ==  0 && saveSlot2Counter == -1))
        {
            if ((unsigned)(saveSlot1Counter + 1) < (unsigned)(saveSlot2Counter + 1))
                gSaveCounter = saveSlot2Counter;
            else
                gSaveCounter = saveSlot1Counter;
        }
        else
        {
            if (saveSlot1Counter < saveSlot2Counter)
                gSaveCounter = saveSlot2Counter;
            else
                gSaveCounter = saveSlot1Counter;
        }
        return SAVE_STATUS_OK;
    }

    // One or both save slots are not OK

    if (saveSlot1Status == SAVE_STATUS_OK)
    {
        gSaveCounter = saveSlot1Counter;
        if (saveSlot2Status == SAVE_STATUS_ERROR)
            return SAVE_STATUS_ERROR; // Slot 2 errored
        return SAVE_STATUS_OK; // Slot 1 is OK, slot 2 is empty
    }

    if (saveSlot2Status == SAVE_STATUS_OK)
    {
        gSaveCounter = saveSlot2Counter;
        if (saveSlot1Status == SAVE_STATUS_ERROR)
            return SAVE_STATUS_ERROR; // Slot 1 errored
        return SAVE_STATUS_OK; // Slot 2 is OK, slot 1 is empty
    }

    // Neither slot is OK, check if both are empty
    if (saveSlot1Status == SAVE_STATUS_EMPTY
     && saveSlot2Status == SAVE_STATUS_EMPTY)
    {
        gSaveCounter = 0;
        gLastWrittenSector = 0;
        return SAVE_STATUS_EMPTY;
    }

    // Both slots errored
    gSaveCounter = 0;
    gLastWrittenSector = 0;
    return SAVE_STATUS_CORRUPT;
}

static u8 TryLoadSaveSector(u8 sectorId, u8 *data, u16 size)
{
    u16 i;
    struct SaveSector *sector = &gSaveDataBuffer;
    ReadFlashSector(sectorId, sector);
    if (sector->signature == SECTOR_SIGNATURE)
    {
        u16 checksum = CalculateChecksum(sector->data, size);
        if (sector->id == checksum)
        {
            // Signature and checksum are correct, copy data
            for (i = 0; i < size; i++)
                data[i] = sector->data[i];
            return SAVE_STATUS_OK;
        }
        else
        {
            // Incorrect checksum
            return SAVE_STATUS_CORRUPT;
        }
    }
    else
    {
        // Incorrect signature value
        return SAVE_STATUS_EMPTY;
    }
}

// Return value always ignored
static bool8 ReadFlashSector(u8 sectorId, struct SaveSector *sector)
{
    ReadFlash(sectorId, 0, sector->data, SECTOR_SIZE);
    return TRUE;
}

static u16 CalculateChecksum(void *data, u16 size)
{
    u16 i;
    u32 checksum = 0;

    for (i = 0; i < (size / 4); i++)
    {
        checksum += *((u32 *)data);
        data += sizeof(u32);
    }

    return ((checksum >> 16) + checksum);
}

static void UpdateSaveAddresses(void)
{
    int i = SECTOR_ID_SAVEBLOCK2;
    gRamSaveSectorLocations[i].data = (void *)(gSaveBlock2Ptr) + sSaveSlotLayout[i].offset;
    gRamSaveSectorLocations[i].size = sSaveSlotLayout[i].size;

    for (i = SECTOR_ID_SAVEBLOCK1_START; i <= SECTOR_ID_SAVEBLOCK1_END; i++)
    {
        gRamSaveSectorLocations[i].data = (void *)(gSaveBlock1Ptr) + sSaveSlotLayout[i].offset;
        gRamSaveSectorLocations[i].size = sSaveSlotLayout[i].size;
    }

    for (; i <= SECTOR_ID_PKMN_STORAGE_END; i++) //setting i to SECTOR_ID_PKMN_STORAGE_START does not match
    {
        gRamSaveSectorLocations[i].data = (void *)(gPokemonStoragePtr) + sSaveSlotLayout[i].offset;
        gRamSaveSectorLocations[i].size = sSaveSlotLayout[i].size;
    }
}

#if defined(XGBA_A2) && XGBA_A2
static int A2NativeRead(void *context, uint8_t physical, uint8_t *buffer)
{
    (void)context;
    if (physical >= SECTORS_COUNT
        || gFlashMemoryPresent != TRUE)
        return 0;
    ReadFlash(physical, 0, buffer, SECTOR_SIZE);
    return 1;
}

static bool8 A2ChooseLoad(void)
{
    struct XgbaA2Cohort selection;

#if defined(XGBA_R8) && XGBA_R8
    // Probe before touching any native/E6 save. Old E7 cannot run R8 games.
    if (!XgbaR8MemoryReady())
        return FALSE;
#endif
    sA2Ready = FALSE;
    sA2FreshSaveArmed = FALSE;
    if (!XgbaA2GuestBusBind(&sA2Bus, (volatile uint8_t *)0x0F000000))
        return FALSE;
    XgbaA2GuestBusCreateIo(&sA2Bus, &sA2Io);

    if (XgbaA2CohortSelect(&sA2Io, A2NativeRead, NULL,
                            (uint8_t *)&gSaveDataBuffer, &selection)
        != XGBA_A2_JOURNAL_OK)
        return FALSE;
    sA2Committed = selection;
    gSaveCounter = selection.nativeCounter;
    return TRUE;
}

static bool8 A2FinishLoad(void)
{
    if (XgbaA2PagerInit(&sA2Pager, &sA2Io,
                         gPokemonStoragePtr, sA2Committed.activePage,
                         sA2Committed.pages) != XGBA_A2_JOURNAL_OK)
        return FALSE;
#if defined(XGBA_R8) && XGBA_R8
    if (XgbaA2PagerEnableRam(&sA2Pager, (void *)XGBA_R8_RAM_BASE,
            XGBA_A2_PC_BYTES * XGBA_A2_PC_PAGES) != XGBA_A2_JOURNAL_OK)
        return FALSE;
#endif
    sA2Ready = TRUE;
#if defined(XGBA_R8_TEST) && XGBA_R8_TEST
    // Real game records must survive a full R8 save and cold restart.
    if (sA2Committed.nativeCounter >= 2 && !XgbaR8VerifyTestPokemon())
    {
        sA2Ready = FALSE;
        return FALSE;
    }
#endif
    return TRUE;
}

static int A2NativeWrite(void *context, uint8_t slot, uint32_t counter,
                         const void *pc, uint32_t bytes)
{
    u8 result;
    (void)context;

    A2_NATIVE_TRACE(1, ((u32)slot << 16) | (counter & 0xFFFF));
    if (slot >= NUM_SAVE_SLOTS)
    {
        A2_NATIVE_TRACE(0xE1, slot);
        return 0;
    }
    if (bytes != sizeof(struct PokemonStorage))
    {
        A2_NATIVE_TRACE(0xE2, bytes);
        return 0;
    }
    if (pc != gPokemonStoragePtr)
    {
        A2_NATIVE_TRACE(0xE3, 0);
        return 0;
    }
    if (counter != gSaveCounter + 1)
    {
        A2_NATIVE_TRACE(0xE4, gSaveCounter);
        return 0;
    }
    if (slot != (counter % NUM_SAVE_SLOTS))
    {
        A2_NATIVE_TRACE(0xE5, slot);
        return 0;
    }

    A2_NATIVE_TRACE(2, gSaveCounter);
    A2_SAVE_TRACE(0x10, (slot << 16) | (counter & 0xFFFF));
    result = WriteSaveSectorOrSlot(FULL_SAVE_SLOT, gRamSaveSectorLocations);
    A2_SAVE_TRACE(0x11, ((u32)result << 16) | (gSaveCounter & 0xFFFF));
    if (result != SAVE_STATUS_OK)
    {
        A2_NATIVE_TRACE(0xE6, result);
        return 0;
    }
    if (gDamagedSaveSectors)
    {
        A2_NATIVE_TRACE(0xE7, gDamagedSaveSectors);
        return 0;
    }
    if (gSaveCounter != counter)
    {
        A2_NATIVE_TRACE(0xE8, gSaveCounter);
        return 0;
    }
    A2_NATIVE_TRACE(3, counter);
    return 1;
}

#if defined(XGBA_R8) && XGBA_R8
bool8 XgbaA2GameFreshUncommitted(void)
{
    return sA2FreshSaveArmed && !sA2Ready;
}

bool8 XgbaA2GameCollectionReady(void)
{
    return sA2Ready && sA2Pager.ready && !sA2Pager.fatal
        && sA2Pager.ramPages != NULL;
}

struct BoxPokemon *XgbaA2GameGlobalBoxMon(u16 globalBox, u8 position)
{
    struct PokemonStorage *storage;
    u8 page;
    u8 box;

    if (!XgbaA2GameCollectionReady()
        || globalBox >= XGBA_A2_PC_PAGES * TOTAL_BOXES_COUNT
        || position >= IN_BOX_COUNT)
        return NULL;
    page = globalBox / TOTAL_BOXES_COUNT;
    box = globalBox % TOTAL_BOXES_COUNT;
    if (page == sA2Pager.activePage)
        storage = gPokemonStoragePtr;
    else
        storage = (struct PokemonStorage *)(sA2Pager.ramPages
            + (u32)page * XGBA_A2_PC_BYTES);
    return &storage->boxes[box][position];
}

bool8 XgbaA2GameSelectGlobalBox(u16 globalBox)
{
    u8 page;
    u8 box;

    if (!XgbaA2GameCollectionReady()
        || globalBox >= XGBA_A2_PC_PAGES * TOTAL_BOXES_COUNT)
        return FALSE;
    page = globalBox / TOTAL_BOXES_COUNT;
    box = globalBox % TOTAL_BOXES_COUNT;
    if (!XgbaA2GameSwitchPage(page))
        return FALSE;
    gPokemonStoragePtr->currentBox = box;
    return TRUE;
}
#endif

bool8 XgbaA2GameFindFreePage(u8 *foundPage)
{
    struct XgbaA2JournalInfo pageInfo;
    struct BoxPokemon sample;
    u8 offset;
    if (!sA2Ready || sA2Pager.fatal || foundPage == NULL)
        return FALSE;

#if defined(XGBA_R8) && XGBA_R8
    if (!XgbaA2GameCollectionReady())
        return FALSE;
    for (offset = 1; offset < XGBA_A2_PC_PAGES; offset++)
    {
        u8 page = (sA2Pager.activePage + offset) % XGBA_A2_PC_PAGES;
        struct PokemonStorage *group = (struct PokemonStorage *)
            (sA2Pager.ramPages + (u32)page * XGBA_A2_PC_BYTES);
        u8 box;
        for (box = 0; box < TOTAL_BOXES_COUNT; box++)
        {
            u8 pos;
            for (pos = 0; pos < IN_BOX_COUNT; pos++)
                if (GetBoxMonData(&group->boxes[box][pos], MON_DATA_SPECIES) == SPECIES_NONE)
                {
                    *foundPage = page;
                    return TRUE;
                }
        }
    }
    return FALSE;
#else
    // Only called when all 420 slots on the resident page are full.
    // Pages are CRC-checked ONCE before inspecting their box data.
    // No 34 KiB temporary page, no 3,360-mon memory allocation.
    for (offset = 1; offset < XGBA_A2_PC_PAGES; offset++)
    {
        u8 page = (sA2Pager.activePage + offset) % XGBA_A2_PC_PAGES;
        u32 generation = sA2Pager.staged[page] != 0
            ? sA2Pager.staged[page] : sA2Pager.committed[page];
        u32 firstOffset;
        u32 mon;
        if (XgbaA2JournalFindPageGeneration(
            &sA2Io, page, generation, &pageInfo) != XGBA_A2_JOURNAL_OK)
        {
            // Cannot establish the complete PC's capacity. Fail closed:
            // never claim that a catch can safely be stored.
            sA2Ready = FALSE;
            return FALSE;
        }
        firstOffset = XGBA_A2_PC_BASE
            + ((u32)page * 2 + pageInfo.slot) * XGBA_A2_PC_STRIDE
            + XGBA_A2_RECORD_HEADER_BYTES
            + offsetof(struct PokemonStorage, boxes);

        for (mon = 0; mon < TOTAL_BOXES_COUNT * IN_BOX_COUNT; mon++)
        {
            if (!sA2Io.read(sA2Io.context,
                            firstOffset + mon * sizeof(struct BoxPokemon),
                            (u8 *)&sample, sizeof(sample)))
            {
                sA2Ready = FALSE;
                return FALSE;
            }
            if (GetBoxMonData(&sample, MON_DATA_SPECIES) == SPECIES_NONE)
            {
                *foundPage = page;
                return TRUE;
            }
        }
    }
    return FALSE;
#endif
}

// Only called when the player explicitly starts a new game.
void XgbaA2GameArmFreshSave(void)
{
    A2_SAVE_TRACE(1, 1);
    sA2FreshSaveArmed = TRUE;
    sA2Ready = FALSE;
}

u8 XgbaA2GamePage(void)
{
    return sA2Ready ? sA2Pager.activePage : 0;
}

void XgbaA2GameRebindRelocatedPc(void)
{
    // MoveSaveBlocks_ResetHeap copies the live PC, then changes its ASLR
    // address. A2's journal pager cannot keep using the previous address:
    // it would compare and save stale memory or reject every later save.
    // Rebind only after the new PokemonStorage has been fully restored.
    // Preserve all staged generations and fatal/ready state unchanged.
    if (!sA2Ready || !sA2Pager.ready || sA2Pager.fatal
        || gPokemonStoragePtr == NULL || sA2Pager.resident == NULL)
        return;
    sA2Pager.resident = gPokemonStoragePtr;
}

bool8 XgbaA2GameSwitchPage(u8 page)
{
    if (!sA2Ready || sA2Pager.fatal || page >= XGBA_A2_PC_PAGES)
        return FALSE;
    if (page == sA2Pager.activePage)
        return TRUE;
    if (XgbaA2PagerSwitch(&sA2Pager, page) != XGBA_A2_JOURNAL_OK)
    {
        // A failed read may have partially replaced the resident PC.
        // Refuse any subsequent save until a clean reload.
        sA2Ready = FALSE;
        return FALSE;
    }
    return TRUE;
}

static u8 A2Save(u8 saveType)
{
    struct XgbaA2Cohort newCohort;
    u32 savedCounter;
    u16 savedSector;
    u32 *backupVar;
    int result;

    A2_SAVE_TRACE(2, (saveType & 0xFF) | (sA2Ready ? 0x100 : 0) | (sA2FreshSaveArmed ? 0x200 : 0));

    // All full-game save requests share the same native+PC transaction.
    // The Hall of Fame archive lives in separate native sectors 28/29;
    // write it only AFTER the game's global commit is independently valid.
    if (saveType != SAVE_NORMAL && saveType != SAVE_LINK
        && saveType != SAVE_EREADER && saveType != SAVE_HALL_OF_FAME
        && saveType != SAVE_OVERWRITE_DIFFERENT_FILE)
    {
        A2_SAVE_TRACE(0xE1, saveType);
        return SAVE_STATUS_ERROR;
    }
    if (saveType == SAVE_OVERWRITE_DIFFERENT_FILE && !sA2FreshSaveArmed)
    {
        A2_SAVE_TRACE(0xE2, saveType);
        return SAVE_STATUS_ERROR;
    }
    if (!sA2Ready && !sA2FreshSaveArmed)
    {
        A2_SAVE_TRACE(0xE3, saveType);
        return SAVE_STATUS_ERROR;
    }
    if (saveType == SAVE_HALL_OF_FAME && gHoFSaveBuffer == NULL)
    {
        A2_SAVE_TRACE(0xE4, saveType);
        return SAVE_STATUS_ERROR;
    }

    backupVar = gTrainerHillVBlankCounter;
    gTrainerHillVBlankCounter = NULL;
    savedCounter = gSaveCounter;
    savedSector = gLastWrittenSector;

    UpdateSaveAddresses();
    if (saveType == SAVE_HALL_OF_FAME
        && GetGameStat(GAME_STAT_ENTERED_HOF) < 999)
        IncrementGameStat(GAME_STAT_ENTERED_HOF);
    CopyPartyAndObjectsToSave();

    if (sA2FreshSaveArmed)
    {
#if defined(XGBA_R8) && XGBA_R8
        if (!XgbaR8MemoryReady())
            goto failed;
#endif
        // One-shot authorization: even an interrupted initial save cannot
        // be retried in the same session without explicit new-game setup.
        sA2FreshSaveArmed = FALSE;
        A2_SAVE_TRACE(3, 1);
        if (!XgbaA2GuestBusBind(&sA2Bus, (volatile uint8_t *)0x0F000000))
        {
            A2_SAVE_TRACE(0xE5, 0);
            goto failed;
        }
        A2_SAVE_TRACE(4, 1);
        XgbaA2GuestBusCreateIo(&sA2Bus, &sA2Io);
        result = XgbaA2BootstrapCommit(
            &sA2Io, A2NativeRead, A2NativeWrite, NULL,
            (uint8_t *)&gSaveDataBuffer, gPokemonStoragePtr, &newCohort);
        if (result != XGBA_A2_JOURNAL_OK)
        {
            A2_SAVE_TRACE(0xE6, result);
            goto failed;
        }
        A2_SAVE_TRACE(5, 1);
        if (XgbaA2PagerInit(&sA2Pager, &sA2Io, gPokemonStoragePtr,
                             newCohort.activePage, newCohort.pages)
            != XGBA_A2_JOURNAL_OK)
        {
            A2_SAVE_TRACE(0xE7, 0);
            goto failed;
        }
#if defined(XGBA_R8) && XGBA_R8
        if (XgbaA2PagerEnableRam(&sA2Pager, (void *)XGBA_R8_RAM_BASE,
                XGBA_A2_PC_BYTES * XGBA_A2_PC_PAGES) != XGBA_A2_JOURNAL_OK)
            goto failed;
#endif
        A2_SAVE_TRACE(6, newCohort.nativeCounter);
        sA2Committed = newCohort;
        sA2Ready = TRUE;
        gTrainerHillVBlankCounter = backupVar;
        return SAVE_STATUS_OK;
    }

#if defined(XGBA_R8_TEST) && XGBA_R8_TEST
    // Only the disposable CI cartridge inserts genuine BoxPokemon records.
    // The metadata-pinned page groups are still committed by ordinary A2.
    if (sA2Committed.nativeCounter == 1
        && !XgbaR8CreateTestPokemon())
        goto failed;
#endif
    A2_SAVE_TRACE(7, sA2Committed.nativeCounter);
    result = XgbaA2TransactionCommit(
        &sA2Pager, &sA2Committed, A2NativeRead, A2NativeWrite,
        NULL, (uint8_t *)&gSaveDataBuffer, &newCohort);
    if (result != XGBA_A2_JOURNAL_OK)
        A2_SAVE_TRACE(0xE8, result);
    if (result == XGBA_A2_JOURNAL_OK)
    {
        A2_SAVE_TRACE(8, newCohort.nativeCounter);
        sA2Committed = newCohort;

        if (saveType == SAVE_HALL_OF_FAME)
        {
            u8 *hofData = (u8 *)gHoFSaveBuffer;
            // These optional Hall of Fame archive sectors are NOT part of
            // the collection commit. A failure is visible and forces reload,
            // but cannot roll back or tear the already committed Pokemon data.
            if (HandleWriteSectorNBytes(SECTOR_ID_HOF_1, hofData,
                                        SECTOR_DATA_SIZE) != SAVE_STATUS_OK)
                goto failed;
            if (HandleWriteSectorNBytes(SECTOR_ID_HOF_2,
                                        hofData + SECTOR_DATA_SIZE,
                                        SECTOR_DATA_SIZE) != SAVE_STATUS_OK)
                goto failed;
        }

        gTrainerHillVBlankCounter = backupVar;
        return SAVE_STATUS_OK;
    }

failed:
    // A failed initialization or transaction may have persisted the new
    // native slot or even the commit marker. No speculative retry is safe.
    sA2Ready = FALSE;
    sA2FreshSaveArmed = FALSE;
    gSaveCounter = savedCounter;
    gLastWrittenSector = savedSector;
    gTrainerHillVBlankCounter = backupVar;
    return SAVE_STATUS_ERROR;
}
#endif

u8 HandleSavingData(u8 saveType)
{
#if defined(XGBA_A2) && XGBA_A2
    // The only permitted gameplay save entrypoint in E6 mode. Every native
    // save must also publish an exact 112-box generation vector.
    return A2Save(saveType);
#else
    u8 i;
    u32 *backupVar = gTrainerHillVBlankCounter;

    gTrainerHillVBlankCounter = NULL;
    UpdateSaveAddresses();
    switch (saveType)
    {
    case SAVE_HALL_OF_FAME_ERASE_BEFORE:
        // Unused. Erases the special save sectors (HOF, Trainer Hill, Recorded Battle)
        // before overwriting HOF.
        for (i = SECTOR_ID_HOF_1; i < SECTORS_COUNT; i++)
            EraseFlashSector(i);
        // fallthrough
    case SAVE_HALL_OF_FAME:
        if (GetGameStat(GAME_STAT_ENTERED_HOF) < 999)
            IncrementGameStat(GAME_STAT_ENTERED_HOF);

        // Write the full save slot first
        CopyPartyAndObjectsToSave();
        WriteSaveSectorOrSlot(FULL_SAVE_SLOT, gRamSaveSectorLocations);

        // Save the Hall of Fame
        if (gHoFSaveBuffer != NULL)
        {
            u8 *tempAddr = (void *) gHoFSaveBuffer;
            HandleWriteSectorNBytes(SECTOR_ID_HOF_1, tempAddr, SECTOR_DATA_SIZE);
            HandleWriteSectorNBytes(SECTOR_ID_HOF_2, tempAddr + SECTOR_DATA_SIZE, SECTOR_DATA_SIZE);
        }
        break;
    case SAVE_NORMAL:
    default:
        CopyPartyAndObjectsToSave();
        WriteSaveSectorOrSlot(FULL_SAVE_SLOT, gRamSaveSectorLocations);
        break;
    case SAVE_LINK:
    case SAVE_EREADER: // Dummied, now duplicate of SAVE_LINK
        // Used by link / Battle Frontier. Upstream wrote only SaveBlocks 1 and 2 here
        // (sectors 0-4, skipping the PC) as a link-latency optimization — but SaveBlock3
        // rides as a saveBlock3Chunk inside EVERY slot sector (see CopyFromSaveBlock3), so
        // a 5-sector write persisted only SB3 bytes 0-579 and silently rolled back the
        // Johto flag bank, Kanto trainer flags, Route 5 daycare mon and obstacle bits to
        // the previous full save. Write the whole slot in place; the latency concern died
        // with the link code (#59), and Frontier saves must carry all of SaveBlock3.
        CopyPartyAndObjectsToSave();
        for (i = SECTOR_ID_SAVEBLOCK2; i <= SECTOR_ID_PKMN_STORAGE_END; i++)
            HandleReplaceSector(i, gRamSaveSectorLocations);
        for (i = SECTOR_ID_SAVEBLOCK2; i <= SECTOR_ID_PKMN_STORAGE_END; i++)
            WriteSectorSignatureByte_NoOffset(i, gRamSaveSectorLocations);
        break;
    case SAVE_OVERWRITE_DIFFERENT_FILE:
        // Erase Hall of Fame
        for (i = SECTOR_ID_HOF_1; i < SECTORS_COUNT; i++)
            EraseFlashSector(i);

        // Overwrite save slot
        CopyPartyAndObjectsToSave();
        WriteSaveSectorOrSlot(FULL_SAVE_SLOT, gRamSaveSectorLocations);
        break;
    }
    gTrainerHillVBlankCounter = backupVar;
    return 0;
#endif
}

u8 TrySavingData(u8 saveType)
{
    if (gFlashMemoryPresent != TRUE)
    {
        gSaveAttemptStatus = SAVE_STATUS_ERROR;
        return SAVE_STATUS_ERROR;
    }

#if defined(XGBA_A2) && XGBA_A2
    if (HandleSavingData(saveType) != SAVE_STATUS_OK)
    {
        gSaveAttemptStatus = SAVE_STATUS_ERROR;
        DoSaveFailedScreen(saveType);
        return SAVE_STATUS_ERROR;
    }
#else
    HandleSavingData(saveType);
#endif
    if (!gDamagedSaveSectors)
    {
        gSaveAttemptStatus = SAVE_STATUS_OK;
        return SAVE_STATUS_OK;
    }
    else
    {
        DoSaveFailedScreen(saveType);
        gSaveAttemptStatus = SAVE_STATUS_ERROR;
        return SAVE_STATUS_ERROR;
    }
}

#if defined(XGBA_A2) && XGBA_A2
static bool8 A2RejectIncrementalSave(void)
{
    // Older trade/mixing paths use TRUE as "all sectors finished".
    // Returning TRUE without a journal commit was silently losing data.
    // Route to a visible fatal save error and force a clean cohort reload.
    sA2Ready = FALSE;
    sA2FreshSaveArmed = FALSE;
    gSaveAttemptStatus = SAVE_STATUS_ERROR;
    DoA2UnsupportedLinkScreen();
    return TRUE;
}
#endif

bool8 LinkFullSave_Init(void)
{
#if defined(XGBA_A2) && XGBA_A2
    return A2RejectIncrementalSave();
#endif
    if (gFlashMemoryPresent != TRUE)
        return TRUE;
    UpdateSaveAddresses();
    CopyPartyAndObjectsToSave();
    RestoreSaveBackupVarsAndIncrement(gRamSaveSectorLocations);
    return FALSE;
}

bool8 LinkFullSave_WriteSector(void)
{
#if defined(XGBA_A2) && XGBA_A2
    return A2RejectIncrementalSave();
#endif
    u8 status = HandleWriteIncrementalSector(NUM_SECTORS_PER_SLOT, gRamSaveSectorLocations);
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_NORMAL);

    // In this case "error" either means that an actual error was encountered
    // or that the given max sector has been reached (meaning it has finished successfully).
    // If there was an actual error the save failed screen above will also be shown.
    if (status == SAVE_STATUS_ERROR)
        return TRUE;
    else
        return FALSE;
}

bool8 LinkFullSave_ReplaceLastSector(void)
{
#if defined(XGBA_A2) && XGBA_A2
    return A2RejectIncrementalSave();
#endif
    HandleReplaceSectorAndVerify(NUM_SECTORS_PER_SLOT, gRamSaveSectorLocations);
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_NORMAL);
    return FALSE;
}

bool8 LinkFullSave_SetLastSectorSignature(void)
{
#if defined(XGBA_A2) && XGBA_A2
    return A2RejectIncrementalSave();
#endif
    CopySectorSignatureByte(NUM_SECTORS_PER_SLOT, gRamSaveSectorLocations);
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_NORMAL);
    return FALSE;
}

bool8 WriteSaveBlock2(void)
{
#if defined(XGBA_A2) && XGBA_A2
    return A2RejectIncrementalSave();
#endif
    if (gFlashMemoryPresent != TRUE)
        return TRUE;

    UpdateSaveAddresses();
    CopyPartyAndObjectsToSave();
    RestoreSaveBackupVars(gRamSaveSectorLocations);

    // Because RestoreSaveBackupVars is called immediately prior, gIncrementalSectorId will always be 0 below,
    // so this function only saves the first sector (SECTOR_ID_SAVEBLOCK2)
    HandleReplaceSectorAndVerify(gIncrementalSectorId + 1, gRamSaveSectorLocations);
    return FALSE;
}

// Used in conjunction with WriteSaveBlock2 to write both for certain link saves.
// This will be called repeatedly in a task, writing each sector of SaveBlock1 incrementally.
// It returns TRUE when finished.
bool8 WriteSaveBlock1Sector(void)
{
#if defined(XGBA_A2) && XGBA_A2
    return A2RejectIncrementalSave();
#endif
    bool32 finished = FALSE;
    u16 sectorId = ++gIncrementalSectorId; // Because WriteSaveBlock2 will have been called prior, this will be SECTOR_ID_SAVEBLOCK1_START
    if (sectorId <= SECTOR_ID_SAVEBLOCK1_END)
    {
        // Write a single sector of SaveBlock1
        HandleReplaceSectorAndVerify(gIncrementalSectorId + 1, gRamSaveSectorLocations);
        WriteSectorSignatureByte(sectorId, gRamSaveSectorLocations);
    }
    else
    {
        // Beyond SaveBlock1, don't write the sector.
        // Does write 1 byte of the next sector's signature field, but as these
        // are the same for all valid sectors it doesn't matter.
        WriteSectorSignatureByte(sectorId, gRamSaveSectorLocations);
        finished = TRUE;
    }

    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_LINK);

    return finished;
}

u8 LoadGameSave(u8 saveType)
{
    u8 status;

    if (gFlashMemoryPresent != TRUE)
    {
        gSaveFileStatus = SAVE_STATUS_NO_FLASH;
        return SAVE_STATUS_ERROR;
    }

    UpdateSaveAddresses();
    switch (saveType)
    {
    case SAVE_NORMAL:
    default:
        status = TryLoadSaveSlot(FULL_SAVE_SLOT, gRamSaveSectorLocations);
        gSaveFileStatus = status;
        gGameContinueCallback = NULL;
#if ALL_REGIONS
        // SAVE_STATUS_ERROR still fully populates SB1/2/3 from the surviving good slot,
        // so a recovered pre-v6 save must migrate too (EMPTY/CORRUPT stay excluded).
        if (status == SAVE_STATUS_OK || status == SAVE_STATUS_ERROR)
        {
            // v7 layout gate. A save older than SAVE_FORMAT_LAYOUT_MIN predates the SaveBlock1
            // reshape and CANNOT be migrated — its flags/vars are already sitting at the wrong
            // offsets in RAM by the time we get here. Report it as EMPTY so the main menu offers
            // NEW GAME only, rather than letting the player Continue into a half-loaded save.
            // Reading saveVersion is safe: it is in SaveBlock2, which this break does not reshape.
            // gSaveVersionRefusal records WHY, so the menu can explain instead of looking blank.
            gSaveVersionRefusal = SAVE_VERSION_REFUSAL_NONE;
            if (gSaveBlock2Ptr->saveVersion < SAVE_FORMAT_LAYOUT_MIN)
            {
                gSaveVersionRefusal = SAVE_VERSION_REFUSAL_TOO_OLD;
                gSaveFileStatus = SAVE_STATUS_EMPTY;
                status = SAVE_STATUS_EMPTY;
            }
            // Forward gate: a save stamped by a NEWER build may carry a reshaped layout this
            // binary can't know about. Without this it would half-load, then the next save
            // would re-stamp it with THIS build's version — permanently downgrading it.
            else if (gSaveBlock2Ptr->saveVersion > SAVE_FORMAT_VERSION)
            {
                gSaveVersionRefusal = SAVE_VERSION_REFUSAL_TOO_NEW;
                gSaveFileStatus = SAVE_STATUS_EMPTY;
                status = SAVE_STATUS_EMPTY;
            }
            else
            {
                // Check integrity BEFORE migrating: a ladder step rewrites the banks, which would
                // make a corrupt SaveBlock3 checksum-clean afterwards and hide the damage.
                // RAM-only: this describes THIS load, not the save, so it must not persist.
                gRegionSaveCorrupt = !VerifyRegionSaveChecksum();
                MigrateSaveFormatIfNeeded();
                // Bind the obstacle bits to this build's generated table (see issue #16). Runs
                // after the ladder so a migrated save is stamped too.
                ResyncClearedObstacleTable();
            }
        }
#endif
        // AFTER the ladder, not before it. LoadObjectEvents() copies SaveBlock1.objectEvents[]
        // into gObjectEvents[], and MigrateDeletedTwinMaps() rewrites the mapNum in exactly that
        // array (src/load_save.c) -- so with the copy running first the renumber never reached a
        // live object, and the next SaveObjectEvents() wrote the stale value straight back over
        // the migrated one. A player who saved in one of the 20 renumbered rooms then had every
        // NPC on that floor carrying a pre-deletion mapNum, so GetObjectEventIdByLocalIdAndMap()
        // matched none of them and every applymovement/removeobject by local id silently no-op'd.
        // Safe to defer: the ladder touches only SaveBlock1/2/3 and never reads gPlayerParty or
        // gObjectEvents.
        CopyPartyAndObjectsFromSave();
        break;
    case SAVE_HALL_OF_FAME:
        if (gHoFSaveBuffer != NULL)
        {
            u8 *hofData = (u8 *) gHoFSaveBuffer;
            status = TryLoadSaveSector(SECTOR_ID_HOF_1, hofData, SECTOR_DATA_SIZE);
            if (status == SAVE_STATUS_OK)
                status = TryLoadSaveSector(SECTOR_ID_HOF_2, &hofData[SECTOR_DATA_SIZE], SECTOR_DATA_SIZE);
        }
        else
        {
            status = SAVE_STATUS_ERROR;
        }
        break;
    }

    return status;
}

u16 GetSaveBlocksPointersBaseOffset(void)
{
    u16 i, slotOffset;
    struct SaveSector *sector;

    sector = gReadWriteSector = &gSaveDataBuffer;
    if (gFlashMemoryPresent != TRUE)
        return 0;
    UpdateSaveAddresses();
#if defined(XGBA_A2) && XGBA_A2
    // On boot this is called BEFORE LoadGameSave: it determines the pointer
    // relocation from SaveBlock2's trainer id. Resolve the globally coherent
    // cohort here, before trusting ANY native save slot for those pointers.
    // Returning 0 for all pre-load calls silently corrupts save-block layout.
    // If new-game bootstrap is armed, do not touch its one-shot permission.
    if (sA2FreshSaveArmed)
        return 0;
    if (sA2Ready)
        gSaveCounter = sA2Committed.nativeCounter;
    else if (!A2ChooseLoad())
        return 0;
#else
    GetSaveValidStatus(gRamSaveSectorLocations);
#endif
    slotOffset = NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);
    for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
    {
        ReadFlashSector(i + slotOffset, gReadWriteSector);

        // Base offset for SaveBlock2 is calculated using the trainer id
        if (gReadWriteSector->id == SECTOR_ID_SAVEBLOCK2)
            return sector->data[offsetof(struct SaveBlock2, playerTrainerId[0])] +
                   sector->data[offsetof(struct SaveBlock2, playerTrainerId[1])] +
                   sector->data[offsetof(struct SaveBlock2, playerTrainerId[2])] +
                   sector->data[offsetof(struct SaveBlock2, playerTrainerId[3])];
    }
    return 0;
}

u32 TryReadSpecialSaveSector(u8 sector, u8 *dst)
{
    s32 i;
    s32 size;
    u8 *savData;

    if (sector != SECTOR_ID_TRAINER_HILL && sector != SECTOR_ID_RECORDED_BATTLE)
        return SAVE_STATUS_ERROR;

    ReadFlash(sector, 0, (u8 *)&gSaveDataBuffer, SECTOR_SIZE);
    if (*(u32 *)(&gSaveDataBuffer.data[0]) != SPECIAL_SECTOR_SENTINEL)
        return SAVE_STATUS_ERROR;

    // Copies whole save sector except u32 counter
    i = 0;
    size = SECTOR_COUNTER_OFFSET - 1;
    savData = &gSaveDataBuffer.data[4]; // data[4] to skip past SPECIAL_SECTOR_SENTINEL
    for (; i <= size; i++)
        dst[i] = savData[i];
    return SAVE_STATUS_OK;
}

u32 TryWriteSpecialSaveSector(u8 sector, u8 *src)
{
    s32 i;
    s32 size;
    u8 *savData;
    void *savDataBuffer;

    if (sector != SECTOR_ID_TRAINER_HILL && sector != SECTOR_ID_RECORDED_BATTLE)
        return SAVE_STATUS_ERROR;

    savDataBuffer = &gSaveDataBuffer;
    *(u32 *)(savDataBuffer) = SPECIAL_SECTOR_SENTINEL;

    // Copies whole save sector except u32 counter
    i = 0;
    size = SECTOR_COUNTER_OFFSET - 1;
    savData = &gSaveDataBuffer.data[4]; // data[4] to skip past SPECIAL_SECTOR_SENTINEL
    for (; i <= size; i++)
        savData[i] = src[i];
    if (ProgramFlashSectorAndVerify(sector, savDataBuffer) != 0)
        return SAVE_STATUS_ERROR;
    return SAVE_STATUS_OK;
}

#define tState         data[0]
#define tTimer         data[1]
#define tInBattleTower data[2]

// Note that this is very different from TrySavingData(SAVE_LINK).
// Most notably it does save the PC data.
void Task_LinkFullSave(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case 0:
        gSoftResetDisabled = TRUE;
        tState = 1;
        break;
    case 1:
        SetLinkStandbyCallback();
        tState = 2;
        break;
    case 2:
        if (IsLinkTaskFinished())
        {
            if (!tInBattleTower)
                SaveMapView();
            tState = 3;
        }
        break;
    case 3:
        if (!tInBattleTower)
            SetContinueGameWarpStatusToDynamicWarp();
        LinkFullSave_Init();
        tState = 4;
        break;
    case 4:
        if (++tTimer == 5)
        {
            tTimer = 0;
            tState = 5;
        }
        break;
    case 5:
        if (LinkFullSave_WriteSector())
            tState = 6;
        else
            tState = 4; // Not finished, delay again
        break;
    case 6:
        LinkFullSave_ReplaceLastSector();
        tState = 7;
        break;
    case 7:
        if (!tInBattleTower)
            ClearContinueGameWarpStatus2();
        SetLinkStandbyCallback();
        tState = 8;
        break;
    case 8:
        if (IsLinkTaskFinished())
        {
            LinkFullSave_SetLastSectorSignature();
            tState = 9;
        }
        break;
    case 9:
        SetLinkStandbyCallback();
        tState = 10;
        break;
    case 10:
        if (IsLinkTaskFinished())
            tState++;
        break;
    case 11:
        if (++tTimer > 5)
        {
            gSoftResetDisabled = FALSE;
            DestroyTask(taskId);
        }
        break;
    }
}

static u32 SaveBlock3Size(u32 sectorId)
{
    s32 begin = sectorId * SAVE_BLOCK_3_CHUNK_SIZE;
    s32 end = (sectorId + 1) * SAVE_BLOCK_3_CHUNK_SIZE;
    return max(0, min(end, (s32)sizeof(gSaveblock3)) - begin);
}

static void CopyToSaveBlock3(u32 sectorId, struct SaveSector *sector)
{
    u32 size = SaveBlock3Size(sectorId);
    memcpy((u8 *)&gSaveblock3 + (sectorId * SAVE_BLOCK_3_CHUNK_SIZE), sector->saveBlock3Chunk, size);
}

static void CopyFromSaveBlock3(u32 sectorId, struct SaveSector *sector)
{
    u32 size = SaveBlock3Size(sectorId);
    memcpy(sector->saveBlock3Chunk, (u8 *)&gSaveblock3 + (sectorId * SAVE_BLOCK_3_CHUNK_SIZE), size);
}
