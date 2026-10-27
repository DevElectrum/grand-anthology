import { readFile, readdir, writeFile, mkdir } from 'node:fs/promises';
import { dirname, join, resolve } from 'node:path';
import { fileURLToPath } from 'node:url';
import { createHash } from 'node:crypto';
import { exportStaticSites } from './export-static-sites.mjs';

const ROOT = resolve(dirname(fileURLToPath(import.meta.url)), '../..');
const OUTPUT = resolve(ROOT, 'website/src/generated/randomizer-data.json');

export function parseSpeciesEnum(source) {
  const body = source.split(/enum\s+__attribute__\(\(packed\)\)\s+Species\s*\{/)[1]?.split(/\n\s*\};/)[0];
  if (!body) throw new Error('Species enum not found');
  const cleaned = body.replace(/\/\*[\s\S]*?\*\//g, '').replace(/\/\/[^\n]*/g, '');
  const byName = {};
  const byId = {};
  let previous = -1;
  for (const token of cleaned.split(',')) {
    const part = token.trim();
    if (!part) continue;
    const match = part.match(/^([A-Z][A-Z0-9_]+)(?:\s*=\s*(\d+|[A-Z][A-Z0-9_]+))?$/);
    if (!match) throw new Error('Unsupported Species enum entry: ' + part);
    const [, name, assignment] = match;
    if (assignment === undefined) previous++;
    else if (/^\d+$/.test(assignment)) previous = Number(assignment);
    else if (byName[assignment] !== undefined) previous = byName[assignment];
    else throw new Error('Unresolved Species enum alias ' + assignment);
    byName[name] = previous;
    if (!byId[previous]) byId[previous] = name;
  }
  if (byName.SPECIES_BULBASAUR !== 1 || byName.SPECIES_CHARIZARD !== 6
      || byName.NUM_SPECIES !== byName.SPECIES_EGG)
    throw new Error('Unexpected internal Species enum layout');
  return { byName, byId, count: byName.NUM_SPECIES };
}

export function parseStarterSlots(source) {
  const output = {};
  for (const [region, name, count] of [
    ['Kanto', 'sKantoStarterSlots', 10],
    ['Johto', 'sJohtoStarterSlots', 9],
    ['Hoenn', 'sHoennStarterSlots', 9]
  ]) {
    const match = source.match(new RegExp('\\b' + name + '\\[\\]\\s*=\\s*\\{([^}]+)\\}'));
    if (!match) throw new Error('Missing game starter roster ' + region);
    const slots = [...match[1].matchAll(/\bSPECIES_[A-Z0-9_]+\b/g)].map(item => item[0]);
    if (slots.length !== count) throw new Error('Unexpected ' + region + ' starter slot count');
    output[region] = slots;
  }
  return output;
}

function gameCompiledVariant(row) {
  // Current game build has OW_TIME_OF_DAY_ENCOUNTERS=FALSE with MORNING fallback.
  // FireRed and Emerald tables compile; duplicate LeafGreen headers do not.
  const label = row.base_label ?? '';
  return !label.includes('LeafGreen')
    && !/(?:_|^)(?:Day|Evening|Night)(?:_|$)/.test(label);
}

export async function exportRandomizerData(root = ROOT) {
  const rawSources = await Promise.all([
    'include/constants/species.h',
    'src/world_starter_randomizer.c',
    'src/data/wild_encounters.json',
    'data/maps/map_groups.json',
    'website/src/generated/pokemon-details.json',
    'include/config/overworld.h'
  ].map(async name => readFile(resolve(root, name), 'utf8')));
  const [enumText, starterText, wildText, groupsText, detailsText, config] = rawSources;
  if (!/#define OW_TIME_OF_DAY_ENCOUNTERS\s+FALSE\b/.test(config)
      || !/#define OW_TIME_OF_DAY_FALLBACK\s+TIME_MORNING\b/.test(config))
    throw new Error('Encounter time configuration changed: update Atlas variant extraction');
  const speciesEnum = parseSpeciesEnum(enumText);
  const rosters = parseStarterSlots(starterText);
  const wild = JSON.parse(wildText);
  const groups = JSON.parse(groupsText);
  const details = JSON.parse(detailsText);
  const mapIndex = {};
  for (let groupId = 0; groupId < groups.group_order.length; groupId++) {
    const group = groups.group_order[groupId];
    for (let mapNum = 0; mapNum < groups[group].length; mapNum++) {
      const folder = groups[group][mapNum];
      const location = JSON.parse(await readFile(resolve(root, 'data/maps', folder, 'map.json'), 'utf8'));
      if (mapIndex[location.id]) throw new Error('Duplicate map enum id: ' + location.id);
      mapIndex[location.id] = { group: groupId, num: mapNum, label: folder,
        region: location.region?.replace(/^REGION_/, '') ?? null,
        frontier: location.battle_scene === 'MAP_BATTLE_SCENE_FRONTIER' };
    }
  }
  const species = Array.from({ length: speciesEnum.count }, (_, id) => {
    const symbol = speciesEnum.byId[id];
    const profile = details.species[symbol];
    const flags = profile?.randomizerFlags ?? {};
    // The C engine uses gSpeciesInfo[id].baseHP > 0 for enabled checks.
    const enabled = Boolean(profile?.stats?.baseHP > 0);
    const excludedForm = ['isMegaEvolution', 'isPrimalReversion', 'isUltraBurst',
      'isGigantamax', 'isTeraForm', 'isTotem'].some(flag => flags[flag]);
    const transient = new Set([
      'SPECIES_GRENINJA_ASH', 'SPECIES_AEGISLASH_BLADE', 'SPECIES_WISHIWASHI_SCHOOL',
      'SPECIES_MIMIKYU_BUSTED', 'SPECIES_PALAFIN_HERO',
      'SPECIES_CRAMORANT_GULPING', 'SPECIES_CRAMORANT_GORGING',
      'SPECIES_MORPEKO_HANGRY', 'SPECIES_ZYGARDE_COMPLETE',
      'SPECIES_CASTFORM_SUNNY', 'SPECIES_CASTFORM_RAINY', 'SPECIES_CASTFORM_SNOWY',
      'SPECIES_DARMANITAN_ZEN', 'SPECIES_DARMANITAN_GALAR_ZEN',
      'SPECIES_CHERRIM_SUNSHINE', 'SPECIES_EISCUE_NOICE',
      'SPECIES_MINIOR_CORE_RED', 'SPECIES_MINIOR_CORE_ORANGE',
      'SPECIES_MINIOR_CORE_YELLOW', 'SPECIES_MINIOR_CORE_GREEN',
      'SPECIES_MINIOR_CORE_BLUE', 'SPECIES_MINIOR_CORE_INDIGO',
      'SPECIES_MINIOR_CORE_VIOLET'
    ]).has(symbol);
    return { id, symbol, dex: profile?.dex ?? null,
      name: profile?.name ?? symbol?.replace(/^SPECIES_/, '').replace(/_/g, ' ') ?? 'Unknown',
      enabled, fieldStable: enabled && !excludedForm && !transient,
      starterEligible: enabled && !excludedForm,
      legendaryStatic: Boolean(flags.isRestrictedLegendary || flags.isSubLegendary || flags.isMythical),
      ultraBeast: Boolean(flags.isUltraBeast),
      paradox: Boolean(flags.isParadox) };
  });
  if (species.filter(mon => mon?.fieldStable).length < 1000)
    throw new Error('Too few field-stable Pokémon; verify game-form flag extraction');

  const mapSlots = [];
  const methodFields = [
    ['land_mons', 0, 'Grass'],
    ['water_mons', 1, 'Surfing'],
    ['rock_smash_mons', 2, 'Rock Smash'],
    ['fishing_mons', 3, 'Fishing']
  ];
  const primary = wild.wild_encounter_groups.find(group => group.label === 'gWildMonHeaders')
    ?? wild.wild_encounter_groups.find(group => group.for_maps);
  if (!primary) throw new Error('Missing game wild headers');
  const variants = new Map();
  for (const row of primary.encounters) {
    if (!gameCompiledVariant(row)) continue;
    for (const [key, area] of methodFields) {
      if (!row[key]?.encounter_rate) continue;
      const id = row.map + ':' + area;
      if (!variants.has(id)) variants.set(id, new Set());
      variants.get(id).add(row.base_label);
    }
  }
  for (const row of primary.encounters) {
    if (!gameCompiledVariant(row)) continue;
    const map = mapIndex[row.map];
    if (!map) throw new Error('Wild encounter references unknown map ' + row.map);
    for (const [key, area, method] of methodFields) {
      const value = row[key];
      if (!value || !value.encounter_rate || !value.mons?.length) continue;
      // Some inherited areas, notably Altering Cave, have multiple authored
      // runtime encounter tables. Preserve them as explicitly labeled variants
      // rather than pretending every variant is simultaneously available.
      const alternate = variants.get(row.map + ':' + area)?.size > 1;
      for (const [slot, mon] of value.mons.entries()) {
        const original = speciesEnum.byName[mon.species];
        if (original == null) throw new Error('Unrecognized encounter species ' + mon.species);
        mapSlots.push({
          map: row.map, group: map.group, num: map.num, region: map.region,
          area, method, slot, original, variant: alternate ? row.base_label : null,
          min: mon.min_level ?? 2, max: mon.max_level ?? 100,
          rate: value.encounter_rate,
          rod: area !== 3 ? null : slot < 2 ? 'Old Rod' : slot < 5 ? 'Good Rod' : 'Super Rod'
        });
      }
    }
  }
  const starterMaps = {
    Kanto: 'MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB',
    Johto: 'MAP_NEW_BARK_TOWN_LAB',
    Hoenn: 'MAP_ROUTE101'
  };
  const starters = {};
  for (const [region, roster] of Object.entries(rosters)) {
    if (!mapIndex[starterMaps[region]]) throw new Error('Missing starter map ' + region);
    starters[region] = { map: starterMaps[region], group: mapIndex[starterMaps[region]].group,
      num: mapIndex[starterMaps[region]].num,
      slots: roster.map(symbol => {
        if (speciesEnum.byName[symbol] == null) throw new Error('Unknown starter ' + symbol);
        return speciesEnum.byName[symbol];
      }) };
  }
  const catalog = (await readFile(resolve(root, 'data/catalog/world_acquisition.jsonl'), 'utf8'))
    .split(/\r?\n/).filter(Boolean).map(JSON.parse);
  const royalSource = await readFile(resolve(root, 'src/script_pokemon_util.c'), 'utf8');
  const staticSources = await exportStaticSites({
    root, catalog, maps: mapIndex, byName: speciesEnum.byName, royalSource
  });
  const output = {
    schema: 2, gameMode: 'New Game / randomizerVersion 1',
    fingerprint: createHash('sha256').update(enumText + starterText + wildText + detailsText + royalSource).digest('hex').slice(0, 16),
    species, maps: mapIndex, wildSlots: mapSlots, starters,
    staticSites: staticSources.sites,
    staticAudit: {
      matched: staticSources.sites.length,
      notMatched: staticSources.unmatched,
      scannedScripts: staticSources.indexedScriptFiles
    }
  };
  if (mapSlots.length < 2000) throw new Error('Suspiciously sparse wild slot export');
  return output;
}

if (process.argv[1] && resolve(process.argv[1]) === fileURLToPath(import.meta.url)) {
  const data = await exportRandomizerData();
  await mkdir(dirname(OUTPUT), { recursive: true });
  await writeFile(OUTPUT, JSON.stringify(data));
  console.log('Randomizer Atlas data: ' + data.wildSlots.length + ' wild slots, ' +
    data.species.length + ' internal species IDs, ' +
    Object.keys(data.maps).length + ' maps, ' + data.staticSites.length +
    ' source-matched statics (' + data.staticAudit.notMatched.length +
    ' catalog candidates unmatched); snapshot ' + data.fingerprint);
}
