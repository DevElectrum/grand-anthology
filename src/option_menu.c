#include "global.h"
#include "world_menu_theme.h"
#include "option_menu.h"
#include "new_game.h"
#include "oak_speech.h"
#include "main_menu.h"
#include "string_util.h"

#include "bg.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "list_menu.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "random.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "gba/m4a_internal.h"
#include "constants/rgb.h"

#define tMenuSelection data[0]
#define tTextSpeed data[1]
#define tBattleStyle data[3]
#define tButtonMode data[5]
#define tWindowFrameType data[6]
#define tScrollOffset data[7]      // top visible MENUITEM_ index (0 when not scrolled)
#define tScrollArrowTaskId data[8] // TASK_NONE unless scroll arrows are active
#define tAutoRun data[9]
#define tAutosave data[10]
#define tRunShortcut data[11]
#define tShinyOdds data[12]
#define tExpMult data[13]
#define tCatchMult data[14]
#define tNicknames data[15]

#define NUM_VISIBLE_OPTIONS 7

enum
{
    MENUITEM_TEXTSPEED,
    MENUITEM_BATTLESTYLE,
    MENUITEM_BUTTONMODE,
    MENUITEM_FRAMETYPE,
    MENUITEM_AUTORUN,
    MENUITEM_AUTOSAVE,
    MENUITEM_RUNSHORTCUT,
    MENUITEM_NICKNAMES,
    MENUITEM_EXPMULT,
    MENUITEM_CATCHMULT,
    MENUITEM_SHAREDEXP,
    MENUITEM_SHINYODDS,
    MENUITEM_HARDMODE,
    MENUITEM_RAND_PRESET,
    MENUITEM_RAND_WILD,
    MENUITEM_RAND_TRAINERS,
    MENUITEM_RAND_STARTERS,
    MENUITEM_RAND_GIFTS,
    MENUITEM_RAND_STATICS,
    MENUITEM_RAND_STATICMODE,
    MENUITEM_RAND_SEED,
    MENUITEM_CANCEL,
    MENUITEM_COUNT,
};

enum
{
    WIN_HEADER,
    WIN_OPTIONS,
    WIN_FOOTER
};

static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void Task_OptionMenuSave(u8 taskId);
static void Task_OptionMenuFadeOut(u8 taskId);
static void HighlightOptionMenuItem(u8 selection);
static bool8 GetOptionMenuItemY(u8 menuItem, s16 scrollOffset, u8 *y);
static void RedrawVisibleOptionsPage(u8 taskId);
static bool8 AdjustOptionMenuScrollOffset(u8 taskId);
static void CreateOptionMenuScrollArrows(u8 taskId);
static u8 TextSpeed_ProcessInput(u8 selection);
static void TextSpeed_DrawChoices(u8 selection, s16 scrollOffset);
static u8 BattleStyle_ProcessInput(u8 selection);
static void BattleStyle_DrawChoices(u8 selection, s16 scrollOffset);
static u8 FrameType_ProcessInput(u8 selection);
static void FrameType_DrawChoices(u8 selection, s16 scrollOffset);
static u8 ButtonMode_ProcessInput(u8 selection);
static void ButtonMode_DrawChoices(u8 selection, s16 scrollOffset);
static u8 AutoRun_ProcessInput(u8 selection);
static void AutoRun_DrawChoices(u8 selection, s16 scrollOffset);
static u8 Autosave_ProcessInput(u8 selection);
static void Autosave_DrawChoices(u8 selection, s16 scrollOffset);
static u8 RunShortcut_ProcessInput(u8 selection);
static void RunShortcut_DrawChoices(u8 selection, s16 scrollOffset);
static u8 ExpMult_ProcessInput(u8 selection);
static void ExpMult_DrawChoices(u8 selection, s16 scrollOffset);
static u8 CatchMult_ProcessInput(u8 selection);
static void CatchMult_DrawChoices(u8 selection, s16 scrollOffset);
static u8 Nicknames_ProcessInput(u8 selection);
static void Nicknames_DrawChoices(u8 selection, s16 scrollOffset);
static u8 SharedExp_ProcessInput(u8 selection);
static void SharedExp_DrawChoices(u8 selection, s16 scrollOffset);
static u8 ShinyOdds_ProcessInput(u8 selection);
static void ShinyOdds_DrawChoices(u8 selection, s16 scrollOffset);
static void DrawHeaderText(void);
static void DrawOptionMenuTexts(s16 scrollOffset);
static void DrawBgWindowFrames(void);

EWRAM_DATA static bool8 sArrowPressed = FALSE;
#if ALL_REGIONS
static bool8 sNewGameOptionConfirmed = FALSE;
static u8 sRandomizerPreset = 0;
static bool8 sSeedEditing = FALSE;
static u8 sSeedDigit = 0;
static u16 sSeedBeforeEditing = 0;
static void Randomizer_DrawChoices(s16 scrollOffset);
static void Randomizer_ProcessInput(u8 item);
static void Randomizer_ProcessSeedInput(u8 taskId);
#endif


static const u8 gText_Option[]             = _("Options");
static const u8 gText_TextSpeedSlow[]      = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}Slow");
static const u8 gText_TextSpeedMid[]       = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}Mid");
static const u8 gText_TextSpeedFast[]      = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}Fast");
static const u8 gText_BattleSceneOn[]      = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}On");
static const u8 gText_BattleSceneOff[]     = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}Off");
static const u8 gText_BattleStyleShift[]   = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}Shift");
static const u8 gText_BattleStyleSet[]     = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}Set");
static const u8 gText_FrameType[]          = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}Type");
static const u8 gText_FrameTypeNumber[]    = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}");
static const u8 gText_ButtonTypeNormal[]   = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}Normal");
static const u8 gText_ButtonTypeLR[]       = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}LR");
static const u8 gText_ButtonTypeLEqualsA[] = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}L=A");
static const u8 gText_RunShortcutCursor[]  = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}Cursor");
static const u8 gText_RunShortcutInstant[] = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}Instant");
static const u8 gText_Mult0_5x[]           = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}0.5x");
static const u8 gText_Mult1x[]             = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1x");
static const u8 gText_Mult1_5x[]           = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1.5x");
static const u8 gText_Mult2x[]             = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}2x");
static const u8 gText_Shiny8192[]          = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1/8192");
static const u8 gText_Shiny4096[]          = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1/4096");
static const u8 gText_Shiny1024[]          = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1/1024");
static const u8 gText_Shiny512[]           = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1/512");
static const u8 gText_Shiny128[]           = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1/128");
static const u8 gText_Shiny64[]            = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1/64");
static const u8 gText_Shiny16[]            = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1/16");
static const u8 gText_Shiny8[]             = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1/8");
static const u8 gText_Shiny4[]             = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1/4");
static const u8 gText_Shiny2[]             = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}1/2");
static const u8 gText_ShinyAlways[]        = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}Always");

