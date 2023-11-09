#include "global.h"
#include "event_object_lock.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "faraway_island.h"
#include "field_camera.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "fldeff.h"
#include "malloc.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "party_menu.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "trig.h"
#include "constants/abilities.h"
#include "constants/event_objects.h"
#include "constants/field_effects.h"
#include "constants/songs.h"
#include "constants/metatile_labels.h"
#include "constants/metatile_behaviors.h"

extern struct MapPosition gPlayerFacingPosition;

extern const u8 FarawayIsland_Interior_EventScript_HideMewWhenGrassCut[];

extern const u8 gFieldEffectPic_CutGrass[];
extern const u16 gFieldEffectObjectPalette3[];

// cut 'square' defines
#define CUT_NORMAL_SIDE 3
#define CUT_HYPER_SIDE 5

#define CUT_SPRITE_ARRAY_COUNT 8

#define CUT_GRASS_BOTTOM 0
#define CUT_GRASS_TOP 1

// this file's functions
static void FieldCallback_CutTree(void);
static void FieldCallback_CutGrass(void);
static void StartCutTreeFieldEffect(void);
static void StartCutGrassFieldEffect(void);
static void SetCutGrassMetatile(s32, s32);
static void CutGrassSpriteCallback1(struct Sprite *);
static void CutGrassSpriteCallback2(struct Sprite *);
static void CutGrassSpriteCallbackEnd(struct Sprite *);

// EWRAM variables
static EWRAM_DATA u8 *sCutGrassSpriteArrayPtr = NULL;

static const u16 sCutGrassMetatileMapping[][2] = {
    {
        [CUT_GRASS_BOTTOM] = METATILE_Fortree_LongGrass_Root,
        [CUT_GRASS_TOP]    = METATILE_General_Grass
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_General_LongGrass,
        [CUT_GRASS_TOP]    = METATILE_General_Grass
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_General_LongGrassCovered,
        [CUT_GRASS_TOP]    = METATILE_General_Grass
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_General_TallGrass,
        [CUT_GRASS_TOP]    = METATILE_General_Grass
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_General_TallGrass_TreeLeft,
        [CUT_GRASS_TOP]    = METATILE_General_Grass_TreeLeft
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_General_TallGrass_TreeRight,
        [CUT_GRASS_TOP]    = METATILE_General_Grass_TreeRight
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_Fortree_SecretBase_LongGrass_BottomLeft,
        [CUT_GRASS_TOP]    = METATILE_Fortree_SecretBase_LongGrass_TopLeft
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_Fortree_SecretBase_LongGrass_BottomMid,
        [CUT_GRASS_TOP]    = METATILE_Fortree_SecretBase_LongGrass_TopMid
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_Fortree_SecretBase_LongGrass_BottomRight,
        [CUT_GRASS_TOP]    = METATILE_Fortree_SecretBase_LongGrass_TopRight
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_Lavaridge_NormalGrass,
        [CUT_GRASS_TOP]    = METATILE_Lavaridge_LavaField
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_Lavaridge_AshGrass,
        [CUT_GRASS_TOP]    = METATILE_Lavaridge_LavaField
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_Fallarbor_AshGrass,
        [CUT_GRASS_TOP]    = METATILE_Fallarbor_AshField
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_General_TallGrass_TreeUp,
        [CUT_GRASS_TOP]    = METATILE_General_Grass_TreeUp
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_RG_General_Plain_Grass,
        [CUT_GRASS_TOP]    = METATILE_RG_General_Plain_Mowed
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_RG_General_ThinTreeTop_Grass,
        [CUT_GRASS_TOP]    = METATILE_RG_General_ThinTreeTop_Mowed
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_RG_General_WideTreeTopLeft_Grass,
        [CUT_GRASS_TOP]    = METATILE_RG_General_WideTreeTopLeft_Mowed
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_RG_General_WideTreeTopRight_Grass,
        [CUT_GRASS_TOP]    = METATILE_RG_General_WideTreeTopRight_Mowed
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_RG_CeladonCity_CyclingRoad_Grass,
        [CUT_GRASS_TOP]    = METATILE_RG_CeladonCity_CyclingRoad_Mowed
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_RG_FuchsiaCity_SafariZoneTreeTopLeft_Grass,
        [CUT_GRASS_TOP]    = METATILE_RG_FuchsiaCity_SafariZoneTreeTopLeft_Mowed
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_RG_FuchsiaCity_SafariZoneTreeTopMiddle_Grass,
        [CUT_GRASS_TOP]    = METATILE_RG_FuchsiaCity_SafariZoneTreeTopMiddle_Mowed
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_RG_FuchsiaCity_SafariZoneTreeTopRight_Grass,
        [CUT_GRASS_TOP]    = METATILE_RG_FuchsiaCity_SafariZoneTreeTopRight_Mowed
    }, {
        [CUT_GRASS_BOTTOM] = METATILE_RG_ViridianForest_HugeTreeTopMiddle_Grass,
        [CUT_GRASS_TOP]    = METATILE_RG_ViridianForest_HugeTreeTopMiddle_Mowed
    }, {
        [CUT_GRASS_BOTTOM] = 0xffff,
        [CUT_GRASS_TOP]    = 0xffff
    }
};

