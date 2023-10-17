#include "global.h"
#include "field_camera.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "fldeff.h"
#include "task.h"
#include "constants/metatile_labels.h"
#include "tilesets.h"

static EWRAM_DATA u8 sEscalatorAnim_TaskId = 0;

static void SetEscalatorMetatile(u8 taskId, const s16 *metatileIds, u16 metatileMasks);
static void Task_DrawEscalator(u8 taskId);

#define ESCALATOR_STAGES     3
#define LAST_ESCALATOR_STAGE (ESCALATOR_STAGES - 1)

struct EscalatorMetatileMapping
{
    const struct Tileset *tileset;
    s16 metatileIds[ESCALATOR_STAGES];
};

static const struct EscalatorMetatileMapping sEscalatorMetatiles_1F_0[2] = {
    {
        .tileset = &gTileset_PokemonCenter,
        .metatileIds =
        {
            METATILE_PokemonCenter_Escalator1F_Tile0_Frame2,
            METATILE_PokemonCenter_Escalator1F_Tile0_Frame1,
            METATILE_PokemonCenter_Escalator1F_Tile0_Frame0
        },
    },
    {
        .tileset = &gTileset_RG_PokemonCenter,
        .metatileIds =
        {
            METATILE_RG_PokemonCenter_Escalator_BottomNextRail_Transition2, 
            METATILE_RG_PokemonCenter_Escalator_BottomNextRail_Transition1, 
            METATILE_RG_PokemonCenter_Escalator_BottomNextRail_Normal
        },
    }
};

static const struct EscalatorMetatileMapping sEscalatorMetatiles_1F_1[2] = {
    {
        .tileset = &gTileset_PokemonCenter,
        .metatileIds =
        {
            METATILE_PokemonCenter_Escalator1F_Tile1_Frame2,
            METATILE_PokemonCenter_Escalator1F_Tile1_Frame1,
            METATILE_PokemonCenter_Escalator1F_Tile1_Frame0
        },
    },
    {
        .tileset = &gTileset_RG_PokemonCenter,
        .metatileIds =
        {
            METATILE_RG_PokemonCenter_Escalator_BottomRail_Transition2, 
            METATILE_RG_PokemonCenter_Escalator_BottomRail_Transition1, 
            METATILE_RG_PokemonCenter_Escalator_BottomRail_Normal
        },
    }
};

static const struct EscalatorMetatileMapping sEscalatorMetatiles_1F_2[2] = {
    {
        .tileset = &gTileset_PokemonCenter,
        .metatileIds =
        {
            METATILE_PokemonCenter_Escalator1F_Tile2_Frame2,
            METATILE_PokemonCenter_Escalator1F_Tile2_Frame1,
            METATILE_PokemonCenter_Escalator1F_Tile2_Frame0
        },
    },
    {
        .tileset = &gTileset_RG_PokemonCenter,
        .metatileIds =
        {
            METATILE_RG_PokemonCenter_Escalator_BottomNext_Transition2, 
            METATILE_RG_PokemonCenter_Escalator_BottomNext_Transition1, 
            METATILE_RG_PokemonCenter_Escalator_BottomNext_Normal
        },
    }
};

static const struct EscalatorMetatileMapping sEscalatorMetatiles_1F_3[2] = {
    {
        .tileset = &gTileset_PokemonCenter,
        .metatileIds =
        {
            METATILE_PokemonCenter_Escalator1F_Tile3_Frame2,
            METATILE_PokemonCenter_Escalator1F_Tile3_Frame1,
            METATILE_PokemonCenter_Escalator1F_Tile3_Frame0
        },
    },
    {
        .tileset = &gTileset_RG_PokemonCenter,
        .metatileIds =
        {
            METATILE_RG_PokemonCenter_Escalator_Bottom_Transition2, 
            METATILE_RG_PokemonCenter_Escalator_Bottom_Transition1, 
            METATILE_RG_PokemonCenter_Escalator_Bottom_Normal
        },
    }
};

static const struct EscalatorMetatileMapping sEscalatorMetatiles_2F_0[2] = {
    {
        .tileset = &gTileset_PokemonCenter,
        .metatileIds =
        {
            METATILE_PokemonCenter_Escalator2F_Tile0_Frame0,
            METATILE_PokemonCenter_Escalator2F_Tile0_Frame1,
            METATILE_PokemonCenter_Escalator2F_Tile0_Frame2
        },
    },
    {
        .tileset = &gTileset_RG_PokemonCenter,
        .metatileIds =
        {
            METATILE_RG_PokemonCenter_Escalator_TopNext_Normal, 
            METATILE_RG_PokemonCenter_Escalator_TopNext_Transition1, 
            METATILE_RG_PokemonCenter_Escalator_TopNext_Transition2
        },
    }
};

static const struct EscalatorMetatileMapping sEscalatorMetatiles_2F_1[2] = {
    {
        .tileset = &gTileset_PokemonCenter,
        .metatileIds =
        {
            METATILE_PokemonCenter_Escalator2F_Tile1_Frame0,
            METATILE_PokemonCenter_Escalator2F_Tile1_Frame1,
            METATILE_PokemonCenter_Escalator2F_Tile1_Frame2
        },
    },
    {
        .tileset = &gTileset_RG_PokemonCenter,
        .metatileIds =
        {
            METATILE_RG_PokemonCenter_Escalator_Top_Normal, 
            METATILE_RG_PokemonCenter_Escalator_Top_Transition1, 
            METATILE_RG_PokemonCenter_Escalator_Top_Transition2
        },
    }
};