// note: this is only used in the Japanese release
static const u8 sEqualSignGfx[] = INCGFX_U8("graphics/interface/option_menu_equals_sign.png", ".4bpp");

static const u8 *const sOptionMenuItemsNames[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]   = COMPOUND_STRING("Text Speed"),
    [MENUITEM_BATTLESTYLE] = COMPOUND_STRING("Battle Style"),
    [MENUITEM_BUTTONMODE]  = COMPOUND_STRING("Button Mode"),
    [MENUITEM_FRAMETYPE]   = COMPOUND_STRING("Frame"),
    [MENUITEM_AUTORUN]     = COMPOUND_STRING("Auto Run"),
    [MENUITEM_AUTOSAVE]    = COMPOUND_STRING("Autosave"),
    [MENUITEM_RUNSHORTCUT] = COMPOUND_STRING("Run Shortcut"),
    [MENUITEM_NICKNAMES]   = COMPOUND_STRING("Nicknames"),
    [MENUITEM_EXPMULT]     = COMPOUND_STRING("Exp Rate"),
    [MENUITEM_CATCHMULT]   = COMPOUND_STRING("Catch Rate"),
    [MENUITEM_SHAREDEXP]   = COMPOUND_STRING("Shared Exp"),
    [MENUITEM_SHINYODDS]   = COMPOUND_STRING("Shiny Odds"),
    [MENUITEM_HARDMODE]    = COMPOUND_STRING("Hard Mode"),
    [MENUITEM_RAND_PRESET] = COMPOUND_STRING("Randomizer"),
    [MENUITEM_RAND_WILD]   = COMPOUND_STRING("  Wild"),
    [MENUITEM_RAND_TRAINERS] = COMPOUND_STRING("  Trainers"),
    [MENUITEM_RAND_STARTERS] = COMPOUND_STRING("  Starters"),
    [MENUITEM_RAND_GIFTS] = COMPOUND_STRING("  Gifts"),
    [MENUITEM_RAND_STATICS] = COMPOUND_STRING("  Statics"),
    [MENUITEM_RAND_STATICMODE] = COMPOUND_STRING("  Static Mode"),
    [MENUITEM_RAND_SEED] = COMPOUND_STRING("  Seed"),
    [MENUITEM_CANCEL]      = COMPOUND_STRING("Cancel"),
};

static const struct WindowTemplate sOptionMenuWinTemplates[] =
{
    [WIN_HEADER] = {.bg = 1, .tilemapLeft = 0, .tilemapTop = 0, .width = 30, .height = 2, .paletteNum = 12, .baseBlock = 2},
    [WIN_OPTIONS] = {.bg = 0, .tilemapLeft = 2, .tilemapTop = 3, .width = 26, .height = 14, .paletteNum = 1, .baseBlock = 0x40},
    [WIN_FOOTER] = {.bg = 1, .tilemapLeft = 1, .tilemapTop = 18, .width = 28, .height = 2, .paletteNum = 1, .baseBlock = 0x1B0},
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    }
};

static const u16 sOptionMenuBg_Pal[] = {RGB(17, 18, 31)};

