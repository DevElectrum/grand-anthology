import json
from pathlib import Path
import unittest

import export_species as exporter


class ExporterTests(unittest.TestCase):
    def test_balanced_fields_and_quotes(self):
        text = '.description = COMPOUND_STRING("hello, world"), .types = MON_TYPES(TYPE_FIRE, TYPE_FLYING), .baseHP = 78'
        result = exporter.fields(text)
        self.assertIn('hello, world', result['description'])
        self.assertEqual(result['baseHP'], '78')
        self.assertEqual(exporter.literal_strings(result['description']), 'hello, world')

    def test_numeric_config_expressions(self):
        self.assertEqual(exporter.number_expr('8 >= 6 ? 120 : 100'), 120)
        self.assertEqual(exporter.number_expr('(20 + 40) * 2'), 120)
        self.assertIsNone(exporter.number_expr('UNRESOLVED_MACRO'))

    def test_evolution_relationships(self):
        edges = exporter.evolution_list('EVOLUTION({EVO_LEVEL, 16, SPECIES_IVYSAUR})')
        self.assertEqual(edges[0]['target'], 'SPECIES_IVYSAUR')
        self.assertEqual(edges[0]['requirement'], 'Level 16')

    def test_game_source_import(self):
        exporter.main()
        generated = json.loads(exporter.OUT.read_text())
        mons = generated['species']
        bulbasaur = mons['SPECIES_BULBASAUR']
        self.assertEqual(bulbasaur['bst'], 318)
        self.assertEqual(bulbasaur['stats']['baseHP'], 45)
        self.assertIn('Grass', bulbasaur['types'])
        self.assertEqual(bulbasaur['hiddenAbility']['name'], 'Chlorophyll')
        self.assertEqual(len(generated['formsByDex']), 1025)
        self.assertIn('SPECIES_ZYGARDE_COMPLETE', mons)
        self.assertIn('SPECIES_TERAPAGOS_STELLAR', mons)
        self.assertIn('SPECIES_ZYGARDE_COMPLETE', generated['formsByDex']['718'])
        self.assertIn('SPECIES_TERAPAGOS_STELLAR', generated['formsByDex']['1024'])
        self.assertEqual(mons['SPECIES_ZYGARDE_COMPLETE']['bst'], 708)
        self.assertEqual(mons['SPECIES_ZYGARDE']['dex'], 718)
        self.assertEqual(mons['SPECIES_TERAPAGOS']['dex'], 1024)


if __name__ == '__main__':
    unittest.main()
