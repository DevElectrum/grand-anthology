#!/usr/bin/env python3
"""Guard Johto's actual Flash, Cut, Strength and Fly badge requirements."""
from pathlib import Path

root = Path(__file__).resolve().parents[2]
source = (root / "src/field_move.c").read_text()

def field_move(name):
    start = f"static bool32 IsFieldMoveUnlocked_{name}(void)"
    assert source.count(start) == 1, name
    return source.split(start, 1)[1].split("static bool32 IsFieldMoveUnlocked_", 1)[0]

cut = field_move("Cut")
flash = field_move("Flash")
strength = field_move("Strength")
fly = field_move("Fly")
rock_smash = field_move("RockSmash")
surf = field_move("Surf")
waterfall = field_move("Waterfall")

assert "region == REGION_HOENN ? 0 : 1" in cut, "Cut: Hive / Cascade / Stone"
assert "region == REGION_HOENN ? 1 : 0" in flash, "Flash: Zephyr / Boulder / Knuckle"
assert "region == REGION_JOHTO ? 2 : 3" in strength, "Strength: Plain / fourth gym elsewhere"
assert "region == REGION_KANTO" in fly and "HasBadge(region, 2)" in fly
assert "region == REGION_JOHTO" in fly and "HasBadge(region, 4)" in fly
assert "return HasBadge(region, 5)" in fly, "Hoenn Fly: Feather"

# Unchanged regional gates and tool-item bypasses remain intact.
assert "region == REGION_KANTO ? 5 : 2" in rock_smash
assert "region == REGION_JOHTO ? 3 : 4" in surf
assert "region == REGION_KANTO ? 6 : 7" in waterfall
for name in ("Cut", "Strength", "RockSmash", "Surf", "Waterfall"):
    assert "CheckBagHasItem" in field_move(name), name
for name in ("Cut", "Flash", "Strength", "Fly"):
    assert "GetCurrentRegion()" in field_move(name), name

# Authoritative Johto gym awards must match the field-move indices.
gyms = (
    ("VioletCity_Gym", "FLAG_JOHTO_BADGE_1", "FLASH"),
    ("AzaleaTown_Gym", "FLAG_JOHTO_BADGE_2", "CUT"),
    ("GoldenrodCity_Gym", "FLAG_JOHTO_BADGE_3", "STRENGTH"),
    ("CianwoodGym", "FLAG_JOHTO_BADGE_5", None),
)
for path, flag, move in gyms:
    text = (root / "data/maps" / path / "scripts.inc").read_text()
    assert f"setflag {flag}" in text, path
    if move:
        assert move in text, (path, move)

print("PASS: regional field move gates match Johto's Zephyr/Hive/Plain/Storm badges")