static void MainCB2(void)
{
    WorldMenu_UpdateBackground();
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_InitOptionMenu(void)
{
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        ChangeBgX(2, 0, BG_COORD_SET);
        ChangeBgY(2, 0, BG_COORD_SET);
        ChangeBgX(3, 0, BG_COORD_SET);
        ChangeBgY(3, 0, BG_COORD_SET);
        InitWindows(sOptionMenuWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG3);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG3 | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_DARKEN);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1F0);
        gMain.state++;
        break;
    case 4:
        LoadPalette(sOptionMenuBg_Pal, BG_PLTT_ID(0), sizeof(sOptionMenuBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        gMain.state++;
        break;
    case 5:
        WorldMenu_LoadTextPalette(1);
        WorldMenu_InitBackground(0);
        gMain.state++;
        break;
    case 6:
        PutWindowTilemap(WIN_HEADER);
        DrawHeaderText();
        gMain.state++;
        break;
    case 7:
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(WIN_OPTIONS);
        gMain.state++;
    case 9:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 10:
    {
        u8 taskId = CreateTask(Task_OptionMenuFadeIn, 0);

        gTasks[taskId].tMenuSelection = 0;
        gTasks[taskId].tTextSpeed = gSaveBlock2Ptr->optionsTextSpeed;
        gTasks[taskId].tBattleStyle = gSaveBlock2Ptr->optionsBattleStyle;
        gTasks[taskId].tButtonMode = gSaveBlock2Ptr->optionsButtonMode;
        gTasks[taskId].tWindowFrameType = gSaveBlock2Ptr->optionsWindowFrameType;
        gTasks[taskId].tAutoRun = gSaveBlock2Ptr->optionsAutoRun;
        gTasks[taskId].tAutosave = gSaveBlock2Ptr->optionsAutosave;
        gTasks[taskId].tRunShortcut = gSaveBlock2Ptr->optionsRunShortcut;
        gTasks[taskId].tExpMult = gSaveBlock2Ptr->optionsExpMultiplier;
        gTasks[taskId].tCatchMult = gSaveBlock2Ptr->optionsCatchMultiplier;
        gTasks[taskId].tShinyOdds = gSaveBlock2Ptr->optionsShinyOdds;
        gTasks[taskId].tNicknames = gSaveBlock2Ptr->optionsNicknames;
        gTasks[taskId].tScrollOffset = 0;
#if ALL_REGIONS
        sNewGameOptionConfirmed = FALSE;
        sSeedEditing = FALSE;
        sSeedDigit = 0;
        sRandomizerPreset = 2;
        if (!gSaveBlock2Ptr->randomizerWild && !gSaveBlock2Ptr->randomizerTrainers
         && !gSaveBlock2Ptr->randomizerStarters && !gSaveBlock2Ptr->randomizerGifts
         && !gSaveBlock2Ptr->randomizerStatics)
            sRandomizerPreset = 0;
        else if (gSaveBlock2Ptr->randomizerWild && gSaveBlock2Ptr->randomizerTrainers
              && gSaveBlock2Ptr->randomizerStarters && gSaveBlock2Ptr->randomizerGifts
              && gSaveBlock2Ptr->randomizerStatics && gSaveBlock2Ptr->randomizerStaticMode == 2)
            sRandomizerPreset = 1;
#endif

        RedrawVisibleOptionsPage(taskId);
        CreateOptionMenuScrollArrows(taskId);

        gMain.state++;
        break;
    }
    case 11:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static void Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OptionMenuProcessInput;
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
#if ALL_REGIONS
    if (IsNewGameSetupActive() && gTasks[taskId].tMenuSelection == MENUITEM_RAND_SEED)
    {
        if (sSeedEditing || JOY_NEW(A_BUTTON | SELECT_BUTTON))
        {
            Randomizer_ProcessSeedInput(taskId);
            return;
        }
    }

    // START is the explicit pre-adventure Confirm & Lock shortcut. It is
    // deliberately unavailable after the save has been initialized.
    if (IsNewGameSetupActive() && JOY_NEW(START_BUTTON))
    {
        sNewGameOptionConfirmed = TRUE;
        gTasks[taskId].func = Task_OptionMenuSave;
        return;
    }
#endif
    if (JOY_NEW(A_BUTTON))
    {
        if (gTasks[taskId].tMenuSelection == MENUITEM_CANCEL)
        {
#if ALL_REGIONS
            if (IsNewGameSetupActive())
                sNewGameOptionConfirmed = TRUE;
#endif
            gTasks[taskId].func = Task_OptionMenuSave;
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (gTasks[taskId].tMenuSelection > 0)
            gTasks[taskId].tMenuSelection--;
        else
            gTasks[taskId].tMenuSelection = MENUITEM_CANCEL;

        if (AdjustOptionMenuScrollOffset(taskId))
            RedrawVisibleOptionsPage(taskId);
        else
            HighlightOptionMenuItem(gTasks[taskId].tMenuSelection - gTasks[taskId].tScrollOffset);
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (gTasks[taskId].tMenuSelection < MENUITEM_CANCEL)
            gTasks[taskId].tMenuSelection++;
        else
            gTasks[taskId].tMenuSelection = 0;

        if (AdjustOptionMenuScrollOffset(taskId))
            RedrawVisibleOptionsPage(taskId);
        else
            HighlightOptionMenuItem(gTasks[taskId].tMenuSelection - gTasks[taskId].tScrollOffset);
    }
    else
    {
        u8 previousOption;

        switch (gTasks[taskId].tMenuSelection)
        {
#if ALL_REGIONS
        case MENUITEM_HARDMODE:
        case MENUITEM_RAND_PRESET:
        case MENUITEM_RAND_WILD:
        case MENUITEM_RAND_TRAINERS:
        case MENUITEM_RAND_STARTERS:
        case MENUITEM_RAND_GIFTS:
        case MENUITEM_RAND_STATICS:
        case MENUITEM_RAND_STATICMODE:
        case MENUITEM_RAND_SEED:
            Randomizer_ProcessInput(gTasks[taskId].tMenuSelection);
            if (sArrowPressed)
                Randomizer_DrawChoices(gTasks[taskId].tScrollOffset);
            break;
#endif
        case MENUITEM_TEXTSPEED:
            previousOption = gTasks[taskId].tTextSpeed;
            gTasks[taskId].tTextSpeed = TextSpeed_ProcessInput(gTasks[taskId].tTextSpeed);

            if (previousOption != gTasks[taskId].tTextSpeed)
                TextSpeed_DrawChoices(gTasks[taskId].tTextSpeed, gTasks[taskId].tScrollOffset);
            break;
        case MENUITEM_BATTLESTYLE:
            previousOption = gTasks[taskId].tBattleStyle;
            gTasks[taskId].tBattleStyle = BattleStyle_ProcessInput(gTasks[taskId].tBattleStyle);

            if (previousOption != gTasks[taskId].tBattleStyle)
                BattleStyle_DrawChoices(gTasks[taskId].tBattleStyle, gTasks[taskId].tScrollOffset);
            break;
        case MENUITEM_BUTTONMODE:
            previousOption = gTasks[taskId].tButtonMode;
            gTasks[taskId].tButtonMode = ButtonMode_ProcessInput(gTasks[taskId].tButtonMode);

            if (previousOption != gTasks[taskId].tButtonMode)
                ButtonMode_DrawChoices(gTasks[taskId].tButtonMode, gTasks[taskId].tScrollOffset);
            break;
        case MENUITEM_FRAMETYPE:
            previousOption = gTasks[taskId].tWindowFrameType;
            gTasks[taskId].tWindowFrameType = FrameType_ProcessInput(gTasks[taskId].tWindowFrameType);

            if (previousOption != gTasks[taskId].tWindowFrameType)
                FrameType_DrawChoices(gTasks[taskId].tWindowFrameType, gTasks[taskId].tScrollOffset);
            break;
        case MENUITEM_AUTORUN:
            previousOption = gTasks[taskId].tAutoRun;
            gTasks[taskId].tAutoRun = AutoRun_ProcessInput(gTasks[taskId].tAutoRun);

            if (previousOption != gTasks[taskId].tAutoRun)
                AutoRun_DrawChoices(gTasks[taskId].tAutoRun, gTasks[taskId].tScrollOffset);
            break;
        case MENUITEM_AUTOSAVE:
            previousOption = gTasks[taskId].tAutosave;
            gTasks[taskId].tAutosave = Autosave_ProcessInput(gTasks[taskId].tAutosave);

            if (previousOption != gTasks[taskId].tAutosave)
                Autosave_DrawChoices(gTasks[taskId].tAutosave, gTasks[taskId].tScrollOffset);
            break;
        case MENUITEM_RUNSHORTCUT:
            previousOption = gTasks[taskId].tRunShortcut;
            gTasks[taskId].tRunShortcut = RunShortcut_ProcessInput(gTasks[taskId].tRunShortcut);

            if (previousOption != gTasks[taskId].tRunShortcut)
                RunShortcut_DrawChoices(gTasks[taskId].tRunShortcut, gTasks[taskId].tScrollOffset);
            break;
        case MENUITEM_EXPMULT:
            previousOption = gTasks[taskId].tExpMult;
            gTasks[taskId].tExpMult = ExpMult_ProcessInput(gTasks[taskId].tExpMult);

            if (previousOption != gTasks[taskId].tExpMult)
                ExpMult_DrawChoices(gTasks[taskId].tExpMult, gTasks[taskId].tScrollOffset);
            break;
        case MENUITEM_CATCHMULT:
            previousOption = gTasks[taskId].tCatchMult;
            gTasks[taskId].tCatchMult = CatchMult_ProcessInput(gTasks[taskId].tCatchMult);

            if (previousOption != gTasks[taskId].tCatchMult)
                CatchMult_DrawChoices(gTasks[taskId].tCatchMult, gTasks[taskId].tScrollOffset);
            break;
        case MENUITEM_NICKNAMES:
            previousOption = gTasks[taskId].tNicknames;
            gTasks[taskId].tNicknames = Nicknames_ProcessInput(gTasks[taskId].tNicknames);

            if (previousOption != gTasks[taskId].tNicknames)
                Nicknames_DrawChoices(gTasks[taskId].tNicknames, gTasks[taskId].tScrollOffset);
            break;
        case MENUITEM_SHINYODDS:
            previousOption = gTasks[taskId].tShinyOdds;
            gTasks[taskId].tShinyOdds = ShinyOdds_ProcessInput(gTasks[taskId].tShinyOdds);
            if (previousOption != gTasks[taskId].tShinyOdds)
                ShinyOdds_DrawChoices(gTasks[taskId].tShinyOdds, gTasks[taskId].tScrollOffset);
            break;
        case MENUITEM_SHAREDEXP:
        {
            // Backed directly by FLAG_QOL_SHARED_EXP (no data[] slot: the array is full at tNicknames).
            u8 prev = FlagGet(FLAG_QOL_SHARED_EXP) ? 1 : 0;
            u8 cur = SharedExp_ProcessInput(prev);

            if (cur != prev)
            {
                if (cur)
                    FlagSet(FLAG_QOL_SHARED_EXP);
                else
                    FlagClear(FLAG_QOL_SHARED_EXP);
                SharedExp_DrawChoices(cur, gTasks[taskId].tScrollOffset);
            }
            break;
        }
        default:
            return;
        }

        if (sArrowPressed)
        {
            sArrowPressed = FALSE;
            CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
        }
    }
}

static void Task_OptionMenuSave(u8 taskId)
{
    // Sound and Battle Scene are baked to stereo/animations; Hard Mode
    // updates directly via the shared Options selector rather than task data.
    gSaveBlock2Ptr->optionsTextSpeed = gTasks[taskId].tTextSpeed;
    gSaveBlock2Ptr->optionsBattleStyle = gTasks[taskId].tBattleStyle;
    gSaveBlock2Ptr->optionsButtonMode = gTasks[taskId].tButtonMode;
    gSaveBlock2Ptr->optionsWindowFrameType = gTasks[taskId].tWindowFrameType;
    gSaveBlock2Ptr->optionsAutoRun = gTasks[taskId].tAutoRun;
    gSaveBlock2Ptr->optionsAutosave = gTasks[taskId].tAutosave;
    gSaveBlock2Ptr->optionsRunShortcut = gTasks[taskId].tRunShortcut;
    gSaveBlock2Ptr->optionsExpMultiplier = gTasks[taskId].tExpMult;
    gSaveBlock2Ptr->optionsCatchMultiplier = gTasks[taskId].tCatchMult;
    gSaveBlock2Ptr->optionsShinyOdds = gTasks[taskId].tShinyOdds;
    gSaveBlock2Ptr->optionsNicknames = gTasks[taskId].tNicknames;

    if (gTasks[taskId].tScrollArrowTaskId != TASK_NONE)
    {
        RemoveScrollIndicatorArrowPair(gTasks[taskId].tScrollArrowTaskId);
        gTasks[taskId].tScrollArrowTaskId = TASK_NONE;
    }

    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_OptionMenuFadeOut;
}

static void Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
#if ALL_REGIONS
        if (IsNewGameSetupActive())
        {
            if (sNewGameOptionConfirmed)
            {
                ConfirmNewGameSetup();
                StartNewGameSceneFrlg();
            }
            else
            {
                CancelNewGameSetup();
                SetMainCallback2(CB2_InitMainMenu);
            }
            return;
        }
#endif
        SetMainCallback2(gMain.savedCallback);
    }
}

