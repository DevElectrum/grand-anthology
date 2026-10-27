import catalog from '../generated/catalog.json';

export { catalog };

export function displayName(name: string): string {
  return String(name).toLowerCase().split(/([ _-]+)/).map(part =>
    /^[a-z]/.test(part) ? part[0].toUpperCase() + part.slice(1) : part
  ).join(' ').replaceAll('_', ' ');
}

export function locationName(id: string): string {
  return displayName(id.replace(/^MAP_/, '').replaceAll('_', ' '));
}

export function generation(dex: number): number {
  if (dex <= 151) return 1;
  if (dex <= 251) return 2;
  if (dex <= 386) return 3;
  if (dex <= 493) return 4;
  if (dex <= 649) return 5;
  if (dex <= 721) return 6;
  if (dex <= 809) return 7;
  if (dex <= 905) return 8;
  return 9;
}

export function sourceMethod(method: string): string {
  const names: Record<string, string> = {
    native_land_mons: 'Grass / ground',
    native_water_mons: 'Surfing',
    native_fishing_mons: 'Fishing',
    native_rock_smash_mons: 'Rock Smash',
    rare_land: 'Rare wild encounter',
    supplemental_land: 'Supplemental grass',
    supplemental_water: 'Supplemental surfing',
    supplemental_fishing: 'Supplemental fishing',
    scripted_battle_candidate: 'Scripted battle candidate',
    scripted_event_candidate: 'Scripted event candidate',
    scripted_gift_candidate: 'Gift candidate',
    overworld_item_ball: 'World item pickup',
    scripted_pickup_candidate: 'Scripted pickup candidate',
    scripted_reward_candidate: 'Scripted reward candidate',
    battle_net_vendor: 'Battle Net vendor',
    battle_net_free_starter_choice: 'Battle Net starter selection',
    hard_rematch_reward: 'Hard rematch reward',
    royal_duo_defeat_reward: 'Royal Duo reward',
    mew_nine_masks_capture: "Mew's Nine Masks",
    permanent_complete_static: 'Complete Zygarde event',
    permanent_stellar_static: 'Stellar Terapagos event'
  };
  return names[method] ?? displayName(method);
}

export function levelLabel(source: { level?: number; levelMin?: number; levelMax?: number }): string {
  if (source.level !== undefined) return `Lv. ${source.level}`;
  if (source.levelMin !== undefined && source.levelMax !== undefined)
    return source.levelMin === source.levelMax ? `Lv. ${source.levelMin}` : `Lv. ${source.levelMin}–${source.levelMax}`;
  return 'Level not indexed';
}

export function evidenceLabel(value?: string | boolean): string {
  if (value === true) return 'Recorded gameplay verification';
  if (value === 'player_reported') return 'Player-reported gameplay test';
  return 'Source-indexed, not individually verified';
}

export function pokemonHref(dex: number): string {
  return `/pokedex/${dex}/`;
}

export function locationHref(slug: string): string {
  return `/locations/${slug}/`;
}
