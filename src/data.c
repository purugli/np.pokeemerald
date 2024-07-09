#include "global.h"
#include "malloc.h"
#include "battle.h"
#include "data.h"
#include "graphics.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/trainers.h"
#include "constants/battle_ai.h"
#include "strings.h"

const u16 gMinigameDigits_Pal[] = INCBIN_U16("graphics/link/minigame_digits.gbapal");
const u32 gMinigameDigits_Gfx[] = INCBIN_U32("graphics/link/minigame_digits.4bpp.lz");

#define BATTLER_OFFSET(i) (gHeap + 0x8000 + MON_PIC_SIZE * (i))

const struct SpriteFrameImage gBattlerPicTable_PlayerLeft[] =
{
    {BATTLER_OFFSET(0), MON_PIC_SIZE},
    {BATTLER_OFFSET(1), MON_PIC_SIZE},
    {BATTLER_OFFSET(2), MON_PIC_SIZE},
    {BATTLER_OFFSET(3), MON_PIC_SIZE},
};

const struct SpriteFrameImage gBattlerPicTable_OpponentLeft[] =
{
    {BATTLER_OFFSET(4), MON_PIC_SIZE},
    {BATTLER_OFFSET(5), MON_PIC_SIZE},
    {BATTLER_OFFSET(6), MON_PIC_SIZE},
    {BATTLER_OFFSET(7), MON_PIC_SIZE},
};

const struct SpriteFrameImage gBattlerPicTable_PlayerRight[] =
{
    {BATTLER_OFFSET(8),  MON_PIC_SIZE},
    {BATTLER_OFFSET(9),  MON_PIC_SIZE},
    {BATTLER_OFFSET(10), MON_PIC_SIZE},
    {BATTLER_OFFSET(11), MON_PIC_SIZE},
};

const struct SpriteFrameImage gBattlerPicTable_OpponentRight[] =
{
    {BATTLER_OFFSET(12), MON_PIC_SIZE},
    {BATTLER_OFFSET(13), MON_PIC_SIZE},
    {BATTLER_OFFSET(14), MON_PIC_SIZE},
    {BATTLER_OFFSET(15), MON_PIC_SIZE},
};

static const struct SpriteFrameImage sTrainerBackPicTable_Brendan[] =
{
    {.data = gTrainerBackPic_Brendan, .size = TRAINER_PIC_SIZE, .relativeFrames = TRUE}
};

static const struct SpriteFrameImage sTrainerBackPicTable_May[] =
{
    {.data = gTrainerBackPic_May, .size = TRAINER_PIC_SIZE, .relativeFrames = TRUE},
};

static const struct SpriteFrameImage sTrainerBackPicTable_FRLGRed[] =
{
    {.data = gTrainerBackPic_FRLGRed, .size = TRAINER_PIC_SIZE, .relativeFrames = TRUE},
};

static const struct SpriteFrameImage sTrainerBackPicTable_FRLGLeaf[] =
{
    {.data = gTrainerBackPic_FRLGLeaf, .size = TRAINER_PIC_SIZE, .relativeFrames = TRUE},
};

static const struct SpriteFrameImage sTrainerBackPicTable_FRLGPokedude[] =
{
    {.data = gTrainerBackPic_FRLGPokedude, .size = TRAINER_PIC_SIZE, .relativeFrames = TRUE},
};

static const struct SpriteFrameImage sTrainerBackPicTable_FRLGOldMan[] =
{
    {.data = gTrainerBackPic_FRLGOldMan, .size = TRAINER_PIC_SIZE, .relativeFrames = TRUE},
};

static const struct SpriteFrameImage sTrainerBackPicTable_RubySapphireBrendan[] =
{
    {.data = gTrainerBackPic_RubySapphireBrendan, .size = TRAINER_PIC_SIZE, .relativeFrames = TRUE},
};

static const struct SpriteFrameImage sTrainerBackPicTable_RubySapphireMay[] =
{
    {.data = gTrainerBackPic_RubySapphireMay, .size = TRAINER_PIC_SIZE, .relativeFrames = TRUE},
};

static const struct SpriteFrameImage sTrainerBackPicTable_Wally[] =
{
    {.data = gTrainerBackPic_Wally, .size = TRAINER_PIC_SIZE, .relativeFrames = TRUE},
};

static const struct SpriteFrameImage sTrainerBackPicTable_Steven[] =
{
    {.data = gTrainerBackPic_Steven, .size = TRAINER_PIC_SIZE, .relativeFrames = TRUE},
};