// Returns FALSE (and draws nothing) if menuItem is scrolled off the visible page.
static bool8 GetOptionMenuItemY(u8 menuItem, s16 scrollOffset, u8 *y)
{
    s16 row = menuItem - scrollOffset;

    if (row < 0 || row >= NUM_VISIBLE_OPTIONS)
        return FALSE;

    *y = row * 16;
    return TRUE;
}

static void HighlightOptionMenuItem(u8 index)
{
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 0, 0, 2, 112);
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(4), 0, index * 16, 2, 16);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(16, DISPLAY_WIDTH - 16));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(index * 16 + 24, index * 16 + 40));
}

#define TAG_OPTIONS_SCROLL_ARROW_GFX 2000
#define TAG_OPTIONS_SCROLL_ARROW_PAL 100

// Creates the scroll-arrow pair when MENUITEM_COUNT exceeds NUM_VISIBLE_OPTIONS. AUTO RUN (#1)
// pushed the list to 8 items over the 7-row window, so this is LIVE: arrows show and CANCEL sits
// below the fold until you scroll. tScrollArrowTaskId stays TASK_NONE only if the list ever fits.
static void CreateOptionMenuScrollArrows(u8 taskId)
{
    gTasks[taskId].tScrollArrowTaskId = TASK_NONE;

    if (MENUITEM_COUNT > NUM_VISIBLE_OPTIONS)
    {
        gTempScrollArrowTemplate.firstArrowType = SCROLL_ARROW_UP;
        gTempScrollArrowTemplate.firstX = 216;
        gTempScrollArrowTemplate.firstY = 24;
        gTempScrollArrowTemplate.secondArrowType = SCROLL_ARROW_DOWN;
        gTempScrollArrowTemplate.secondX = 216;
        gTempScrollArrowTemplate.secondY = 136;
        gTempScrollArrowTemplate.fullyUpThreshold = 0;
        gTempScrollArrowTemplate.fullyDownThreshold = MENUITEM_COUNT - NUM_VISIBLE_OPTIONS;
        gTempScrollArrowTemplate.tileTag = TAG_OPTIONS_SCROLL_ARROW_GFX;
        gTempScrollArrowTemplate.palTag = TAG_OPTIONS_SCROLL_ARROW_PAL;
        gTempScrollArrowTemplate.palNum = 0;

        gTasks[taskId].tScrollArrowTaskId = AddScrollIndicatorArrowPair(&gTempScrollArrowTemplate, (u16 *)&gTasks[taskId].tScrollOffset);
    }
}

// Clamps tScrollOffset so tMenuSelection stays on the visible page. Returns TRUE if
// the offset changed (caller must do a full repaint) or FALSE if only the highlight moved.
static bool8 AdjustOptionMenuScrollOffset(u8 taskId)
{
    s16 sel = gTasks[taskId].tMenuSelection;
    s16 scrollOffset = gTasks[taskId].tScrollOffset;

    if (sel < scrollOffset)
        scrollOffset = sel;
    else if (sel > scrollOffset + NUM_VISIBLE_OPTIONS - 1)
        scrollOffset = sel - NUM_VISIBLE_OPTIONS + 1;

    if (scrollOffset == gTasks[taskId].tScrollOffset)
        return FALSE;

    gTasks[taskId].tScrollOffset = scrollOffset;
    return TRUE;
}

