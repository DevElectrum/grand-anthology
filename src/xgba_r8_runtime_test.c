#include "global.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "xgba_a2_game.h"
#include "xgba_r8_runtime_test.h"

#if defined(XGBA_R8_TEST) && XGBA_R8_TEST
// Disposable CI-only failure locator. Never used by production builds.
EWRAM_DATA volatile u32 gXgbaR8FixtureStage = 0;
// Instrumentation for genuine graphical PC callbacks, disposable ROM only.
EWRAM_DATA volatile u32 gXgbaR8PcUiTrace = 0;
// Bit31: global chooser preview, bit30: resident graphical PC idle.
// Low 16 bits: preview global box or resident local box.
// Bits 16..23: resident page. Never read this instrumentation in production.
EWRAM_DATA volatile u32 gXgbaR8PcChooserTrace = 0;
// Test-only status for a controller-driven withdrawal from global Box 15.
// Area and position are sampled by the real PC callbacks, not Lua pokes.
EWRAM_DATA volatile u32 gXgbaR8PcC3DTrace = 0;

bool8 XgbaR8C3DWithdrawProof(void)
{
    struct BoxPokemon *source = XgbaA2GameGlobalBoxMon(14, 5);
    u8 matches = 0;
    u8 position;

    if (source == NULL || GetBoxMonData(source, MON_DATA_SPECIES) != SPECIES_NONE)
        return FALSE;

    for (position = 0; position < PARTY_SIZE; position++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][position];
        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_SQUIRTLE
            || GetMonData(mon, MON_DATA_PERSONALITY) != 0x3456789AU)
            continue;
        if (GetMonData(mon, MON_DATA_HELD_ITEM) != ITEM_POTION
            || (GetMonData(mon, MON_DATA_MARKINGS) & 8) == 0)
            return FALSE;
        matches++;
    }
    return matches == 1;
}

