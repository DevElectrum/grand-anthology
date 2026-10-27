// Cloudflare Pages output guard: run after "npm run build".
import { readdir, readFile, stat } from 'node:fs/promises';
import { resolve, join, relative, sep, extname } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = resolve(fileURLToPath(new URL('../dist/', import.meta.url)));
const MAX_FILES = 20000;
const MAX_FILE_BYTES = 25 * 1024 * 1024;
const files = new Set();
const bad = [];
let totalBytes = 0;
let largest = { path: '', bytes: 0 };

async function walk(directory) {
  for (const item of await readdir(directory, { withFileTypes: true })) {
    const path = join(directory, item.name);
    if (item.isDirectory()) {
      await walk(path);
      continue;
    }
    if (!item.isFile()) throw new Error('Unexpected deployment artifact type: ' + path);
    const info = await stat(path);
    const rel = relative(root, path).split(sep).join('/');
    files.add(rel);
    totalBytes += info.size;
    if (info.size > largest.bytes) largest = { path: rel, bytes: info.size };
    if (info.size > MAX_FILE_BYTES)
      bad.push('Over 25 MiB Cloudflare asset limit: ' + rel + ' (' + info.size + ' bytes)');
  }
}

function checkUrl(link, source) {
  if (!link.startsWith('/') || link.startsWith('//')) return;
  let path;
  try { path = decodeURIComponent(link.split(/[?#]/)[0]); }
  catch { bad.push('Malformed URL in ' + source + ': ' + link); return; }
  if (!path || path === '/') path = '/index.html';
  const rel = path.replace(/^\/+/, '');
  if (rel.split('/').includes('..')) {
    bad.push('Unsafe internal link in ' + source + ': ' + link);
    return;
  }
  const candidate = rel.endsWith('/') ? rel + 'index.html' : rel;
  if (files.has(candidate) || files.has(candidate + '/index.html')) return;
  bad.push('Missing deployment target in ' + source + ': ' + link);
}

await walk(root);
if (files.size > MAX_FILES)
  bad.push('Cloudflare Pages Free has a 20,000-file limit; output has ' + files.size);
for (const required of [
  'index.html', 'favicon.svg', 'robots.txt', '_headers',
  'pokedex/index.html', 'pokedex/133/index.html',
  'pokedex/718/index.html', 'pokedex/1025/index.html',
  'locations/index.html', 'randomizer/index.html',
  'mega-stones/index.html', 'about/index.html', 'status/index.html'
]) if (!files.has(required)) bad.push('Required site file absent: ' + required);

let pages = 0;
let linked = 0;
for (const file of files) {
  if (extname(file) !== '.html' && extname(file) !== '.css') continue;
  const content = await readFile(join(root, file), 'utf8');
  if (file.endsWith('.html')) {
    pages++;
    for (const match of content.matchAll(/\b(?:href|src)=["'](\/(?!\/)[^"']*)["']/g)) {
      linked++;
      checkUrl(match[1], file);
    }
    if (!content.includes('rel="icon"') || !content.includes('property="og:title"'))
      bad.push('Missing launch metadata: ' + file);
  }
  if (file.endsWith('.css')) {
    for (const match of content.matchAll(/url\(\s*["']?(\/(?!\/)[^"')\s]+)["']?\s*\)/g)) {
      linked++;
      checkUrl(match[1], file);
    }
  }
}
const kb = size => (size / 1024).toFixed(1);
console.log('Deployment audit: ' + files.size + '/' + MAX_FILES + ' files, ' +
  pages + ' HTML pages, ' + linked + ' local URLs; total ' + kb(totalBytes) +
  ' KiB; largest ' + largest.path + ' (' + kb(largest.bytes) + ' KiB).');
if (bad.length) {
  console.error(bad.slice(0, 50).join('\n'));
  if (bad.length > 50) console.error('...and ' + (bad.length - 50) + ' more errors');
  process.exitCode = 1;
} else {
  console.log('PASS: Cloudflare Pages Free artifact limits, page links, assets and launch metadata.');
}
