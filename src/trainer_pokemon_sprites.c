#include "global.h"
#include "sprite.h"
#include "window.h"
#include "malloc.h"
#include "palette.h"
#include "decompress.h"
#include "trainer_pokemon_sprites.h"
#include "data.h"
#include "pokemon.h"
#include "constants/trainers.h"
#include "trainer_card.h"
#include "link.h"

#define PICS_COUNT 8

// Needs to be large enough to store either a decompressed Pokémon pic or trainer pic
#define PIC_SPRITE_SIZE max(MON_PIC_SIZE, TRAINER_PIC_SIZE)
#define MAX_PIC_FRAMES  max(MAX_MON_PIC_FRAMES, MAX_TRAINER_PIC_FRAMES)

struct PicData
{
    u8 *frames;
    struct SpriteFrameImage *images;
    u16 paletteTag;
    u8 spriteId;
    u8 active;
};

static EWRAM_DATA struct SpriteTemplate sCreatingSpriteTemplate = {};
static EWRAM_DATA struct PicData sSpritePics[PICS_COUNT] = {};

static const struct PicData sDummyPicData = {};

static const struct OamData sOamData_Normal =
{
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64)
};

static const struct OamData sOamData_Affine =
{
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64)
};

static void DummyPicSpriteCallback(struct Sprite *sprite)
{

}

bool16 ResetAllPicSprites(void)
{
    memset(sSpritePics, 0, sizeof(sSpritePics));

    return FALSE;
}

u16 CreateMonPicSprite(u16 species, u32 otId, u32 personality, bool8 useAffine, s16 x, s16 y, u8 paletteSlot, u16 paletteTag)
{
    u8 *framePics;
    struct SpriteFrameImage *images;
    int j;
    u32 i;
    u8 spriteId;
    const struct SpritePalette *palette;

    for (i = 0; i < PICS_COUNT; i ++)
    {
        if (!sSpritePics[i].active)
            break;
    }
    if (i == PICS_COUNT)
        return 0xFFFF;

    framePics = Alloc(MON_PIC_SIZE * MAX_MON_PIC_FRAMES);
    if (!framePics)
        return 0xFFFF;

    images = Alloc(sizeof(struct SpriteFrameImage) * MAX_MON_PIC_FRAMES);
    if (!images)
    {
        Free(framePics);
        return 0xFFFF;
    }
    LoadSpecialPokePic(framePics, species, personality, TRUE, TRUE);
    for (j = 0; j < MAX_MON_PIC_FRAMES; j ++)
    {
        images[j].data = framePics + MON_PIC_SIZE * j;
        images[j].size = MON_PIC_SIZE;
    }
    sCreatingSpriteTemplate.tileTag = TAG_NONE;
    sCreatingSpriteTemplate.anims = gMonFrontAnimsPtrTable[species];
    sCreatingSpriteTemplate.images = images;
    if (useAffine)
    {
        sCreatingSpriteTemplate.affineAnims = gAffineAnims_BattleSpriteOpponentSide;
        sCreatingSpriteTemplate.oam = &sOamData_Affine;
    }
    else
    {
        sCreatingSpriteTemplate.oam = &sOamData_Normal;
        sCreatingSpriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
    }
    sCreatingSpriteTemplate.callback = DummyPicSpriteCallback;
    sCreatingSpriteTemplate.paletteTag = paletteTag;
    palette = GetMonSpritePalStructFromOtIdPersonality(species, otId, personality);
    if (paletteTag == TAG_NONE)
        LoadPalette(palette->data, OBJ_PLTT_ID(paletteSlot), PLTT_SIZE_4BPP);
    else
        LoadSpritePalette(palette);
    spriteId = CreateSprite(&sCreatingSpriteTemplate, x, y, 0);
    if (paletteTag == TAG_NONE)
        gSprites[spriteId].oam.paletteNum = paletteSlot;
    sSpritePics[i].frames = framePics;
    sSpritePics[i].images = images;
    sSpritePics[i].paletteTag = paletteTag;
    sSpritePics[i].spriteId = spriteId;
    sSpritePics[i].active = TRUE;
    return spriteId;
}