static const struct EscalatorMetatileMapping sEscalatorMetatiles_2F_2[2] = {
    {
        .tileset = &gTileset_PokemonCenter,
        .metatileIds =
        {
            METATILE_PokemonCenter_Escalator2F_Tile2_Frame0,
            METATILE_PokemonCenter_Escalator2F_Tile2_Frame1,
            METATILE_PokemonCenter_Escalator2F_Tile2_Frame2
        },
    },
    {
        .tileset = &gTileset_RG_PokemonCenter,
        .metatileIds =
        {
            METATILE_RG_PokemonCenter_Escalator_TopNextRail_Normal, 
            METATILE_RG_PokemonCenter_Escalator_TopNextRail_Transition1, 
            METATILE_RG_PokemonCenter_Escalator_TopNextRail_Transition2
        },
    }
};

#define tState            data[0]
#define tTransitionStage  data[1]
#define tGoingUp          data[2]
#define tDrawingEscalator data[3]
#define tPlayerX          data[4]
#define tPlayerY          data[5]

static void SetEscalatorMetatile(u8 taskId, const s16 *metatileIds, u16 metatileMasks)
{
    s16 x = gTasks[taskId].tPlayerX - 1;
    s16 y = gTasks[taskId].tPlayerY - 1;
    s16 transitionStage = gTasks[taskId].tTransitionStage;
    s16 i;
    s16 j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            s32 tileX = x + j, tileY = y + i;
            s32 metatileId = MapGridGetMetatileIdAt(tileX, tileY);
            u32 nextStage = transitionStage + 1;
            u32 lastStage = 0;
            u32 currentStage = transitionStage;

            // Check all the escalator sections and only progress the selected one to the next stage
            if (gTasks[taskId].tGoingUp)
            {
                nextStage = 1 - transitionStage;
                lastStage = LAST_ESCALATOR_STAGE;
                currentStage = LAST_ESCALATOR_STAGE - transitionStage;
            }

            if (metatileIds[currentStage] == metatileId)
            {
                if (transitionStage != LAST_ESCALATOR_STAGE)
                    MapGridSetMetatileIdAt(tileX, tileY, metatileMasks | metatileIds[nextStage]);
                else
                    MapGridSetMetatileIdAt(tileX, tileY, metatileMasks | metatileIds[lastStage]);
            }
        }
    }
}

static const s16 *GetEscalatorMetatile(const struct EscalatorMetatileMapping *arr)
{
    u32 i;

    for (i = 0; i < ESCALATOR_STAGES; i++)
    {
        if (gMapHeader.mapLayout->secondaryTileset == arr[i].tileset)
            return arr[i].metatileIds;
    }
    return NULL;
}

static void Task_DrawEscalator(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    const s16 *metatileIds;

    tDrawingEscalator = TRUE;

    // Set tile for each section of the escalator in sequence for current transition stage
    switch (tState)
    {
        case 0:
            metatileIds = GetEscalatorMetatile(sEscalatorMetatiles_1F_0);
            SetEscalatorMetatile(taskId, metatileIds, 0);
            break;
        case 1:
            metatileIds = GetEscalatorMetatile(sEscalatorMetatiles_1F_1);
            SetEscalatorMetatile(taskId, metatileIds, 0);
            break;
        case 2:
            metatileIds = GetEscalatorMetatile(sEscalatorMetatiles_1F_2);
            SetEscalatorMetatile(taskId, metatileIds, MAPGRID_COLLISION_MASK);
            break;
        case 3:
            metatileIds = GetEscalatorMetatile(sEscalatorMetatiles_1F_3);
            SetEscalatorMetatile(taskId, metatileIds, 0);
            break;
        case 4:
            metatileIds = GetEscalatorMetatile(sEscalatorMetatiles_2F_0);
            SetEscalatorMetatile(taskId, metatileIds, MAPGRID_COLLISION_MASK);
            break;
        case 5:
            metatileIds = GetEscalatorMetatile(sEscalatorMetatiles_2F_1);
            SetEscalatorMetatile(taskId, metatileIds, 0);
            break;
        case 6:
            metatileIds = GetEscalatorMetatile(sEscalatorMetatiles_2F_2);
            SetEscalatorMetatile(taskId, metatileIds, 0);
            break;
    }

    tState = (tState + 1) & 7;

    // If all metatiles of the escalator have been set, draw map and progress to next stage
    if (tState == 0)
    {
        DrawWholeMapView();
        tTransitionStage = (tTransitionStage + 1) % ESCALATOR_STAGES;
        tDrawingEscalator = FALSE;
    }
}

static u8 CreateEscalatorTask(bool16 goingUp)
{
    u8 taskId = CreateTask(Task_DrawEscalator, 0);
    s16 *data = gTasks[taskId].data;

    PlayerGetDestCoords(&tPlayerX, &tPlayerY);
    tState = 0;
    tTransitionStage = 0;
    tGoingUp = goingUp;
    Task_DrawEscalator(taskId);
    return taskId;
}

void StartEscalator(bool8 goingUp)
{
    sEscalatorAnim_TaskId = CreateEscalatorTask(goingUp);
}

void StopEscalator(void)
{
    DestroyTask(sEscalatorAnim_TaskId);
}

bool8 IsEscalatorMoving(void)
{
    if (gTasks[sEscalatorAnim_TaskId].tDrawingEscalator == FALSE
     && gTasks[sEscalatorAnim_TaskId].tTransitionStage == LAST_ESCALATOR_STAGE)
        return FALSE;
    else
        return TRUE;
}

#undef tState
#undef tTransitionStage
#undef tGoingUp
#undef tDrawingEscalator
#undef tPlayerX
#undef tPlayerY
