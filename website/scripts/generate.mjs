import { readFile, mkdir, writeFile, copyFile } from 'node:fs/promises';
import { dirname, resolve } from 'node:path';
import { fileURLToPath } from 'node:url';
import { normalizeSourceEntries } from './normalize-sources.mjs';
import { execFileSync } from 'node:child_process';

const website = resolve(dirname(fileURLToPath(import.meta.url)), '..');
const repository = resolve(website, '..');
const catalogPath = resolve(repository, 'data/catalog/world_acquisition.jsonl');
const output = resolve(website, 'src/generated/catalog.json');

const raw = await readFile(catalogPath, 'utf8');
const rawRecords = raw.split(/\r?\n/).filter(Boolean).map((line, index) => {
  try { return JSON.parse(line); }
  catch (error) { throw new Error(`Invalid catalog JSON at line ${index + 1}: ${error.message}`); }
});
const records = normalizeSourceEntries(rawRecords);
execFileSync('python3', [resolve(website, 'scripts/export_species.py')], { cwd: repository, stdio: 'inherit' });

const meta = records.find(record => record.kind === 'meta');
const pokemon = records.filter(record => record.kind === 'pokemon');
const items = records.filter(record => record.kind === 'item');
const megaStones = items.filter(item => item.mega_stone);
if (!meta || pokemon.length !== 1025 || new Set(pokemon.map(mon => mon.dex)).size !== 1025)
  throw new Error('Expected metadata and 1,025 distinct National Pokédex entries');
if (items.length !== 874 || megaStones.length !== 92)
  throw new Error('Catalog item / Mega Stone counts do not match the current schema');
if (pokemon.some(mon => !Array.isArray(mon.sources) || !Array.isArray(mon.evolves_from)))
  throw new Error('One or more Pokémon records have invalid source or evolution fields');
if ([...pokemon, ...items].some(entry => !Array.isArray(entry.sources)))
  throw new Error('One or more catalog records have invalid source fields');

const mapGroups = JSON.parse(await readFile(resolve(repository, 'data/maps/map_groups.json'), 'utf8'));
const regionByMap = new Map();
let currentRegion = 'Hoenn';
for (const group of mapGroups.group_order) {
  if (group === 'gMapGroup_Link_Frlg') currentRegion = 'Kanto';
  if (group === 'gMapGroup_JohtoTownsAndRoutes') currentRegion = 'Johto';
  if (group === 'gMapGroup_RegionHub') currentRegion = null;
  for (const map of mapGroups[group] ?? []) {
    if (currentRegion) regionByMap.set(map, currentRegion);
  }
}
const locationMap = new Map();
for (const entry of [...pokemon, ...items]) {
  for (const source of entry.sources) {
    if (!source.map || !source.method) continue;
    let location = locationMap.get(source.map);
    if (!location) {
      const slug = source.map.toLowerCase().replace(/[^a-z0-9]+/g, '-').replace(/^-|-$/g, '');
      location = { id: source.map, slug, regions: [], entries: [] };
      locationMap.set(source.map, location);
    }
    const mappedRegion = regionByMap.get(source.map)
      ?? (source.map.startsWith('MAP_JOHTO_') ? 'Johto' : null)
      ?? (source.map.endsWith('_FRLG') ? 'Kanto' : null);
    if (mappedRegion && !location.regions.includes(mappedRegion))
      location.regions.push(mappedRegion);
    if (!mappedRegion && source.region && !location.regions.includes(source.region))
      location.regions.push(source.region);
    location.entries.push({
      kind: entry.kind, name: entry.name,
      dex: entry.kind === 'pokemon' ? entry.dex : undefined,
      itemId: entry.kind === 'item' ? entry.id : undefined,
      method: source.method, level: source.level, levelMin: source.level_min,
      levelMax: source.level_max, form: source.form,
      note: source.note, detail: source.detail,
      required: source.required ?? source.requires,
      gameplayVerified: source.gameplay_verified,
      sourceFile: source.source
    });
  }
}
const locations = [...locationMap.values()].sort((a, b) => a.id.localeCompare(b.id));
const slugs = locations.map(location => location.slug);
if (new Set(slugs).size !== slugs.length)
  throw new Error('Location slug collision: update slug generation before publishing');
const data = { meta, pokemon, items, locations };
await mkdir(dirname(output), { recursive: true });
await writeFile(output, JSON.stringify(data));
const visuals = resolve(website, 'public/visuals');
await mkdir(visuals, { recursive: true });
const images = [
  ['.github/assets/grand-anthology-readme-banner.jpg', 'atlas.jpg'],
  ['graphics/title_screen/world/grand_anthology_title.png', 'title.png'],
  ['graphics/branding/screenshots/showcase/04-kanto-vermilion.png', 'kanto.png'],
  ['graphics/branding/screenshots/showcase/07-johto-ecruteak.png', 'johto.png'],
  ['graphics/branding/screenshots/showcase/11-hoenn-fortree.png', 'hoenn.png'],
  ['graphics/branding/screenshots/showcase/02-world-transit.png', 'hub.png']
];
for (const [from, to] of images) await copyFile(resolve(repository, from), resolve(visuals, to));
console.log(`Generated site data: ${pokemon.length} Pokémon, ${items.length} items, ${megaStones.length} Mega Stones, ${locations.length} indexed map identifiers.`);
console.log(`Website presentation merged ${rawRecords.reduce((count, entry) => count + (entry.sources?.length ?? 0), 0) - records.reduce((count, entry) => count + (entry.sources?.length ?? 0), 0)} redundant script-source references; distinct acquisition methods remain separate.`);
console.log('Acquisition entries are source-level candidates, NOT proof of obtainable or tested encounters.');
