#include "xgba_r8_memory.h"

#if defined(XGBA_R8) && XGBA_R8
static const uint8_t sR8Descriptor[16] = {
    'X', 'G', 'B', 'A', 'R', 'A', 'M', '8',
    1, 0, 0, 0, 0, 0, 0x40, 0
};

int XgbaR8MemoryReady(void)
{
    volatile const uint8_t *descriptor =
        (volatile const uint8_t *)XGBA_R8_DESCRIPTOR_ROM;
    volatile uint32_t *first = (volatile uint32_t *)XGBA_R8_RAM_BASE;
    volatile uint32_t *last = (volatile uint32_t *)(XGBA_R8_RAM_BASE + XGBA_R8_RAM_BYTES - 4);
    uint32_t priorFirst, priorLast;
    uint32_t i;
    int ok;

    for (i = 0; i < sizeof(sR8Descriptor); i++)
        if (descriptor[i] != sR8Descriptor[i])
            return 0;

    priorFirst = *first;
    priorLast = *last;
    *first = 0xA85FC631U;
    *last = 0x739D2BE4U;
    ok = *first == 0xA85FC631U && *last == 0x739D2BE4U;
    *first = priorFirst;
    *last = priorLast;
    return ok;
}
#else
int XgbaR8MemoryReady(void)
{
    return 0;
}
#endif
