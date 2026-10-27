#include "global.h"
#include "clear_save_data_menu.h"
#include "decompress.h"
#include "event_data.h"
#include "graphics.h"
#include "gpu_regs.h"
#include "intro.h"
#include "m4a.h"
#include "main.h"
#include "main_menu.h"
#include "palette.h"
#include "quickstart.h"
#include "reset_rtc_screen.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "title_screen.h"
#include "constants/rgb.h"
#include "constants/songs.h"

#if ALL_REGIONS

// The 240x160 title contains no prompt; reuse the original World blinking sprite.
#define ANTHOLOGY_SCREEN_BLOCK 30
#define ANTHOLOGY_TITLE_TILES_RAW_BYTES (577 * 64)
#define ANTHOLOGY_PRESS_START_TAG 1001
#define ANTHOLOGY_PRESS_START_PIECES 5
#define CLEAR_SAVE_COMBO (B_BUTTON | SELECT_BUTTON | DPAD_UP)
#define RESET_RTC_COMBO (B_BUTTON | SELECT_BUTTON | DPAD_LEFT)

static const struct OamData sAnthologyPressStartOam =
{
    .y = DISPLAY_HEIGHT,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sAnthologyPromptFrame0[] = { ANIMCMD_FRAME(1, 4), ANIMCMD_END };
static const union AnimCmd sAnthologyPromptFrame1[] = { ANIMCMD_FRAME(5, 4), ANIMCMD_END };
static const union AnimCmd sAnthologyPromptFrame2[] = { ANIMCMD_FRAME(9, 4), ANIMCMD_END };
static const union AnimCmd sAnthologyPromptFrame3[] = { ANIMCMD_FRAME(13, 4), ANIMCMD_END };
static const union AnimCmd sAnthologyPromptFrame4[] = { ANIMCMD_FRAME(17, 4), ANIMCMD_END };

static const union AnimCmd *const sAnthologyPromptAnims[] =
{
    sAnthologyPromptFrame0,
    sAnthologyPromptFrame1,
    sAnthologyPromptFrame2,
    sAnthologyPromptFrame3,
    sAnthologyPromptFrame4,
};

static void SpriteCB_AnthologyPressStart(struct Sprite *sprite)
{
    sprite->invisible = !(++sprite->data[0] & 16);
}

static const struct SpriteTemplate sAnthologyPressStartTemplate =
{
    .tileTag = ANTHOLOGY_PRESS_START_TAG,
    .paletteTag = ANTHOLOGY_PRESS_START_TAG,
    .oam = &sAnthologyPressStartOam,
    .anims = sAnthologyPromptAnims,
    .callback = SpriteCB_AnthologyPressStart,
};

static const struct CompressedSpriteSheet sAnthologyPressStartSheet =
{
    .data = gTitleScreenPressStartGfx,
    .size = 0x520,
    .tag = ANTHOLOGY_PRESS_START_TAG,
};

static const struct SpritePalette sAnthologyPressStartPalette =
{
    .data = gTitleScreenPressStartPal,
    .tag = ANTHOLOGY_PRESS_START_TAG,
};

static void CreateAnthologyPressStart(void)
{
    u8 i;
    for (i = 0; i < ANTHOLOGY_PRESS_START_PIECES; i++)
    {
        u8 spriteId = CreateSprite(&sAnthologyPressStartTemplate, 56 + i * 32, 145, 0);
        if (spriteId != MAX_SPRITES)
            StartSpriteAnim(&gSprites[spriteId], i);
    }
}

static const u32 sAnthologyTiles[] = INCBIN_U32("graphics/title_screen/world/anthology_title_tiles.bin");
static const u16 sAnthologyPalette[] = INCBIN_U16("graphics/title_screen/world/anthology_title_palette.bin");
static const u16 sAnthologyMap[] = INCBIN_U16("graphics/title_screen/world/anthology_title_map.bin");

STATIC_ASSERT(ANTHOLOGY_TITLE_TILES_RAW_BYTES <= ANTHOLOGY_SCREEN_BLOCK * 0x800, AnthologyTitleTilesFit);
STATIC_ASSERT(sizeof(sAnthologyTiles) == ANTHOLOGY_TITLE_TILES_RAW_BYTES, AnthologyTitleTilesLength);
STATIC_ASSERT(sizeof(sAnthologyPalette) == BG_PLTT_SIZE, AnthologyTitlePaletteSize);
STATIC_ASSERT(sizeof(sAnthologyMap) == 0x800, AnthologyTitleMapSize);

static void MainCB2_WorldTitleScreen(void);
static void Task_WorldTitleScreen(u8 taskId);
static void CB2_LeaveWorldTitleScreen(void);

static EWRAM_DATA MainCallback sWorldTitleNextCallback = NULL;

static void VBlankCB_WorldTitleScreen(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_InitWorldTitleScreen(void)
{
    u32 i;

    switch (gMain.state)
    {
    case 0:
        SetVBlankCallback(NULL);
        SetHBlankCallback(NULL);
        ScanlineEffect_Stop();
        ScanlineEffect_Clear();
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_FORCED_BLANK);
        for (i = REG_OFFSET_BG0CNT; i <= REG_OFFSET_BG3VOFS; i += 2)
            SetGpuReg(i, 0);
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WIN1H, 0);
        SetGpuReg(REG_OFFSET_WIN1V, 0);
        SetGpuReg(REG_OFFSET_WININ, 0);
        SetGpuReg(REG_OFFSET_WINOUT, 0);
        SetGpuReg(REG_OFFSET_MOSAIC, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
        DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
        DmaFill16(3, 0, (void *)PLTT, PLTT_SIZE);
        ResetPaletteFade();
        ResetTasks();
        ResetSpriteData();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 0;
        sWorldTitleNextCallback = NULL;
        gMain.state++;
        break;
    case 1:
        // A direct aligned DMA copy avoids VRAM LZ10 halfword decoding differences.
        // The art is only 38 KiB raw, so ROM overhead is small.
        DmaCopy32(3, sAnthologyTiles, (void *)VRAM, sizeof(sAnthologyTiles));
        DmaCopy16(3, sAnthologyMap, (void *)BG_SCREEN_ADDR(ANTHOLOGY_SCREEN_BLOCK), sizeof(sAnthologyMap));
        LoadPalette(sAnthologyPalette, 0, sizeof(sAnthologyPalette));
        LoadCompressedSpriteSheet(&sAnthologyPressStartSheet);
        LoadSpritePalette(&sAnthologyPressStartPalette);
        if (QUICKSTART && QUICKSTART_HUD)
            CreateQuickstartHud();
        BuildOamBuffer();
        gMain.state++;
        break;
    default:
        SetGpuReg(REG_OFFSET_BG0CNT, BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(ANTHOLOGY_SCREEN_BLOCK) | BGCNT_256COLOR | BGCNT_TXT256x256);
        CreateAnthologyPressStart();
        CreateTask(Task_WorldTitleScreen, 0);
        BeginNormalPaletteFade(PALETTES_ALL, 1, 16, 0, RGB_WHITEALPHA);
        SetVBlankCallback(VBlankCB_WorldTitleScreen);
        EnableInterrupts(INTR_FLAG_VBLANK);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON);
        m4aSongNumStart(MUS_TITLE);
        SetMainCallback2(MainCB2_WorldTitleScreen);
        break;
    }
}

