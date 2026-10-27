import { readFile } from 'node:fs/promises';
import { resolve } from 'node:path';

// Only index sources whose actual map scripts contain a concrete species
// and level instruction. A machine catalog entry alone is not battle proof.
// The script is retained as source evidence; map/story gates need gameplay QA.
export function scriptStaticCommands(script, source) {
  const result = [];
  const lines = script.split(/\r?\n/);
  let label = null;
  for (let i = 0; i < lines.length; i++) {
    const text = lines[i].replace(/@.*$/, '').replace(/\/\/.*$/, '').trim();
    const matchLabel = text.match(/^([A-Za-z][A-Za-z0-9_]+):{1,2}$/);
    if (matchLabel) label = matchLabel[1];
    const match = text.match(/^(setwildbattle|seteventmon)\s+(.+)$/i);
    if (!match) continue;
    const params = match[2].split(',').map(x => x.trim());
    if (!/^SPECIES_[A-Z0-9_]+$/.test(params[0])) continue;
    if (!/^\d+$/.test(params[1])) continue;
    const one = { symbol: params[0], level: Number(params[1]), slot: 0 };
    const second = params[3]?.match(/^SPECIES_[A-Z0-9_]+$/) ? {
      symbol: params[3], level: Number(params[4]), slot: 0
    } : null;
    const before = lines.slice(Math.max(0, i - 15), i).join('\n');
    for (const mon of [one, second].filter(Boolean)) {
      if (!Number.isInteger(mon.level) || mon.level < 1 || mon.level > 100) continue;
      result.push({
        source, line: i + 1, scriptLabel: label,
        method: match[1].toLowerCase(),
        noCatch: /setflag\s+FLAG_SYS_NO_CATCHING/.test(before),
        ...mon
      });
    }
  }
  return result;
}

export async function exportStaticSites({ root, catalog, maps, byName, royalSource }) {
  const relevant = new Map();
  for (const pokemon of catalog) {
    if (pokemon.kind !== 'pokemon') continue;
    for (const source of pokemon.sources ?? []) {
      if (source.method !== 'scripted_battle_candidate') continue;
      if (!source.source?.startsWith('data/maps/') || !source.source.endsWith('/scripts.inc')) continue;
      if (!relevant.has(source.source)) relevant.set(source.source, []);
      relevant.get(source.source).push({
        symbol: source.form ?? pokemon.symbol, level: source.level,
        dex: pokemon.dex, map: source.map
      });
    }
  }

  const sites = [];
  const unmatched = [];
  const scanned = await Promise.all([...relevant.entries()].map(async ([source, candidates]) => {
    const script = await readFile(resolve(root, source), 'utf8');
    return { source, candidates, commands: scriptStaticCommands(script, source) };
  }));

  for (const { source, candidates, commands } of scanned) {
    const folder = source.split('/')[2];
    const mapEntry = Object.entries(maps).find(([, entry]) => entry.label === folder);
    if (!mapEntry) throw new Error('Scripted static map missing from map_groups: ' + source);
    const [map, coords] = mapEntry;
    const selected = new Map();
    for (const candidate of candidates) {
      const matches = commands.filter(item => item.symbol === candidate.symbol
        && item.level === candidate.level);
      if (!matches.length) {
        unmatched.push({ ...candidate, source });
        continue;
      }
      for (const item of matches) {
        const id = source + ':' + item.line + ':' + item.symbol + ':' + item.slot;
        selected.set(id, {
          id, source, sourceLine: item.line, scriptLabel: item.scriptLabel,
          map, region: coords.region, group: coords.group, num: coords.num,
          original: byName[item.symbol], slot: item.slot, level: item.level,
          method: item.method, noCatch: item.noCatch,
          acquisition: item.noCatch ? 'battle_only' : 'conditional_scripted_encounter',
          sourceConfidence: 'script_command_matched'
        });
      }
    }
    for (const item of selected.values()) {
      if (item.original === undefined)
        throw new Error('Unknown scripted static species in internal enum: ' + item.id);
      sites.push(item);
    }
  }

  // Special Royale Duo uses a C constructor with deliberately distinct
  // slots 0 and 1. Battle is no-catch; after winning, both are awarded.
  if (!/WorldRandomizeStaticSpecies\(original\[i\], i\)/.test(royalSource)
      || !/SPECIES_ZACIAN_HERO,\s*SPECIES_ZAMAZENTA_HERO/.test(royalSource)
      || !/CreateMonWithIVs\(mon, species, 50,/.test(royalSource))
    throw new Error('Royal Duo special static constructor changed');
  const route27 = maps.MAP_ROUTE27;
  if (!route27) throw new Error('Route 27 Royal Duo map missing');
  for (const [slot, symbol] of ['SPECIES_ZACIAN_HERO', 'SPECIES_ZAMAZENTA_HERO'].entries()) {
    sites.push({
      id: 'RoyalDuoPrepareBattle:' + slot, source: 'src/script_pokemon_util.c',
      scriptLabel: 'Route27_EventScript_RoyalDuo', map: 'MAP_ROUTE27',
      region: route27.region, group: route27.group, num: route27.num,
      original: byName[symbol], slot, level: 50, method: 'royal_duo',
      noCatch: true, acquisition: 'defeat_to_receive_both',
      sourceConfidence: 'special_C_constructor'
    });
    if (byName[symbol] == null) throw new Error('Royal Duo species enum missing: ' + symbol);
  }
  sites.sort((a,b)=>a.map.localeCompare(b.map)||a.sourceLine-b.sourceLine||a.slot-b.slot);
  return { sites, unmatched, indexedScriptFiles: relevant.size };
}