static void RedrawVisibleOptionsPage(u8 taskId)
{
    s16 scrollOffset = gTasks[taskId].tScrollOffset;

    DrawOptionMenuTexts(scrollOffset);
    TextSpeed_DrawChoices(gTasks[taskId].tTextSpeed, scrollOffset);
    BattleStyle_DrawChoices(gTasks[taskId].tBattleStyle, scrollOffset);
    ButtonMode_DrawChoices(gTasks[taskId].tButtonMode, scrollOffset);
    FrameType_DrawChoices(gTasks[taskId].tWindowFrameType, scrollOffset);
    AutoRun_DrawChoices(gTasks[taskId].tAutoRun, scrollOffset);
    Autosave_DrawChoices(gTasks[taskId].tAutosave, scrollOffset);
    RunShortcut_DrawChoices(gTasks[taskId].tRunShortcut, scrollOffset);
    ExpMult_DrawChoices(gTasks[taskId].tExpMult, scrollOffset);
    CatchMult_DrawChoices(gTasks[taskId].tCatchMult, scrollOffset);
    ShinyOdds_DrawChoices(gTasks[taskId].tShinyOdds, scrollOffset);
    Nicknames_DrawChoices(gTasks[taskId].tNicknames, scrollOffset);
    SharedExp_DrawChoices(FlagGet(FLAG_QOL_SHARED_EXP) ? 1 : 0, scrollOffset);
#if ALL_REGIONS
    Randomizer_DrawChoices(scrollOffset);
#endif
    HighlightOptionMenuItem(gTasks[taskId].tMenuSelection - scrollOffset);

    // Scroll repaint is a full window copy, unlike the single-row COPYWIN_GFX
    // update used for in-place L/R edits, since every visible row can change.
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style)
{
    u8 dst[16];
    u16 i;

    for (i = 0; *text != EOS && i < ARRAY_COUNT(dst) - 1; i++)
        dst[i] = *(text++);

    if (style != 0)
    {
        dst[2] = TEXT_COLOR_RED;
        dst[5] = TEXT_COLOR_LIGHT_RED;
    }

    dst[i] = EOS;
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_SMALL, dst, x, y + 1, TEXT_SKIP_DRAW, NULL);
}

static u8 TextSpeed_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void TextSpeed_DrawChoices(u8 selection, s16 scrollOffset)
{
    u8 styles[3];
    s32 widthSlow, widthMid, widthFast, xMid;
    u8 y;

    if (!GetOptionMenuItemY(MENUITEM_TEXTSPEED, scrollOffset, &y))
        return;

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_TextSpeedSlow, 104, y, styles[0]);

    widthSlow = GetStringWidth(FONT_SMALL, gText_TextSpeedSlow, 0);
    widthMid = GetStringWidth(FONT_SMALL, gText_TextSpeedMid, 0);
    widthFast = GetStringWidth(FONT_SMALL, gText_TextSpeedFast, 0);

    widthMid -= 94;
    xMid = (widthSlow - widthMid - widthFast) / 2 + 104;
    DrawOptionMenuChoice(gText_TextSpeedMid, xMid, y, styles[1]);

    DrawOptionMenuChoice(gText_TextSpeedFast, GetStringRightAlignXOffset(FONT_SMALL, gText_TextSpeedFast, 198), y, styles[2]);
}

static u8 BattleStyle_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void BattleStyle_DrawChoices(u8 selection, s16 scrollOffset)
{
    u8 styles[2];
    u8 y;

    if (!GetOptionMenuItemY(MENUITEM_BATTLESTYLE, scrollOffset, &y))
        return;

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleStyleShift, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_BattleStyleSet, GetStringRightAlignXOffset(FONT_SMALL, gText_BattleStyleSet, 198), y, styles[1]);
}

static u8 FrameType_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection < WINDOW_FRAMES_COUNT - 1)
            selection++;
        else
            selection = 0;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1F0);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = WINDOW_FRAMES_COUNT - 1;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1F0);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        sArrowPressed = TRUE;
    }
    return selection;
}

static void FrameType_DrawChoices(u8 selection, s16 scrollOffset)
{
    u8 text[16] = {EOS};
    u8 n = selection + 1;
    u16 i;
    u8 y;

    if (!GetOptionMenuItemY(MENUITEM_FRAMETYPE, scrollOffset, &y))
        return;

    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];

    // Convert a number to decimal string
    if (n / 10 != 0)
    {
        text[i] = n / 10 + CHAR_0;
        i++;
        text[i] = n % 10 + CHAR_0;
        i++;
    }
    else
    {
        text[i] = n % 10 + CHAR_0;
        i++;
        text[i] = CHAR_SPACER;
        i++;
    }

    text[i] = EOS;

    DrawOptionMenuChoice(gText_FrameType, 104, y, 0);
    DrawOptionMenuChoice(text, 128, y, 1);
}

static u8 ButtonMode_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void ButtonMode_DrawChoices(u8 selection, s16 scrollOffset)
{
    s32 widthNormal, widthLR, widthLA, xLR;
    u8 styles[3];
    u8 y;

    if (!GetOptionMenuItemY(MENUITEM_BUTTONMODE, scrollOffset, &y))
        return;

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_ButtonTypeNormal, 104, y, styles[0]);

    widthNormal = GetStringWidth(FONT_SMALL, gText_ButtonTypeNormal, 0);
    widthLR = GetStringWidth(FONT_SMALL, gText_ButtonTypeLR, 0);
    widthLA = GetStringWidth(FONT_SMALL, gText_ButtonTypeLEqualsA, 0);

    widthLR -= 94;
    xLR = (widthNormal - widthLR - widthLA) / 2 + 104;
    DrawOptionMenuChoice(gText_ButtonTypeLR, xLR, y, styles[1]);

    DrawOptionMenuChoice(gText_ButtonTypeLEqualsA, GetStringRightAlignXOffset(FONT_SMALL, gText_ButtonTypeLEqualsA, 198), y, styles[2]);
}

