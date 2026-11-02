#include "global.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "xgba_a2_game.h"
#include "xgba_r8_runtime_test.h"

#if defined(XGBA_R8_TEST) && XGBA_R8_TEST
static int VerifyOne(struct BoxPokemon *mon, enum Species species,
                     u32 personality, enum Item held)
{
    return mon != NULL
        && GetBoxMonData(mon, MON_DATA_SPECIES) == species
        && GetBoxMonData(mon, MON_DATA_PERSONALITY) == personality
        && GetBoxMonData(mon, MON_DATA_HELD_ITEM) == held
        && (GetBoxMonData(mon, MON_DATA_MARKINGS) & 8) != 0;
}

static int VerifyTestPokemonRecords(void)
{
    return VerifyOne(XgbaA2GameGlobalBoxMon(0, 0), SPECIES_BULBASAUR,
                     0x12345678U, ITEM_POTION)
        && VerifyOne(XgbaA2GameGlobalBoxMon(111, 29), SPECIES_CHARMANDER,
                     0x23456789U, ITEM_SUPER_POTION);
}

int XgbaR8CreateTestPokemon(void)
{
    struct BoxPokemon *first = XgbaA2GameGlobalBoxMon(0, 0);
    struct BoxPokemon *last = XgbaA2GameGlobalBoxMon(111, 29);
    struct Pokemon mon;
    enum Item item;
    u8 favorite = 8;

    if (first == NULL || last == NULL
        || GetBoxMonData(first, MON_DATA_SPECIES) != SPECIES_NONE
        || GetBoxMonData(last, MON_DATA_SPECIES) != SPECIES_NONE)
        return 0;

    CreateMon(&mon, SPECIES_BULBASAUR, 25, 0x12345678U, OTID_STRUCT_PLAYER_ID);
    memcpy(first, &mon.box, sizeof(mon.box));
    item = ITEM_POTION;
    SetBoxMonData(first, MON_DATA_HELD_ITEM, &item);
    SetBoxMonData(first, MON_DATA_MARKINGS, &favorite);

    CreateMon(&mon, SPECIES_CHARMANDER, 26, 0x23456789U, OTID_STRUCT_PLAYER_ID);
    memcpy(last, &mon.box, sizeof(mon.box));
    item = ITEM_SUPER_POTION;
    SetBoxMonData(last, MON_DATA_HELD_ITEM, &item);
    SetBoxMonData(last, MON_DATA_MARKINGS, &favorite);

    // Traverse each of the 112 global box IDs, checking the selected
    // group/local index and both distant real Pokemon after each change.
    {
        u16 box;
        for (box = 0; box < XGBA_A2_PC_PAGES * TOTAL_BOXES_COUNT; box++)
        {
            if (!XgbaA2GameSelectGlobalBox(box)
                || XgbaA2GamePage() != box / TOTAL_BOXES_COUNT
                || StorageGetCurrentBox() != box % TOTAL_BOXES_COUNT
                || !VerifyTestPokemonRecords())
                return 0;
        }
    }

    // Explicit 112 -> 1 wrap; persist global Box 30 (group 2, local 2).
    if (!XgbaA2GameSelectGlobalBox(0)
        || !VerifyTestPokemonRecords()
        || !XgbaA2GameSelectGlobalBox(29))
        return 0;
    return XgbaR8VerifyTestPokemon();
}

int XgbaR8VerifyTestPokemon(void)
{
    return VerifyTestPokemonRecords()
        && XgbaA2GamePage() == 2
        && StorageGetCurrentBox() == 1;
}
#endif