static const struct OamData sOamData_CutGrass =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 1,
    .priority = 1,
    .paletteNum = 1,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_CutGrass[] =
{
    ANIMCMD_FRAME(0, 30),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sSpriteAnimTable_CutGrass[] =
{
    sSpriteAnim_CutGrass,
};

static const struct SpriteFrameImage sSpriteImageTable_CutGrass[] =
{
    {gFieldEffectPic_CutGrass, 0x20},
};

const struct SpritePalette gSpritePalette_GeneralFieldEffect3 = {gFieldEffectObjectPalette3, FLDEFF_PAL_TAG_GENERAL_3};

static const struct SpriteTemplate sSpriteTemplate_CutGrass =
{
    .tileTag = TAG_NONE,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_3,
    .oam = &sOamData_CutGrass,
    .anims = sSpriteAnimTable_CutGrass,
    .images = sSpriteImageTable_CutGrass,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = CutGrassSpriteCallback1,
};

// code
bool8 SetUpFieldMove_Cut(void)
{
    s32 x, y;
    u32 i, j;

    if (CheckObjectGraphicsInFrontOfPlayer(OBJ_EVENT_GFX_CUTTABLE_TREE) == TRUE)
    {
        // Standing in front of cuttable tree.
        gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
        gPostMenuFieldCallback = FieldCallback_CutTree;
        return TRUE;
    }
    else
    {
        PlayerGetDestCoords(&gPlayerFacingPosition.x, &gPlayerFacingPosition.y);
        for (i = 0; i < CUT_NORMAL_SIDE; i++)
        {
            y = gPlayerFacingPosition.y - 1 + i;
            for (j = 0; j < CUT_NORMAL_SIDE; j++)
            {
                x = gPlayerFacingPosition.x - 1 + j;
                if (MapGridGetElevationAt(x, y) == gPlayerFacingPosition.elevation)
                {
                    u32 tileBehavior = MapGridGetMetatileBehaviorAt(x, y);
                    if (MetatileBehavior_IsPokeGrass(tileBehavior)
                    || MetatileBehavior_IsAshGrass(tileBehavior))
                    {
                        // Standing in front of grass.
                        gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
                        gPostMenuFieldCallback = FieldCallback_CutGrass;
                        return TRUE;
                    }
                }
            }
        }

        return FALSE;
    }
}

static void FieldCallback_CutGrass(void)
{
    FieldEffectStart(FLDEFF_USE_CUT_ON_GRASS);
    gFieldEffectArguments[0] = GetCursorSelectionMonId();
}

bool8 FldEff_UseCutOnGrass(void)
{
    u8 taskId = CreateFieldMoveTask();

    gTasks[taskId].data[8] = (u32)StartCutGrassFieldEffect >> 16;
    gTasks[taskId].data[9] = (u32)StartCutGrassFieldEffect;
    IncrementGameStat(GAME_STAT_USED_CUT);
    return FALSE;
}

static void FieldCallback_CutTree(void)
{
    gFieldEffectArguments[0] = GetCursorSelectionMonId();
    ScriptContext_SetupScript(EventScript_UseCut);
}

bool8 FldEff_UseCutOnTree(void)
{
    u8 taskId = CreateFieldMoveTask();

    gTasks[taskId].data[8] = (u32)StartCutTreeFieldEffect >> 16;
    gTasks[taskId].data[9] = (u32)StartCutTreeFieldEffect;
    IncrementGameStat(GAME_STAT_USED_CUT);
    return FALSE;
}

static void StartCutGrassFieldEffect(void)
{
    FieldEffectActiveListRemove(FLDEFF_USE_CUT_ON_GRASS);
    FieldEffectStart(FLDEFF_CUT_GRASS);
}

bool8 FldEff_CutGrass(void)
{
    u32 i, j, cutRange, userAbility, tileCountFromPlayer;
    s32 x, y, lowerY;

    userAbility = GetMonAbility(&gPlayerParty[GetCursorSelectionMonId()]);
    if (userAbility == ABILITY_HYPER_CUTTER)
    {
        cutRange = CUT_HYPER_SIDE;
        tileCountFromPlayer = 2;
    }
    else
    {
        cutRange = CUT_NORMAL_SIDE;
        tileCountFromPlayer = 1;
    }

    PlaySE(SE_M_CUT);
    PlayerGetDestCoords(&gPlayerFacingPosition.x, &gPlayerFacingPosition.y);
    for (i = 0; i < cutRange; i++)
    {
        y = gPlayerFacingPosition.y - 1 + i;
        if (cutRange == CUT_HYPER_SIDE)
            y -= 1;

        for (j = 0; j < cutRange; j++)
        {
            x = gPlayerFacingPosition.x - 1 + j;
            if (cutRange == CUT_HYPER_SIDE)
                x -= 1;

            if (MapGridGetElevationAt(x, y) == gPlayerFacingPosition.elevation)
            {
                if (TestMetatileAttributeBit(MapGridGetMetatileAttributeAt(x, y, METATILE_ATTRIBUTE_TERRAIN), TILE_TERRAIN_GRASS))
                {
                    SetCutGrassMetatile(x, y);
                    AllowObjectAtPosTriggerGroundEffects(x, y);
                }
            }
        }
    }

    y = gPlayerFacingPosition.y - (1 + tileCountFromPlayer);
    lowerY = y + cutRange;
    for (i = 0; i < cutRange; i++)
    {
        s32 currentX = gPlayerFacingPosition.x - tileCountFromPlayer + i;
        FixLongGrassMetatilesWindowTop(currentX, y);
        FixLongGrassMetatilesWindowBottom(currentX, lowerY);
    }

    DrawWholeMapView();
    sCutGrassSpriteArrayPtr = AllocZeroed(CUT_SPRITE_ARRAY_COUNT);

    // populate sprite ID array
    for (i = 0; i < CUT_SPRITE_ARRAY_COUNT; i++)
    {
        sCutGrassSpriteArrayPtr[i] = CreateSprite(&sSpriteTemplate_CutGrass,
        gSprites[gPlayerAvatar.spriteId].oam.x + 8, gSprites[gPlayerAvatar.spriteId].oam.y + 20, 0);
        gSprites[sCutGrassSpriteArrayPtr[i]].data[2] = 32 * i;
    }

    return FALSE;
}

// set map grid metatile depending on x, y
static void SetCutGrassMetatile(s32 x, s32 y)
{
    u32 i = 0;
    u16 metatileId = MapGridGetMetatileIdAt(x, y);
    while (1)
    {
        const u16 *metatileMapping = sCutGrassMetatileMapping[i];
        if (metatileMapping[CUT_GRASS_BOTTOM] != 0xFFFF)
        {
            if (metatileMapping[CUT_GRASS_BOTTOM] == metatileId)
            {
                MapGridSetMetatileIdAt(x, y, metatileMapping[CUT_GRASS_TOP]);
                break;
            }
            i++;
        }
    }
}

static void CutGrassSpriteCallback1(struct Sprite *sprite)
{
    sprite->data[0] = 8;
    sprite->data[1] = 0;
    sprite->data[3] = 0;
    sprite->callback = CutGrassSpriteCallback2;
}

static void CutGrassSpriteCallback2(struct Sprite *sprite)
{
    sprite->x2 = Sin(sprite->data[2], sprite->data[0]);
    sprite->y2 = Cos(sprite->data[2], sprite->data[0]);

    sprite->data[2] = (sprite->data[2] + 8) & 0xFF;
    sprite->data[0] += 1 + (sprite->data[3] >> 2); // right shift by 2 is dividing by 4
    sprite->data[3]++;

    if (sprite->data[1] != 28)
        sprite->data[1]++;
    else
        sprite->callback = CutGrassSpriteCallbackEnd; // done rotating the grass, execute clean up function
}

static void CutGrassSpriteCallbackEnd(struct Sprite *sprite)
{
    u32 i;

    for (i = 1; i < CUT_SPRITE_ARRAY_COUNT; i++)
        DestroySprite(&gSprites[sCutGrassSpriteArrayPtr[i]]);

    FieldEffectStop(&gSprites[sCutGrassSpriteArrayPtr[0]], FLDEFF_CUT_GRASS);
    FREE_AND_SET_NULL(sCutGrassSpriteArrayPtr);
    ScriptUnfreezeObjectEvents();
    UnlockPlayerFieldControls();

    if (IsMewPlayingHideAndSeek() == TRUE)
        ScriptContext_SetupScript(FarawayIsland_Interior_EventScript_HideMewWhenGrassCut);
}

// get map grid metatile depending on x, y
static u32 GetCutGrassMetatile(s32 x, s32 y, bool32 isTop)
{   
    u32 i;
    u32 metatileId = MapGridGetMetatileIdAt(x, y);
    for (i = 0; sCutGrassMetatileMapping[i][CUT_GRASS_BOTTOM] != 0xFFFF; i++)
    {
        const u16 *metatileMapping = sCutGrassMetatileMapping[i];
        if (metatileMapping[!isTop] == metatileId)
            return metatileMapping[isTop];
    }
    return metatileId;
}

void FixLongGrassMetatilesWindowTop(s32 x, s32 y)
{
    u32 metatileBehavior = MapGridGetMetatileBehaviorAt(x, y);
    if (MetatileBehavior_IsLongGrass(metatileBehavior))
        MapGridSetMetatileIdAt(x, y + 1, GetCutGrassMetatile(x, y + 1, CUT_GRASS_BOTTOM));
}

void FixLongGrassMetatilesWindowBottom(s32 x, s32 y)
{
    if (MapGridGetMetatileIdAt(x, y) == GetCutGrassMetatile(x, y, CUT_GRASS_TOP))
    {
        u32 metatileBehavior = MapGridGetMetatileBehaviorAt(x, y + 1);
        if (metatileBehavior == MB_LONG_GRASS_SOUTH_EDGE)
            SetCutGrassMetatile(x, y + 1);
        else if (metatileBehavior == MB_LONG_GRASS_COVERED)
            MapGridSetMetatileIdAt(x, y + 1, METATILE_General_LongGrass);
    }
}

static void StartCutTreeFieldEffect(void)
{
    PlaySE(SE_M_CUT);
    FieldEffectActiveListRemove(FLDEFF_USE_CUT_ON_TREE);
    ScriptContext_Enable();
}