static u8 AutoRun_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void AutoRun_DrawChoices(u8 selection, s16 scrollOffset)
{
    u8 styles[2];
    u8 y;

    if (!GetOptionMenuItemY(MENUITEM_AUTORUN, scrollOffset, &y))
        return;

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    // optionsAutoRun is normal polarity (0 = OFF, 1 = ON), unlike BattleScene's inverted var,
    // so OFF must be the index-0 (value-0) choice or the menu would highlight the wrong state.
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_SMALL, gText_BattleSceneOn, 198), y, styles[1]);
}

static u8 Autosave_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void Autosave_DrawChoices(u8 selection, s16 scrollOffset)
{
    u8 styles[2];
    u8 y;

    if (!GetOptionMenuItemY(MENUITEM_AUTOSAVE, scrollOffset, &y))
        return;

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    // optionsAutosave is normal polarity (0 = OFF, 1 = ON) like AutoRun above, NOT like
    // BattleScene's inverted var: OFF must stay the index-0 (value-0) choice.
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_SMALL, gText_BattleSceneOn, 198), y, styles[1]);
}

// optionsRunShortcut stores CURSOR as 0 (old-save compat) but the menu cycles
// OFF->CURSOR->INSTANT, so map value<->position; {1, 0, 2} is its own inverse.
static const u8 sRunShortcutValuePos[] = {1, 0, 2};

static u8 RunShortcut_ProcessInput(u8 selection)
{
    u8 pos = sRunShortcutValuePos[selection];

    if (JOY_NEW(DPAD_RIGHT))
    {
        pos = pos <= 1 ? pos + 1 : 0;
        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        pos = pos != 0 ? pos - 1 : 2;
        sArrowPressed = TRUE;
    }
    return sRunShortcutValuePos[pos];
}

static void RunShortcut_DrawChoices(u8 selection, s16 scrollOffset)
{
    // OFF/CURSOR/INSTANT don't fit as columns, so show only the current choice
    // (FrameType-style). Indexed by STORED value: 0 = CURSOR, 1 = OFF, 2 = INSTANT.
    static const u8 *const sTexts[] = {gText_RunShortcutCursor, gText_BattleSceneOff, gText_RunShortcutInstant};
    u8 y;

    if (!GetOptionMenuItemY(MENUITEM_RUNSHORTCUT, scrollOffset, &y))
        return;

    // The three labels differ in width, so wipe the choice area before printing.
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 104, y, 104, 16);
    DrawOptionMenuChoice(sTexts[selection], 104, y, 1);
}

// optionsExpMultiplier stores 1x as 0 (old-save compat) but the menu columns run
// 0.5x/1x/1.5x/2x, so map value<->column; {1, 0, 2, 3} is its own inverse.
static const u8 sExpMultValueCol[] = {1, 0, 2, 3};

static u8 ExpMult_ProcessInput(u8 selection)
{
    u8 col = sExpMultValueCol[selection];

    if (JOY_NEW(DPAD_RIGHT))
    {
        col = col <= 2 ? col + 1 : 0;
        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        col = col != 0 ? col - 1 : 3;
        sArrowPressed = TRUE;
    }
    return sExpMultValueCol[col];
}

static void ExpMult_DrawChoices(u8 selection, s16 scrollOffset)
{
    u8 styles[4];
    u8 y;

    if (!GetOptionMenuItemY(MENUITEM_EXPMULT, scrollOffset, &y))
        return;

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[3] = 0;
    styles[sExpMultValueCol[selection]] = 1; // stored value -> menu column

    DrawOptionMenuChoice(gText_Mult0_5x, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_Mult1x, 134, y, styles[1]);
    DrawOptionMenuChoice(gText_Mult1_5x, 155, y, styles[2]);
    DrawOptionMenuChoice(gText_Mult2x, GetStringRightAlignXOffset(FONT_SMALL, gText_Mult2x, 198), y, styles[3]);
}

// Menu columns run 0.5x/1x/1.5x/2x, but optionsCatchMultiplier keeps its old-save values
// (0=1x, 1=1.5x, 2=2x) and adds 0.5x as 3, so two lookups convert between value and column.
static const u8 sCatchMultValueToCol[] = {1, 2, 3, 0}; // stored 1x/1.5x/2x/0.5x -> column
static const u8 sCatchMultColToValue[] = {3, 0, 1, 2}; // column 0.5x/1x/1.5x/2x -> stored

static u8 CatchMult_ProcessInput(u8 selection)
{
    u8 col = sCatchMultValueToCol[selection];

    if (JOY_NEW(DPAD_RIGHT))
    {
        col = col <= 2 ? col + 1 : 0;
        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        col = col != 0 ? col - 1 : 3;
        sArrowPressed = TRUE;
    }
    return sCatchMultColToValue[col];
}

static void CatchMult_DrawChoices(u8 selection, s16 scrollOffset)
{
    u8 styles[4];
    u8 y;

    if (!GetOptionMenuItemY(MENUITEM_CATCHMULT, scrollOffset, &y))
        return;

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[3] = 0;
    styles[sCatchMultValueToCol[selection]] = 1; // stored value -> menu column

    DrawOptionMenuChoice(gText_Mult0_5x, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_Mult1x, 134, y, styles[1]);
    DrawOptionMenuChoice(gText_Mult1_5x, 155, y, styles[2]);
    DrawOptionMenuChoice(gText_Mult2x, GetStringRightAlignXOffset(FONT_SMALL, gText_Mult2x, 198), y, styles[3]);
}

// Show one option at a time, as the full odds set cannot fit on one row.
static const u8 *const sShinyOddsTexts[OPTIONS_SHINY_ODDS_COUNT] =
{
    [OPTIONS_SHINY_ODDS_1_128]  = gText_Shiny128,
    [OPTIONS_SHINY_ODDS_1_8192] = gText_Shiny8192,
    [OPTIONS_SHINY_ODDS_1_4096] = gText_Shiny4096,
    [OPTIONS_SHINY_ODDS_1_1024] = gText_Shiny1024,
    [OPTIONS_SHINY_ODDS_1_512]  = gText_Shiny512,
    [OPTIONS_SHINY_ODDS_1_64]   = gText_Shiny64,
    [OPTIONS_SHINY_ODDS_1_16]   = gText_Shiny16,
    [OPTIONS_SHINY_ODDS_1_8]    = gText_Shiny8,
    [OPTIONS_SHINY_ODDS_1_4]    = gText_Shiny4,
    [OPTIONS_SHINY_ODDS_1_2]    = gText_Shiny2,
    [OPTIONS_SHINY_ODDS_ALWAYS] = gText_ShinyAlways,
};

