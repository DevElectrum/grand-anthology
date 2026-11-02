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
    struct BoxPokemon *old14 = XgbaA2GameGlobalBoxMon(13, 4);
    struct BoxPokemon *old28 = XgbaA2GameGlobalBoxMon(27, 7);
    return VerifyOne(XgbaA2GameGlobalBoxMon(0, 0), SPECIES_BULBASAUR,
                     0x12345678U, ITEM_POTION)
        && VerifyOne(XgbaA2GameGlobalBoxMon(111, 29), SPECIES_CHARMANDER,
                     0x23456789U, ITEM_SUPER_POTION)
        && VerifyOne(XgbaA2GameGlobalBoxMon(14, 5), SPECIES_SQUIRTLE,
                     0x3456789AU, ITEM_POTION)
        && VerifyOne(XgbaA2GameGlobalBoxMon(28, 8), SPECIES_PIDGEY,
                     0x456789ABU, ITEM_SUPER_POTION)
        && old14 != NULL && old28 != NULL
        && GetBoxMonData(old14, MON_DATA_SPECIES) == SPECIES_NONE
        && GetBoxMonData(old28, MON_DATA_SPECIES) == SPECIES_NONE;
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

    // Use genuine game-created records and exercise cross-group movement
    // BEFORE the save. The old slots must become empty and the destination
    // slots must retain species, PID, held item and favorite markings.
    {
        struct BoxPokemon *old14 = XgbaA2GameGlobalBoxMon(13, 4);
        struct BoxPokemon *old28 = XgbaA2GameGlobalBoxMon(27, 7);
        struct BoxPokemon firstBefore;
        struct BoxPokemon lastBefore;

        if (old14 == NULL || old28 == NULL
            || GetBoxMonData(old14, MON_DATA_SPECIES) != SPECIES_NONE
            || GetBoxMonData(old28, MON_DATA_SPECIES) != SPECIES_NONE
            || GetBoxMonData(XgbaA2GameGlobalBoxMon(14, 5), MON_DATA_SPECIES) != SPECIES_NONE
            || GetBoxMonData(XgbaA2GameGlobalBoxMon(28, 8), MON_DATA_SPECIES) != SPECIES_NONE)
            return 0;

        CreateMon(&mon, SPECIES_SQUIRTLE, 27, 0x3456789AU, OTID_STRUCT_PLAYER_ID);
        memcpy(old14, &mon.box, sizeof(mon.box));
        item = ITEM_POTION;
        SetBoxMonData(old14, MON_DATA_HELD_ITEM, &item);
        SetBoxMonData(old14, MON_DATA_MARKINGS, &favorite);

        CreateMon(&mon, SPECIES_PIDGEY, 28, 0x456789ABU, OTID_STRUCT_PLAYER_ID);
        memcpy(old28, &mon.box, sizeof(mon.box));
        item = ITEM_SUPER_POTION;
        SetBoxMonData(old28, MON_DATA_HELD_ITEM, &item);
        SetBoxMonData(old28, MON_DATA_MARKINGS, &favorite);

        // Reject invalid indexes, self-move, occupied destination, and
        // swap with an empty slot. All of these MUST be no-ops.
        if (XgbaA2GameMoveGlobalBoxMon(112, 0, 14, 5)
            || XgbaA2GameMoveGlobalBoxMon(13, 30, 14, 5)
            || XgbaA2GameMoveGlobalBoxMon(13, 4, 13, 4)
            || XgbaA2GameMoveGlobalBoxMon(0, 0, 111, 29)
            || XgbaA2GameSwapGlobalBoxMons(13, 4, 14, 5))
            return 0;

        // Swap two occupied distant boxes and then swap back. A complete
        // encrypted record comparison proves no changes to ancillary data.
        memcpy(&firstBefore, first, sizeof(firstBefore));
        memcpy(&lastBefore, last, sizeof(lastBefore));
        if (!XgbaA2GameSwapGlobalBoxMons(0, 0, 111, 29)
            || !XgbaA2GameSwapGlobalBoxMons(0, 0, 111, 29)
            || memcmp(first, &firstBefore, sizeof(firstBefore)) != 0
            || memcmp(last, &lastBefore, sizeof(lastBefore)) != 0)
            return 0;

        if (!XgbaA2GameMoveGlobalBoxMon(13, 4, 14, 5)
            || !XgbaA2GameMoveGlobalBoxMon(27, 7, 28, 8)
            || !VerifyTestPokemonRecords())
            return 0;
    }

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
