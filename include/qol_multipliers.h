#ifndef GUARD_QOL_MULTIPLIERS_H
#define GUARD_QOL_MULTIPLIERS_H

#include "global.h"
#include "event_data.h"
#include "constants/vars.h"

// This shared, checksummed SaveBlock1 var was previously unused.
// Its high byte is a signature, so existing saves cannot accidentally
// select a new multiplier. Low nibbles store extended IDs only (4+).
// Existing option fields and their numeric IDs stay unchanged.
#define QOL_MULT_EXT_SIGNATURE 0xA500

static inline u16 GetQolMultiplierExtras(void)
{
    u16 value = VarGet(VAR_QOL_MULTIPLIERS);
    return (value & 0xFF00) == QOL_MULT_EXT_SIGNATURE ? value : 0;
}

static inline u8 GetExpMultiplierChoice(void)
{
    u8 extra = (GetQolMultiplierExtras() >> 4) & 0xF;
    if (extra >= OPTIONS_EXP_MULT_0_25X && extra < OPTIONS_EXP_MULT_COUNT)
        return extra;
    return gSaveBlock2Ptr->optionsExpMultiplier;
}

static inline u8 GetCatchMultiplierChoice(void)
{
    u8 extra = GetQolMultiplierExtras() & 0xF;
    if (extra >= OPTIONS_CATCH_MULT_3X && extra < OPTIONS_CATCH_MULT_COUNT)
        return extra;
    return gSaveBlock2Ptr->optionsCatchMultiplier;
}

static inline void SetQolMultiplierChoices(u8 expChoice, u8 catchChoice)
{
    u16 extra = 0;

    if (expChoice < OPTIONS_EXP_MULT_0_25X)
        gSaveBlock2Ptr->optionsExpMultiplier = expChoice;
    else
    {
        gSaveBlock2Ptr->optionsExpMultiplier = OPTIONS_EXP_MULT_1X;
        extra |= expChoice << 4;
    }

    if (catchChoice < OPTIONS_CATCH_MULT_3X)
        gSaveBlock2Ptr->optionsCatchMultiplier = catchChoice;
    else
    {
        gSaveBlock2Ptr->optionsCatchMultiplier = OPTIONS_CATCH_MULT_1X;
        extra |= catchChoice;
    }

    VarSet(VAR_QOL_MULTIPLIERS, extra ? QOL_MULT_EXT_SIGNATURE | extra : 0);
}

#endif // GUARD_QOL_MULTIPLIERS_H
