#!/usr/bin/env python3
"""Regression gate for extended battle multipliers and unchanged save ABI."""
from pathlib import Path
import re
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
header = (root / "include/qol_multipliers.h").read_text()
opts = (root / "src/option_menu.c").read_text()
battle = (root / "src/battle_script_commands.c").read_text()
start = (root / "src/new_game.c").read_text()
fields = (root / "include/global.h").read_text()
ids = (root / "include/constants/global.h").read_text()
vars_file = (root / "include/constants/vars.h").read_text()
kanto = (root / "include/constants/vars_frlg.h").read_text()

def need(text, token):
    assert token in text, "Missing code contract: " + token

need(fields, "u16 optionsExpMultiplier:2;")
need(fields, "u16 optionsCatchMultiplier:2;")
need(fields, "sizeof(struct SaveBlock2) == 0xF2C")
need(vars_file, "#define VAR_QOL_MULTIPLIERS                               0x4091")
assert len(re.findall(r'\bVAR_0x4091\b', kanto)) == 1
need(header, "QOL_MULT_EXT_SIGNATURE 0xA500")
need(header, "extra >= OPTIONS_EXP_MULT_0_25X")
need(header, "extra >= OPTIONS_CATCH_MULT_3X")
need(header, "VarSet(VAR_QOL_MULTIPLIERS, extra ? QOL_MULT_EXT_SIGNATURE | extra : 0)")
need(start, "sPreviousQolMultiplierExtras = VarGet(VAR_QOL_MULTIPLIERS);")
need(start, "sNewGameQolMultiplierExtras = VarGet(VAR_QOL_MULTIPLIERS);")
need(start, "VarSet(VAR_QOL_MULTIPLIERS, sPreviousQolMultiplierExtras);")
need(start, "VarSet(VAR_QOL_MULTIPLIERS, sNewGameQolMultiplierExtras);")
assert start.index("ClearSav1();") < start.index("InitEventData();") < start.index("VarSet(VAR_QOL_MULTIPLIERS, sNewGameQolMultiplierExtras);")
need(opts, "SetQolMultiplierChoices(gTasks[taskId].tExpMult, gTasks[taskId].tCatchMult)")
need(opts, "gTasks[taskId].tExpMult = GetExpMultiplierChoice()")
need(opts, "gTasks[taskId].tCatchMult = GetCatchMultiplierChoice()")
need(opts, "sExpMultMenuOrder[OPTIONS_EXP_MULT_COUNT]")
need(opts, "sCatchMultMenuOrder[OPTIONS_CATCH_MULT_COUNT]")
need(opts, "sExpMultTexts[selection]")
need(opts, "sCatchMultTexts[selection]")
need(battle, "switch (GetExpMultiplierChoice())")
need(battle, "switch (GetCatchMultiplierChoice())")
need(battle, "value > 0xFFFFFFFFu ? 0xFFFFFFFFu : value")
for n in [3, 5, 10]:
    need(battle, f"(u64)odds * {n} > 255 ? 255 : odds * {n}")

# Compile the actual production header against a minimal save/variable stub.
# Both old option fields intentionally have only TWO bits.
defs = "\n".join(re.findall(r"^#define OPTIONS_(?:EXP|CATCH)_MULT_\w+\s+\d+$", ids, re.M))
assert len(defs.splitlines()) == 17, "Expected 8 EXP, 7 Catch options and 2 count constants"
mocked = header.replace('#include "global.h"', "").replace('#include "event_data.h"', "").replace('#include "constants/vars.h"', "")
harness = r"""
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
typedef uint8_t u8;
typedef uint16_t u16;
struct SaveBlock2 {
    u16 optionsExpMultiplier:2;
    u16 optionsCatchMultiplier:2;
};
static struct SaveBlock2 save;
static struct SaveBlock2 *gSaveBlock2Ptr = &save;
#define VAR_QOL_MULTIPLIERS 0x4091
static u16 stored;
static u16 VarGet(u16 id) { assert(id == VAR_QOL_MULTIPLIERS); return stored; }
static int VarSet(u16 id, u16 value) { assert(id == VAR_QOL_MULTIPLIERS); stored = value; return 1; }
"""
test = r"""
int main(void)
{
    for (u8 exp = 0; exp < OPTIONS_EXP_MULT_COUNT; exp++)
    for (u8 catchRate = 0; catchRate < OPTIONS_CATCH_MULT_COUNT; catchRate++)
    {
        save.optionsExpMultiplier = 0;
        save.optionsCatchMultiplier = 0;
        stored = 0;
        SetQolMultiplierChoices(exp, catchRate);
        assert(GetExpMultiplierChoice() == exp);
        assert(GetCatchMultiplierChoice() == catchRate);
        assert(save.optionsExpMultiplier == (exp < 4 ? exp : 0));
        assert(save.optionsCatchMultiplier == (catchRate < 4 ? catchRate : 0));
        assert((stored == 0) == (exp < 4 && catchRate < 4));
        if (stored)
            assert((stored & 0xFF00) == QOL_MULT_EXT_SIGNATURE);
    }
    // Existing saves may contain arbitrary old bits in the reserved var.
    for (int oldExp = 0; oldExp < 4; oldExp++)
    for (int oldCatch = 0; oldCatch < 4; oldCatch++)
    {
        save.optionsExpMultiplier = oldExp;
        save.optionsCatchMultiplier = oldCatch;
        stored = 0x1234; // not the new signature
        assert(GetExpMultiplierChoice() == oldExp);
        assert(GetCatchMultiplierChoice() == oldCatch);
        stored = 0xA5FF; // signed, but invalid high-option IDs
        assert(GetExpMultiplierChoice() == oldExp);
        assert(GetCatchMultiplierChoice() == oldCatch);
    }
    puts("PASS: 56 multiplier choices and old-save fallback compiled from actual header");
    return 0;
}
"""
with tempfile.TemporaryDirectory(prefix="qol-multipliers-") as dirname:
    file = Path(dirname) / "test.c"
    executable = Path(dirname) / "test"
    file.write_text(harness + "\n" + defs + "\n" + mocked + "\n" + test)
    subprocess.run(["gcc", "-std=c11", "-Wall", "-Wextra", "-Werror", "-O2",
                    str(file), "-o", str(executable)], check=True)
    subprocess.run([str(executable)], check=True)

print("PASS: expanded battle options, ABI, new-game rollback and arithmetic source guards")
