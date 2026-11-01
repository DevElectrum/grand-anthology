#include "xgba_a2_fingerprint.h"

#include <stddef.h>
#include <string.h>

struct Sha256State
{
    uint32_t words[8];
    uint64_t processed;
    uint8_t block[64];
    uint32_t used;
};

static const uint32_t sK[64] =
{
    0x428a2f98U, 0x71374491U, 0xb5c0fbcfU, 0xe9b5dba5U,
    0x3956c25bU, 0x59f111f1U, 0x923f82a4U, 0xab1c5ed5U,
    0xd807aa98U, 0x12835b01U, 0x243185beU, 0x550c7dc3U,
    0x72be5d74U, 0x80deb1feU, 0x9bdc06a7U, 0xc19bf174U,
    0xe49b69c1U, 0xefbe4786U, 0x0fc19dc6U, 0x240ca1ccU,
    0x2de92c6fU, 0x4a7484aaU, 0x5cb0a9dcU, 0x76f988daU,
    0x983e5152U, 0xa831c66dU, 0xb00327c8U, 0xbf597fc7U,
    0xc6e00bf3U, 0xd5a79147U, 0x06ca6351U, 0x14292967U,
    0x27b70a85U, 0x2e1b2138U, 0x4d2c6dfcU, 0x53380d13U,
    0x650a7354U, 0x766a0abbU, 0x81c2c92eU, 0x92722c85U,
    0xa2bfe8a1U, 0xa81a664bU, 0xc24b8b70U, 0xc76c51a3U,
    0xd192e819U, 0xd6990624U, 0xf40e3585U, 0x106aa070U,
    0x19a4c116U, 0x1e376c08U, 0x2748774cU, 0x34b0bcb5U,
    0x391c0cb3U, 0x4ed8aa4aU, 0x5b9cca4fU, 0x682e6ff3U,
    0x748f82eeU, 0x78a5636fU, 0x84c87814U, 0x8cc70208U,
    0x90befffaU, 0xa4506cebU, 0xbef9a3f7U, 0xc67178f2U
};

static uint32_t RotateRight(uint32_t value, uint32_t count)
{
    return (value >> count) | (value << (32 - count));
}

static uint32_t BigEndian32(const uint8_t *p)
{
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16)
        | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

static void Store32(uint8_t *p, uint32_t value)
{
    p[0] = (uint8_t)(value >> 24);
    p[1] = (uint8_t)(value >> 16);
    p[2] = (uint8_t)(value >> 8);
    p[3] = (uint8_t)value;
}

static void Compress(struct Sha256State *sha)
{
    uint32_t w[64];
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t i;

    for (i = 0; i < 16; i++)
        w[i] = BigEndian32(sha->block + i * 4);
    for (i = 16; i < 64; i++)
    {
        uint32_t s0 = RotateRight(w[i - 15], 7)
            ^ RotateRight(w[i - 15], 18) ^ (w[i - 15] >> 3);
        uint32_t s1 = RotateRight(w[i - 2], 17)
            ^ RotateRight(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }

    a = sha->words[0];
    b = sha->words[1];
    c = sha->words[2];
    d = sha->words[3];
    e = sha->words[4];
    f = sha->words[5];
    g = sha->words[6];
    h = sha->words[7];

    for (i = 0; i < 64; i++)
    {
        uint32_t s1 = RotateRight(e, 6) ^ RotateRight(e, 11) ^ RotateRight(e, 25);
        uint32_t choice = (e & f) ^ (~e & g);
        uint32_t t1 = h + s1 + choice + sK[i] + w[i];
        uint32_t s0 = RotateRight(a, 2) ^ RotateRight(a, 13) ^ RotateRight(a, 22);
        uint32_t majority = (a & b) ^ (a & c) ^ (b & c);
        uint32_t t2 = s0 + majority;

        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    sha->words[0] += a;
    sha->words[1] += b;
    sha->words[2] += c;
    sha->words[3] += d;
    sha->words[4] += e;
    sha->words[5] += f;
    sha->words[6] += g;
    sha->words[7] += h;
}

static void Begin(struct Sha256State *sha)
{
    static const uint32_t initial[8] =
    {
        0x6a09e667U, 0xbb67ae85U, 0x3c6ef372U, 0xa54ff53aU,
        0x510e527fU, 0x9b05688cU, 0x1f83d9abU, 0x5be0cd19U
    };
    memcpy(sha->words, initial, sizeof(initial));
    sha->processed = 0;
    sha->used = 0;
}

static void Update(struct Sha256State *sha, const uint8_t *buffer, uint32_t bytes)
{
    uint32_t i;
    sha->processed += bytes;

    for (i = 0; i < bytes; i++)
    {
        sha->block[sha->used++] = buffer[i];
        if (sha->used == sizeof(sha->block))
        {
            Compress(sha);
            sha->used = 0;
        }
    }
}

static void Finish(struct Sha256State *sha, uint8_t digest[32])
{
    uint8_t padding = 0x80;
    uint8_t zero = 0;
    uint8_t length[8];
    uint64_t bits = sha->processed * 8;
    uint32_t i;

    for (i = 0; i < sizeof(length); i++)
        length[7 - i] = (uint8_t)(bits >> (i * 8));

    Update(sha, &padding, 1);
    while (sha->used != 56)
        Update(sha, &zero, 1);
    Update(sha, length, sizeof(length));

    for (i = 0; i < 8; i++)
        Store32(digest + 4 * i, sha->words[i]);
}

int XgbaA2FingerprintNativeSlot(XgbaA2ReadNativeSector reader,
                                void *context, uint8_t slot,
                                uint8_t *sectorBuffer4096,
                                uint8_t digest16[16])
{
    struct Sha256State sha;
    uint8_t digest[32];
    uint32_t sector;

    if (reader == NULL || sectorBuffer4096 == NULL
        || digest16 == NULL || slot >= 2)
        return 0;

    Begin(&sha);
    for (sector = 0; sector < XGBA_A2_NATIVE_SLOT_SECTORS; sector++)
    {
        uint8_t physical = (uint8_t)(slot * XGBA_A2_NATIVE_SLOT_SECTORS + sector);
        if (!reader(context, physical, sectorBuffer4096))
            return 0;
        Update(&sha, sectorBuffer4096, XGBA_A2_NATIVE_SECTOR_BYTES);
    }

    Finish(&sha, digest);
    memcpy(digest16, digest, 16);
    return 1;
}