static const union AnimCmd sAnim_GeneralFrame0[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_GeneralFrame3[] =
{
    ANIMCMD_FRAME(3, 0),
    ANIMCMD_END,
};

// Many of these affine anims seem to go unused, and
// instead SetSpriteRotScale is used to manipulate
// the battler sprites directly (for instance, in AnimTask_SwitchOutShrinkMon).
// Those with explicit indexes are referenced elsewhere.

static const union AffineAnimCmd sAffineAnim_Battler_Normal[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_Flipped[] =
{
    AFFINEANIMCMD_FRAME(-0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_Emerge[] =
{
    AFFINEANIMCMD_FRAME(0x28, 0x28, 0, 0),
    AFFINEANIMCMD_FRAME(0x12, 0x12, 0, 12),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_Return[] =
{
    AFFINEANIMCMD_FRAME( -0x2,  -0x2, 0, 18),
    AFFINEANIMCMD_FRAME(-0x10, -0x10, 0, 15),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_HorizontalSquishLoop[] =
{
    AFFINEANIMCMD_FRAME(0xA0, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME( 0x4,   0x0, 0, 8),
    AFFINEANIMCMD_FRAME(-0x4,   0x0, 0, 8),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd sAffineAnim_Battler_Grow[] =
{
    AFFINEANIMCMD_FRAME(0x2, 0x2, 0, 20),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_Shrink[] =
{
    AFFINEANIMCMD_FRAME(-0x2, -0x2, 0, 20),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_BigToSmall[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(-0x10, -0x10, 0, 9),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_GrowLarge[] =
{
    AFFINEANIMCMD_FRAME(0x4, 0x4, 0, 63),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_TipRight[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -3, 5),
    AFFINEANIMCMD_FRAME(0x0, 0x0,  3, 5),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gAffineAnims_BattleSpritePlayerSide[] =
{
    [BATTLER_AFFINE_NORMAL] = sAffineAnim_Battler_Normal,
    [BATTLER_AFFINE_EMERGE] = sAffineAnim_Battler_Emerge,
    [BATTLER_AFFINE_RETURN] = sAffineAnim_Battler_Return,
    sAffineAnim_Battler_HorizontalSquishLoop,
    sAffineAnim_Battler_Grow,
    sAffineAnim_Battler_Shrink,
    sAffineAnim_Battler_GrowLarge,
    sAffineAnim_Battler_TipRight,
    sAffineAnim_Battler_BigToSmall,
};

static const union AffineAnimCmd sAffineAnim_Battler_SpinShrink[] =
{
    AFFINEANIMCMD_FRAME(-0x4, -0x4, 4, 63),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_TipLeft[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0,  3, 5),
    AFFINEANIMCMD_FRAME(0x0, 0x0, -3, 5),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_RotateUpAndBack[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -5, 20),
    AFFINEANIMCMD_FRAME(0x0, 0x0,  0, 20),
    AFFINEANIMCMD_FRAME(0x0, 0x0,  5, 20),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Battler_Spin[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 9, 110),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gAffineAnims_BattleSpriteOpponentSide[] =
{
    [BATTLER_AFFINE_NORMAL] = sAffineAnim_Battler_Normal,
    [BATTLER_AFFINE_EMERGE] = sAffineAnim_Battler_Emerge,
    [BATTLER_AFFINE_RETURN] = sAffineAnim_Battler_Return,
    sAffineAnim_Battler_HorizontalSquishLoop,
    sAffineAnim_Battler_Grow,
    sAffineAnim_Battler_Shrink,
    sAffineAnim_Battler_SpinShrink,
    sAffineAnim_Battler_TipLeft,
    sAffineAnim_Battler_RotateUpAndBack,
    sAffineAnim_Battler_BigToSmall,
    sAffineAnim_Battler_Spin,
};

const union AffineAnimCmd *const gAffineAnims_BattleSpriteContest[] =
{
    [BATTLER_AFFINE_NORMAL] = sAffineAnim_Battler_Flipped,
    [BATTLER_AFFINE_EMERGE] = sAffineAnim_Battler_Emerge,
    [BATTLER_AFFINE_RETURN] = sAffineAnim_Battler_Return,
    sAffineAnim_Battler_HorizontalSquishLoop,
    sAffineAnim_Battler_Grow,
    sAffineAnim_Battler_Shrink,
    sAffineAnim_Battler_SpinShrink,
    sAffineAnim_Battler_TipLeft,
    sAffineAnim_Battler_RotateUpAndBack,
    sAffineAnim_Battler_BigToSmall,
    sAffineAnim_Battler_Spin,
};

static const union AnimCmd sAnim_MonPic_1[] =
{
    ANIMCMD_FRAME(1, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_MonPic_2[] =
{
    ANIMCMD_FRAME(2, 0),
    ANIMCMD_END,
};

const union AnimCmd *const gAnims_MonPic[MAX_MON_PIC_FRAMES] =
{
    sAnim_GeneralFrame0,
    sAnim_MonPic_1,
    sAnim_MonPic_2,
    sAnim_GeneralFrame3,
};

const union AnimCmd *const gAnims_None[] =
{
    sAnim_GeneralFrame0,
    sAnim_GeneralFrame0,
};

#define SPECIES_SPRITE(species, sprite) [SPECIES_##species] = sprite
#define SPECIES_PAL(species, pal) [SPECIES_##species] = {pal, SPECIES_##species}
#define SPECIES_SHINY_PAL(species, pal) [SPECIES_##species] = {pal, SPECIES_##species + SPECIES_SHINY_TAG}

#include "data/pokemon_graphics/front_pic_coordinates.h"
#include "data/pokemon_graphics/front_pic_table.h"
#include "data/pokemon_graphics/back_pic_coordinates.h"

#include "data/pokemon_graphics/back_pic_table.h"
#include "data/pokemon_graphics/palette_table.h"
#include "data/pokemon_graphics/shiny_palette_table.h"

#include "data/trainer_graphics/front_pic_tables.h"
#include "data/trainer_graphics/back_pic_tables.h"

#include "data/pokemon_graphics/enemy_mon_elevation.h"
#include "data/pokemon_graphics/front_pic_anims.h"

#include "data/trainer_parties.h"
#include "data/text/trainer_class_names.h"
#include "data/trainers.h"
#include "data/text/japanese_species_names.h"
#include "data/text/species_names.h"
#include "data/text/french_species_names.h"
#include "data/text/german_species_names.h"
#include "data/text/move_names.h"
