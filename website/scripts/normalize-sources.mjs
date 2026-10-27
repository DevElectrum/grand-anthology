// The inventory can index one encounter twice: once as a special script
// and once as its general script category. Collapse only source-identical,
// explicitly recognized aliases for the *website presentation*. The original
// game catalog stays untouched, including all genuinely different methods.
export function normalizeSourceEntries(records) {
  return records.map(entry => {
    if (!Array.isArray(entry.sources)) return entry;
    const sources = entry.sources.filter(source => {
      if (entry.kind === 'pokemon' && source.method === 'scripted_battle_candidate') {
        return !entry.sources.some(other =>
          (other.method === 'permanent_complete_static' || other.method === 'permanent_stellar_static') &&
          other.map === source.map &&
          other.form === source.form &&
          other.level === source.level &&
          other.source === source.source);
      }
      if (entry.kind === 'item' && entry.mega_stone && source.method === 'scripted_pickup_candidate') {
        const mapDirectory = `data/maps/${source.map}`;
        const expectedScript = `Anthology_MegaStone_${entry.symbol?.replace(/^ITEM_/, '')}`;
        return !entry.sources.some(other =>
          other.method === 'overworld_item_ball' &&
          other.map === source.map &&
          other.source === `${mapDirectory}/map.json` &&
          source.source === `${mapDirectory}/scripts.inc` &&
          other.script === expectedScript &&
          Array.isArray(other.coordinates) &&
          other.flag);
      }
      return true;
    });
    return sources.length === entry.sources.length ? entry : { ...entry, sources };
  });
}
