#ifndef GUARD_XGBA_A2_FINGERPRINT_H
#define GUARD_XGBA_A2_FINGERPRINT_H

#include <stdint.h>

#define XGBA_A2_NATIVE_SLOT_SECTORS 14
#define XGBA_A2_NATIVE_SECTOR_BYTES 4096

// Reads one complete physical native Flash sector (including footer).
// The caller owns a reusable 4096-byte buffer (e.g. gSaveDataBuffer).
// The function never allocates large buffers in EWRAM or on the stack.
typedef int (*XgbaA2ReadNativeSector)(void *context, uint8_t physicalSector,
                                     uint8_t *buffer);

int XgbaA2FingerprintNativeSlot(XgbaA2ReadNativeSector reader,
                                void *context, uint8_t slot,
                                uint8_t *sectorBuffer4096,
                                uint8_t digest16[16]);

#endif
