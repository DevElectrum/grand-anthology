#include "global.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "pokedex.h"
#include "pokemon.h"
#include "regions.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "starter_choose.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "trainer_pokemon_sprites.h"
#include "trig.h"
#include "window.h"
#include "constants/songs.h"
#include "constants/rgb.h"

#define STARTER_MON_COUNT   3 // Rendered Poké Balls per page
#define STARTER_GENERATION_PAGES 3 // Kanto has a fourth Poipole-only page
#define STARTER_SPECIES_COUNT 10 // Kanto's eventual extra Poipole choice

// Position of the sprite of the selected starter Pokémon
#define STARTER_PKMN_POS_X (DISPLAY_WIDTH / 2)
#define STARTER_PKMN_POS_Y 64

#define TAG_POKEBALL_SELECT 0x1000
#define TAG_STARTER_CIRCLE  0x1001

static void CB2_StarterChoose(void);
static void ClearStarterLabel(void);
static void Task_StarterChoose(u8 taskId);
static void Task_HandleStarterChooseInput(u8 taskId);
static void Task_WaitForStarterSprite(u8 taskId);
static void Task_AskConfirmStarter(u8 taskId);
static void Task_HandleConfirmStarterInput(u8 taskId);
static void Task_DeclineStarter(u8 taskId);
static void Task_MoveStarterChooseCursor(u8 taskId);
static void Task_CreateStarterLabel(u8 taskId);
static void CreateStarterPokemonLabel(u8 starterId);
static u8 CreatePokemonFrontSprite(enum Species species, u8 x, u8 y);
static void SpriteCB_SelectionHand(struct Sprite *sprite);
static void SpriteCB_Pokeball(struct Sprite *sprite);
static void SpriteCB_StarterPokemon(struct Sprite *sprite);

static u16 sStarterLabelWindowId;

const u16 gBirchBagGrass_Pal[] = INCGFX_U16("graphics/starter_choose/tiles.png", ".gbapal");
static const u16 sPokeballSelection_Pal[] = INCGFX_U16("graphics/starter_choose/pokeball_selection.png", ".gbapal");
static const u16 sStarterCircle_Pal[] = INCGFX_U16("graphics/starter_choose/starter_circle.png", ".gbapal");
const u32 gBirchBagTilemap[] = INCGFX_U32("graphics/starter_choose/birch_bag.bin", ".smolTM");
const u32 gBirchGrassTilemap[] = INCGFX_U32("graphics/starter_choose/birch_grass.bin", ".smolTM");
const u32 gBirchBagGrass_Gfx[] = INCGFX_U32("graphics/starter_choose/tiles.png", ".4bpp.smol");
const u32 gPokeballSelection_Gfx[] = INCGFX_U32("graphics/starter_choose/pokeball_selection.png", ".4bpp.smol");
static const u32 sStarterCircle_Gfx[] = INCGFX_U32("graphics/starter_choose/starter_circle.png", ".4bpp.smol");