// Preserve the save-format IDs (especially old Always=7), but display the
// additional probabilities in increasing likelihood before Always.
static const u8 sShinyOddsMenuOrder[OPTIONS_SHINY_ODDS_COUNT] =
{
    OPTIONS_SHINY_ODDS_1_128,
    OPTIONS_SHINY_ODDS_1_8192,
    OPTIONS_SHINY_ODDS_1_4096,
    OPTIONS_SHINY_ODDS_1_1024,
    OPTIONS_SHINY_ODDS_1_512,
    OPTIONS_SHINY_ODDS_1_64,
    OPTIONS_SHINY_ODDS_1_16,
    OPTIONS_SHINY_ODDS_1_8,
    OPTIONS_SHINY_ODDS_1_4,
    OPTIONS_SHINY_ODDS_1_2,
    OPTIONS_SHINY_ODDS_ALWAYS,
};

static u8 ShinyOdds_ProcessInput(u8 selection)
{
    u8 index;

    if (selection >= OPTIONS_SHINY_ODDS_COUNT)
        selection = OPTIONS_SHINY_ODDS_1_128;

    for (index = 0; index < OPTIONS_SHINY_ODDS_COUNT; index++)
    {
        if (sShinyOddsMenuOrder[index] == selection)
            break;
    }
    if (JOY_NEW(DPAD_RIGHT))
    {
        index = (index + 1) % OPTIONS_SHINY_ODDS_COUNT;
        sArrowPressed = TRUE;
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        index = (index + OPTIONS_SHINY_ODDS_COUNT - 1) % OPTIONS_SHINY_ODDS_COUNT;
        sArrowPressed = TRUE;
    }
    return sShinyOddsMenuOrder[index];
}

static void ShinyOdds_DrawChoices(u8 selection, s16 scrollOffset)
{
    u8 y;
    if (!GetOptionMenuItemY(MENUITEM_SHINYODDS, scrollOffset, &y))
        return;
    if (selection >= OPTIONS_SHINY_ODDS_COUNT)
        selection = OPTIONS_SHINY_ODDS_1_128;
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 104, y, 104, 16);
    DrawOptionMenuChoice(sShinyOddsTexts[selection], 104, y, 1);
}

static u8 Nicknames_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void Nicknames_DrawChoices(u8 selection, s16 scrollOffset)
{
    u8 styles[2];
    u8 y;

    if (!GetOptionMenuItemY(MENUITEM_NICKNAMES, scrollOffset, &y))
        return;

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    // optionsNicknames: normal polarity, ON at the left/default column (value 0 =
    // OPTIONS_NICKNAMES_ON = catch/hatch prompts shown, vanilla), OFF at the right
    // (value 1 = OPTIONS_NICKNAMES_OFF = prompts skipped, default species name kept).
    DrawOptionMenuChoice(gText_BattleSceneOn, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(FONT_SMALL, gText_BattleSceneOff, 198), y, styles[1]);
}

static u8 SharedExp_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void SharedExp_DrawChoices(u8 selection, s16 scrollOffset)
{
    u8 styles[2];
    u8 y;

    if (!GetOptionMenuItemY(MENUITEM_SHAREDEXP, scrollOffset, &y))
        return;

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    // FLAG_QOL_SHARED_EXP is normal polarity (0 = OFF, 1 = ON) like HardMode above,
    // so OFF must stay the index-0 (value-0) choice.
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_SMALL, gText_BattleSceneOn, 198), y, styles[1]);
}


#if ALL_REGIONS
static void Randomizer_PrintValue(u8 item, s16 scrollOffset, const u8 *value)
{
    u8 y;
    if (!GetOptionMenuItemY(item, scrollOffset, &y))
        return;
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 132, y, 76, 16);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_SMALL, value, 132, y + 1, TEXT_SKIP_DRAW, NULL);
}

static void Randomizer_DrawChoices(s16 scrollOffset)
{
    u8 seedText[8];
    Randomizer_PrintValue(MENUITEM_HARDMODE, scrollOffset,
        gSaveBlock2Ptr->optionsHardMode ? COMPOUND_STRING("On") : COMPOUND_STRING("Off"));
    Randomizer_PrintValue(MENUITEM_RAND_PRESET, scrollOffset,
        sRandomizerPreset == 0 ? COMPOUND_STRING("Off") :
        sRandomizerPreset == 1 ? COMPOUND_STRING("Full") : COMPOUND_STRING("Custom"));
    Randomizer_PrintValue(MENUITEM_RAND_WILD, scrollOffset,
        gSaveBlock2Ptr->randomizerWild ? COMPOUND_STRING("On") : COMPOUND_STRING("Off"));
    Randomizer_PrintValue(MENUITEM_RAND_TRAINERS, scrollOffset,
        gSaveBlock2Ptr->randomizerTrainers ? COMPOUND_STRING("On") : COMPOUND_STRING("Off"));
    Randomizer_PrintValue(MENUITEM_RAND_STARTERS, scrollOffset,
        gSaveBlock2Ptr->randomizerStarters ? COMPOUND_STRING("On") : COMPOUND_STRING("Off"));
    Randomizer_PrintValue(MENUITEM_RAND_GIFTS, scrollOffset,
        gSaveBlock2Ptr->randomizerGifts ? COMPOUND_STRING("On") : COMPOUND_STRING("Off"));
    Randomizer_PrintValue(MENUITEM_RAND_STATICS, scrollOffset,
        gSaveBlock2Ptr->randomizerStatics ? COMPOUND_STRING("On") : COMPOUND_STRING("Off"));
    Randomizer_PrintValue(MENUITEM_RAND_STATICMODE, scrollOffset,
        gSaveBlock2Ptr->randomizerStaticMode == 1 ? COMPOUND_STRING("Shuffle") :
        gSaveBlock2Ptr->randomizerStaticMode == 2 ? COMPOUND_STRING("Chaos") : COMPOUND_STRING("Ordinary"));
    ConvertIntToDecimalStringN(seedText, gSaveBlock2Ptr->randomizerSeed, STR_CONV_MODE_LEADING_ZEROS, 5);
    Randomizer_PrintValue(MENUITEM_RAND_SEED, scrollOffset, seedText);
    if (sSeedEditing)
    {
        u8 y;
        if (GetOptionMenuItemY(MENUITEM_RAND_SEED, scrollOffset, &y))
        {
            u8 prefix[6];
            u8 currentDigit[2] = {seedText[sSeedDigit], EOS};
            u32 x;
            memcpy(prefix, seedText, sSeedDigit);
            prefix[sSeedDigit] = EOS;
            x = 132 + GetStringWidth(FONT_SMALL, prefix, 0);
            FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(4), x, y, 7, 14);
            AddTextPrinterParameterized(WIN_OPTIONS, FONT_SMALL, currentDigit, x, y + 1, TEXT_SKIP_DRAW, NULL);
        }
    }
}

