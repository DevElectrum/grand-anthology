import assert from 'node:assert/strict';
import { test } from 'node:test';
import { readFile } from 'node:fs/promises';
import { fileURLToPath } from 'node:url';
import { mix32, seededWild, seededStarters, buildWildAtlas, buildStarterAtlas } from '../src/lib/randomizer.mjs';
import { exportRandomizerData, parseSpeciesEnum, parseStarterSlots } from './export-randomizer.mjs';

function eligibleData(n = 170) {
  return Array.from({ length: n }, (_, id) => ({
    id, symbol: 'SPECIES_' + id, dex: id <= 0 ? null : Math.max(1, Math.floor(id / 2)),
    fieldStable: id > 0 && id < n && id % 13 !== 0,
    starterEligible: id > 0 && id < n && id % 13 !== 0
  }));
}

test('C-style hash avalanche is unsigned and stable', () => {
  const u32 = v => v >>> 0;
  function mirror(seed, original, slot, area, group, num, count) {
    let hash = u32(0x9E3779B9 ^ seed);
    hash = u32(hash ^ u32(original * 0x85EBCA6B));
    hash = u32(hash ^ u32((slot + 1) * 0xC2B2AE35));
    hash = u32(hash ^ ((group << 24) | (num << 16) | (area << 8)));
    hash ^= hash >>> 16;
    hash = Math.imul(hash, 0x7FEB352D) >>> 0;
    hash ^= hash >>> 15;
    hash = Math.imul(hash, 0x846CA68B) >>> 0;
    hash ^= hash >>> 16;
    return 1 + u32(hash) % (count - 1);
  }
  const species = eligibleData(1500);
  for (const seed of [0, 1, 2026, 2027, 32768, 65535]) {
    for (const area of [0, 1, 2, 3]) {
      const raw = mirror(seed, 25, 3, area, 37, 4, species.length);
      let expect = raw;
      while (!species[expect].fieldStable)
        expect = expect + 1 >= species.length ? 1 : expect + 1;
      assert.equal(seededWild(seed, 25, 3, area, 37, 4, species), expect);
    }
  }
  assert.equal(mix32(0x12345678), mix32(0x12345678));
  assert.throws(() => seededWild(65536, 25, 0, 0, 0, 0, species), RangeError);
});

test('all 65536 seed values produce deterministic, valid species indices', () => {
  const species = eligibleData();
  const seen = new Set();
  for (let seed = 0; seed < 65536; seed++) {
    const mon = seededWild(seed, 25, 3, 0, 40, 1, species);
    assert.ok(species[mon].fieldStable, 'Invalid wild result at seed ' + seed);
    seen.add(mon);
  }
  assert.ok(seen.size > 100);
});

test('starter slots avoid duplicates by National Dex, even across alternate forms', () => {
  const species = eligibleData(220);
  const roster = { group: 1, num: 2, slots: [1, 3, 5, 7, 9, 11, 15, 17, 19, 21] };
  for (let seed = 0; seed < 1000; seed++) {
    const picked = seededStarters(seed, roster, species);
    assert.equal(picked.length, 10);
    assert.equal(new Set(picked.map(x => species[x].dex)).size, 10);
    assert.deepEqual(seededStarters(seed, roster, species), picked);
  }
});

test('source Species enum and regional roster parsing match the live game declarations', async () => {
  const enums = await readFile(new URL('../../include/constants/species.h', import.meta.url), 'utf8');
  const starters = await readFile(new URL('../../src/world_starter_randomizer.c', import.meta.url), 'utf8');
  const { byName, count } = parseSpeciesEnum(enums);
  const rosters = parseStarterSlots(starters);
  assert.equal(byName.SPECIES_BULBASAUR, 1);
  assert.equal(byName.SPECIES_MEW, 151);
  assert.ok(count >= 1500);
  assert.equal(rosters.Kanto.length, 10);
  assert.equal(rosters.Johto.length, 9);
  assert.equal(rosters.Hoenn.length, 9);
});

test('generated seed atlas uses real game map and form indexes', async () => {
  const data = await exportRandomizerData();
  assert.equal(data.starters.Kanto.slots.length, 10);
  assert.ok(data.wildSlots.length > 2000);
  assert.ok(data.species.filter(s => s.fieldStable).length > 1000);
  assert.ok(data.wildSlots.some(x => x.map === 'MAP_ROUTE1' && x.area === 0));
  assert.ok(data.wildSlots.some(x => x.area === 3));
  const wild = buildWildAtlas(2026, data);
  assert.equal(wild.length, data.wildSlots.length);
  const starters = buildStarterAtlas(2026, data);
  for (const [region, selections] of Object.entries(starters)) {
    assert.equal(new Set(selections.map(x => data.species[x.id].dex)).size, data.starters[region].slots.length, region);
  }
  assert.notDeepEqual(buildStarterAtlas(2026, data), buildStarterAtlas(2027, data));
});
