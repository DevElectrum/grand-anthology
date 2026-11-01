#ifndef GUARD_XGBA_A2_GAME_H
#define GUARD_XGBA_A2_GAME_H

#include "global.h"

// Available ONLY in a separately qualified A2 experimental build.
// Page index is zero-based, with 14 local boxes per page.
// A failed switch locks out saves until reloading the last valid cohort.
#if defined(XGBA_A2) && XGBA_A2
u8 XgbaA2GamePage(void);
bool8 XgbaA2GameSwitchPage(u8 page);
#endif

#endif
