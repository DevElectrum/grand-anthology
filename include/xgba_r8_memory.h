#ifndef GUARD_XGBA_R8_MEMORY_H
#define GUARD_XGBA_R8_MEMORY_H

#include <stdint.h>

#define XGBA_R8_RAM_BASE 0x20000000U
#define XGBA_R8_RAM_BYTES 0x00400000U
#define XGBA_R8_DESCRIPTOR_ROM (0x10000000U + 0x0FFFFF00U)

// Requires both the ROM's exact capability descriptor and live volatile RAM.
// Restores its probe cells even when the hardware does not echo a write.
int XgbaR8MemoryReady(void);

#endif
