#include "xgba_a2_journal.h"

#include <stddef.h>
#include <string.h>

#define HEADER_BYTES 32
#define TRANSFER_BYTES 128
#define META_BYTES 24

static const uint8_t sPageMagic[8] = { 'X', 'G', 'B', 'A', 'A', '2', 'P', 0 };
static const uint8_t sMetaMagic[8] = { 'X', 'G', 'B', 'A', 'A', '2', 'M', 0 };

struct Candidate
{
    uint32_t generation;
    uint32_t dataCrc;
    uint8_t slot;
    uint8_t valid;
};

static uint16_t Read16(const uint8_t *p)
{
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t Read32(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8)
        | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void Write16(uint8_t *p, uint16_t value)
{
    p[0] = (uint8_t)value;
    p[1] = (uint8_t)(value >> 8);
}

static void Write32(uint8_t *p, uint32_t value)
{
    p[0] = (uint8_t)value;
    p[1] = (uint8_t)(value >> 8);
    p[2] = (uint8_t)(value >> 16);
    p[3] = (uint8_t)(value >> 24);
}

// ROM-resident, table-driven CRC32 matches Python zlib and the tested E6
// prototype. Avoid bit-serial CRC for every 34 KiB page transition.
static const uint32_t sCrcTable[256] = {
    0x00000000U, 0x77073096U, 0xEE0E612CU, 0x990951BAU, 0x076DC419U, 0x706AF48FU, 0xE963A535U, 0x9E6495A3U,
    0x0EDB8832U, 0x79DCB8A4U, 0xE0D5E91EU, 0x97D2D988U, 0x09B64C2BU, 0x7EB17CBDU, 0xE7B82D07U, 0x90BF1D91U,
    0x1DB71064U, 0x6AB020F2U, 0xF3B97148U, 0x84BE41DEU, 0x1ADAD47DU, 0x6DDDE4EBU, 0xF4D4B551U, 0x83D385C7U,
    0x136C9856U, 0x646BA8C0U, 0xFD62F97AU, 0x8A65C9ECU, 0x14015C4FU, 0x63066CD9U, 0xFA0F3D63U, 0x8D080DF5U,
    0x3B6E20C8U, 0x4C69105EU, 0xD56041E4U, 0xA2677172U, 0x3C03E4D1U, 0x4B04D447U, 0xD20D85FDU, 0xA50AB56BU,
    0x35B5A8FAU, 0x42B2986CU, 0xDBBBC9D6U, 0xACBCF940U, 0x32D86CE3U, 0x45DF5C75U, 0xDCD60DCFU, 0xABD13D59U,
    0x26D930ACU, 0x51DE003AU, 0xC8D75180U, 0xBFD06116U, 0x21B4F4B5U, 0x56B3C423U, 0xCFBA9599U, 0xB8BDA50FU,
    0x2802B89EU, 0x5F058808U, 0xC60CD9B2U, 0xB10BE924U, 0x2F6F7C87U, 0x58684C11U, 0xC1611DABU, 0xB6662D3DU,
    0x76DC4190U, 0x01DB7106U, 0x98D220BCU, 0xEFD5102AU, 0x71B18589U, 0x06B6B51FU, 0x9FBFE4A5U, 0xE8B8D433U,
    0x7807C9A2U, 0x0F00F934U, 0x9609A88EU, 0xE10E9818U, 0x7F6A0DBBU, 0x086D3D2DU, 0x91646C97U, 0xE6635C01U,
    0x6B6B51F4U, 0x1C6C6162U, 0x856530D8U, 0xF262004EU, 0x6C0695EDU, 0x1B01A57BU, 0x8208F4C1U, 0xF50FC457U,
    0x65B0D9C6U, 0x12B7E950U, 0x8BBEB8EAU, 0xFCB9887CU, 0x62DD1DDFU, 0x15DA2D49U, 0x8CD37CF3U, 0xFBD44C65U,
    0x4DB26158U, 0x3AB551CEU, 0xA3BC0074U, 0xD4BB30E2U, 0x4ADFA541U, 0x3DD895D7U, 0xA4D1C46DU, 0xD3D6F4FBU,
    0x4369E96AU, 0x346ED9FCU, 0xAD678846U, 0xDA60B8D0U, 0x44042D73U, 0x33031DE5U, 0xAA0A4C5FU, 0xDD0D7CC9U,
    0x5005713CU, 0x270241AAU, 0xBE0B1010U, 0xC90C2086U, 0x5768B525U, 0x206F85B3U, 0xB966D409U, 0xCE61E49FU,
    0x5EDEF90EU, 0x29D9C998U, 0xB0D09822U, 0xC7D7A8B4U, 0x59B33D17U, 0x2EB40D81U, 0xB7BD5C3BU, 0xC0BA6CADU,
    0xEDB88320U, 0x9ABFB3B6U, 0x03B6E20CU, 0x74B1D29AU, 0xEAD54739U, 0x9DD277AFU, 0x04DB2615U, 0x73DC1683U,
    0xE3630B12U, 0x94643B84U, 0x0D6D6A3EU, 0x7A6A5AA8U, 0xE40ECF0BU, 0x9309FF9DU, 0x0A00AE27U, 0x7D079EB1U,
    0xF00F9344U, 0x8708A3D2U, 0x1E01F268U, 0x6906C2FEU, 0xF762575DU, 0x806567CBU, 0x196C3671U, 0x6E6B06E7U,
    0xFED41B76U, 0x89D32BE0U, 0x10DA7A5AU, 0x67DD4ACCU, 0xF9B9DF6FU, 0x8EBEEFF9U, 0x17B7BE43U, 0x60B08ED5U,
    0xD6D6A3E8U, 0xA1D1937EU, 0x38D8C2C4U, 0x4FDFF252U, 0xD1BB67F1U, 0xA6BC5767U, 0x3FB506DDU, 0x48B2364BU,
    0xD80D2BDAU, 0xAF0A1B4CU, 0x36034AF6U, 0x41047A60U, 0xDF60EFC3U, 0xA867DF55U, 0x316E8EEFU, 0x4669BE79U,
    0xCB61B38CU, 0xBC66831AU, 0x256FD2A0U, 0x5268E236U, 0xCC0C7795U, 0xBB0B4703U, 0x220216B9U, 0x5505262FU,
    0xC5BA3BBEU, 0xB2BD0B28U, 0x2BB45A92U, 0x5CB36A04U, 0xC2D7FFA7U, 0xB5D0CF31U, 0x2CD99E8BU, 0x5BDEAE1DU,
    0x9B64C2B0U, 0xEC63F226U, 0x756AA39CU, 0x026D930AU, 0x9C0906A9U, 0xEB0E363FU, 0x72076785U, 0x05005713U,
    0x95BF4A82U, 0xE2B87A14U, 0x7BB12BAEU, 0x0CB61B38U, 0x92D28E9BU, 0xE5D5BE0DU, 0x7CDCEFB7U, 0x0BDBDF21U,
    0x86D3D2D4U, 0xF1D4E242U, 0x68DDB3F8U, 0x1FDA836EU, 0x81BE16CDU, 0xF6B9265BU, 0x6FB077E1U, 0x18B74777U,
    0x88085AE6U, 0xFF0F6A70U, 0x66063BCAU, 0x11010B5CU, 0x8F659EFFU, 0xF862AE69U, 0x616BFFD3U, 0x166CCF45U,
    0xA00AE278U, 0xD70DD2EEU, 0x4E048354U, 0x3903B3C2U, 0xA7672661U, 0xD06016F7U, 0x4969474DU, 0x3E6E77DBU,
    0xAED16A4AU, 0xD9D65ADCU, 0x40DF0B66U, 0x37D83BF0U, 0xA9BCAE53U, 0xDEBB9EC5U, 0x47B2CF7FU, 0x30B5FFE9U,
    0xBDBDF21CU, 0xCABAC28AU, 0x53B39330U, 0x24B4A3A6U, 0xBAD03605U, 0xCDD70693U, 0x54DE5729U, 0x23D967BFU,
    0xB3667A2EU, 0xC4614AB8U, 0x5D681B02U, 0x2A6F2B94U, 0xB40BBE37U, 0xC30C8EA1U, 0x5A05DF1BU, 0x2D02EF8DU,

};

static uint32_t CrcUpdate(uint32_t crc, const uint8_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
        crc = sCrcTable[(crc ^ buffer[i]) & 0xFF] ^ (crc >> 8);
    return crc;
}

static uint32_t CrcRam(const uint8_t *data, uint32_t bytes)
{
    return ~CrcUpdate(0xFFFFFFFFU, data, bytes);
}

static int IoReady(const struct XgbaA2JournalIo *io)
{
    return io != NULL && io->read != NULL && io->write != NULL;
}

static uint32_t RecordOffset(uint8_t isMeta, uint8_t page, uint8_t slot)
{
    if (isMeta)
        return XGBA_A2_PC_META_BASE + (uint32_t)slot * XGBA_A2_PC_META_STRIDE;
    return XGBA_A2_PC_BASE + ((uint32_t)page * 2 + slot) * XGBA_A2_PC_STRIDE;
}

static int ReadPayloadCrc(const struct XgbaA2JournalIo *io, uint32_t offset,
                          uint32_t bytes, uint32_t *result)
{
    uint8_t buffer[TRANSFER_BYTES];
    uint32_t remaining = bytes;
    uint32_t crc = 0xFFFFFFFFU;
    offset += HEADER_BYTES;

    while (remaining != 0)
    {
        uint32_t amount = remaining > TRANSFER_BYTES ? TRANSFER_BYTES : remaining;
        if (!io->read(io->context, offset, buffer, amount))
            return 0;
        crc = CrcUpdate(crc, buffer, amount);
        offset += amount;
        remaining -= amount;
    }
    *result = ~crc;
    return 1;
}

static int CheckCandidate(const struct XgbaA2JournalIo *io, uint8_t isMeta,
                          uint8_t page, uint8_t slot, uint32_t bytes,
                          struct Candidate *out)
{
    const uint8_t *magic = isMeta ? sMetaMagic : sPageMagic;
    const uint32_t offset = RecordOffset(isMeta, page, slot);
    uint8_t header[HEADER_BYTES];
    uint32_t dataCrc;

    memset(out, 0, sizeof(*out));
    out->slot = slot;
    if (!io->read(io->context, offset, header, HEADER_BYTES))
        return XGBA_A2_JOURNAL_IO_ERROR;

    if (memcmp(header, magic, 8) != 0
        || Read16(header + 8) != XGBA_A2_RECORD_VERSION
        || Read16(header + 10) != 0
        || header[12] != page || header[13] != 0
        || Read16(header + 14) != 0
        || Read32(header + 16) == 0
        || Read32(header + 20) != bytes
        || Read32(header + 28) != CrcRam(header, 28))
        return XGBA_A2_JOURNAL_OK;

    if (!ReadPayloadCrc(io, offset, bytes, &dataCrc))
        return XGBA_A2_JOURNAL_IO_ERROR;

    if (dataCrc == Read32(header + 24))
    {
        out->valid = 1;
        out->generation = Read32(header + 16);
        out->dataCrc = dataCrc;
    }
    return XGBA_A2_JOURNAL_OK;
}

static int FindRecord(const struct XgbaA2JournalIo *io, uint8_t isMeta,
                      uint8_t page, uint32_t bytes,
                      struct Candidate *best, uint8_t *count)
{
    struct Candidate left, right;
    int status = CheckCandidate(io, isMeta, page, 0, bytes, &left);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;
    status = CheckCandidate(io, isMeta, page, 1, bytes, &right);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;

    *count = (uint8_t)(left.valid + right.valid);
    if (*count == 0)
        return XGBA_A2_JOURNAL_MISSING;

    if (right.valid && (!left.valid || right.generation > left.generation))
        *best = right;
    else
        *best = left;
    return XGBA_A2_JOURNAL_OK;
}

static int ReadRecord(const struct XgbaA2JournalIo *io, uint8_t isMeta,
                      uint8_t page, void *destination, uint32_t bytes,
                      struct XgbaA2JournalInfo *info)
{
    struct Candidate best;
    uint8_t count;
    uint8_t *result = destination;
    uint32_t offset, remaining;
    int status;

    if (!IoReady(io) || destination == NULL
        || page >= (isMeta ? 1 : XGBA_A2_PC_PAGES)
        || bytes != (isMeta ? META_BYTES : XGBA_A2_PC_BYTES))
        return XGBA_A2_JOURNAL_INVALID_ARG;

    // CRC validate the record BEFORE changing the caller's live page buffer.
    status = FindRecord(io, isMeta, page, bytes, &best, &count);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;

    offset = RecordOffset(isMeta, page, best.slot) + HEADER_BYTES;
    remaining = bytes;
    while (remaining != 0)
    {
        uint32_t amount = remaining > TRANSFER_BYTES ? TRANSFER_BYTES : remaining;
        if (!io->read(io->context, offset, result, amount))
            return XGBA_A2_JOURNAL_IO_ERROR;
        offset += amount;
        result += amount;
        remaining -= amount;
    }

    if (info != NULL)
    {
        info->generation = best.generation;
        info->slot = best.slot;
        info->validCopies = count;
    }
    return XGBA_A2_JOURNAL_OK;
}

static int SameAsRecord(const struct XgbaA2JournalIo *io, uint32_t offset,
                        const uint8_t *source, uint32_t bytes)
{
    uint8_t data[TRANSFER_BYTES];
    uint32_t remaining = bytes;
    offset += HEADER_BYTES;

    while (remaining != 0)
    {
        uint32_t amount = remaining > TRANSFER_BYTES ? TRANSFER_BYTES : remaining;
        if (!io->read(io->context, offset, data, amount))
            return -1;
        if (memcmp(data, source, amount) != 0)
            return 0;
        source += amount;
        offset += amount;
        remaining -= amount;
    }
    return 1;
}

static int WriteRecord(const struct XgbaA2JournalIo *io, uint8_t isMeta,
                       uint8_t page, const void *source, uint32_t bytes,
                       struct XgbaA2JournalInfo *info)
{
    struct Candidate best;
    const uint8_t *payload = source;
    const uint8_t *magic = isMeta ? sMetaMagic : sPageMagic;
    uint8_t header[HEADER_BYTES] = {0};
    uint8_t invalidate[8] = {0};
    uint8_t targetSlot;
    uint8_t count;
    uint32_t offset;
    uint32_t remaining;
    uint32_t actualCrc;
    int compareResult, status;

    if (!IoReady(io) || source == NULL
        || page >= (isMeta ? 1 : XGBA_A2_PC_PAGES)
        || bytes != (isMeta ? META_BYTES : XGBA_A2_PC_BYTES))
        return XGBA_A2_JOURNAL_INVALID_ARG;

    // Never initialize or reconstruct a page that has lost both valid copies.
    // Doing so would transform corruption into a silently empty collection.
    status = FindRecord(io, isMeta, page, bytes, &best, &count);
    if (status != XGBA_A2_JOURNAL_OK)
        return status;

    offset = RecordOffset(isMeta, page, best.slot);
    if (best.dataCrc == CrcRam(payload, bytes))
    {
        compareResult = SameAsRecord(io, offset, payload, bytes);
        if (compareResult < 0)
            return XGBA_A2_JOURNAL_IO_ERROR;
        if (compareResult > 0)
        {
            if (info != NULL)
            {
                info->generation = best.generation;
                info->slot = best.slot;
                info->validCopies = count;
            }
            return XGBA_A2_JOURNAL_UNCHANGED;
        }
    }

    if (best.generation == UINT32_MAX)
        return XGBA_A2_JOURNAL_GENERATION_FULL;

    targetSlot = (uint8_t)(1 - best.slot);
    offset = RecordOffset(isMeta, page, targetSlot);

    // Start by destroying the non-active copy's magic. The last 8-byte magic
    // write is the commit marker. Any incomplete payload/header is ignored.
    if (!io->write(io->context, offset, invalidate, sizeof(invalidate)))
        return XGBA_A2_JOURNAL_IO_ERROR;

    remaining = bytes;
    while (remaining != 0)
    {
        uint32_t done = bytes - remaining;
        uint32_t amount = remaining > TRANSFER_BYTES ? TRANSFER_BYTES : remaining;
        if (!io->write(io->context, offset + HEADER_BYTES + done,
                       payload + done, amount))
            return XGBA_A2_JOURNAL_IO_ERROR;
        remaining -= amount;
    }

    if (!ReadPayloadCrc(io, offset, bytes, &actualCrc)
        || actualCrc != CrcRam(payload, bytes))
        return XGBA_A2_JOURNAL_IO_ERROR;

    memcpy(header, magic, 8);
    Write16(header + 8, XGBA_A2_RECORD_VERSION);
    Write16(header + 10, 0);
    header[12] = page;
    Write32(header + 16, best.generation + 1);
    Write32(header + 20, bytes);
    Write32(header + 24, actualCrc);
    Write32(header + 28, CrcRam(header, 28));

    if (!io->write(io->context, offset + 8, header + 8, HEADER_BYTES - 8))
        return XGBA_A2_JOURNAL_IO_ERROR;

    // A 7/8-byte partial magic write could accidentally complete the magic
    // because its final byte is already zero. Keep byte zero INVALID until
    // the last single, atomic 8-bit store. This is the actual commit point.
    if (!io->write(io->context, offset + 1, header + 1, 7))
        return XGBA_A2_JOURNAL_IO_ERROR;
    if (!io->write(io->context, offset, header, 1))
        return XGBA_A2_JOURNAL_IO_ERROR;

    // Verify the committed record from physical storage, not assumptions.
    struct Candidate verified;
    status = CheckCandidate(io, isMeta, page, targetSlot, bytes, &verified);
    if (status != XGBA_A2_JOURNAL_OK || !verified.valid
        || verified.generation != best.generation + 1)
        return XGBA_A2_JOURNAL_IO_ERROR;

    if (info != NULL)
    {
        info->generation = verified.generation;
        info->slot = targetSlot;
        info->validCopies = 2;
    }
    return XGBA_A2_JOURNAL_OK;
}

int XgbaA2JournalReadPage(const struct XgbaA2JournalIo *io, uint8_t page,
                          void *destination, uint32_t bytes,
                          struct XgbaA2JournalInfo *info)
{
    return ReadRecord(io, 0, page, destination, bytes, info);
}

int XgbaA2JournalWritePage(const struct XgbaA2JournalIo *io, uint8_t page,
                           const void *source, uint32_t bytes,
                           struct XgbaA2JournalInfo *info)
{
    return WriteRecord(io, 0, page, source, bytes, info);
}

int XgbaA2JournalReadMeta(const struct XgbaA2JournalIo *io,
                          void *destination, uint32_t bytes,
                          struct XgbaA2JournalInfo *info)
{
    return ReadRecord(io, 1, 0, destination, bytes, info);
}

int XgbaA2JournalWriteMeta(const struct XgbaA2JournalIo *io,
                           const void *source, uint32_t bytes,
                           struct XgbaA2JournalInfo *info)
{
    return WriteRecord(io, 1, 0, source, bytes, info);
}
