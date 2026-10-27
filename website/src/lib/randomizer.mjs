// Browser and Node implementation of the game's current new-game randomizers.
// All arithmetic is 32-bit unsigned, just like the C implementations.
// No ROM or game save is modified.
const MUL_ORIGINAL = 0x85EBCA6B;
const MUL_SLOT = 0xC2B2AE35;

export function mix32(value) {
  let hash = value >>> 0;
  hash ^= hash >>> 16;
  hash = Math.imul(hash, 0x7FEB352D) >>> 0;
  hash ^= hash >>> 15;
  hash = Math.imul(hash, 0x846CA68B) >>> 0;
  hash ^= hash >>> 16;
  return hash >>> 0;
}

export function seededWild(seed, original, slot, area, group, num, species, frontier = false) {
  if (!Number.isInteger(seed) || seed < 0 || seed > 65535)
    throw new RangeError('Seed must be an integer between 0 and 65535');
  if (frontier || original === 0 || original === species.length - 1) return original;
  let hash = (0x9E3779B9 ^ seed) >>> 0;
  hash ^= Math.imul(original, MUL_ORIGINAL);
  hash ^= Math.imul(slot + 1, MUL_SLOT);
  hash ^= ((group << 24) | (num << 16) | (area << 8));
  hash = mix32(hash);
  let result = 1 + (hash % (species.length - 1));
  for (let attempts = 0; attempts < species.length - 1; attempts++) {
    if (species[result]?.fieldStable) return result;
    result++;
    if (result >= species.length) result = 1;
  }
  return original;
}

export function seededStarters(seed, starter, species) {
  if (!Number.isInteger(seed) || seed < 0 || seed > 65535)
    throw new RangeError('Seed must be an integer between 0 and 65535');
  const picked = [];
  const map = ((starter.group << 8) | starter.num) >>> 0;
  for (const original of starter.slots) {
    let hash = (0x9E3779B9 ^ seed) >>> 0;
    hash ^= Math.imul(original, MUL_ORIGINAL);
    hash ^= ((map << 8) | 0x5300);
    hash = mix32(hash);
    let candidate = 1 + hash % (species.length - 1);
    let selected = original;
    for (let attempt = 0; attempt < species.length - 1; attempt++) {
      const entry = species[candidate];
      if (entry?.starterEligible && entry.dex != null
        && !picked.some(previous => species[previous]?.dex === entry.dex)) {
        selected = candidate;
        break;
      }
      candidate++;
      if (candidate >= species.length) candidate = 1;
    }
    picked.push(selected);
  }
  return picked;
}

export function buildWildAtlas(seed, data, randomized = true) {
  const rows = [];
  for (const slot of data.wildSlots) {
    const map = data.maps[slot.map];
    const resolved = randomized
      ? seededWild(seed, slot.original, slot.slot, slot.area, slot.group, slot.num, data.species, map?.frontier)
      : slot.original;
    const mon = data.species[resolved];
    if (!mon?.symbol) throw new Error('Unresolved randomized Pokémon ' + resolved);
    rows.push({ ...slot, speciesId: resolved });
  }
  return rows;
}

export function buildStarterAtlas(seed, data, randomized = true) {
  return Object.fromEntries(Object.entries(data.starters).map(([region, roster]) => [
    region, (randomized ? seededStarters(seed, roster, data.species) : roster.slots)
      .map((id, slot) => ({ id, original: roster.slots[slot], slot }))
  ]));
}
