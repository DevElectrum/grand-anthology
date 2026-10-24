#!/usr/bin/env python3
"""Guard lossless Bard byte packing and both full-format exception tables."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
PACKED = ["pokemon","moves","trainer","status","battle","greetings","speech","endings","feelings","conditions","actions"]
def main():
    total = 0
    for name in PACKED:
        text = (ROOT / "src/data/bard_music" / (name + ".h")).read_text()
        assert re.search(r"const u8 sBardSoundTemplates_\w+\[", text), name
        assert not re.search(r"\.(?:songId|lengthAdjustment|unused|volume)\s*=", text), name
        assert "NULL_BARD_SOUND" not in text and "PREV_BARD_SOUND" not in text, name
        for match in re.finditer(r"\[(?:SPECIES_|MOVE_|EC_INDEX\()[^\n]+\]\s*=\s*\{([^{}]+)\}", text):
            sounds = [s.strip() for s in match[1].split(",") if s.strip()]
            assert len(sounds) == 6, (name, sounds)
            for s in sounds:
                assert re.fullmatch(r"PHONEME_ID\(PH_[A-Z0-9_]+\)|PHONEME_ID_NONE|NUM_PHONEME_SONGS", s), (name, s)
            total += 1
    for name in ("people", "voices"):
        text = (ROOT / "src/data/bard_music" / (name + ".h")).read_text()
        assert "struct BardSoundTemplate" in text and ".lengthAdjustment" in text, name
    print(f"PASS: {len(PACKED)} byte-packed Bard groups, {total} verified six-phoneme records; adjustment groups preserved")
if __name__ == "__main__":
    main()
