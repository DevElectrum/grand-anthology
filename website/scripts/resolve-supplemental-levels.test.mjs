import { test } from 'node:test';
import assert from 'node:assert/strict';
import { readFile } from 'node:fs/promises';
import { fileURLToPath } from 'node:url';
import { enrichSupplementalLevels } from './resolve-supplemental-levels.mjs';

function native(min, max) {
  return { min_level: min, max_level: max, species: 'SPECIES_TEST' };
}

const source = [{
  kind: 'pokemon', dex: 133, name: 'EEVEE',
  sources: [
    { method: 'supplemental_land', map: 'MAP_ROUTE115' },
    { method: 'supplemental_water', map: 'MAP_ROUTE115' },
    { method: 'supplemental_fishing', map: 'MAP_ROUTE115' },
    { method: 'scripted_gift_candidate', map: 'Example', level: 25 }
  ]
}];

const map = {
  wild_encounter_groups: [{
    for_maps: true,
    encounters: [
      {
        map: 'MAP_ROUTE115', base_label: 'Route115_Morning',
        land_mons: { encounter_rate: 20, mons: [native(11, 15), native(1, 3)] },
        water_mons: { encounter_rate: 30, mons: [native(18, 22), native(4, 8)] },
        fishing_mons: { encounter_rate: 40, mons: [
          native(3, 5), native(5, 6), native(10, 12), native(13, 16),
          native(14, 18), native(20, 24), native(21, 25), native(25, 29),
          native(30, 31), native(33, 35)
        ] }
      },
      {
        map: 'MAP_ROUTE115', base_label: 'Route115_LeafGreen',
        land_mons: { encounter_rate: 20, mons: [native(90, 95)] }
      },
      {
        map: 'MAP_ROUTE115', base_label: 'Route115_Night',
        land_mons: { encounter_rate: 20, mons: [native(80, 85)] }
      }
    ]
  }]
};

test('uses first slot rather than all native slots for grass and Surf levels', () => {
  const { records, counters } = enrichSupplementalLevels(source, map);
  assert.deepEqual(records[0].sources[0].level_min, 11);
  assert.deepEqual(records[0].sources[0].level_max, 15);
  assert.deepEqual(records[0].sources[1].level_min, 18);
  assert.deepEqual(records[0].sources[1].level_max, 22);
  assert.equal(counters.resolved, 3);
  assert.equal(source[0].sources[0].level_min, undefined, 'no catalog mutation');
  assert.equal(records[0].sources[3].level, 25, 'gift remains unchanged');
});

test('fishing levels retain exact rod ranges, not a guessed species level', () => {
  const { records } = enrichSupplementalLevels(source, map);
  const fishing = records[0].sources[2];
  assert.deepEqual(fishing.rod_levels, [
    { rod: 'Old Rod', min: 3, max: 6 },
    { rod: 'Good Rod', min: 10, max: 18 },
    { rod: 'Super Rod', min: 20, max: 35 }
  ]);
  assert.equal(fishing.level_min, 3);
  assert.equal(fishing.level_max, 35);
});

test('nonexistent wild tables remain unknown instead of fabricated', () => {
  const { records, counters } = enrichSupplementalLevels(
    [{ kind: 'pokemon', sources: [{ map: 'MAP_NOWHERE', method: 'supplemental_land' }] }],
    map
  );
  assert.equal(records[0].sources[0].level_min, undefined);
  assert.equal(counters.unresolved, 1);
});

test('real-game Eevee and aquatic source levels derive from current wild tables', async () => {
  const repository = fileURLToPath(new URL('../../', import.meta.url));
  const rawCatalog = await readFile(new URL('../../data/catalog/world_acquisition.jsonl', import.meta.url), 'utf8');
  const records = rawCatalog.trim().split(/\r?\n/).map(JSON.parse);
  const wildData = JSON.parse(await readFile(new URL('../../src/data/wild_encounters.json', import.meta.url), 'utf8'));
  const enriched = enrichSupplementalLevels(records, wildData);
  const eevee = enriched.records.find(entry => entry.kind === 'pokemon' && entry.name === 'EEVEE');
  const habitats = eevee.sources.filter(entry => entry.method === 'supplemental_land');
  assert.equal(habitats.length, 3);
  for (const entry of habitats) {
    assert.ok(Number.isInteger(entry.level_min), entry.map);
    assert.ok(Number.isInteger(entry.level_max), entry.map);
  }
  const feebas = enriched.records.find(entry => entry.kind === 'pokemon' && entry.name === 'FEEBAS');
  const fishing = feebas.sources.find(entry => entry.method === 'supplemental_fishing');
  assert.ok(fishing?.rod_levels?.length, 'Route 120 fishing uses real rod slots');
  assert.ok(enriched.counters.resolved > 100, 'supplemental habitats should be resolved broadly');
});
