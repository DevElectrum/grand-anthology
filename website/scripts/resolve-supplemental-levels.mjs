// Website-only level attribution. The catalog's supplemental habitat definitions
// contain species and map identifiers, but no independent encounter levels.
// wild_encounter.c chooses their level from native slots at the same map.
const methods = {
  rare_land: 'land_mons',
  supplemental_land: 'land_mons',
  supplemental_water: 'water_mons',
  supplemental_fishing: 'fishing_mons'
};

const rods = [
  ['Old Rod', 0, 2],
  ['Good Rod', 2, 5],
  ['Super Rod', 5, 10]
];

const valid = value => Number.isInteger(value) && value >= 1 && value <= 100;

function nativeSlot(slot) {
  if (!slot || !valid(slot.min_level) || !valid(slot.max_level)) return null;
  return {
    min: Math.min(slot.min_level, slot.max_level),
    max: Math.max(slot.min_level, slot.max_level)
  };
}

function mergeLevels(ranges) {
  if (!ranges.length) return null;
  return {
    level_min: Math.min(...ranges.map(range => range.min)),
    level_max: Math.max(...ranges.map(range => range.max))
  };
}

// The game's default build has time-of-day encounters disabled, with a
// Morning fallback. Its generated header also excludes LeafGreen duplicates.
function compiledVariant(row) {
  const label = row.base_label ?? '';
  if (label.includes('LeafGreen')) return false;
  return !/(?:_|^)(?:Day|Evening|Night)(?:_|$)/.test(label);
}

export function enrichSupplementalLevels(records, wildEncounters) {
  const maps = new Map();
  for (const group of wildEncounters.wild_encounter_groups ?? []) {
    if (!group.for_maps) continue;
    for (const row of group.encounters ?? []) {
      if (!row.map || !compiledVariant(row)) continue;
      if (!maps.has(row.map)) maps.set(row.map, []);
      maps.get(row.map).push(row);
    }
  }

  const counters = { candidates: 0, resolved: 0, unresolved: 0 };
  const result = records.map(record => {
    if (!Array.isArray(record.sources)) return record;
    const sources = record.sources.map(source => {
      const area = methods[source.method];
      if (!area || !source.map || source.level != null || source.level_min != null) return source;
      counters.candidates += 1;
      const options = (maps.get(source.map) ?? [])
        .map(row => row[area])
        .filter(table => table?.encounter_rate > 0 && Array.isArray(table.mons));

      if (source.method === 'supplemental_fishing') {
        const rodLevels = [];
        for (const [rod, start, end] of rods) {
          const levels = [];
          for (const table of options) {
            for (const slot of table.mons.slice(start, end)) {
              const level = nativeSlot(slot);
              if (level) levels.push(level);
            }
          }
          const merged = mergeLevels(levels);
          if (merged) rodLevels.push({ rod, min: merged.level_min, max: merged.level_max });
        }
        const all = mergeLevels(rodLevels);
        if (all) {
          counters.resolved += 1;
          return { ...source, level_min: all.level_min, level_max: all.level_max,
            rod_levels: rodLevels, level_basis: 'native_rod_slots' };
        }
      } else {
        const firstSlots = options.map(table => nativeSlot(table.mons[0])).filter(Boolean);
        const level = mergeLevels(firstSlots);
        if (level) {
          counters.resolved += 1;
          return { ...source, ...level, level_basis: 'native_first_slot' };
        }
      }
      counters.unresolved += 1;
      return source;
    });
    return { ...record, sources };
  });
  return { records: result, counters };
}
