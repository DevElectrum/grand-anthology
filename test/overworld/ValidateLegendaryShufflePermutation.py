#!/usr/bin/env python3
"""Compile the actual game's Legendary Shuffle C body and compare both saved versions.

No GBA toolchain or game assets needed: gcc runs this source-level unit harness
on the host. It deliberately verifies version 1 rotation as well as v2 shuffle.
"""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
source = (ROOT / "src/script_pokemon_util.c").read_text()
start = source.index("static bool32 IsLegendaryStaticSpecies(enum Species species)")
end = source.index("enum Species WorldRandomizeStaticSpecies(enum Species original", start)
body = source[start:end]

SIZE = 96
SEEDS = (0, 1, 2, 2026, 32768, 65535)
MASK = 0xFFFFFFFF

header = r'''
#include <stdio.h>
#include <stdint.h>
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef uint32_t bool32;
enum Species { SPECIES_NONE = 0, SPECIES_EGG = 96 };
#define NUM_SPECIES 96
#define EWRAM_DATA
struct SpeciesInfo {
    u8 isRestrictedLegendary;
    u8 isSubLegendary;
    u8 isMythical;
};
static struct SpeciesInfo gSpeciesInfo[NUM_SPECIES];
static struct SaveBlock2 {
    u16 randomizerSeed;
    u8 randomizerVersion;
} state;
static struct SaveBlock2 *gSaveBlock2Ptr = &state;
static bool32 WorldIsFieldStableDiscoverySpecies(enum Species species)
{
    return species > 0 && species < NUM_SPECIES && species != 42 && species != 77;
}
'''
footer = r'''
int main(void)
{
    const u16 seeds[] = {0, 1, 2, 2026, 32768, 65535};
    for (u32 i = 1; i < NUM_SPECIES; i++)
        if (i % 3 == 0 || i % 7 == 0)
            gSpeciesInfo[i].isSubLegendary = 1;

    for (u32 version = 1; version <= 2; version++)
    {
        state.randomizerVersion = version;
        for (u32 s = 0; s < sizeof(seeds) / sizeof(seeds[0]); s++)
        {
            state.randomizerSeed = seeds[s];
            for (u32 original = 1; original < NUM_SPECIES; original++)
                if (gSpeciesInfo[original].isSubLegendary
                 && WorldIsFieldStableDiscoverySpecies(original))
                    printf("%u %u %u %u\n", version, state.randomizerSeed, original,
                           WorldShuffleLegendarySpecies((enum Species)original));
        }
    }
    return 0;
}
'''

def mix32(value):
    value ^= value >> 16
    value = (value * 0x7FEB352D) & MASK
    value ^= value >> 15
    value = (value * 0x846CA68B) & MASK
    value ^= value >> 16
    return value & MASK

def expected(seed, version, pool):
    if version == 1:
        return [pool[(i + seed % len(pool)) % len(pool)] for i in range(len(pool))]
    shuffled = list(pool)
    state = mix32(0xA511E9B3 ^ seed)
    if state == 0:
        state = 0x6D2B79F5
    for i in range(len(shuffled) - 1, 0, -1):
        state = (state ^ (state << 13)) & MASK
        state = (state ^ (state >> 17)) & MASK
        state = (state ^ (state << 5)) & MASK
        j = state % (i + 1)
        shuffled[i], shuffled[j] = shuffled[j], shuffled[i]
    return shuffled

with tempfile.TemporaryDirectory(prefix="anthology-static-shuffle-") as temporary:
    root = Path(temporary)
    c = root / "shuffle.c"
    exe = root / "shuffle"
    c.write_text(header + "\n" + body + "\n" + footer)
    subprocess.run(["gcc", "-std=c11", "-O2", "-Wall", "-Wextra",
                    str(c), "-o", str(exe)], check=True)
    actual = subprocess.check_output([str(exe)], text=True)

pool = [i for i in range(1, SIZE) if (i % 3 == 0 or i % 7 == 0) and i not in (42, 77)]
observed = {}
for line in actual.splitlines():
    version, seed, original, replacement = map(int, line.split())
    observed.setdefault((version, seed), {})[original] = replacement

for version in (1, 2):
    for seed in SEEDS:
        result = observed[(version, seed)]
        predicted = expected(seed, version, pool)
        assert list(result.keys()) == pool, ("Species ordering changed", version, seed)
        assert list(result.values()) == predicted, ("C/independent-reference mismatch", version, seed)
        assert len(set(result.values())) == len(pool), ("Not a permutation", version, seed)

assert expected(2026, 1, pool) != expected(2026, 2, pool)
assert "#define SAVE_FORMAT_VERSION 13" in (ROOT / "include/constants/global.h").read_text()
assert "gSaveBlock2Ptr->randomizerVersion = 2;" in (ROOT / "src/new_game.c").read_text()
assert "gSaveBlock2Ptr->randomizerVersion = 0;" in (ROOT / "src/load_save.c").read_text()
print("PASS: compiled game C shuffle matches reference for v1/v2 across", len(SEEDS),
      "seeds and", len(pool), "eligible species; saved-version compatibility intact.")
