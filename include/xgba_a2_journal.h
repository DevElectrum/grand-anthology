#ifndef GUARD_XGBA_A2_JOURNAL_H
#define GUARD_XGBA_A2_JOURNAL_H

#include <stdint.h>
#include "xgba_a2_storage_format.h"

// Offsets are absolute 16 MiB save-file offsets, not guest CPU addresses.
// Each callback must use ONLY byte transfers on the 0x0F guest save window.
// The driver does not allocate a second PokemonStorage or a large stack buffer.
struct XgbaA2JournalIo
{
    void *context;
    int (*read)(void *context, uint32_t offset, uint8_t *destination, uint32_t bytes);
    int (*write)(void *context, uint32_t offset, const uint8_t *source, uint32_t bytes);
};

struct XgbaA2JournalInfo
{
    uint32_t generation;
    uint8_t slot;
    uint8_t validCopies;
};

enum XgbaA2JournalResult
{
    XGBA_A2_JOURNAL_OK = 0,
    XGBA_A2_JOURNAL_UNCHANGED = 1,
    XGBA_A2_JOURNAL_INVALID_ARG = -1,
    XGBA_A2_JOURNAL_MISSING = -2,
    XGBA_A2_JOURNAL_IO_ERROR = -3,
    XGBA_A2_JOURNAL_GENERATION_FULL = -4
};

int XgbaA2JournalReadPage(const struct XgbaA2JournalIo *io, uint8_t page,
                          void *destination, uint32_t bytes,
                          struct XgbaA2JournalInfo *info);
int XgbaA2JournalWritePage(const struct XgbaA2JournalIo *io, uint8_t page,
                           const void *source, uint32_t bytes,
                           struct XgbaA2JournalInfo *info);
int XgbaA2JournalReadMeta(const struct XgbaA2JournalIo *io,
                          void *destination, uint32_t bytes,
                          struct XgbaA2JournalInfo *info);
int XgbaA2JournalWriteMeta(const struct XgbaA2JournalIo *io,
                           const void *source, uint32_t bytes,
                           struct XgbaA2JournalInfo *info);

#endif
