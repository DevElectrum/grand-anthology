import assert from 'node:assert/strict';
import { test } from 'node:test';
import { readFile } from 'node:fs/promises';
import { fileURLToPath } from 'node:url';
import { mix32, seededWild, seededStarters, buildWildAtlas, buildStarterAtlas, findSeededPokemonSources, pokemonDexHref, seededStatic, buildStaticAtlas } from '../src/lib/randomizer.mjs';
import { exportRandomizerData, parseSpeciesEnum, parseStarterSlots } from './export-randomizer.mjs';
import { scriptStaticCommands } from './export-static-sites.mjs';

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


test('every seed resolves to legal game Pokémon using actual internal species indexes', async () => {
  const data = await exportRandomizerData();
  const unknown = data.species.filter(mon => mon.starterEligible && (!mon.dex || mon.dex > 1025));
  assert.deepEqual(unknown.map(mon => mon.symbol), [], 'No starter-eligible game form may lack National Dex identity');
  const target = data.wildSlots.find(row => row.map === 'MAP_ROUTE1' && row.area === 0);
  assert.ok(target, 'Route 1 source slot');
  for (let seed = 0; seed < 65536; seed++) {
    const id = seededWild(seed, target.original, target.slot, target.area,
      target.group, target.num, data.species);
    assert.ok(data.species[id].fieldStable, 'Invalid real-data mapping for seed ' + seed);
  }
  for (const seed of [0, 1, 100, 2026, 2027, 32768, 65535]) {
    for (const [region, roster] of Object.entries(data.starters)) {
      const ids = seededStarters(seed, roster, data.species);
      assert.equal(ids.length, roster.slots.length, region);
      assert.equal(new Set(ids.map(id => data.species[id].dex)).size, ids.length, region + ' seed ' + seed);
    }
  }
});


test('seed 02026 exposes Oak choice 7 Moltres in the unified Finder and Pokédex link', async () => {
  const data = await exportRandomizerData();
  const starterRows = buildStarterAtlas(2026, data);
  const wildRows = buildWildAtlas(2026, data);
  const oak = starterRows.Kanto.find(choice => choice.slot === 6);
  assert.equal(data.species[oak.id].symbol, 'SPECIES_MOLTRES');
  assert.equal(data.species[oak.original].symbol, 'SPECIES_ROWLET');
  assert.equal(pokemonDexHref(oak.id, data.species), '/pokedex/146/');

  const all = findSeededPokemonSources(wildRows, starterRows, data.species, { query: 'moltres' });
  assert.ok(all.starters.some(row => row.region === 'Kanto' && row.slot === 6 && row.id === oak.id));
  assert.ok(all.wild.length > 0, 'Wild Moltres sources must still be searchable');

  const starterOnly = findSeededPokemonSources(wildRows, starterRows, data.species,
    { query: 'moltres', method: 'starters' });
  assert.equal(starterOnly.wild.length, 0);
  assert.ok(starterOnly.starters.some(row => row.region === 'Kanto' && row.slot === 6));

  const grass = findSeededPokemonSources(wildRows, starterRows, data.species,
    { query: 'moltres', method: '0' });
  assert.equal(grass.starters.length, 0);
  assert.ok(grass.wild.every(row => row.area === 0));

  const johto = findSeededPokemonSources(wildRows, starterRows, data.species,
    { query: 'moltres', region: 'JOHTO' });
  assert.ok(!johto.starters.some(row => row.region === 'Kanto'));

  const nonRandomized = findSeededPokemonSources(wildRows, buildStarterAtlas(2026, data, false),
    data.species, { query: 'moltres', method: 'starters' });
  assert.equal(nonRandomized.starters.length, 0);

  for (const roster of Object.values(starterRows)) {
    for (const choice of roster) {
      assert.match(pokemonDexHref(choice.id, data.species), /^\/pokedex\/\d+\/$/);
    }
  }
  assert.equal(pokemonDexHref(0, data.species), null);
});


test('static C hash matches independent 32-bit mixing and each protection policy', () => {
  const species = eligibleData(180).map(x => ({...x, legendaryStatic: x.id % 11 === 0,
    ultraBeast: x.id === 42, paradox: x.id === 44 }));
  const original = 22, seed = 2026, slot = 0, group = 38, num = 7;
  assert.equal(seededStatic(seed, original, slot, group, num, species, 0), original);
  assert.equal(seededStatic(seed, original, slot, group, num, species, 1),
    seededStatic(seed, original, slot, group, num, species, 1));
  assert.equal(seededStatic(seed, 42, 0, group, num, species, 0), 42);
  assert.equal(seededStatic(seed, 44, 0, group, num, species, 0), 44);

  // Same map/slot hashing as C, but independent from the tested function.
  function referenceHash(id, index) {
    let h = (0x9E3779B9 ^ seed) >>> 0;
    h ^= Math.imul(id, 0x85EBCA6B);
    h ^= Math.imul(index + 1, 0xC2B2AE35);
    h ^= (group << 24) | (num << 16) | 0x5300;
    h ^= h >>> 16;
    h = Math.imul(h, 0x7FEB352D) >>> 0;
    h ^= h >>> 15;
    h = Math.imul(h, 0x846CA68B) >>> 0;
    h ^= h >>> 16;
    h >>>= 0;
    let candidate = 1 + h % (species.length - 1);
    while (!species[candidate].fieldStable) {
      candidate++;
      if (candidate >= species.length) candidate = 1;
    }
    return candidate;
  }
  assert.equal(seededStatic(seed, 25, slot, group, num, species, 2), referenceHash(25, 0));
  assert.equal(seededStatic(seed, 25, 1, group, num, species, 2), referenceHash(25, 1));
  assert.equal(seededStatic(seed, 25, 0, group, num, species, 2, false), 25);
  assert.throws(() => seededStatic(65536, 25, 0, group, num, species, 2), RangeError);
});

