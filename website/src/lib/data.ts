import catalog from '../generated/catalog.json';
import pokemonDetails from '../generated/pokemon-details.json';

export { catalog, pokemonDetails };

export function displayName(name: string): string {
  return String(name).toLowerCase().split(/([ _-]+)/).map(part =>
    /^[a-z]/.test(part) ? part[0].toUpperCase() + part.slice(1) : part
  ).join(' ').replaceAll('_', ' ');
}

export function locationName(id: string): string {
  return displayName(id.replace(/^MAP_/, '').replace(/_F[Rr][Ll][Gg]$/, '').replace(/([a-z])([A-Z])/g, '$1 $2').replace(/\b(Route|Floor)(\d+)/gi, '$1 $2').replaceAll('_', ' '));
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
    rare_land: 'Rare grass encounter',
    supplemental_land: 'Grass (rare)',
    supplemental_water: 'Surfing (rare)',
    supplemental_fishing: 'Fishing (rare)',
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

export function levelLabel(source: {
  level?: number;
  levelMin?: number;
  levelMax?: number;
  level_min?: number;
  level_max?: number;
  rodLevels?: { rod: string; min: number; max: number }[];
  rod_levels?: { rod: string; min: number; max: number }[];
}): string {
  const range = (min: number, max: number) => min === max ? 'Lv. ' + min : 'Lv. ' + min + '–' + max;
  const rods = source.rodLevels ?? source.rod_levels;
  if (rods?.length) return rods.map(row => row.rod + ': ' + range(row.min, row.max)).join(' · ');
  if (source.level !== undefined) return 'Lv. ' + source.level;
  const low = source.levelMin ?? source.level_min;
  const high = source.levelMax ?? source.level_max;
  if (low !== undefined && high !== undefined) return range(low, high);
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


export function formName(symbol: string, gameName?: string | null): string {
  const raw = symbol.replace(/^SPECIES_/, '');
  const special: Record<string, string> = {
    ZYGARDE_10_AURA_BREAK: '10% Forme (Aura Break)',
    ZYGARDE_10_POWER_CONSTRUCT: '10% Forme (Power Construct)',
    ZYGARDE_50_AURA_BREAK: '50% Forme (Aura Break)',
    ZYGARDE_50_POWER_CONSTRUCT: '50% Forme (Power Construct)',
    ZYGARDE_COMPLETE: 'Complete Forme',
    TERAPAGOS_STELLAR: 'Stellar Form',
    TERAPAGOS_TERASTAL: 'Terastal Form'
  };
  if (special[raw]) return special[raw];
  if (raw.endsWith('_MEGA')) return 'Mega ' + (gameName || displayName(raw.slice(0, -5)));
  if (raw.endsWith('_MEGA_X')) return 'Mega ' + (gameName || displayName(raw.slice(0, -7))) + ' X';
  if (raw.endsWith('_MEGA_Y')) return 'Mega ' + (gameName || displayName(raw.slice(0, -7))) + ' Y';
  if (gameName && raw === gameName.toUpperCase().replace(/[^A-Z0-9]+/g, '_')) return gameName;
  return displayName(raw);
}

export function genderLabel(ratio?: string | null): string {
  if (!ratio) return 'Not recorded';
  if (ratio.includes('GENDERLESS')) return 'Genderless';
  const match = ratio.match(/PERCENT_FEMALE\(([\d.]+)\)/);
  if (match) return match[1] + '% female';
  return 'Game-specific';
}
