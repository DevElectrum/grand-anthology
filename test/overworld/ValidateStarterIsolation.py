#!/usr/bin/env python3
"""P0 guard: three starter choices must survive inter-region travel independently.

This checks the save/migration source contract, not emulator runtime or backwards
recovery of already-overwritten v10 starter choices.
"""
from pathlib import Path
import re

root=Path(__file__).resolve().parents[2]
global_h=(root/"include/constants/global.h").read_text()
johto=(root/"include/constants/johto_vars.h").read_text()
kanto=(root/"include/constants/vars_frlg.h").read_text()
event=(root/"src/event_data.c").read_text()
migration=(root/"src/load_save.c").read_text()
region=(root/"src/region_switch.c").read_text()
assert "#define SAVE_FORMAT_VERSION 12" in global_h  # v11 starter isolation remains; v12 adds independent randomizer options
assert "#define VAR_JOHTO_STARTER_MON           VAR_JOHTO_SLICE(0x3c)" in johto
assert "#define VAR_STARTER_MON_FRLG               VAR_KANTO_SLICE(0x0C)" in kanto
part=event.split("u16 *GetVarPointer(u16 id)",1)[1].split("else if (id < SPECIAL_VARS_START)",1)[0]
assert "if (id == VAR_STARTER_MON)" in part
assert "gCurrentRegion == REGION_KANTO" in part and "VAR_STARTER_MON_FRLG - REGION_VARS_START" in part
assert "gCurrentRegion == REGION_JOHTO" in part and "VAR_JOHTO_STARTER_MON - REGION_VARS_START" in part
assert "return &gSaveBlock1Ptr->vars[id - VARS_START];" in event
assert "if (savedVersion < 11)" in migration
assert "gSaveBlock1Ptr->vars[VAR_STARTER_MON - VARS_START]" in migration
assert "VarSet(VAR_STARTER_MON_FRLG, legacyStarter)" in migration
assert "VarSet(VAR_JOHTO_STARTER_MON, legacyStarter)" in migration
assert "gSaveBlock2Ptr->currentRegion = region" in region
print("P0 per-region starter and v11 migration source contract: PASS")
