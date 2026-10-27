import { test } from 'node:test';
import assert from 'node:assert/strict';
import { readFile, mkdtemp, rm } from 'node:fs/promises';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { fileURLToPath } from 'node:url';
import { relativeForm, formFolderMatch, buildSpriteManifest } from './sprite-manifest.mjs';

test('distinguishes default forms, Mega forms and alternate form paths', () => {
  assert.equal(relativeForm('SPECIES_BULBASAUR', 'SPECIES_BULBASAUR'), '');
  assert.equal(relativeForm('SPECIES_CHARIZARD_MEGA_X', 'SPECIES_CHARIZARD'), 'mega_x');
  assert.equal(relativeForm('SPECIES_TERAPAGOS_STELLAR', 'SPECIES_TERAPAGOS'), 'stellar');
  assert.equal(relativeForm('SPECIES_ZYGARDE_10_POWER_CONSTRUCT', 'SPECIES_ZYGARDE'), '10_percent');
  assert.equal(relativeForm('SPECIES_ZYGARDE_50_AURA_BREAK', 'SPECIES_ZYGARDE'), '');
  assert.equal(relativeForm('SPECIES_GRENINJA_ASH', 'SPECIES_CHARIZARD'), null);
  assert.ok(formFolderMatch('mega_x', 'mega_x'));
  assert.ok(!formFolderMatch('mega_y', 'mega_x'));
});

test('materialize real game fronts and icons with form-specific fallbacks', async () => {
  const repository = fileURLToPath(new URL('../../', import.meta.url));
  const pokemon = (await readFile(new URL('../../data/catalog/world_acquisition.jsonl', import.meta.url), 'utf8')).split('\n')
    .filter(Boolean).map(JSON.parse).filter(item => item.kind === 'pokemon');
  const details = JSON.parse(await readFile(new URL('../src/generated/pokemon-details.json', import.meta.url), 'utf8').catch(async () => {
    // Test runner also supports invocation before website generation.
    const { execFileSync } = await import('node:child_process');
    execFileSync('python3', [fileURLToPath(new URL('./export_species.py', import.meta.url))], { cwd: repository });
    return readFile(new URL('../src/generated/pokemon-details.json', import.meta.url), 'utf8');
  }));
  const temp = await mkdtemp(join(tmpdir(), 'anthology-sprites-'));
  try {
    const { coverage, manifest, missing } = await buildSpriteManifest({ repository, website: temp, pokemon, details, quiet: true });
    assert.equal(coverage.national, 1025);
    assert.ok(coverage.front >= 980, 'Unexpectedly low front-sprite coverage: ' + missing.join(', '));
    assert.ok(coverage.icon >= 980, 'Unexpectedly low menu-icon coverage');
    assert.ok(manifest.SPECIES_BULBASAUR.front?.includes('bulbasaur/'), 'Bulbasaur art');
    assert.ok(manifest.SPECIES_CHARIZARD_MEGA_X.front?.includes('charizard/mega_x/'), 'Mega X art');
    assert.ok(manifest.SPECIES_ZYGARDE_COMPLETE.front?.includes('zygarde/complete/'), 'Complete Zygarde');
    assert.ok(manifest.SPECIES_ZYGARDE_10_AURA_BREAK.front?.includes('zygarde/10_percent/'), '10% Zygarde');
    assert.ok(manifest.SPECIES_TERAPAGOS_STELLAR.front?.includes('terapagos/stellar/'), 'Stellar Terapagos');
    assert.ok(manifest.SPECIES_UNOWN_B?.front?.includes('unown/b/'), 'Unown B');
  } finally {
    await rm(temp, { recursive: true, force: true });
  }
});