test('script parser finds concrete static calls and does not infer nested double-battle slot 1', () => {
  const script = [
    'Some_EventScript_Double::',
    '  setwildbattle SPECIES_ZACIAN_HERO, 50, ITEM_NONE, SPECIES_ZAMAZENTA_HERO, 50, ITEM_NONE',
    '  dowildbattle',
    'Another_EventScript_Foe::',
    '  seteventmon SPECIES_MEW, 30',
    '  special BattleSetup_StartLegendaryBattle',
    '@ seteventmon SPECIES_DITTO, 42'
  ].join('\n');
  const result = scriptStaticCommands(script, 'data/maps/Example/scripts.inc');
  assert.equal(result.length, 3);
  assert.deepEqual(result.slice(0,2).map(x=>x.slot), [0,0]);
  assert.deepEqual(result.slice(0,2).map(x=>x.symbol),
    ['SPECIES_ZACIAN_HERO','SPECIES_ZAMAZENTA_HERO']);
  assert.equal(result[2].scriptLabel, 'Another_EventScript_Foe');
});

test('actual scripted static sources and three modes mirror current game source', async () => {
  const data = await exportRandomizerData();
  const sites = data.staticSites;
  assert.ok(sites.length >= 70, 'Static sources are suspiciously sparse: ' + sites.length);
  const legendaryCount = data.species.filter(mon => mon.legendaryStatic && mon.fieldStable).length;
  console.log('Static pool diagnostic', legendaryCount,
    data.species.filter(mon => mon.legendaryStatic).length,
    data.species.filter(mon => mon.ultraBeast).length,
    ['SPECIES_ARTICUNO', 'SPECIES_MEWTWO', 'SPECIES_MEW', 'SPECIES_RAYQUAZA', 'SPECIES_JIRACHI'].map(symbol => {
      const entry = data.species.find(mon => mon.symbol === symbol);
      return [symbol, entry?.id, entry?.legendaryStatic, entry?.fieldStable];
    }));
  assert.ok(legendaryCount >= 50, 'Legendary pool is missing configured C flags: ' + legendaryCount);
  assert.ok(data.species.some(mon => mon.ultraBeast), 'Ultra Beast flags missing');
  assert.ok(data.species.some(mon => mon.paradox), 'Paradox flags missing');
  assert.ok(data.staticAudit.notMatched.length < 25,
    'Too many catalog source candidates lack matching source script commands');
  assert.ok(sites.every(site => site.original >= 1 && site.original < data.species.length));
  const jirachi = sites.find(site => site.map === 'MAP_ROUTE102'
    && data.species[site.original].symbol === 'SPECIES_JIRACHI');
  assert.ok(jirachi, 'Early Route 102 Jirachi scripted battle');
  const ordinary = buildStaticAtlas(2026, data, 0);
  assert.equal(ordinary.find(x => x.id === jirachi.id).speciesId, jirachi.original,
    'Ordinary mode preserves Legendary/Mythical statics');
  const legendary = buildStaticAtlas(2026, data, 1);
  assert.notEqual(legendary.find(x => x.id === jirachi.id).speciesId, jirachi.original,
    'Legendary Shuffle rotates Jirachi in normal non-zero seed');
  const pool = data.species.filter(mon => mon?.legendaryStatic && mon.fieldStable).map(x=>x.id);
  const permuted = pool.map(id => seededStatic(2026, id, 0, jirachi.group, jirachi.num, data.species, 1));
  assert.equal(new Set(permuted).size, pool.length, 'Legendary Shuffle must be one-to-one');

  const duo = sites.filter(site => site.method === 'royal_duo');
  assert.equal(duo.length, 2);
  assert.deepEqual(duo.map(x=>x.slot), [0, 1]);
  assert.deepEqual(duo.map(x=>data.species[x.original].symbol),
    ['SPECIES_ZACIAN_HERO','SPECIES_ZAMAZENTA_HERO']);
  assert.ok(duo.every(x => x.acquisition === 'defeat_to_receive_both'));
  const chaos = buildStaticAtlas(2026, data, 2);
  assert.equal(chaos.length, sites.length);
  for (const row of chaos)
    assert.ok(data.species[row.speciesId].fieldStable);
  const normal = buildStaticAtlas(2026, data, 2, false);
  assert.ok(normal.every(x => x.original === x.speciesId));
});
