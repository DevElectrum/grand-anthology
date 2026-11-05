#!/usr/bin/env python3
"""Dependency-free safety tests for the deterministic Sinnoh importer."""
import importlib.util
import json
import tempfile
from pathlib import Path

module_path = Path(__file__).with_name("import_source.py")
spec = importlib.util.spec_from_file_location("sinnoh_import", module_path)
imp = importlib.util.module_from_spec(spec)
spec.loader.exec_module(imp)

def run():
    assert imp.blob_hash(b"test") == "30d74d258442c7c65512eafab474568dd706c430"
    with tempfile.TemporaryDirectory() as root:
        root = Path(root)
        old = root / "data/maps"
        old.mkdir(parents=True)
        (old / "map_groups.json").write_text(json.dumps({
            "group_order": ["legacy"], "legacy": ["Original"]
        }))
        layoutdir = root / "data/layouts"
        layoutdir.mkdir(parents=True)
        (layoutdir / "layouts.json").write_text(json.dumps({
            "layouts_table_label": "gMapLayouts",
            "layouts": [{"id": "LAYOUT_ORIGINAL"}]
        }))
        m = {"target": {
            "legacy_groups": ["legacy"], "legacy_map_count": 1,
        }, "group_import": [
            {"upstream_group": "sinnoh", "maps": ["Test" + str(i) for i in range(93)]}
        ], "layout_import": [
            {"id": "LAYOUT_SINNOH_" + str(i), "name": "test" + str(i)}
            for i in range(64)
        ]}
        world, layouts = imp.check_existing_world(root, m)
        merged, output = imp.generate_registries(m, world, layouts)
        assert merged["group_order"] == ["legacy", "sinnoh"]
        assert merged["legacy"] == ["Original"]
        assert len(merged["sinnoh"]) == 93
        assert len(output["layouts"]) == 65
        test_file = root / "file.txt"
        test_file.write_text("original")
        source = root / "source"
        source.mkdir()
        (source / "file.txt").write_text("changed")
        try:
            imp.install_files(source, root, [
                {"path": "file.txt", "git_blob": imp.blob_hash(b"changed")}
            ])
        except ValueError:
            pass
        else:
            raise AssertionError("Existing-file overwrite was accepted")
        assert test_file.read_text() == "original"
        try:
            imp.safe_child(root, "../outside")
        except ValueError:
            pass
        else:
            raise AssertionError("Path traversal was accepted")
        (source / "new.txt").write_text("new")
        added, same = imp.install_files(source, root, [
            {"path": "new.txt", "git_blob": imp.blob_hash(b"new")}
        ])
        assert (added, same) == (1, 0)
        assert (root / "new.txt").read_text() == "new"
        midi = Path("sound/songs/midi/midi.cfg")
        (root / midi).parent.mkdir(parents=True)
        (source / midi).parent.mkdir(parents=True)
        (root / midi).write_text("legacy.mid: -E -V080\n")
        (source / midi).write_text("legacy.mid: -E -V080\nsinnoh.mid: -E -G191 -V100\n")
        records = [{"path": "sound/songs/midi/sinnoh.mid"}]
        assert imp.merge_midi_config(source, root, records) == 1
        assert imp.merge_midi_config(source, root, records) == 0
        lines = (root / midi).read_text()
        assert lines.count("sinnoh.mid:") == 1
        assert "legacy.mid: -E -V080" in lines
    print("PASS: Sinnoh import hash, mapping, overwrite, traversal and MIDI rules")

if __name__ == "__main__":
    run()
