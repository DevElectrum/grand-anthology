#!/usr/bin/env python3
"""P0 global PC search contract (source-level guard, not emulator runtime proof).

The saved 14x30 PC ABI must stay unchanged until the separately planned
large-capacity persistence backend is designed and validated.
"""
from pathlib import Path
base=Path(__file__).resolve().parents[2]
def read(name):
    return (base/name).read_text()
storage=read("include/pokemon_storage_system.h")
header=read("include/world_pc_search.h")
search=read("src/world_pc_search.c")
ui=read("src/debug.c")
assert "#define TOTAL_BOXES_COUNT       14" in storage
assert "#define IN_BOX_ROWS             5" in storage
assert "#define IN_BOX_COLUMNS          6" in storage
assert "offsetof(struct PokemonStorage, boxes) == 0x0004" in storage
assert "offsetof(struct PokemonStorage, boxNames) == 0x8344" in storage
for name in ("WORLD_PC_SORT_LOCATION", "WORLD_PC_SORT_SPECIES", "WORLD_PC_SORT_LEVEL",
             "WORLD_PC_SORT_ITEM", "species;", "heldItem;", "minLevel;",
             "maxLevel;", "shinyOnly;", "includeEggs;"):
    assert name in header, name
for guarded in ("GetBoxedMonPtr(box, slot)", "MON_DATA_SANITY_HAS_SPECIES",
                "MON_DATA_SANITY_IS_BAD_EGG", "MON_DATA_SANITY_IS_EGG",
                "MON_DATA_SPECIES", "MON_DATA_HELD_ITEM", "MON_DATA_IS_SHINY",
                "GetLevelFromBoxMonExp(mon)", "gPokemonStoragePtr == NULL",
                "if (out == NULL)", "WorldPcSortResults(out, stored, sortMode)"):
    assert guarded in search, guarded
for unsafe in ("SetBoxMonData(", "SetBoxMonDataAt(", "SetBoxMonAt(",
               "ZeroBoxMonAt(", "memcpy(gPokemonStoragePtr", "SetMonData("):
    assert unsafe not in search, unsafe
assert "for (box = 0; box < TOTAL_BOXES_COUNT; box++)" in search
assert "for (slot = 0; slot < IN_BOX_COUNT; slot++)" in search
assert "for (i = count / 2; i > 0; i--)" in search
assert "WorldPcSiftDown(results, 0, i, mode)" in search
assert "static void DebugAction_PcSearch_Open(u8 taskId)" in ui
assert "static void DebugAction_PcSearch_SelectSpecies(u8 taskId)" in ui
assert 'COMPOUND_STRING("Find in PC")' in ui
# Actual full-result browsing (not just displaying the first match).
assert "WorldPcSearch(&filter, gTasks[taskId].tPcSort," in ui
assert "sPlayerPcSearchResults, TOTAL_BOXES_COUNT * IN_BOX_COUNT)" in ui
assert "sPlayerPcSearchResults = Alloc(TOTAL_BOXES_COUNT * IN_BOX_COUNT" in ui
assert "DebugAction_PcSearch_FreeResults();" in ui
assert "GetSpeciesName(result->species)" in ui
assert "result->boxId + 1" in ui and "result->boxSlot + 1" in ui
assert "tPcCursor" in ui and "tPcTotal" in ui and "tPcSort" in ui
assert "DebugAction_PcSearch_Browse(u8 taskId)" in ui
assert "JOY_REPEAT(DPAD_UP)" in ui and "JOY_REPEAT(DPAD_DOWN)" in ui
assert "JOY_NEW(DPAD_LEFT)" in ui and "JOY_NEW(DPAD_RIGHT)" in ui
assert "JOY_NEW(SELECT_BUTTON)" in ui
assert "filter.shinyOnly = gTasks[taskId].tPcShinyOnly;" in ui
assert "filter.minLevel = gTasks[taskId].tPcMinLevel;" in ui
assert "if (JOY_NEW(START_BUTTON))" in ui
assert "gTasks[taskId].tPcMinLevel += 25" in ui
assert "gTasks[taskId].tPcMinLevel = 0;" in ui
assert "filter.includeEggs = TRUE;" in ui
assert "gTasks[taskId].tInput = SPECIES_NONE;" in ui
assert "gTasks[taskId].tPcSort + WORLD_PC_SORT_ITEM" in ui
assert "gTasks[taskId].tPcSort + 1" in ui
finder=ui.split("static void DebugAction_PcSearch_DrawSpecies(u8 taskId)\n{",1)[1].split("static void DebugAction_Give_MaxMoney(u8 taskId)\n{",1)[0]
assert chr(92)*2 + "n" not in finder, "GBA text macros cannot encode escaped literal backslashes"
# The earlier UI regression inserted PC functions into the seed editor's
# B-button branch: compiler reported "invalid storage class for function".
# Catch truncated C function boundaries before launching the costly GBA build.
previous_handler=ui.split("static void DebugAction_Randomizer_HandleSeed(u8 taskId)\n{",1)[1].split("// A search over the complete saved PC.",1)[0]
assert "\n}\n\n// P0 searchable shared PC:" in previous_handler, "seed editor must finish before PC finder"
assert previous_handler.count("{")+1 == previous_handler.count("}"), "unbalanced seed editor, PC finder nested"
pc_block=ui.split("// A search over the complete saved PC.",1)[1].split("static void DebugAction_Give_MaxMoney(u8 taskId)\n{",1)[0]
assert pc_block.count("{") == pc_block.count("}"), "unbalanced PC finder function definitions"

print("P0 global PC search + browsable sorted results/shiny/all-species filter, leak-safe view: PASS")