static const struct WindowTemplate sWindowTemplates[] =
{
    {
        .bg = 0,
        .tilemapLeft = 3,
        .tilemapTop = 15,
        .width = 24,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 0x0200
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct WindowTemplate sWindowTemplate_ConfirmStarter =
{
    .bg = 0,
    .tilemapLeft = 24,
    .tilemapTop = 9,
    .width = 5,
    .height = 4,
    .paletteNum = 14,
    .baseBlock = 0x0260
};

static const struct WindowTemplate sWindowTemplate_StarterLabel =
{
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 0,
    .width = 13,
    .height = 4,
    .paletteNum = 14,
    .baseBlock = 0x0274
};

static const u8 sPokeballCoords[STARTER_MON_COUNT][2] =
{
    {60, 64},
    {120, 88},
    {180, 64},
};

static const u8 sStarterLabelCoords[STARTER_MON_COUNT][2] =
{
    {0, 9},
    {16, 10},
    {8, 4},
};

// The original three starter indices in every region MUST remain stable for
// authentic saves and original story branches. Extra pages are grouped by
// introductory generation. Kanto preserves its original 0=Bulbasaur,
// 1=Squirtle, 2=Charmander indexing. Index 9 is reserved for Kanto Poipole;
// Kanto's fourth page holds Poipole alone; all regions share the three-ball UI.
static const u16 sStarterMon[REGIONS_COUNT][STARTER_SPECIES_COUNT] =
{
    [REGION_HOENN] = { SPECIES_TREECKO, SPECIES_TORCHIC, SPECIES_MUDKIP,
                       SPECIES_CHESPIN, SPECIES_FENNEKIN, SPECIES_FROAKIE,
                       SPECIES_SPRIGATITO, SPECIES_FUECOCO, SPECIES_QUAXLY },
    [REGION_KANTO] = { SPECIES_BULBASAUR, SPECIES_SQUIRTLE, SPECIES_CHARMANDER,
                       SPECIES_TURTWIG, SPECIES_PIPLUP, SPECIES_CHIMCHAR,
                       SPECIES_ROWLET, SPECIES_POPPLIO, SPECIES_LITTEN,
                       SPECIES_POIPOLE },
    [REGION_JOHTO] = { SPECIES_CHIKORITA, SPECIES_CYNDAQUIL, SPECIES_TOTODILE,
                       SPECIES_SNIVY, SPECIES_TEPIG, SPECIES_OSHAWOTT,
                       SPECIES_GROOKEY, SPECIES_SCORBUNNY, SPECIES_SOBBLE },
};

static const struct BgTemplate sBgTemplates[3] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 7,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
    {
        .bg = 3,
        .charBaseIndex = 0,
        .mapBaseIndex = 6,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
};

static const u8 sTextColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY};

static const struct OamData sOam_Hand =
{
    .y = DISPLAY_HEIGHT,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct OamData sOam_Pokeball =
{
    .y = DISPLAY_HEIGHT,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct OamData sOam_StarterCircle =
{
    .y = DISPLAY_HEIGHT,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const u8 sCursorCoords[][2] =
{
    {60, 32},
    {120, 56},
    {180, 32},
};

static const union AnimCmd sAnim_Hand[] =
{
    ANIMCMD_FRAME(48, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Pokeball_Still[] =
{
    ANIMCMD_FRAME(0, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Pokeball_Moving[] =
{
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(0, 32),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(32, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(32, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_StarterCircle[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_Hand[] =
{
    sAnim_Hand,
};

static const union AnimCmd *const sAnims_Pokeball[] =
{
    sAnim_Pokeball_Still,
    sAnim_Pokeball_Moving,
};

static const union AnimCmd *const sAnims_StarterCircle[] =
{
    sAnim_StarterCircle,
};

static const union AffineAnimCmd sAffineAnim_StarterPokemon[] =
{
    AFFINEANIMCMD_FRAME(16, 16, 0, 0),
    AFFINEANIMCMD_FRAME(16, 16, 0, 15),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_StarterCircle[] =
{
    AFFINEANIMCMD_FRAME(20, 20, 0, 0),
    AFFINEANIMCMD_FRAME(20, 20, 0, 15),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_StarterPokemon = {sAffineAnim_StarterPokemon};
static const union AffineAnimCmd *const sAffineAnims_StarterCircle[] = {sAffineAnim_StarterCircle};

static const struct CompressedSpriteSheet sSpriteSheet_PokeballSelect[] =
{
    {
        .data = gPokeballSelection_Gfx,
        .size = 0x0800,
        .tag = TAG_POKEBALL_SELECT
    },
    {}
};

static const struct CompressedSpriteSheet sSpriteSheet_StarterCircle[] =
{
    {
        .data = sStarterCircle_Gfx,
        .size = 0x0800,
        .tag = TAG_STARTER_CIRCLE
    },
    {}
};

static const struct SpritePalette sSpritePalettes_StarterChoose[] =
{
    {
        .data = sPokeballSelection_Pal,
        .tag = TAG_POKEBALL_SELECT
    },
    {
        .data = sStarterCircle_Pal,
        .tag = TAG_STARTER_CIRCLE
    },
    {},
};

static const struct SpriteTemplate sSpriteTemplate_Hand =
{
    .tileTag = TAG_POKEBALL_SELECT,
    .paletteTag = TAG_POKEBALL_SELECT,
    .oam = &sOam_Hand,
    .anims = sAnims_Hand,
    .callback = SpriteCB_SelectionHand
};

static const struct SpriteTemplate sSpriteTemplate_Pokeball =
{
    .tileTag = TAG_POKEBALL_SELECT,
    .paletteTag = TAG_POKEBALL_SELECT,
    .oam = &sOam_Pokeball,
    .anims = sAnims_Pokeball,
    .callback = SpriteCB_Pokeball
};

static const struct SpriteTemplate sSpriteTemplate_StarterCircle =
{
    .tileTag = TAG_STARTER_CIRCLE,
    .paletteTag = TAG_STARTER_CIRCLE,
    .oam = &sOam_StarterCircle,
    .anims = sAnims_StarterCircle,
    .affineAnims = sAffineAnims_StarterCircle,
    .callback = SpriteCB_StarterPokemon
};

// .text
u16 GetStarterPokemon(u16 chosenStarterId)
{
    enum Region region = GetCurrentRegion();

    if (region != REGION_KANTO && region != REGION_JOHTO)
        region = REGION_HOENN;
    if (chosenStarterId >= STARTER_SPECIES_COUNT
     || (region != REGION_KANTO && chosenStarterId == STARTER_SPECIES_COUNT - 1))
        chosenStarterId = 0;
    return sStarterMon[region][chosenStarterId];
}


// Full selection is saved for Dex/credits; original Kanto scripts still have
// three rival battle branches. Poipole uses the Charmander branch's story
// staging, but Blue actually begins with Rhyhorn in battle (battle_main.c).
static u16 GetKantoStarterBranch(u16 index)
{
    return index == 9 ? 0 : index % STARTER_MON_COUNT;
}

void Anthology_GetKantoStarterStoryBranch(struct ScriptContext *ctx)
{
    gSpecialVar_Result = GetKantoStarterBranch(VarGet(VAR_STARTER_MON));
}

void Anthology_GetKantoPendingStarterBranch(struct ScriptContext *ctx)
{
    gSpecialVar_Result = GetKantoStarterBranch(VarGet(VAR_TEMP_1));
}

void Anthology_GetKantoChosenStarterSpecies(struct ScriptContext *ctx)
{
    gSpecialVar_Result = GetStarterPokemon(VarGet(VAR_TEMP_1));
}

// Hoenn story scripts were authored for three starter *types*, not nine species.
// VAR_STARTER_MON still stores the full selected species index, making credits
// and IsStarterInParty accurate; only script rival/doll dispatch uses its type.
void Anthology_GetHoennStarterStoryBranch(struct ScriptContext *ctx)
{
    gSpecialVar_Result = VarGet(VAR_STARTER_MON) % STARTER_MON_COUNT;
}

// Silver's canonical teams still follow the original elemental branch.
void Anthology_GetJohtoStarterStoryBranch(struct ScriptContext *ctx)
{
    gSpecialVar_Result = VarGet(VAR_STARTER_MON) % STARTER_MON_COUNT;
}

void Anthology_GetJohtoChosenStarterSpecies(struct ScriptContext *ctx)
{
    gSpecialVar_Result = GetStarterPokemon(VarGet(VAR_STARTER_MON));
}


static void VblankCB_StarterChoose(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

// Data for Task_StarterChoose
#define tStarterSelection   data[0]
#define tPkmnSpriteId       data[1]
#define tCircleSpriteId     data[2]
#define tStarterPage        data[3]

// Data for sSpriteTemplate_Pokeball
#define sTaskId data[0]
#define sBallId data[1]

void CB2_ChooseStarter(void)
{
    u8 taskId;
    u8 spriteId;

    SetVBlankCallback(NULL);

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG3CNT, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);

    ChangeBgX(0, 0, BG_COORD_SET);
    ChangeBgY(0, 0, BG_COORD_SET);
    ChangeBgX(1, 0, BG_COORD_SET);
    ChangeBgY(1, 0, BG_COORD_SET);
    ChangeBgX(2, 0, BG_COORD_SET);
    ChangeBgY(2, 0, BG_COORD_SET);
    ChangeBgX(3, 0, BG_COORD_SET);
    ChangeBgY(3, 0, BG_COORD_SET);

    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);

    DecompressDataWithHeaderVram(gBirchBagGrass_Gfx, (void *)VRAM);
    DecompressDataWithHeaderVram(gBirchBagTilemap, (void *)(BG_SCREEN_ADDR(6)));
    DecompressDataWithHeaderVram(gBirchGrassTilemap, (void *)(BG_SCREEN_ADDR(7)));

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    InitWindows(sWindowTemplates);

    DeactivateAllTextPrinters();
    LoadUserWindowBorderGfx(0, 0x2A8, BG_PLTT_ID(13));
    ClearScheduledBgCopiesToVram();
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    ResetPaletteFade();
    FreeAllSpritePalettes();
    ResetAllPicSprites();

    LoadPalette(GetOverworldTextboxPalettePtr(), BG_PLTT_ID(14), PLTT_SIZE_4BPP);
    LoadPalette(gBirchBagGrass_Pal, BG_PLTT_ID(0), sizeof(gBirchBagGrass_Pal));
    LoadCompressedSpriteSheet(&sSpriteSheet_PokeballSelect[0]);
    LoadCompressedSpriteSheet(&sSpriteSheet_StarterCircle[0]);
    LoadSpritePalettes(sSpritePalettes_StarterChoose);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);

    EnableInterrupts(DISPSTAT_VBLANK);
    SetVBlankCallback(VblankCB_StarterChoose);
    SetMainCallback2(CB2_StarterChoose);

    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 7);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);

    ShowBg(0);
    ShowBg(2);
    ShowBg(3);

    taskId = CreateTask(Task_StarterChoose, 0);
    // Oak and Elm's original leftmost starter is index zero. The
    // original Birch bag opens with its center ball highlighted instead.
    // Starting Elm on index one silently gifts Cyndaquil when the player
    // accepts the default choice, despite touching Chikorita's lab ball.
    gTasks[taskId].tStarterSelection = GetCurrentRegion() == REGION_HOENN ? 1 : 0;
    gTasks[taskId].tStarterPage = 0;

    // Create hand sprite
    spriteId = CreateSprite(&sSpriteTemplate_Hand, 120, 56, 2);
    gSprites[spriteId].data[0] = taskId;

    // Create three Poké Ball sprites
    spriteId = CreateSprite(&sSpriteTemplate_Pokeball, sPokeballCoords[0][0], sPokeballCoords[0][1], 2);
    gSprites[spriteId].sTaskId = taskId;
    gSprites[spriteId].sBallId = 0;

    spriteId = CreateSprite(&sSpriteTemplate_Pokeball, sPokeballCoords[1][0], sPokeballCoords[1][1], 2);
    gSprites[spriteId].sTaskId = taskId;
    gSprites[spriteId].sBallId = 1;

    spriteId = CreateSprite(&sSpriteTemplate_Pokeball, sPokeballCoords[2][0], sPokeballCoords[2][1], 2);
    gSprites[spriteId].sTaskId = taskId;
    gSprites[spriteId].sBallId = 2;

    sStarterLabelWindowId = WINDOW_NONE;
}

static void CB2_StarterChoose(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static const u8 sChooseStarterJohtoPrompt[] = _("Choose a POKeMON.\nL/R: More starter groups");
static const u8 sChooseStarterKantoPrompt[] = _("OAK: Choose a POKeMON.\nL/R: More starter groups");
static const u8 sChooseStarterBirchPrompt[] = _("Help BIRCH! Choose a POKeMON.\nL/R: More starter groups");

static void Task_StarterChoose(u8 taskId)
{
    CreateStarterPokemonLabel(gTasks[taskId].tStarterPage * STARTER_MON_COUNT
                              + gTasks[taskId].tStarterSelection);
    DrawStdFrameWithCustomTileAndPalette(0, FALSE, 0x2A8, 0xD);
    AddTextPrinterParameterized(0, FONT_NORMAL,
                                GetCurrentRegion() == REGION_JOHTO ? sChooseStarterJohtoPrompt
                                    : GetCurrentRegion() == REGION_KANTO ? sChooseStarterKantoPrompt
                                    : sChooseStarterBirchPrompt,
                                0, 1, 0, NULL);
    PutWindowTilemap(0);
    ScheduleBgCopyTilemapToVram(0);
    gTasks[taskId].func = Task_HandleStarterChooseInput;
}

static void Task_HandleStarterChooseInput(u8 taskId)
{
    u8 selection = gTasks[taskId].tStarterSelection;

    if (JOY_NEW(A_BUTTON))
    {
        u8 spriteId;

        ClearStarterLabel();

        // Create white circle background
        spriteId = CreateSprite(&sSpriteTemplate_StarterCircle, sPokeballCoords[selection][0], sPokeballCoords[selection][1], 1);
        gTasks[taskId].tCircleSpriteId = spriteId;

        // Create Pokémon sprite
        spriteId = CreatePokemonFrontSprite(GetStarterPokemon(gTasks[taskId].tStarterPage * STARTER_MON_COUNT
                                                        + gTasks[taskId].tStarterSelection), sPokeballCoords[selection][0], sPokeballCoords[selection][1]);
        gSprites[spriteId].affineAnims = &sAffineAnims_StarterPokemon;
        gSprites[spriteId].callback = SpriteCB_StarterPokemon;

        gTasks[taskId].tPkmnSpriteId = spriteId;
        gTasks[taskId].func = Task_WaitForStarterSprite;
    }
    else if (JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON))
    {
        // Three ball slots are reused for each generation; no extra OAM
        // sprites, graphics or save allocation needed.
        u8 pages = GetCurrentRegion() == REGION_KANTO ? 4 : STARTER_GENERATION_PAGES;
        if (JOY_NEW(R_BUTTON))
            gTasks[taskId].tStarterPage = (gTasks[taskId].tStarterPage + 1) % pages;
        else
            gTasks[taskId].tStarterPage = (gTasks[taskId].tStarterPage + pages - 1) % pages;
        // The fourth Kanto page has Poipole in its only occupied slot.
        if (gTasks[taskId].tStarterPage == 3)
            gTasks[taskId].tStarterSelection = 0;
        gTasks[taskId].func = Task_MoveStarterChooseCursor;
    }
    else if (JOY_NEW(DPAD_LEFT) && selection > 0)
    {
        gTasks[taskId].tStarterSelection--;
        gTasks[taskId].func = Task_MoveStarterChooseCursor;
    }
    else if (JOY_NEW(DPAD_RIGHT) && selection < STARTER_MON_COUNT - 1
          && !(GetCurrentRegion() == REGION_KANTO && gTasks[taskId].tStarterPage == 3))
    {
        gTasks[taskId].tStarterSelection++;
        gTasks[taskId].func = Task_MoveStarterChooseCursor;
    }
}

static void Task_WaitForStarterSprite(u8 taskId)
{
    if (gSprites[gTasks[taskId].tCircleSpriteId].affineAnimEnded &&
        gSprites[gTasks[taskId].tCircleSpriteId].x == STARTER_PKMN_POS_X &&
        gSprites[gTasks[taskId].tCircleSpriteId].y == STARTER_PKMN_POS_Y)
    {
        gTasks[taskId].func = Task_AskConfirmStarter;
    }
}

static void Task_AskConfirmStarter(u8 taskId)
{
    PlayCry_Normal(GetStarterPokemon(gTasks[taskId].tStarterPage * STARTER_MON_COUNT
                                                        + gTasks[taskId].tStarterSelection), 0);
    FillWindowPixelBuffer(0, PIXEL_FILL(1));
    AddTextPrinterParameterized(0, FONT_NORMAL, gText_ConfirmStarterChoice, 0, 1, 0, NULL);
    ScheduleBgCopyTilemapToVram(0);
    CreateYesNoMenu(&sWindowTemplate_ConfirmStarter, 0x2A8, 0xD, 0);
    gTasks[taskId].func = Task_HandleConfirmStarterInput;
}

static void Task_HandleConfirmStarterInput(u8 taskId)
{
    u8 spriteId;

    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:  // YES
        // Return the starter choice and exit.
        gSpecialVar_Result = gTasks[taskId].tStarterPage * STARTER_MON_COUNT
                           + gTasks[taskId].tStarterSelection;
        ResetAllPicSprites();
        SetMainCallback2(gMain.savedCallback);
        break;
    case 1:  // NO
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        spriteId = gTasks[taskId].tPkmnSpriteId;
        FreeOamMatrix(gSprites[spriteId].oam.matrixNum);
        FreeAndDestroyMonPicSprite(spriteId);

        spriteId = gTasks[taskId].tCircleSpriteId;
        FreeOamMatrix(gSprites[spriteId].oam.matrixNum);
        DestroySprite(&gSprites[spriteId]);
        gTasks[taskId].func = Task_DeclineStarter;
        break;
    }
}

static void Task_DeclineStarter(u8 taskId)
{
    gTasks[taskId].func = Task_StarterChoose;
}

static void CreateStarterPokemonLabel(u8 starterId)
{
    u8 categoryText[32];
    struct WindowTemplate winTemplate;
    const u8 *speciesName;
    s32 width;
    u8 labelLeft, labelRight, labelTop, labelBottom;

    enum Species species = GetStarterPokemon(starterId);
    u8 selection = starterId % STARTER_MON_COUNT;
    CopyMonCategoryText(species, categoryText);
    speciesName = GetSpeciesName(species);

    winTemplate = sWindowTemplate_StarterLabel;
    winTemplate.tilemapLeft = sStarterLabelCoords[selection][0];
    winTemplate.tilemapTop = sStarterLabelCoords[selection][1];

    sStarterLabelWindowId = AddWindow(&winTemplate);
    FillWindowPixelBuffer(sStarterLabelWindowId, PIXEL_FILL(0));

    width = GetStringCenterAlignXOffset(FONT_NARROW, categoryText, 0x68);
    AddTextPrinterParameterized3(sStarterLabelWindowId, FONT_NARROW, width, 1, sTextColors, 0, categoryText);

    width = GetStringCenterAlignXOffset(FONT_NORMAL, speciesName, 0x68);
    AddTextPrinterParameterized3(sStarterLabelWindowId, FONT_NORMAL, width, 17, sTextColors, 0, speciesName);

    PutWindowTilemap(sStarterLabelWindowId);
    ScheduleBgCopyTilemapToVram(0);

    labelLeft = sStarterLabelCoords[selection][0] * 8 - 4;
    labelRight = (sStarterLabelCoords[selection][0] + 13) * 8 + 4;
    labelTop = sStarterLabelCoords[selection][1] * 8;
    labelBottom = (sStarterLabelCoords[selection][1] + 4) * 8;
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(labelLeft, labelRight));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(labelTop, labelBottom));
}

static void ClearStarterLabel(void)
{
    FillWindowPixelBuffer(sStarterLabelWindowId, PIXEL_FILL(0));
    ClearWindowTilemap(sStarterLabelWindowId);
    RemoveWindow(sStarterLabelWindowId);
    sStarterLabelWindowId = WINDOW_NONE;
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    ScheduleBgCopyTilemapToVram(0);
}

static void Task_MoveStarterChooseCursor(u8 taskId)
{
    ClearStarterLabel();
    gTasks[taskId].func = Task_CreateStarterLabel;
}

static void Task_CreateStarterLabel(u8 taskId)
{
    CreateStarterPokemonLabel(gTasks[taskId].tStarterPage * STARTER_MON_COUNT
                              + gTasks[taskId].tStarterSelection);
    gTasks[taskId].func = Task_HandleStarterChooseInput;
}

static u8 CreatePokemonFrontSprite(enum Species species, u8 x, u8 y)
{
    u8 spriteId;

    spriteId = CreateMonPicSprite_Affine(species, FALSE, 0, MON_PIC_AFFINE_FRONT, x, y, 14, TAG_NONE);
    gSprites[spriteId].oam.priority = 0;
    return spriteId;
}

static void SpriteCB_SelectionHand(struct Sprite *sprite)
{
    // Float up and down above selected Poké Ball
    sprite->x = sCursorCoords[gTasks[sprite->data[0]].tStarterSelection][0];
    sprite->y = sCursorCoords[gTasks[sprite->data[0]].tStarterSelection][1];
    sprite->y2 = Sin(sprite->data[1], 8);
    sprite->data[1] = (u8)(sprite->data[1]) + 4;
}

static void SpriteCB_Pokeball(struct Sprite *sprite)
{
    sprite->invisible = (GetCurrentRegion() == REGION_KANTO
                      && gTasks[sprite->sTaskId].tStarterPage == 3 && sprite->sBallId != 0);
    // Animate Poké Ball if currently selected
    if (gTasks[sprite->sTaskId].tStarterSelection == sprite->sBallId)
        StartSpriteAnimIfDifferent(sprite, 1);
    else
        StartSpriteAnimIfDifferent(sprite, 0);
}

static void SpriteCB_StarterPokemon(struct Sprite *sprite)
{
    // Move sprite to upper center of screen
    if (sprite->x > STARTER_PKMN_POS_X)
        sprite->x -= 4;
    if (sprite->x < STARTER_PKMN_POS_X)
        sprite->x += 4;
    if (sprite->y > STARTER_PKMN_POS_Y)
        sprite->y -= 2;
    if (sprite->y < STARTER_PKMN_POS_Y)
        sprite->y += 2;
}
