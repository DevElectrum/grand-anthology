#!/usr/bin/env python3
"""Emit an isolated map assembly source with exactly one chosen layout include.

This rewrite happens BEFORE tools/preproc -s, which resolves .include even
inside #if branches that arm-none-eabi-cpp would later discard.
"""
import argparse
from pathlib import Path


MARKER = '.include "data/layouts/layouts.inc"'


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("source")
    parser.add_argument("selected_layout")
    parser.add_argument("destination")
    args = parser.parse_args()
    source = Path(args.source)
    selected = Path(args.selected_layout)
    destination = Path(args.destination)

    if source.resolve() == destination.resolve():
        parser.error("Generated source must not overwrite tracked map template")
    if not selected.is_file():
        parser.error("Active build layout does not exist")
    if not destination.parent.is_dir():
        parser.error("Active build output directory does not exist")

    text = source.read_text()
    if text.count(MARKER) != 1:
        parser.error("Expected exactly one ordinary map-layout include")
    if ".include \"build/xgba-" in text:
        parser.error("Tracked source already contains a variant-specific include")

    replacement = '.include "' + selected.as_posix() + '"'
    generated = text.replace(MARKER, replacement)
    if generated.count(replacement) != 1:
        parser.error("Generated map source has invalid include count")
    destination.write_text(generated)


if __name__ == "__main__":
    main()
