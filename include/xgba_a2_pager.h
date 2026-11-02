#ifndef GUARD_XGBA_A2_PAGER_H
#define GUARD_XGBA_A2_PAGER_H

#include "xgba_a2_journal.h"

// One live 14-box PokemonStorage buffer; pending records reside only in E6.
// The containing save coordinator must verify the native+metadata cohort
// before initialization, and must commit metadata AFTER the new native slot.
struct XgbaA2Pager
{
    struct XgbaA2JournalIo io;
    void *resident;
    uint32_t committed[XGBA_A2_PC_PAGES];
    uint32_t staged[XGBA_A2_PC_PAGES];
    uint8_t activePage;
    // Optional R8 RAM cache; never serialized and never owned by E6.
    uint8_t *ramPages;
    uint8_t ready;
    uint8_t fatal;
};

int XgbaA2PagerInit(struct XgbaA2Pager *pager,
                    const struct XgbaA2JournalIo *io,
                    void *currentNativePc, uint8_t selectedPage,
                    const uint32_t committedGenerations[XGBA_A2_PC_PAGES]);
int XgbaA2PagerSwitch(struct XgbaA2Pager *pager, uint8_t destination);
int XgbaA2PagerPrepareSave(struct XgbaA2Pager *pager,
                           uint32_t pageGenerations[XGBA_A2_PC_PAGES]);
// Enables all 112 cached boxes only after eight exact-generation reads.
// The active native PC must byte-match the selected committed page.
int XgbaA2PagerEnableRam(struct XgbaA2Pager *pager, void *ram, uint32_t bytes);

int XgbaA2PagerCommit(struct XgbaA2Pager *pager,
                      const uint32_t committedGenerations[XGBA_A2_PC_PAGES]);

#endif