static void MainCB2_WorldTitleScreen(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void LeaveWorldTitleScreen(MainCallback nextCallback, u16 fadeColor)
{
    ScanlineEffect_Stop();
    FadeOutBGM(4);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, fadeColor);
    sWorldTitleNextCallback = nextCallback;
    SetMainCallback2(CB2_LeaveWorldTitleScreen);
}

static void CB2_LeaveWorldTitleScreen(void)
{
    if (!UpdatePaletteFade())
        SetMainCallback2(sWorldTitleNextCallback);
}

static void Task_WorldTitleScreen(u8 taskId)
{
    (void)taskId;
    if (gPaletteFade.active)
        return;

    if (JOY_HELD(CLEAR_SAVE_COMBO) == CLEAR_SAVE_COMBO)
        LeaveWorldTitleScreen(CB2_InitClearSaveDataScreen, RGB_WHITEALPHA);
    else if (JOY_HELD(RESET_RTC_COMBO) == RESET_RTC_COMBO)
    {
        if (CanResetRTC())
            LeaveWorldTitleScreen(CB2_InitResetRtcScreen, RGB_BLACK);
    }
    else if (JOY_NEW(A_BUTTON | START_BUTTON))
        LeaveWorldTitleScreen(CB2_InitMainMenu, RGB_WHITEALPHA);
    else if (QUICKSTART && JOY_NEW(SELECT_BUTTON))
        Quickstart();
    else if ((gMPlayInfo_BGM.status & 0xFFFF) == 0)
        LeaveWorldTitleScreen(CB2_InitCopyrightScreenAfterTitleScreen, RGB_WHITEALPHA);
}

#endif // ALL_REGIONS
