#ifndef GUARD_XGBA_A2_GUEST_BUS_H
#define GUARD_XGBA_A2_GUEST_BUS_H

#include <stdint.h>
#include "xgba_a2_journal.h"

// The host/test adapter and guest hardware adapter share the same callback
// contract. The backing address is 0x0F000000 on a tagged E6 cartridge.
// This code is NOT active on the A1 ROM (revision 0, native Flash only).
struct XgbaA2GuestBus
{
    volatile uint8_t *extension;
    uint8_t enabled;
};

// Does not write, allocate, or activate emulator save features.
int XgbaA2GuestBusBind(struct XgbaA2GuestBus *bus,
                       volatile uint8_t *extension);
int XgbaA2GuestBusRead(void *context, uint32_t fileOffset,
                       uint8_t *destination, uint32_t bytes);
int XgbaA2GuestBusWrite(void *context, uint32_t fileOffset,
                        const uint8_t *source, uint32_t bytes);
void XgbaA2GuestBusCreateIo(struct XgbaA2GuestBus *bus,
                            struct XgbaA2JournalIo *io);

#endif
