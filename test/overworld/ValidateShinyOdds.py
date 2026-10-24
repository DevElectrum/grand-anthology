#!/usr/bin/env python3
"""P0 source guard: shiny options saved in the existing block and used in generation."""
from pathlib import Path
import re
root = Path(__file__).resolve().parents[2]
g = (root / "include/global.h").read_text()
n = (root / "src/new_game.c").read_text()
m = (root / "src/option_menu.c").read_text()
p = (root / "src/pokemon.c").read_text()
k = (root / "include/constants/global.h").read_text()
assert "optionsShinyOdds:4" in g
# The new selector consumes only a previously-unused bit, not a byte;
# all original 0..7 save IDs must be fixed and the SaveBlock2 ABI unchanged.
assert "sizeof(struct SaveBlock2) == 0xF2C" in g
assert "OPTIONS_SHINY_ODDS_1_128    0" in k
assert "OPTIONS_SHINY_ODDS_COUNT   11" in k
save_ids = dict((a, int(b)) for a,b in re.findall(r"#define\s+(OPTIONS_SHINY_ODDS_\w+)\s+(\d+)", k))
expected_ids = {
    "OPTIONS_SHINY_ODDS_1_128":0, "OPTIONS_SHINY_ODDS_1_8192":1,
    "OPTIONS_SHINY_ODDS_1_4096":2, "OPTIONS_SHINY_ODDS_1_1024":3,
    "OPTIONS_SHINY_ODDS_1_512":4, "OPTIONS_SHINY_ODDS_1_64":5,
    "OPTIONS_SHINY_ODDS_1_16":6, "OPTIONS_SHINY_ODDS_ALWAYS":7,
    "OPTIONS_SHINY_ODDS_1_8":8, "OPTIONS_SHINY_ODDS_1_4":9,
    "OPTIONS_SHINY_ODDS_1_2":10, "OPTIONS_SHINY_ODDS_COUNT":11,
}
assert save_ids == expected_ids, (save_ids, expected_ids)
assert "optionsShinyOdds = OPTIONS_SHINY_ODDS_1_128" in n
assert "MENUITEM_SHINYODDS" in m
assert "gTasks[taskId].tShinyOdds = gSaveBlock2Ptr->optionsShinyOdds;" in m
assert "gSaveBlock2Ptr->optionsShinyOdds = gTasks[taskId].tShinyOdds;" in m
block = re.search(r"static const u32 sShinyThresholds\[OPTIONS_SHINY_ODDS_COUNT\] =\s*\{([^}]+)\};", p)
assert block
vals = dict((a, int(b)) for a,b in re.findall(r"\[(OPTIONS_SHINY_ODDS_\w+)\]\s*=\s*(\d+)", block.group(1)))
assert vals == dict(zip(["OPTIONS_SHINY_ODDS_1_128","OPTIONS_SHINY_ODDS_1_8192","OPTIONS_SHINY_ODDS_1_4096","OPTIONS_SHINY_ODDS_1_1024","OPTIONS_SHINY_ODDS_1_512","OPTIONS_SHINY_ODDS_1_64","OPTIONS_SHINY_ODDS_1_16","OPTIONS_SHINY_ODDS_1_8","OPTIONS_SHINY_ODDS_1_4","OPTIONS_SHINY_ODDS_1_2","OPTIONS_SHINY_ODDS_ALWAYS"],[512,8,16,64,128,1024,4096,8192,16384,32768,65536]))
for denominator, symbol in ((8,"OPTIONS_SHINY_ODDS_1_8"),(4,"OPTIONS_SHINY_ODDS_1_4"),(2,"OPTIONS_SHINY_ODDS_1_2")):
    assert vals[symbol] * denominator == 65536
order_match = re.search(r"static const u8 sShinyOddsMenuOrder\[OPTIONS_SHINY_ODDS_COUNT\]\s*=\s*\{([^}]+)\};", m)
assert order_match, "shiny odds menu order not declared"
order = re.findall(r"OPTIONS_SHINY_ODDS_(?:\w+)", order_match.group(1))
assert order == ["OPTIONS_SHINY_ODDS_1_128","OPTIONS_SHINY_ODDS_1_8192","OPTIONS_SHINY_ODDS_1_4096","OPTIONS_SHINY_ODDS_1_1024","OPTIONS_SHINY_ODDS_1_512","OPTIONS_SHINY_ODDS_1_64","OPTIONS_SHINY_ODDS_1_16","OPTIONS_SHINY_ODDS_1_8","OPTIONS_SHINY_ODDS_1_4","OPTIONS_SHINY_ODDS_1_2","OPTIONS_SHINY_ODDS_ALWAYS"]
assert len(set(order)) == 11
for label in ("gText_Shiny8","gText_Shiny4","gText_Shiny2"):
    assert label in m
assert "sShinyOddsMenuOrder[index]" in m
assert "GET_SHINY_VALUE(value, personality) < threshold" in p
print("P0 shiny odds source contract: PASS")
