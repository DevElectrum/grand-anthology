import assert from 'node:assert/strict';
import { readFile } from 'node:fs/promises';
import { fileURLToPath } from 'node:url';
import { test } from 'node:test';
import { normalizeSourceEntries } from './normalize-sources.mjs';

const sourcePath = fileURLToPath(new URL('../../data/catalog/world_acquisition.jsonl', import.meta.url));
const raw = (await readFile(sourcePath, 'utf8')).trim().split(/\r?\n/).map(JSON.parse);
const normalized = normalizeSourceEntries(raw);
function find(kind, name) {
  return normalized.find(entry => entry.kind === kind && entry.name === name);
}
function countAt(entry, map, form) {
  return entry.sources.filter(source => source.map === map && (!form || source.form === form)).length;
}

test('two catalog descriptions of one scripted static are displayed once', () => {
  assert.equal(countAt(find('pokemon', 'ZYGARDE'), 'Route101', 'SPECIES_ZYGARDE_COMPLETE'), 1);
  assert.equal(countAt(find('pokemon', 'TERAPAGOS'), 'CaveOfOrigin_B1F', 'SPECIES_TERAPAGOS_STELLAR'), 1);
  assert.equal(find('pokemon', 'ZYGARDE').sources.length, 3);
  assert.equal(find('pokemon', 'TERAPAGOS').sources.length, 2);
});

test('named mega stone pickup and its script alias are displayed once', () => {
  const sites = [
    ['ZYGARDITE', 'Route101'],
    ['HEATRANITE', 'RockTunnel_B1F_Frlg'],
    ['DARKRANITE', 'PokemonTower_7F_Frlg'],
    ['ZERAORITE', 'PowerPlant_Frlg'],
    ['MAGEARNITE', 'SilphCo_7F_Frlg']
  ];
  for (const [name, map] of sites) {
    const entry = find('item', name);
    assert.equal(countAt(entry, map), 1, name);
    assert.equal(entry.sources.find(source => source.map === map)?.method, 'overworld_item_ball');
  }
});

test('different ways to obtain a Pokémon or stone must not be collapsed', () => {
  const golduck = find('pokemon', 'GOLDUCK');
  const site = golduck.sources.filter(source => source.map === 'MAP_JOHTO_VICTORY_ROAD_B2F');
  assert.deepEqual(site.map(source => source.method).sort(), ['native_fishing_mons', 'native_water_mons']);
  const venusaurite = find('item', 'VENUSAURITE');
  assert.deepEqual(venusaurite.sources.map(source => source.method).sort(), ['battle_net_free_starter_choice', 'battle_net_vendor']);
  const mew = find('pokemon', 'MEW');
  assert.equal(mew.sources.length, 2);
});

test('normalizer does not mutate source catalog or hide distinct forms or levels', () => {
  const original = raw.find(entry => entry.kind === 'pokemon' && entry.name === 'ZYGARDE');
  assert.equal(original.sources.length, 4);
  const synthetic = [{
    kind: 'pokemon', name: 'TEST', sources: [
      { method: 'permanent_complete_static', map: 'Route101', form: 'FORM_A', level: 70, source: 'file' },
      { method: 'scripted_battle_candidate', map: 'Route101', form: 'FORM_B', level: 70, source: 'file' },
      { method: 'scripted_battle_candidate', map: 'Route101', form: 'FORM_A', level: 71, source: 'file' }
    ]
  }];
  assert.equal(normalizeSourceEntries(synthetic)[0].sources.length, 3);
  assert.equal(synthetic[0].sources.length, 3);
});