static void Randomizer_ProcessSeedInput(u8 taskId)
{
    static const u16 powers[5] = {10000, 1000, 100, 10, 1};
    u32 seed;
    u32 digit;

    if (!IsNewGameSetupActive())
        return;
    if (!sSeedEditing)
    {
        if (JOY_NEW(SELECT_BUTTON))
            gSaveBlock2Ptr->randomizerSeed = Random32() & 0xFFFF;
        else if (JOY_NEW(A_BUTTON))
        {
            sSeedBeforeEditing = gSaveBlock2Ptr->randomizerSeed;
            sSeedEditing = TRUE;
            sSeedDigit = 0;
        }
        RedrawVisibleOptionsPage(taskId);
        return;
    }

    if (JOY_NEW(A_BUTTON))
        sSeedEditing = FALSE;
    else if (JOY_NEW(B_BUTTON))
    {
        gSaveBlock2Ptr->randomizerSeed = sSeedBeforeEditing;
        sSeedEditing = FALSE;
    }
    else if (JOY_NEW(DPAD_LEFT))
        sSeedDigit = (sSeedDigit + 4) % 5;
    else if (JOY_NEW(DPAD_RIGHT))
        sSeedDigit = (sSeedDigit + 1) % 5;
    else if (JOY_NEW(DPAD_UP | DPAD_DOWN))
    {
        seed = gSaveBlock2Ptr->randomizerSeed;
        digit = (seed / powers[sSeedDigit]) % 10;
        if (JOY_NEW(DPAD_UP))
        {
            if (digit < 9)
                seed += powers[sSeedDigit];
            else
                seed -= 9 * powers[sSeedDigit];
        }
        else if (digit > 0)
            seed -= powers[sSeedDigit];
        if (seed <= 65535)
            gSaveBlock2Ptr->randomizerSeed = seed;
    }
    RedrawVisibleOptionsPage(taskId);
}

static void Randomizer_ProcessInput(u8 item)
{
    // All ordinary gameplay options remain editable. Only the randomizer
    // categories, static mode and seed become read-only after confirmation.
    if (item != MENUITEM_HARDMODE && !IsNewGameSetupActive())
        return;
    if (!(JOY_NEW(DPAD_LEFT | DPAD_RIGHT | L_BUTTON | R_BUTTON)))
        return;
    sArrowPressed = TRUE;
    switch (item)
    {
    case MENUITEM_HARDMODE:
        gSaveBlock2Ptr->optionsHardMode ^= 1;
        break;
    case MENUITEM_RAND_PRESET:
        sRandomizerPreset = (sRandomizerPreset + (JOY_NEW(DPAD_LEFT) ? 2 : 1)) % 3;
        if (sRandomizerPreset != 2)
        {
            bool8 enable = sRandomizerPreset == 1;
            gSaveBlock2Ptr->randomizerWild = enable;
            gSaveBlock2Ptr->randomizerTrainers = enable;
            gSaveBlock2Ptr->randomizerStarters = enable;
            gSaveBlock2Ptr->randomizerGifts = enable;
            gSaveBlock2Ptr->randomizerStatics = enable;
            gSaveBlock2Ptr->randomizerStaticMode = enable ? 2 : 0;
        }
        break;
    case MENUITEM_RAND_WILD:
        gSaveBlock2Ptr->randomizerWild ^= 1;
        sRandomizerPreset = 2;
        break;
    case MENUITEM_RAND_TRAINERS:
        gSaveBlock2Ptr->randomizerTrainers ^= 1;
        sRandomizerPreset = 2;
        break;
    case MENUITEM_RAND_STARTERS:
        gSaveBlock2Ptr->randomizerStarters ^= 1;
        sRandomizerPreset = 2;
        break;
    case MENUITEM_RAND_GIFTS:
        gSaveBlock2Ptr->randomizerGifts ^= 1;
        sRandomizerPreset = 2;
        break;
    case MENUITEM_RAND_STATICS:
        gSaveBlock2Ptr->randomizerStatics ^= 1;
        sRandomizerPreset = 2;
        break;
    case MENUITEM_RAND_STATICMODE:
        gSaveBlock2Ptr->randomizerStaticMode =
            (gSaveBlock2Ptr->randomizerStaticMode + (JOY_NEW(DPAD_LEFT) ? 2 : 1)) % 3;
        sRandomizerPreset = 2;
        break;
    case MENUITEM_RAND_SEED:
        break;
    }
}
#endif

static void DrawHeaderText(void)
{
    WorldMenu_DrawHeader(WIN_HEADER, gText_Option);
    FillWindowPixelBuffer(WIN_FOOTER, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_FOOTER, FONT_SMALL,
#if ALL_REGIONS
        IsNewGameSetupActive() ? COMPOUND_STRING("Seed: A Edit  SEL Random  START Lock") :
#endif
        COMPOUND_STRING("Left/Right Change    B Done"), 4, 2, TEXT_SKIP_DRAW, NULL);
    WorldMenu_RoundWindowTopCorners(WIN_FOOTER);
    PutWindowTilemap(WIN_FOOTER);
    CopyWindowToVram(WIN_FOOTER, COPYWIN_FULL);
}

static void DrawOptionMenuTexts(s16 scrollOffset)
{
    u8 i;

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    for (i = 0; i < NUM_VISIBLE_OPTIONS && scrollOffset + i < MENUITEM_COUNT; i++)
    {
        u8 item = scrollOffset + i;
#if ALL_REGIONS
        const u8 *text = item == MENUITEM_CANCEL && IsNewGameSetupActive()
            ? COMPOUND_STRING("Confirm & Lock") : sOptionMenuItemsNames[item];
#else
        const u8 *text = sOptionMenuItemsNames[item];
#endif
        AddTextPrinterParameterized(WIN_OPTIONS, FONT_SMALL, text, 8, (i * 16) + 1, TEXT_SKIP_DRAW, NULL);
    }
    // No CopyWindowToVram here: the sole caller (RedrawVisibleOptionsPage) draws the choice
    // values on top and does the full window copy, so a copy here would just be a wasted DMA.
}

#define TILE_TOP_CORNER_L 0x1F0
#define TILE_TOP_EDGE     0x1F1
#define TILE_TOP_CORNER_R 0x1F2
#define TILE_LEFT_EDGE    0x1F3
#define TILE_RIGHT_EDGE   0x1F5
#define TILE_BOT_CORNER_L 0x1F6
#define TILE_BOT_EDGE     0x1F7
#define TILE_BOT_CORNER_R 0x1F8

static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  2,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  2, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  2,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  3,  1, 14,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  3,  1, 14,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 17,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 17, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 17,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}
