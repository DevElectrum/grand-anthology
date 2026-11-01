#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "xgba_a2_fingerprint.h"

static int sFailSector = -1;

static int SectorRead(void *context, uint8_t sector, uint8_t *buffer)
{
    uint32_t i;
    (void)context;

    if (sector == sFailSector)
        return 0;
    for (i = 0; i < XGBA_A2_NATIVE_SECTOR_BYTES; i++)
        buffer[i] = (uint8_t)((sector * 13U + i * 7U) & 255U);
    return 1;
}

int main(void)
{
    uint8_t buffer[XGBA_A2_NATIVE_SECTOR_BYTES];
    uint8_t digest[16];
    static const uint8_t expected0[16] =
        {0x84,0xd3,0xa9,0x24,0x83,0x67,0xf0,0x3a,
         0x1d,0xc0,0x18,0x60,0x60,0x38,0x38,0xcd};
    static const uint8_t expected1[16] =
        {0xee,0x0b,0x5d,0x41,0x7a,0xbd,0xe4,0x75,
         0x86,0x5f,0xba,0x13,0x44,0xfa,0xf9,0x6e};

    assert(XgbaA2FingerprintNativeSlot(SectorRead, NULL, 0, buffer, digest));
    assert(memcmp(digest, expected0, sizeof(expected0)) == 0);
    assert(XgbaA2FingerprintNativeSlot(SectorRead, NULL, 1, buffer, digest));
    assert(memcmp(digest, expected1, sizeof(expected1)) == 0);
    assert(!XgbaA2FingerprintNativeSlot(SectorRead, NULL, 2, buffer, digest));
    sFailSector = 15;
    assert(!XgbaA2FingerprintNativeSlot(SectorRead, NULL, 1, buffer, digest));
    puts("A2 native-slot SHA-256 matches Python hashlib for both slots; bad reads fail closed.");
    return 0;
}