u16 CreateTrainerPicSprite(u16 trainerPicId, s16 x, s16 y, u8 paletteSlot)
{
    u32 i;
    u8 *framePics;
    struct SpriteFrameImage *images;
    int j;
    u8 spriteId;
    const struct TrainerSprite *trainerSprite = &gTrainerSpriteTable[trainerPicId];

    for (i = 0; i < PICS_COUNT; i++)
    {
        if (!sSpritePics[i].active)
            break;
    }
    if (i == PICS_COUNT)
        return 0xFFFF;

    framePics = Alloc(TRAINER_PIC_SIZE * MAX_TRAINER_PIC_FRAMES);
    if (!framePics)
        return 0xFFFF;

    images = Alloc(sizeof(struct SpriteFrameImage) * MAX_TRAINER_PIC_FRAMES);
    if (!images)
    {
        Free(framePics);
        return 0xFFFF;
    }
    LZ77UnCompWram(trainerSprite->sprite.data, framePics);
    for (j = 0; j < MAX_TRAINER_PIC_FRAMES; j ++)
    {
        images[j].data = framePics + TRAINER_PIC_SIZE * j;
        images[j].size = TRAINER_PIC_SIZE;
    }
    sCreatingSpriteTemplate.tileTag = TAG_NONE;
    sCreatingSpriteTemplate.oam = &sOamData_Normal;
    sCreatingSpriteTemplate.anims = gAnims_None;
    sCreatingSpriteTemplate.images = images;
    sCreatingSpriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
    sCreatingSpriteTemplate.callback = DummyPicSpriteCallback;
    sCreatingSpriteTemplate.paletteTag = TAG_NONE;
    LoadPalette(trainerSprite->palette, OBJ_PLTT_ID(paletteSlot), PLTT_SIZE_4BPP);
    spriteId = CreateSprite(&sCreatingSpriteTemplate, x, y, 0);
    gSprites[spriteId].oam.paletteNum = paletteSlot;
    sSpritePics[i].frames = framePics;
    sSpritePics[i].images = images;
    sSpritePics[i].paletteTag = TAG_NONE;
    sSpritePics[i].spriteId = spriteId;
    sSpritePics[i].active = TRUE;
    return spriteId;
}

u16 FreeAndDestroyPicSprite(u16 spriteId)
{
    u32 i;
    u8 *framePics;
    struct SpriteFrameImage *images;

    for (i = 0; i < PICS_COUNT; i ++)
    {
        if (sSpritePics[i].spriteId == spriteId)
            break;
    }
    if (i == PICS_COUNT)
        return 0xFFFF;

    framePics = sSpritePics[i].frames;
    images = sSpritePics[i].images;
    if (sSpritePics[i].paletteTag != TAG_NONE)
        FreeSpritePaletteByTag(GetSpritePaletteTagByPaletteNum(gSprites[spriteId].oam.paletteNum));
    DestroySprite(&gSprites[spriteId]);
    Free(framePics);
    Free(images);
    sSpritePics[i] = sDummyPicData;
    return 0;
}

u16 CreateTrainerCardTrainerPicSprite(u16 trainerPicId, u16 destX, u16 destY)
{
    u8 *framePics;

    framePics = Alloc(TRAINER_PIC_SIZE);
    if (framePics)
    {
        const struct TrainerSprite *trainerSprite = &gTrainerSpriteTable[trainerPicId];
        LZ77UnCompWram(trainerSprite->sprite.data, framePics);
        BlitBitmapRectToWindow(WIN_TRAINER_PIC, framePics, 0, 0, TRAINER_PIC_WIDTH, TRAINER_PIC_HEIGHT, destX, destY, TRAINER_PIC_WIDTH, TRAINER_PIC_HEIGHT);
        LoadPalette(trainerSprite->palette, PLTT_ID(8), PLTT_SIZE_4BPP);
        Free(framePics);
        return 0;
    }
    return 0xFFFF;
}

u16 GetPlayerFrontTrainerPicId(u8 version, u8 gender)
{
    u16 trainerPicId = TRAINER_PIC_BRENDAN;
    switch (GetVersionId(version))
    {
    case 1:
        trainerPicId = TRAINER_PIC_RS_BRENDAN;
        break;
    case 2:
        trainerPicId = TRAINER_PIC_RG_RED;
        break;
    }
    return trainerPicId + gender;
}

u8 GetPlayerBackTrainerPicId(u8 version, u8 gender)
{
    switch (GetVersionId(version))
    {
    case 1:
        gender += TRAINER_BACK_PIC_RUBY_SAPPHIRE_BRENDAN;
        break;
    case 2:
        gender += TRAINER_BACK_PIC_RG_RED;
        break;
    }
    return gender;
}
