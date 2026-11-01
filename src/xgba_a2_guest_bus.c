#include "xgba_a2_guest_bus.h"

#include <stddef.h>

static const uint8_t sE6Signature[24] =
{
    'X', 'G', 'B', 'A', 'E', '6', 'S', 0,
    2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1,
    0xB0, 0x2B, 0x5B, 0x12
};

int XgbaA2GuestBusBind(struct XgbaA2GuestBus *bus,
                       volatile uint8_t *extension)
{
    uint32_t i;

    if (bus == NULL)
        return 0;
    bus->extension = extension;
    bus->enabled = 0;
    if (extension == NULL)
        return 0;

    // Emulator probes the versioned header before exposing its E6 bank.
    // Guest probes the SAME bytes, using exactly 8-bit volatile loads.
    for (i = 0; i < sizeof(sE6Signature); i++)
    {
        if (extension[i] != sE6Signature[i])
            return 0;
    }

    bus->enabled = 1;
    return 1;
}

static int RangeValid(const struct XgbaA2GuestBus *bus,
                      uint32_t fileOffset, uint32_t bytes)
{
    if (bus == NULL || !bus->enabled || bus->extension == NULL)
        return 0;
    if (fileOffset < XGBA_A2_NATIVE_BYTES || fileOffset >= XGBA_A2_SAVE_BYTES)
        return 0;
    if (bytes == 0 || bytes > XGBA_A2_SAVE_BYTES - fileOffset)
        return 0;
    return 1;
}

int XgbaA2GuestBusRead(void *context, uint32_t fileOffset,
                       uint8_t *destination, uint32_t bytes)
{
    struct XgbaA2GuestBus *bus = context;
    uint32_t i;
    uint32_t index;

    if (destination == NULL || !RangeValid(bus, fileOffset, bytes))
        return 0;
    index = fileOffset - XGBA_A2_NATIVE_BYTES;
    for (i = 0; i < bytes; i++)
        destination[i] = bus->extension[index + i];
    return 1;
}

int XgbaA2GuestBusWrite(void *context, uint32_t fileOffset,
                        const uint8_t *source, uint32_t bytes)
{
    struct XgbaA2GuestBus *bus = context;
    uint32_t i;
    uint32_t index;

    if (source == NULL || !RangeValid(bus, fileOffset, bytes))
        return 0;
    index = fileOffset - XGBA_A2_NATIVE_BYTES;
    for (i = 0; i < bytes; i++)
        bus->extension[index + i] = source[i];
    return 1;
}

void XgbaA2GuestBusCreateIo(struct XgbaA2GuestBus *bus,
                            struct XgbaA2JournalIo *io)
{
    if (io == NULL)
        return;
    io->context = bus;
    io->read = XgbaA2GuestBusRead;
    io->write = XgbaA2GuestBusWrite;
}