static int R8FixtureFail(u32 stage)
{
    gXgbaR8FixtureStage = stage;
    return 0;
}
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
        && GetBoxMonData(old28, MON_DATA_SPECIES) == SPECIES_NONE
        && XgbaA2GameCountGlobalBoxMons(0) == 1
        && XgbaA2GameCountGlobalBoxMons(13) == 0
        && XgbaA2GameCountGlobalBoxMons(14) == 1
        && XgbaA2GameCountGlobalBoxMons(27) == 0
        && XgbaA2GameCountGlobalBoxMons(28) == 1
        && XgbaA2GameCountGlobalBoxMons(111) == 1
        && XgbaA2GameCountGlobalBoxMons(112) == -1
        && XgbaA2GameGlobalBoxName(14) != NULL
        && VerifyOne(XgbaA2GameGlobalBoxMon(42, 0), SPECIES_EEVEE,
                     0x56789ABCU, ITEM_POTION);
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
        return R8FixtureFail(1);

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
            return R8FixtureFail(2);

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
            return R8FixtureFail(3);

        // Swap two occupied distant boxes and then swap back. A complete
        // encrypted record comparison proves no changes to ancillary data.
        memcpy(&firstBefore, first, sizeof(firstBefore));
        memcpy(&lastBefore, last, sizeof(lastBefore));
        if (!XgbaA2GameSwapGlobalBoxMons(0, 0, 111, 29)
            || !XgbaA2GameSwapGlobalBoxMons(0, 0, 111, 29)
            || memcmp(first, &firstBefore, sizeof(firstBefore)) != 0
            || memcmp(last, &lastBefore, sizeof(lastBefore)) != 0)
            return R8FixtureFail(4);

        if (!XgbaA2GameMoveGlobalBoxMon(13, 4, 14, 5)
            || !XgbaA2GameMoveGlobalBoxMon(27, 7, 28, 8)
            || !VerifyOne(XgbaA2GameGlobalBoxMon(0, 0), SPECIES_BULBASAUR,
                          0x12345678U, ITEM_POTION)
            || !VerifyOne(XgbaA2GameGlobalBoxMon(111, 29), SPECIES_CHARMANDER,
                          0x23456789U, ITEM_SUPER_POTION)
            || !VerifyOne(XgbaA2GameGlobalBoxMon(14, 5), SPECIES_SQUIRTLE,
                          0x3456789AU, ITEM_POTION)
            || !VerifyOne(XgbaA2GameGlobalBoxMon(28, 8), SPECIES_PIDGEY,
                          0x456789ABU, ITEM_SUPER_POTION))
            return R8FixtureFail(5);
    }

    // C3A party-to-remote-PC deposit. All invalid attempts must preserve
    // the actual party and the already-occupied box exactly.
    {
        u8 partyPosition = CalculatePlayerPartyCount();
        u8 chosen = 255;
        struct Pokemon *partyMon;
        struct BoxPokemon *occupied = XgbaA2GameGlobalBoxMon(0, 0);
        struct BoxPokemon *remote = XgbaA2GameGlobalBoxMon(42, 0);
        struct BoxPokemon before;
        if (partyPosition > PARTY_SIZE - 2 || occupied == NULL || remote == NULL
            || GetBoxMonData(remote, MON_DATA_SPECIES) != SPECIES_NONE)
            return R8FixtureFail(6);

        CreateMon(&gParties[B_TRAINER_PLAYER][partyPosition],
                  SPECIES_ZIGZAGOON, 29, 0x6789ABCDU, OTID_STRUCT_PLAYER_ID);
        CreateMon(&gParties[B_TRAINER_PLAYER][partyPosition + 1],
                  SPECIES_EEVEE, 30, 0x56789ABCU, OTID_STRUCT_PLAYER_ID);
        // CreateMon initializes the record, but not its battle stats or HP.
        // Without these calls, Zigzagoon is counted as fainted and the
        // legitimate last-usable-party-mon deposit guard rejects Eevee.
        CalculateMonStats(&gParties[B_TRAINER_PLAYER][partyPosition]);
        CalculateMonStats(&gParties[B_TRAINER_PLAYER][partyPosition + 1]);
        if (GetMonData(&gParties[B_TRAINER_PLAYER][partyPosition], MON_DATA_HP) == 0
            || GetMonData(&gParties[B_TRAINER_PLAYER][partyPosition + 1], MON_DATA_HP) == 0)
            return R8FixtureFail(61);
        partyMon = &gParties[B_TRAINER_PLAYER][partyPosition + 1];
        item = ITEM_POTION;
        SetMonData(partyMon, MON_DATA_HELD_ITEM, &item);
        SetMonData(partyMon, MON_DATA_MARKINGS, &favorite);

        memcpy(&before, occupied, sizeof(before));
        if (XgbaA2GameDepositPartyMonGlobal(PARTY_SIZE, 42, NULL)
            || XgbaA2GameDepositPartyMonGlobal(partyPosition + 1, 112, NULL)
            || GetMonData(partyMon, MON_DATA_SPECIES) != SPECIES_EEVEE
            || memcmp(&before, occupied, sizeof(before)) != 0)
            return R8FixtureFail(7);

        // Separate transaction failure from return-slot, source-zero and
        // record checks. The E9 trace records the precise failing stage.
        if (!XgbaA2GameDepositPartyMonGlobal(partyPosition + 1, 42, &chosen))
            return R8FixtureFail(81);
        if (chosen != 0)
            return R8FixtureFail(82);
        if (GetMonData(partyMon, MON_DATA_SPECIES) != SPECIES_NONE)
            return R8FixtureFail(83);
        if (!VerifyOne(XgbaA2GameGlobalBoxMon(42, 0), SPECIES_EEVEE,
                       0x56789ABCU, ITEM_POTION))
            return R8FixtureFail(84);
        if (!VerifyTestPokemonRecords())
            return R8FixtureFail(85);
        if (GetMonData(&gParties[B_TRAINER_PLAYER][partyPosition], MON_DATA_SPECIES)
            != SPECIES_ZIGZAGOON)
            return R8FixtureFail(9);

        // C3B: a complete encrypted Eevee round trip from distant Box 43
        // must preserve its identity, item and markings before Save2.
        {
            struct BoxPokemon boxedBefore;
            struct Pokemon savedParty[PARTY_SIZE];
            u8 insertedPosition = 255;
            memcpy(&boxedBefore, remote, sizeof(boxedBefore));
            if (XgbaA2GameWithdrawGlobalBoxMon(112, 0, NULL)
                || XgbaA2GameWithdrawGlobalBoxMon(42, IN_BOX_COUNT, NULL)
                || XgbaA2GameWithdrawGlobalBoxMon(42, 1, NULL)
                || memcmp(remote, &boxedBefore, sizeof(boxedBefore)) != 0
                || GetMonData(partyMon, MON_DATA_SPECIES) != SPECIES_NONE)
                return R8FixtureFail(86);

            // A synthetic full-party state must reject withdrawal without
            // clearing the boxed source; the snapshot is restored verbatim.
            memcpy(savedParty, gParties[B_TRAINER_PLAYER], sizeof(savedParty));
            for (u8 pos = partyPosition + 1; pos < PARTY_SIZE; pos++)
                gParties[B_TRAINER_PLAYER][pos] = gParties[B_TRAINER_PLAYER][partyPosition];
            if (CalculatePlayerPartyCount() != PARTY_SIZE
                || XgbaA2GameWithdrawGlobalBoxMon(42, 0, NULL)
                || memcmp(remote, &boxedBefore, sizeof(boxedBefore)) != 0)
                return R8FixtureFail(87);
            memcpy(gParties[B_TRAINER_PLAYER], savedParty, sizeof(savedParty));
            CalculatePlayerPartyCount();

            if (!XgbaA2GameWithdrawGlobalBoxMon(42, 0, &insertedPosition)
                || insertedPosition != partyPosition + 1
                || memcmp(&partyMon->box, &boxedBefore, sizeof(boxedBefore)) != 0
                || GetBoxMonData(remote, MON_DATA_SPECIES) != SPECIES_NONE
                || GetMonData(partyMon, MON_DATA_SPECIES) != SPECIES_EEVEE)
                return R8FixtureFail(88);
            chosen = 255;
            if (!XgbaA2GameDepositPartyMonGlobal(partyPosition + 1, 42, &chosen)
                || chosen != 0
                || GetMonData(partyMon, MON_DATA_SPECIES) != SPECIES_NONE
                || memcmp(remote, &boxedBefore, sizeof(boxedBefore)) != 0
                || !VerifyTestPokemonRecords())
                return R8FixtureFail(89);
        }
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
                return R8FixtureFail(10);
        }
    }

    // Explicit 112 -> 1 wrap; persist global Box 30 (group 2, local 2).
    if (!XgbaA2GameSelectGlobalBox(0)
        || !VerifyTestPokemonRecords()
        || !XgbaA2GameSelectGlobalBox(29))
        return R8FixtureFail(11);
    return XgbaR8VerifyTestPokemon();
}

int XgbaR8VerifyTestPokemon(void)
{
    return VerifyTestPokemonRecords()
        && XgbaA2GamePage() == 2
        && StorageGetCurrentBox() == 1;
}

// Save2 is the established 112-box backend checkpoint (Box 30 selected).
// Save3 is only reached by C3C's actual graphical Jump into Box 15.
// The load-time verifier checks the full test-record set on both commits
// and rejects missing/wrong selected groups rather than falling back.
int XgbaR8VerifyTestPokemonAtCounter(u32 counter)
{
    if (counter == 2)
        return XgbaR8VerifyTestPokemon();
    if (counter == 3)
        return VerifyTestPokemonRecords()
            && XgbaA2GamePage() == 1
            && StorageGetCurrentBox() == 0;
    return FALSE;
}
#endif
