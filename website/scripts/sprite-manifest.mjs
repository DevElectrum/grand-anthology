import { copyFile, mkdir, readdir, stat, rm, writeFile } from 'node:fs/promises';
import { join, relative, resolve, dirname, sep } from 'node:path';

const imageNames = { front: ['front.png', 'anim_front.png'], icon: ['icon.png'] };
const specialForms = {
  ZYGARDE_10_AURA_BREAK: '10_percent',
  ZYGARDE_10_POWER_CONSTRUCT: '10_percent',
  ZYGARDE_50_AURA_BREAK: '',
  ZYGARDE_50_POWER_CONSTRUCT: '',
  ZYGARDE_COMPLETE: 'complete',
  TERAPAGOS_TERASTAL: 'terastal',
  TERAPAGOS_STELLAR: 'stellar'
};

const slug = name => name.replace(/^SPECIES_/, '').toLowerCase();
const compact = name => name.replace(/[^a-z0-9]/g, '');

async function isDirectory(path) {
  try { return (await stat(path)).isDirectory(); } catch { return false; }
}

async function findFile(directory, names) {
  for (const name of names) {
    const path = join(directory, name);
    try { if ((await stat(path)).isFile()) return path; } catch { /* absent source */ }
  }
  return null;
}

async function subfolders(root, depth = 0) {
  if (depth > 3 || !await isDirectory(root)) return [];
  const result = [];
  for (const entry of await readdir(root, { withFileTypes: true })) {
    if (!entry.isDirectory()) continue;
    const directory = join(root, entry.name);
    result.push(directory);
    result.push(...await subfolders(directory, depth + 1));
  }
  return result;
}

export function relativeForm(symbol, baseSymbol) {
  const form = slug(symbol);
  const base = slug(baseSymbol);
  if (symbol === baseSymbol) return '';
  if (Object.prototype.hasOwnProperty.call(specialForms, symbol.replace(/^SPECIES_/, '')))
    return specialForms[symbol.replace(/^SPECIES_/, '')];
  if (!form.startsWith(base + '_')) return null;
  return form.slice(base.length + 1);
}

// Make sure a local sprite really belongs to a form. Never claim an unrelated
// animation is a precise regional/Mega sprite merely because names overlap.
export function formFolderMatch(relative, suffix) {
  if (!suffix) return false;
  const components = relative.split('/').filter(Boolean);
  const name = compact(components.join('_'));
  const target = compact(suffix);
  return name === target
    || name === compact(suffix.replace(/_forme?$/, ''))
    || name === compact(suffix.replace(/_percent$/, ''));
}

async function chooseFormDirectory(root, suffix) {
  if (suffix === '') return root;
  if (!suffix) return null;
  const exact = join(root, ...suffix.split('/'));
  if (await isDirectory(exact)) return exact;
  const variants = await subfolders(root);
  // Do not guess when multiple nested forms reduce to the same label.
  const matches = variants.filter(path => formFolderMatch(relative(root, path).split(sep).join('/'), suffix));
  return matches.length === 1 ? matches[0] : null;
}

export async function buildSpriteManifest({ repository, website, pokemon, details, quiet = false }) {
  const graphics = resolve(repository, 'graphics/pokemon');
  const publicDir = resolve(website, 'public/pokemon-sprites');
  const generated = resolve(website, 'src/generated/sprites.json');
  const species = details.species;
  const byDex = details.formsByDex;
  const manifests = {};
  const base = {};
  const copied = new Set();
  const filesByDirectory = new Map();
  const coverage = { national: pokemon.length, front: 0, icon: 0, exactForms: 0, fallbackForms: 0, unresolvedForms: 0 };
  const missing = [];

  if (!await isDirectory(graphics)) throw new Error('Game sprite directory missing: ' + graphics);
  await rm(publicDir, { recursive: true, force: true });
  await mkdir(publicDir, { recursive: true });

  async function imagesFor(directory) {
    if (!directory) return null;
    if (filesByDirectory.has(directory)) return filesByDirectory.get(directory);
    const front = await findFile(directory, imageNames.front);
    const icon = await findFile(directory, imageNames.icon);
    const image = front || icon ? { front, icon } : null;
    filesByDirectory.set(directory, image);
    return image;
  }

  async function publish(path) {
    if (!path) return null;
    const part = relative(graphics, path).split(sep).join('/');
    if (part.startsWith('../') || part === '..') throw new Error('Sprite file outside game assets');
    const target = join(publicDir, part);
    if (!copied.has(part)) {
      await mkdir(dirname(target), { recursive: true });
      await copyFile(path, target);
      copied.add(part);
    }
    return '/pokemon-sprites/' + part;
  }

  for (const item of pokemon) {
    const rootName = slug(item.symbol);
    let root = join(graphics, rootName);
    if (!await isDirectory(root)) {
      const alternative = String(item.name).toLowerCase().replace(/[^a-z0-9]+/g, '_').replace(/^_|_$/g, '');
      const other = join(graphics, alternative);
      if (await isDirectory(other)) root = other;
    }
    const entry = await imagesFor(root);
    const front = await publish(entry?.front);
    const icon = await publish(entry?.icon);
    base[item.dex] = { front, icon };
    if (front) coverage.front++;
    if (icon) coverage.icon++;
    if (!front && !icon) missing.push(item.symbol);
  }

  for (const item of pokemon) {
    const root = join(graphics, slug(item.symbol));
    const fallback = base[item.dex];
    for (const symbol of new Set([item.symbol, ...(byDex[String(item.dex)] ?? [])])) {
      if (!species[symbol]) continue;
      const suffix = relativeForm(symbol, item.symbol);
      const ownRoot = await isDirectory(root) ? root : join(graphics, String(item.name).toLowerCase().replace(/[^a-z0-9]+/g, '_').replace(/^_|_$/g, ''));
      const directory = await chooseFormDirectory(ownRoot, suffix);
      const selected = await imagesFor(directory);
      const front = await publish(selected?.front) ?? fallback.front;
      const icon = await publish(selected?.icon) ?? fallback.icon;
      const exact = Boolean(directory && selected && (selected.front || selected.icon));
      manifests[symbol] = { front, icon, exact };
      if (symbol !== item.symbol) {
        if (exact) coverage.exactForms++;
        else if (front || icon) coverage.fallbackForms++;
        else coverage.unresolvedForms++;
      }
    }
  }

  await mkdir(dirname(generated), { recursive: true });
  await writeFile(generated, JSON.stringify({ schema: 1, sprites: manifests, byDex: base }));
  if (!quiet) {
    console.log('Pokédex sprites: ' + coverage.front + '/' + coverage.national +
      ' front art, ' + coverage.icon + '/' + coverage.national + ' icons; ' +
      coverage.exactForms + ' exact additional forms, ' + coverage.fallbackForms +
      ' base-art form fallbacks, ' + coverage.unresolvedForms + ' unavailable.');
    console.log('Published ' + copied.size + ' unique game image assets into website-only output.');
    if (missing.length) console.warn('Unresolved National sprite roots (' + missing.length + '): ' + missing.join(', '));
  }
  return { coverage, missing, manifest: manifests };
}
