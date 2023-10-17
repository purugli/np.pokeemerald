#include "global.h"
#include "event_data.h"
#include "field_door.h"
#include "field_camera.h"
#include "fieldmap.h"
#include "metatile_behavior.h"
#include "task.h"
#include "constants/songs.h"
#include "constants/metatile_labels.h"
#include "tilesets.h"

#define DOOR_SOUND_NORMAL  0
#define DOOR_SOUND_SLIDING 1
#define DOOR_SOUND_ARENA   2
#define DOOR_SOUND_FRLG    3

enum {
    DOOR_SIZE_1x1,
    DOOR_SIZE_1x2,
    DOOR_SIZE_2x2,
};

struct DoorGraphics
{
    u16 metatileNum;
    u8 sound;
    u8 size;
    const void *tiles;
    const void *palettes;
    const struct Tileset *tileset;
};

struct DoorAnimFrame
{
    u8 time;
    u16 offset;
};

static bool8 ShouldUseMultiCorridorDoor(void);

static const u8 sDoorAnimTiles_Littleroot[] = INCBIN_U8("graphics/door_anims/littleroot.4bpp");
static const u8 sDoorAnimTiles_BirchsLab[] = INCBIN_U8("graphics/door_anims/birchs_lab.4bpp");
static const u8 sDoorAnimTiles_FallarborLightRoof[] = INCBIN_U8("graphics/door_anims/fallarbor_light_roof.4bpp");
static const u8 sDoorAnimTiles_Lilycove[] = INCBIN_U8("graphics/door_anims/lilycove.4bpp");
static const u8 sDoorAnimTiles_LilycoveWooden[] = INCBIN_U8("graphics/door_anims/lilycove_wooden.4bpp");
static const u8 sDoorAnimTiles_General[] = INCBIN_U8("graphics/door_anims/general.4bpp");
static const u8 sDoorAnimTiles_PokeCenter[] = INCBIN_U8("graphics/door_anims/poke_center.4bpp");
static const u8 sDoorAnimTiles_Gym[] = INCBIN_U8("graphics/door_anims/gym.4bpp");
static const u8 sDoorAnimTiles_PokeMart[] = INCBIN_U8("graphics/door_anims/poke_mart.4bpp");
static const u8 sDoorAnimTiles_RustboroTan[] = INCBIN_U8("graphics/door_anims/rustboro_tan.4bpp");
static const u8 sDoorAnimTiles_RustboroGray[] = INCBIN_U8("graphics/door_anims/rustboro_gray.4bpp");
static const u8 sDoorAnimTiles_Oldale[] = INCBIN_U8("graphics/door_anims/oldale.4bpp");
static const u8 sDoorAnimTiles_Mauville[] = INCBIN_U8("graphics/door_anims/mauville.4bpp");
static const u8 sDoorAnimTiles_Verdanturf[] = INCBIN_U8("graphics/door_anims/verdanturf.4bpp");
static const u8 sDoorAnimTiles_Slateport[] = INCBIN_U8("graphics/door_anims/slateport.4bpp");
static const u8 sDoorAnimTiles_Dewford[] = INCBIN_U8("graphics/door_anims/dewford.4bpp");
static const u8 sDoorAnimTiles_Contest[] = INCBIN_U8("graphics/door_anims/contest.4bpp");
static const u8 sDoorAnimTiles_Mossdeep[] = INCBIN_U8("graphics/door_anims/mossdeep.4bpp");
static const u8 sDoorAnimTiles_SootopolisPeakedRoof[] = INCBIN_U8("graphics/door_anims/sootopolis_peaked_roof.4bpp");
static const u8 sDoorAnimTiles_Sootopolis[] = INCBIN_U8("graphics/door_anims/sootopolis.4bpp");
static const u8 sDoorAnimTiles_PokemonLeague[] = INCBIN_U8("graphics/door_anims/pokemon_league.4bpp");
static const u8 sDoorAnimTiles_Pacifidlog[] = INCBIN_U8("graphics/door_anims/pacifidlog.4bpp");
static const u8 sDoorAnimTiles_PetalburgGym[] = INCBIN_U8("graphics/door_anims/petalburg_gym.4bpp");
static const u8 sDoorAnimTiles_CyclingRoad[] = INCBIN_U8("graphics/door_anims/cycling_road.4bpp");
static const u8 sDoorAnimTiles_LilycoveDeptStore[] = INCBIN_U8("graphics/door_anims/lilycove_dept_store.4bpp");
static const u8 sDoorAnimTiles_SafariZone[] = INCBIN_U8("graphics/door_anims/safari_zone.4bpp");
static const u8 sDoorAnimTiles_MossdeepSpaceCenter[] = INCBIN_U8("graphics/door_anims/mossdeep_space_center.4bpp");
static const u8 sDoorAnimTiles_CableClub[] = INCBIN_U8("graphics/door_anims/cable_club.4bpp");
static const u8 sDoorAnimTiles_AbandonedShip[] = INCBIN_U8("graphics/door_anims/abandoned_ship.4bpp");
static const u8 sDoorAnimTiles_FallarborDarkRoof[] = INCBIN_U8("graphics/door_anims/fallarbor_dark_roof.4bpp");
static const u8 sDoorAnimTiles_AbandonedShipRoom[] = INCBIN_U8("graphics/door_anims/abandoned_ship_room.4bpp");
static const u8 sDoorAnimTiles_LilycoveDeptStoreElevator[] = INCBIN_U8("graphics/door_anims/lilycove_dept_store_elevator.4bpp");
static const u8 sDoorAnimTiles_RG_General[] = INCBIN_U8("graphics/door_anims/frlg/general.4bpp");
static const u8 sDoorAnimTiles_BattleTowerElevator[] = INCBIN_U8("graphics/door_anims/battle_tower_elevator.4bpp");
static const u8 sDoorAnimTiles_BattleDome[] = INCBIN_U8("graphics/door_anims/battle_dome.4bpp");
static const u8 sDoorAnimTiles_BattleFactory[] = INCBIN_U8("graphics/door_anims/battle_factory.4bpp");
static const u8 sDoorAnimTiles_BattleTower[] = INCBIN_U8("graphics/door_anims/battle_tower.4bpp");
static const u8 sDoorAnimTiles_BattleArena[] = INCBIN_U8("graphics/door_anims/battle_arena.4bpp");
static const u8 sDoorAnimTiles_BattleArenaLobby[] = INCBIN_U8("graphics/door_anims/battle_arena_lobby.4bpp");
static const u8 sDoorAnimTiles_BattleDomeLobby[] = INCBIN_U8("graphics/door_anims/battle_dome_lobby.4bpp");
static const u8 sDoorAnimTiles_BattlePalaceLobby[] = INCBIN_U8("graphics/door_anims/battle_palace_lobby.4bpp");
static const u8 sDoorAnimTiles_BattleTent[] = INCBIN_U8("graphics/door_anims/battle_tent.4bpp");
static const u8 sDoorAnimTiles_BattleDomeCorridor[] = INCBIN_U8("graphics/door_anims/battle_dome_corridor.4bpp");
static const u8 sDoorAnimTiles_BattleTowerMultiCorridor[] = INCBIN_U8("graphics/door_anims/battle_tower_multi_corridor.4bpp");
static const u8 sDoorAnimTiles_BattleFrontier[] = INCBIN_U8("graphics/door_anims/battle_frontier.4bpp");
static const u8 sDoorAnimTiles_BattleFrontierSliding[] = INCBIN_U8("graphics/door_anims/battle_frontier_sliding.4bpp");
static const u8 sDoorAnimTiles_BattleDomePreBattleRoom[] = INCBIN_U8("graphics/door_anims/battle_dome_pre_battle_room.4bpp");
static const u8 sDoorAnimTiles_BattleTentInterior[] = INCBIN_U8("graphics/door_anims/battle_tent_interior.4bpp");
static const u8 sDoorAnimTiles_TrainerHillLobbyElevator[] = INCBIN_U8("graphics/door_anims/trainer_hill_lobby_elevator.4bpp");
static const u8 sDoorAnimTiles_TrainerHillRoofElevator[] = INCBIN_U8("graphics/door_anims/trainer_hill_roof_elevator.4bpp");
static const u8 sDoorAnimTiles_RG_SlidingSingle[] = INCBIN_U8("graphics/door_anims/frlg/sliding_single.4bpp");
static const u8 sDoorAnimTiles_RG_SlidingDouble[] = INCBIN_U8("graphics/door_anims/frlg/sliding_double.4bpp");
static const u8 sDoorAnimTiles_RG_Pallet[] = INCBIN_U8("graphics/door_anims/frlg/pallet.4bpp");
static const u8 sDoorAnimTiles_RG_OaksLab[] = INCBIN_U8("graphics/door_anims/frlg/oaks_lab.4bpp");
static const u8 sDoorAnimTiles_RG_Viridian[] = INCBIN_U8("graphics/door_anims/frlg/viridian.4bpp");
static const u8 sDoorAnimTiles_RG_Pewter[] = INCBIN_U8("graphics/door_anims/frlg/pewter.4bpp");
static const u8 sDoorAnimTiles_RG_Saffron[] = INCBIN_U8("graphics/door_anims/frlg/saffron.4bpp");
static const u8 sDoorAnimTiles_RG_SilphCo[] = INCBIN_U8("graphics/door_anims/frlg/silph_co.4bpp");
static const u8 sDoorAnimTiles_RG_Cerulean[] = INCBIN_U8("graphics/door_anims/frlg/cerulean.4bpp");
static const u8 sDoorAnimTiles_RG_Lavender[] = INCBIN_U8("graphics/door_anims/frlg/lavender.4bpp");
static const u8 sDoorAnimTiles_RG_Vermilion[] = INCBIN_U8("graphics/door_anims/frlg/vermilion.4bpp");
static const u8 sDoorAnimTiles_RG_DeptStore[] = INCBIN_U8("graphics/door_anims/frlg/dept_store.4bpp");
static const u8 sDoorAnimTiles_RG_Fuchsia[] = INCBIN_U8("graphics/door_anims/frlg/fuchsia.4bpp");
static const u8 sDoorAnimTiles_RG_SafariZone[] = INCBIN_U8("graphics/door_anims/frlg/safari_zone.4bpp");
static const u8 sDoorAnimTiles_RG_CinnabarLab[] = INCBIN_U8("graphics/door_anims/frlg/cinnabar_lab.4bpp");
static const u8 sDoorAnimTiles_RG_DeptStoreElevator[] = INCBIN_U8("graphics/door_anims/frlg/dept_store_elevator.4bpp");
static const u8 sDoorAnimTiles_RG_CableClub[] = INCBIN_U8("graphics/door_anims/frlg/cable_club.4bpp");
static const u8 sDoorAnimTiles_RG_HideoutElevator[] = INCBIN_U8("graphics/door_anims/frlg/hideout_elevator.4bpp");
static const u8 sDoorAnimTiles_RG_SSAnne[] = INCBIN_U8("graphics/door_anims/frlg/ss_anne.4bpp");
static const u8 sDoorAnimTiles_RG_SilphCoElevator[] = INCBIN_U8("graphics/door_anims/frlg/silph_co_elevator.4bpp");
static const u8 sDoorAnimTiles_RG_Sevii123[] = INCBIN_U8("graphics/door_anims/frlg/sevii_123.4bpp");
static const u8 sDoorAnimTiles_RG_JoyfulGameCorner[] = INCBIN_U8("graphics/door_anims/frlg/joyful_game_corner.4bpp");
static const u8 sDoorAnimTiles_RG_OneIslandPokeCenter[] = INCBIN_U8("graphics/door_anims/frlg/one_island_poke_center.4bpp");
static const u8 sDoorAnimTiles_RG_Sevii45[] = INCBIN_U8("graphics/door_anims/frlg/sevii_45.4bpp");
static const u8 sDoorAnimTiles_RG_FourIslandDayCare[] = INCBIN_U8("graphics/door_anims/frlg/four_island_day_care.4bpp");
static const u8 sDoorAnimTiles_RG_RocketWarehouse[] = INCBIN_U8("graphics/door_anims/frlg/rocket_warehouse.4bpp");
static const u8 sDoorAnimTiles_RG_Sevii67[] = INCBIN_U8("graphics/door_anims/frlg/sevii_67.4bpp");
static const u8 sDoorAnimTiles_RG_Teleporter[] = INCBIN_U8("graphics/door_anims/frlg/teleporter.4bpp");
static const u8 sDoorAnimTiles_RG_TrainerTowerLobbyElevator[] = INCBIN_U8("graphics/door_anims/frlg/trainer_tower_lobby_elevator.4bpp");
static const u8 sDoorAnimTiles_RG_TrainerTowerRoofElevator[] = INCBIN_U8("graphics/door_anims/frlg/trainer_tower_roof_elevator.4bpp");

#define CLOSED_DOOR_TILES_OFFSET 0xFFFF

static const struct DoorAnimFrame sDoorOpenAnimFrames[] =
{
    {4, CLOSED_DOOR_TILES_OFFSET},
    {4, 0 * TILE_SIZE_4BPP},
    {4, 8 * TILE_SIZE_4BPP},
    {4, 16 * TILE_SIZE_4BPP},
    {},
};

static const struct DoorAnimFrame sDoorCloseAnimFrames[] =
{
    {4, 16 * TILE_SIZE_4BPP},
    {4, 8 * TILE_SIZE_4BPP},
    {4, 0 * TILE_SIZE_4BPP},
    {4, CLOSED_DOOR_TILES_OFFSET},
    {},
};

static const struct DoorAnimFrame sBigDoorOpenAnimFrames[] =
{
    {4, CLOSED_DOOR_TILES_OFFSET},
    {4, 0 * TILE_SIZE_4BPP},
    {4, 16 * TILE_SIZE_4BPP},
    {4, 32 * TILE_SIZE_4BPP},
    {},
};

static const struct DoorAnimFrame sBigDoorCloseAnimFrames[] =
{
    {4, 32 * TILE_SIZE_4BPP},
    {4, 16 * TILE_SIZE_4BPP},
    {4, 0 * TILE_SIZE_4BPP},
    {4, CLOSED_DOOR_TILES_OFFSET},
    {},
};

static const struct DoorAnimFrame sSmallDoorOpenAnimFrames[] =
{
    {4, CLOSED_DOOR_TILES_OFFSET},
    {4, 0 * TILE_SIZE_4BPP},
    {4, 4 * TILE_SIZE_4BPP},
    {4, 8 * TILE_SIZE_4BPP},
    {},
};

static const struct DoorAnimFrame sSmallDoorCloseAnimFrames[] =
{
    {4, 8 * TILE_SIZE_4BPP},
    {4, 4 * TILE_SIZE_4BPP},
    {4, 0 * TILE_SIZE_4BPP},
    {4, CLOSED_DOOR_TILES_OFFSET},
    {},
};

static const u8 sDoorAnimPalettes_General[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_PokeCenter[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_Gym[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_PokeMart[] = {0, 0, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_Littleroot[] = {10, 10, 6, 6, 6, 6, 6, 6};
static const u8 sDoorAnimPalettes_BirchsLab[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_RustboroTan[] = {11, 11, 11, 11, 11, 11, 11, 11};
static const u8 sDoorAnimPalettes_RustboroGray[] = {10, 10, 10, 10, 10, 10, 10, 10};
static const u8 sDoorAnimPalettes_FallarborLightRoof[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_Lilycove[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_Oldale[] = {10, 10, 9, 9, 9, 9, 9, 9};
static const u8 sDoorAnimPalettes_Mossdeep[] = {9, 9, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_PokemonLeague[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_Pacifidlog[] = {9, 9, 9, 9, 9, 9, 9, 9};
static const u8 sDoorAnimPalettes_SootopolisPeakedRoof[] = {6, 6, 6, 6, 6, 6, 6, 6};
static const u8 sDoorAnimPalettes_Sootopolis[] = {6, 6, 6, 6, 6, 6, 6, 6};
static const u8 sDoorAnimPalettes_Dewford[] = {0, 0, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_Slateport[] = {6, 6, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_Mauville[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_Verdanturf[] = {6, 6, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_LilycoveWooden[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_Contest[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_PetalburgGym[] = {6, 6, 6, 6, 6, 6, 6, 6};
static const u8 sDoorAnimPalettes_CyclingRoad[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_LilycoveDeptStore[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_SafariZone[] = {9, 9, 9, 9, 9, 9, 9, 9};
static const u8 sDoorAnimPalettes_MossdeepSpaceCenter[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_CableClub[] = {6, 6, 6, 6, 6, 6, 6, 6};
static const u8 sDoorAnimPalettes_AbandonedShip[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_FallarborDarkRoof[] = {11, 11, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_AbandonedShipRoom[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_LilycoveDeptStoreElevator[] = {6, 6, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_RG_General[] = {2, 2, 2, 2, 2, 2, 2, 2};
static const u8 sDoorAnimPalettes_BattleTowerElevator[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattleDome[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_BattleFactory[] = {9, 9, 9, 9, 9, 9, 9, 9};
static const u8 sDoorAnimPalettes_BattleTower[] = {0, 0, 0, 0, 0, 0, 0, 0};
static const u8 sDoorAnimPalettes_BattleArena[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_BattleArenaLobby[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattleDomeLobby[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattlePalaceLobby[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattleTent[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_BattleDomeCorridor[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattleTowerMultiCorridor[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattleFrontier[] = {1, 1, 1, 1, 1, 1, 1, 1};
static const u8 sDoorAnimPalettes_BattleDomePreBattleRoom[] = {9, 9, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_BattleTentInterior[] = {9, 9, 9, 9, 9, 9, 9, 9};
static const u8 sDoorAnimPalettes_TrainerHillLobbyElevator[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_TrainerHillRoofElevator[] = {9, 9, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_RG_SlidingSingle[] = {3, 3, 3, 3, 3, 3, 3, 3};
static const u8 sDoorAnimPalettes_RG_SlidingDouble[] = {3, 3, 3, 3, 3, 3, 3, 3};
static const u8 sDoorAnimPalettes_RG_Pallet[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_RG_OaksLab[] = {10, 10, 10, 10, 10, 10, 10, 10};
static const u8 sDoorAnimPalettes_RG_Viridian[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_RG_Pewter[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_RG_Saffron[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_RG_SilphCo[] = {3, 3, 3, 3, 3, 3, 3, 3};
static const u8 sDoorAnimPalettes_RG_Cerulean[] = {12, 12, 12, 12, 12, 12, 12, 12};
static const u8 sDoorAnimPalettes_RG_Lavender[] = {9, 9, 9, 9, 9, 9, 9, 9};
static const u8 sDoorAnimPalettes_RG_Vermilion[] = {9, 9, 9, 9, 9, 9, 9, 9};
static const u8 sDoorAnimPalettes_RG_DeptStore[] = {3, 3, 3, 3, 3, 3, 3, 3};
static const u8 sDoorAnimPalettes_RG_Fuchsia[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_RG_SafariZone[] = {9, 9, 9, 9, 9, 9, 9, 9};
static const u8 sDoorAnimPalettes_RG_CinnabarLab[] = {3, 3, 3, 3, 3, 3, 3, 3};
static const u8 sDoorAnimPalettes_RG_DeptStoreElevator[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_RG_CableClub[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_RG_HideoutElevator[] = {12, 12, 2, 2, 2, 2, 2, 2};
static const u8 sDoorAnimPalettes_RG_SSAnne[] = {7, 7, 7, 7, 7, 7, 7, 7};
static const u8 sDoorAnimPalettes_RG_SilphCoElevator[] = {8, 8, 2, 2, 2, 2, 2, 2};
static const u8 sDoorAnimPalettes_RG_Sevii123[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_RG_JoyfulGameCorner[] = {3, 3, 3, 3, 3, 3, 3, 3};
static const u8 sDoorAnimPalettes_RG_OneIslandPokeCenter[] = {3, 3, 3, 3, 3, 3, 3, 3};
static const u8 sDoorAnimPalettes_RG_Sevii45[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_RG_FourIslandDayCare[] = {3, 3, 3, 3, 3, 3, 3, 3};
static const u8 sDoorAnimPalettes_RG_RocketWarehouse[] = {10, 10, 10, 10, 10, 10, 10, 10};
static const u8 sDoorAnimPalettes_RG_Sevii67[] = {5, 5, 5, 5, 5, 5, 5, 5};
static const u8 sDoorAnimPalettes_RG_Teleporter[] = {8, 8, 8, 8, 8, 8, 8, 8};
static const u8 sDoorAnimPalettes_RG_TrainerTowerLobbyElevator[] = {8, 8, 2, 2, 2, 2, 2, 2};
static const u8 sDoorAnimPalettes_RG_TrainerTowerRoofElevator[] = {11, 11, 2, 2, 2, 2, 2, 2};

static const struct DoorGraphics sDoorAnimGraphicsTable[] =
{
    {METATILE_General_Door,                                 DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_General, sDoorAnimPalettes_General, &gTileset_General},
    {METATILE_General_Door_PokeCenter,                      DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_PokeCenter, sDoorAnimPalettes_PokeCenter, &gTileset_General},
    {METATILE_General_Door_Gym,                             DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_Gym, sDoorAnimPalettes_Gym, &gTileset_General},
    {METATILE_General_Door_PokeMart,                        DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_PokeMart, sDoorAnimPalettes_PokeMart, &gTileset_General},
    {METATILE_Petalburg_Door_Littleroot,                    DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_Littleroot, sDoorAnimPalettes_Littleroot, &gTileset_Petalburg},
    {METATILE_Petalburg_Door_BirchsLab,                     DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_BirchsLab, sDoorAnimPalettes_BirchsLab, &gTileset_Petalburg},
    {METATILE_Rustboro_Door_Tan,                            DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_RustboroTan, sDoorAnimPalettes_RustboroTan, &gTileset_Rustboro},
    {METATILE_Rustboro_Door_Gray,                           DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_RustboroGray, sDoorAnimPalettes_RustboroGray, &gTileset_Rustboro},
    {METATILE_Fallarbor_Door_LightRoof,                     DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_FallarborLightRoof, sDoorAnimPalettes_FallarborLightRoof, &gTileset_Fallarbor},
    {METATILE_Petalburg_Door_Oldale,                        DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_Oldale, sDoorAnimPalettes_Oldale, &gTileset_Petalburg},
    {METATILE_Mauville_Door,                                DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_Mauville, sDoorAnimPalettes_Mauville, &gTileset_Mauville},
    {METATILE_Mauville_Door_Verdanturf,                     DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_Verdanturf, sDoorAnimPalettes_Verdanturf, &gTileset_Mauville},
    {METATILE_Slateport_Door,                               DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_Slateport, sDoorAnimPalettes_Slateport, &gTileset_Slateport},
    {METATILE_Dewford_Door,                                 DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_Dewford, sDoorAnimPalettes_Dewford, &gTileset_Dewford},
    {METATILE_General_Door_Contest,                         DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_Contest, sDoorAnimPalettes_Contest, &gTileset_General},
    {METATILE_Lilycove_Door,                                DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_Lilycove, sDoorAnimPalettes_Lilycove, &gTileset_Lilycove},
    {METATILE_Lilycove_Door_Wooden,                         DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_LilycoveWooden, sDoorAnimPalettes_LilycoveWooden, &gTileset_Lilycove},
    {METATILE_Mossdeep_Door,                                DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_Mossdeep, sDoorAnimPalettes_Mossdeep, &gTileset_Mossdeep},
    {METATILE_Sootopolis_Door_PeakedRoof,                   DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_SootopolisPeakedRoof, sDoorAnimPalettes_SootopolisPeakedRoof, &gTileset_Sootopolis},
    {METATILE_Sootopolis_Door,                              DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_Sootopolis, sDoorAnimPalettes_Sootopolis, &gTileset_Sootopolis},
    {METATILE_EverGrande_Door_PokemonLeague,                DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_PokemonLeague, sDoorAnimPalettes_PokemonLeague, &gTileset_EverGrande},
    {METATILE_Pacifidlog_Door,                              DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_Pacifidlog, sDoorAnimPalettes_Pacifidlog, &gTileset_Pacifidlog},
    {METATILE_PetalburgGym_Door,                            DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_PetalburgGym, sDoorAnimPalettes_PetalburgGym, &gTileset_PetalburgGym},
    {METATILE_Mauville_Door_CyclingRoad,                    DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_CyclingRoad, sDoorAnimPalettes_CyclingRoad, &gTileset_Mauville},
    {METATILE_Lilycove_Door_DeptStore,                      DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_LilycoveDeptStore, sDoorAnimPalettes_LilycoveDeptStore, &gTileset_Lilycove},
    {METATILE_Lilycove_Door_SafariZone,                     DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_SafariZone, sDoorAnimPalettes_SafariZone, &gTileset_Lilycove},
    {METATILE_Mossdeep_Door_SpaceCenter,                    DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_MossdeepSpaceCenter, sDoorAnimPalettes_MossdeepSpaceCenter, &gTileset_Mossdeep},
    {METATILE_PokemonCenter_Door_CableClub,                 DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_CableClub, sDoorAnimPalettes_CableClub, &gTileset_PokemonCenter},
    {METATILE_InsideShip_IntactDoor_Bottom_Unlocked,        DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_AbandonedShip, sDoorAnimPalettes_AbandonedShip, &gTileset_InsideShip},
    {METATILE_Fallarbor_Door_DarkRoof,                      DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_FallarborDarkRoof, sDoorAnimPalettes_FallarborDarkRoof, &gTileset_Fallarbor},
    {METATILE_InsideShip_IntactDoor_Bottom_Interior,        DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_AbandonedShipRoom, sDoorAnimPalettes_AbandonedShipRoom, &gTileset_InsideShip},
    {METATILE_Shop_Door_Elevator,                           DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_LilycoveDeptStoreElevator, sDoorAnimPalettes_LilycoveDeptStoreElevator, &gTileset_Shop},
    {METATILE_RG_General_Door,                              DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_General, sDoorAnimPalettes_RG_General, &gTileset_RG_General},
    {METATILE_BattleFrontier_Door_Elevator,                 DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleTowerElevator, sDoorAnimPalettes_BattleTowerElevator, &gTileset_BattleFrontier},
    {METATILE_BattleFrontierOutsideWest_Door_BattleDome,    DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleDome, sDoorAnimPalettes_BattleDome, &gTileset_BattleFrontierOutsideWest},
    {METATILE_BattleFrontierOutsideWest_Door_BattleFactory, DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleFactory, sDoorAnimPalettes_BattleFactory, &gTileset_BattleFrontierOutsideWest},
    {METATILE_BattleFrontierOutsideEast_Door_BattleTower,   DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleTower, sDoorAnimPalettes_BattleTower, &gTileset_BattleFrontierOutsideEast},
    {METATILE_BattleFrontierOutsideEast_Door_BattleArena,   DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_BattleArena, sDoorAnimPalettes_BattleArena, &gTileset_BattleFrontierOutsideEast},
    {METATILE_BattleArena_Door,                             DOOR_SOUND_ARENA,   DOOR_SIZE_1x2, sDoorAnimTiles_BattleArenaLobby, sDoorAnimPalettes_BattleArenaLobby, &gTileset_BattleArena},
    {METATILE_BattleDome_Door_Lobby,                        DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleDomeLobby, sDoorAnimPalettes_BattleDomeLobby, &gTileset_BattleDome},
    {METATILE_BattlePalace_Door,                            DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_BattlePalaceLobby, sDoorAnimPalettes_BattlePalaceLobby, &gTileset_BattlePalace},
    {METATILE_Slateport_Door_BattleTent,                    DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleTent, sDoorAnimPalettes_BattleTent, &gTileset_Slateport},
    {METATILE_Mauville_Door_BattleTent,                     DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleTent, sDoorAnimPalettes_BattleTent, &gTileset_Mauville},
    {METATILE_Fallarbor_Door_BattleTent,                    DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleTent, sDoorAnimPalettes_BattleTent, &gTileset_Fallarbor},
    {METATILE_BattleDome_Door_Corridor,                     DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleDomeCorridor, sDoorAnimPalettes_BattleDomeCorridor, &gTileset_BattleDome},
    {METATILE_BattleFrontier_Door_MultiCorridor,            DOOR_SOUND_SLIDING, DOOR_SIZE_2x2, sDoorAnimTiles_BattleTowerMultiCorridor, sDoorAnimPalettes_BattleTowerMultiCorridor, &gTileset_BattleFrontier},
    {METATILE_BattleFrontierOutsideWest_Door,               DOOR_SOUND_NORMAL,  DOOR_SIZE_1x2, sDoorAnimTiles_BattleFrontier, sDoorAnimPalettes_BattleFrontier, &gTileset_BattleFrontierOutsideWest},
    {METATILE_BattleFrontierOutsideWest_Door_Sliding,       DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleFrontierSliding, sDoorAnimPalettes_BattleFrontier, &gTileset_BattleFrontierOutsideWest},
    {METATILE_BattleDome_Door_PreBattleRoom,                DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleDomePreBattleRoom, sDoorAnimPalettes_BattleDomePreBattleRoom, &gTileset_BattleDome},
    {METATILE_BattleTent_Door,                              DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_BattleTentInterior, sDoorAnimPalettes_BattleTentInterior, &gTileset_BattleTent},
    {METATILE_TrainerHill_Door_Elevator_Lobby,              DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_TrainerHillLobbyElevator, sDoorAnimPalettes_TrainerHillLobbyElevator, &gTileset_TrainerHill},
    {METATILE_TrainerHill_Door_Elevator_Roof,               DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_TrainerHillRoofElevator, sDoorAnimPalettes_TrainerHillRoofElevator, &gTileset_TrainerHill},
    {METATILE_RG_General_SlidingSingleDoor,                 DOOR_SOUND_SLIDING, DOOR_SIZE_1x1, sDoorAnimTiles_RG_SlidingSingle, sDoorAnimPalettes_RG_SlidingSingle, &gTileset_RG_General},
    {METATILE_RG_General_SlidingDoubleDoor,                 DOOR_SOUND_SLIDING, DOOR_SIZE_1x1, sDoorAnimTiles_RG_SlidingDouble, sDoorAnimPalettes_RG_SlidingDouble, &gTileset_RG_General},
    {METATILE_RG_PalletTown_Door,                           DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_Pallet, sDoorAnimPalettes_RG_Pallet, &gTileset_RG_PalletTown},
    {METATILE_RG_PalletTown_OaksLabDoor,                    DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_OaksLab, sDoorAnimPalettes_RG_OaksLab, &gTileset_RG_PalletTown},
    {METATILE_RG_ViridianCity_Door,                         DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_Viridian, sDoorAnimPalettes_RG_Viridian, &gTileset_RG_ViridianCity},
    {METATILE_RG_PewterCity_Door,                           DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_Pewter, sDoorAnimPalettes_RG_Pewter, &gTileset_RG_PewterCity},
    {METATILE_RG_SaffronCity_Door,                          DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_Saffron, sDoorAnimPalettes_RG_Saffron, &gTileset_RG_SaffronCity},
    {METATILE_RG_SaffronCity_SilphCoDoor,                   DOOR_SOUND_SLIDING, DOOR_SIZE_1x1, sDoorAnimTiles_RG_SilphCo, sDoorAnimPalettes_RG_SilphCo, &gTileset_RG_SaffronCity},
    {METATILE_RG_CeruleanCity_Door,                         DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_Cerulean, sDoorAnimPalettes_RG_Cerulean, &gTileset_RG_CeruleanCity},
    {METATILE_RG_LavenderTown_Door,                         DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_Lavender, sDoorAnimPalettes_RG_Lavender, &gTileset_RG_LavenderTown},
    {METATILE_RG_VermilionCity_Door,                        DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_Vermilion, sDoorAnimPalettes_RG_Vermilion, &gTileset_RG_VermilionCity},
    {METATILE_RG_CeladonCity_DeptStoreDoor,                 DOOR_SOUND_SLIDING, DOOR_SIZE_1x1, sDoorAnimTiles_RG_DeptStore, sDoorAnimPalettes_RG_DeptStore, &gTileset_RG_CeladonCity},
    {METATILE_RG_FuchsiaCity_Door,                          DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_Fuchsia, sDoorAnimPalettes_RG_Fuchsia, &gTileset_RG_FuchsiaCity},
    {METATILE_RG_FuchsiaCity_SafariZoneDoor,                DOOR_SOUND_SLIDING, DOOR_SIZE_1x1, sDoorAnimTiles_RG_SafariZone, sDoorAnimPalettes_RG_SafariZone, &gTileset_RG_FuchsiaCity},
    {METATILE_RG_CinnabarIsland_LabDoor,                    DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_CinnabarLab, sDoorAnimPalettes_RG_CinnabarLab, &gTileset_RG_CinnabarIsland},
    {METATILE_RG_SeviiIslands123_Door,                      DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_Sevii123, sDoorAnimPalettes_RG_Sevii123, &gTileset_RG_SeviiIslands123},
    {METATILE_RG_SeviiIslands123_GameCornerDoor,            DOOR_SOUND_SLIDING, DOOR_SIZE_1x1, sDoorAnimTiles_RG_JoyfulGameCorner, sDoorAnimPalettes_RG_JoyfulGameCorner, &gTileset_RG_SeviiIslands123},
    {METATILE_RG_SeviiIslands123_PokeCenterDoor,            DOOR_SOUND_SLIDING, DOOR_SIZE_1x1, sDoorAnimTiles_RG_OneIslandPokeCenter, sDoorAnimPalettes_RG_OneIslandPokeCenter, &gTileset_RG_SeviiIslands123},
    {METATILE_RG_SeviiIslands45_Door,                       DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_Sevii45, sDoorAnimPalettes_RG_Sevii45, &gTileset_RG_SeviiIslands45},
    {METATILE_RG_SeviiIslands45_DayCareDoor,                DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_FourIslandDayCare, sDoorAnimPalettes_RG_FourIslandDayCare, &gTileset_RG_SeviiIslands45},
    {METATILE_RG_SeviiIslands45_RocketWarehouseDoor_Unlocked, DOOR_SOUND_FRLG,  DOOR_SIZE_1x1, sDoorAnimTiles_RG_RocketWarehouse, sDoorAnimPalettes_RG_RocketWarehouse, &gTileset_RG_SeviiIslands45},
    {METATILE_RG_SeviiIslands67_Door,                       DOOR_SOUND_FRLG,    DOOR_SIZE_1x1, sDoorAnimTiles_RG_Sevii67, sDoorAnimPalettes_RG_Sevii67, &gTileset_RG_SeviiIslands67},
    {METATILE_RG_DepartmentStore_ElevatorDoor,              DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_RG_DeptStoreElevator, sDoorAnimPalettes_RG_DeptStoreElevator, &gTileset_RG_DepartmentStore},
    {METATILE_RG_PokemonCenter_CableClubDoor,               DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_RG_CableClub, sDoorAnimPalettes_RG_CableClub, &gTileset_RG_PokemonCenter},
    {METATILE_RG_SilphCo_HideoutElevatorDoor,               DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_RG_HideoutElevator, sDoorAnimPalettes_RG_HideoutElevator, &gTileset_RG_SilphCo},
    {METATILE_RG_SSAnne_Door,                               DOOR_SOUND_FRLG,    DOOR_SIZE_1x2, sDoorAnimTiles_RG_SSAnne, sDoorAnimPalettes_RG_SSAnne, &gTileset_RG_SSAnne},
    {METATILE_RG_SilphCo_ElevatorDoor,                      DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_RG_SilphCoElevator, sDoorAnimPalettes_RG_SilphCoElevator, &gTileset_RG_SilphCo},
    {METATILE_RG_SeaCottage_Teleporter_Door,                DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_RG_Teleporter, sDoorAnimPalettes_RG_Teleporter, &gTileset_RG_SeaCottage},
    {METATILE_RG_TrainerTower_LobbyElevatorDoor,            DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_RG_TrainerTowerLobbyElevator, sDoorAnimPalettes_RG_TrainerTowerLobbyElevator, &gTileset_RG_TrainerTower},
    {METATILE_RG_TrainerTower_RoofElevatorDoor,             DOOR_SOUND_SLIDING, DOOR_SIZE_1x2, sDoorAnimTiles_RG_TrainerTowerRoofElevator, sDoorAnimPalettes_RG_TrainerTowerRoofElevator, &gTileset_RG_TrainerTower},
    {},
};

// NOTE: The tiles of a door's animation must be copied to VRAM because they are not already part of any given tileset.
//       This means that if there are any pre-existing tiles in this copied region that are visible when the door
//       animation is played they will be overwritten.
#define DOOR_TILE_START_SIZE1 (NUM_TILES_TOTAL - 8)
#define DOOR_TILE_START_SIZE2 (NUM_TILES_TOTAL - 16)

static void BuildDoorTiles(u16 *tiles, u16 tileNum, const u8 *paletteNums)
{
    int i;
    u16 tile;

    // Only the first 4 tiles of each metatile (bottom layer) actually use the door tiles
    for (i = 0; i < 4; i++)
    {
        tile = *(paletteNums++) << 12;
        tiles[i] = tile | (tileNum + i);
    }

    // The remaining layers are left as tile 0 (with the same palette)
    for (; i < 8; i++)
    {
        tile = *(paletteNums++) << 12;
        tiles[i] = tile;
    }
}

static void DrawCurrentDoorAnimFrame(const struct DoorGraphics *gfx, u32 x, u32 y, const u8 *paletteNums)
{
    u16 tiles[24];

    if (gfx->size == DOOR_SIZE_2x2)
    {
        // Top left metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 0, &paletteNums[0]);
        DrawDoorMetatileAt(x, y - 1, &tiles[8]);

        // Bottom left metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 4, &paletteNums[4]);
        DrawDoorMetatileAt(x, y, &tiles[8]);

        // Top right metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 8, &paletteNums[0]);
        DrawDoorMetatileAt(x + 1, y - 1, &tiles[8]);

        // Bottom right metatile
        BuildDoorTiles(&tiles[8], DOOR_TILE_START_SIZE2 + 12, &paletteNums[4]);
        DrawDoorMetatileAt(x + 1, y, &tiles[8]);
    }
    else
    {
        // Top metatile
        BuildDoorTiles(&tiles[0], DOOR_TILE_START_SIZE1 + 0, &paletteNums[0]);
        if (gfx->size == DOOR_SIZE_1x2)
        {
            DrawDoorMetatileAt(x, y - 1, &tiles[0]);

            // Bottom metatile
            BuildDoorTiles(&tiles[0], DOOR_TILE_START_SIZE1 + 4, &paletteNums[4]);
        }
        DrawDoorMetatileAt(x, y, &tiles[0]);
    }
}

static void DrawClosedDoorTiles(const struct DoorGraphics *gfx, u32 x, u32 y)
{
    if (gfx->size != DOOR_SIZE_1x1)
        CurrentMapDrawMetatileAt(x, y - 1);
    CurrentMapDrawMetatileAt(x, y);

    if (gfx->size == DOOR_SIZE_2x2)
    {
        CurrentMapDrawMetatileAt(x + 1, y - 1);
        CurrentMapDrawMetatileAt(x + 1, y);
    }
}

static void DrawDoor(const struct DoorGraphics *gfx, const struct DoorAnimFrame *frame, u32 x, u32 y)
{
    if (frame->offset == CLOSED_DOOR_TILES_OFFSET)
    {
        DrawClosedDoorTiles(gfx, x, y);
        if (ShouldUseMultiCorridorDoor())
            DrawClosedDoorTiles(gfx, gSpecialVar_0x8004 + MAP_OFFSET, gSpecialVar_0x8005 + MAP_OFFSET);
    }
    else
    {
        u32 offset, size;
        if (gfx->size == DOOR_SIZE_2x2)
        {
            offset = TILE_OFFSET_4BPP(DOOR_TILE_START_SIZE2);
            size = 16 * TILE_SIZE_4BPP;
        }
        else
        {
            offset = TILE_OFFSET_4BPP(DOOR_TILE_START_SIZE1);
            size = 8 * TILE_SIZE_4BPP;
        }
        CpuFastCopy(gfx->tiles + frame->offset, (void *)(VRAM + offset), size);
        DrawCurrentDoorAnimFrame(gfx, x, y, gfx->palettes);
        if (ShouldUseMultiCorridorDoor())
            DrawCurrentDoorAnimFrame(gfx, gSpecialVar_0x8004 + MAP_OFFSET, gSpecialVar_0x8005 + MAP_OFFSET, gfx->palettes);
    }
}

#define tFramesHi data[0]
#define tFramesLo data[1]
#define tGfxHi data[2]
#define tGfxLo data[3]
#define tFrameId data[4]
#define tCounter data[5]
#define tX data[6]
#define tY data[7]

// Draws a single frame of the door animation, or skips drawing to wait between frames.
// Returns FALSE when the final frame has been reached
static bool32 AnimateDoorFrame(struct DoorGraphics *gfx, struct DoorAnimFrame *frames, s16 *data)
{
    if (tCounter == 0)
        DrawDoor(gfx, &frames[tFrameId], tX, tY);

    if (tCounter == frames[tFrameId].time)
    {
        tCounter = 0;
        tFrameId++;
        if (frames[tFrameId].time == 0)
            return FALSE;
        else
            return TRUE;
    }
    tCounter++;
    return TRUE;
}

static void Task_AnimateDoor(u8 taskId)
{
    u16 *data = gTasks[taskId].data;
    struct DoorAnimFrame *frames = (struct DoorAnimFrame *)(tFramesHi << 16 | tFramesLo);
    struct DoorGraphics *gfx = (struct DoorGraphics *)(tGfxHi << 16 | tGfxLo);

    if (AnimateDoorFrame(gfx, frames, data) == FALSE)
        DestroyTask(taskId);
}

static const struct DoorAnimFrame *GetLastDoorFrame(const struct DoorAnimFrame *frame)
{
    while (frame->time != 0)
        frame++;
    return frame - 1;
}

static const struct DoorGraphics *GetDoorGraphics(u32 metatileNum)
{
    const struct DoorGraphics *gfx = sDoorAnimGraphicsTable;
    while (gfx->tiles != NULL)
    {
        if (gfx->metatileNum == metatileNum
        && (gMapHeader.mapLayout->primaryTileset == gfx->tileset || gMapHeader.mapLayout->secondaryTileset == gfx->tileset))
            return gfx;
        gfx++;
    }
    return NULL;
}

static s8 StartDoorAnimationTask(const struct DoorGraphics *gfx, const struct DoorAnimFrame *frames, u32 x, u32 y)
{
    if (gfx == NULL)
        return -1;

    if (FieldIsDoorAnimationRunning())
        return -1;
    else
    {
        u8 taskId = CreateTask(Task_AnimateDoor, 0x50);
        s16 *data = gTasks[taskId].data;

        tX = x;
        tY = y;

        tFramesLo = (u32)frames;
        tFramesHi = (u32)frames >> 16;

        tGfxLo = (u32)gfx;
        tGfxHi = (u32)gfx >> 16;

        return taskId;
    }
}

static s8 GetDoorSoundType(u32 x, u32 y)
{
    const struct DoorGraphics *gfx = GetDoorGraphics(MapGridGetMetatileIdAt(x, y));
    if (gfx == NULL)
        return -1;
    else
        return gfx->sound;
}

void FieldSetDoorOpened(u32 x, u32 y)
{
    if (MetatileBehavior_IsDoor(MapGridGetMetatileBehaviorAt(x, y)))
    {
        const struct DoorGraphics *gfx = GetDoorGraphics(MapGridGetMetatileIdAt(x, y));
        if (gfx != NULL)
            DrawDoor(gfx, GetLastDoorFrame(gfx->size == DOOR_SIZE_1x1 ? sSmallDoorOpenAnimFrames : sDoorOpenAnimFrames), x, y);
    }
}

void FieldSetDoorClosed(u32 x, u32 y)
{
    if (MetatileBehavior_IsDoor(MapGridGetMetatileBehaviorAt(x, y)))
        DrawClosedDoorTiles(sDoorAnimGraphicsTable, x, y);
}

s8 FieldAnimateDoorClose(u32 x, u32 y)
{
    if (!MetatileBehavior_IsDoor(MapGridGetMetatileBehaviorAt(x, y)))
        return -1;
    else
    {
        const struct DoorAnimFrame *frames;
        const struct DoorGraphics *gfx = GetDoorGraphics(MapGridGetMetatileIdAt(x, y));

        if (gfx->size == DOOR_SIZE_2x2)
            frames = sBigDoorCloseAnimFrames;
        else if (gfx->size == DOOR_SIZE_1x2)
            frames = sDoorCloseAnimFrames;
        else
            frames = sSmallDoorCloseAnimFrames;
        return StartDoorAnimationTask(gfx, frames, x, y);
    }
    return -1;
}

s8 FieldAnimateDoorOpen(u32 x, u32 y)
{
    if (!MetatileBehavior_IsDoor(MapGridGetMetatileBehaviorAt(x, y)))
        return -1;
    else
    {
        const struct DoorAnimFrame *frames;
        const struct DoorGraphics *gfx = GetDoorGraphics(MapGridGetMetatileIdAt(x, y));

        if (gfx->size == DOOR_SIZE_2x2)
            frames = sBigDoorOpenAnimFrames;
        else if (gfx->size == DOOR_SIZE_1x2)
            frames = sDoorOpenAnimFrames;
        else
            frames = sSmallDoorOpenAnimFrames;
        return StartDoorAnimationTask(gfx, frames, x, y);
    }
}

bool8 FieldIsDoorAnimationRunning(void)
{
    return FuncIsActiveTask(Task_AnimateDoor);
}

u32 GetDoorSoundEffect(u32 x, u32 y)
{
    int sound = GetDoorSoundType(x, y);

    if (sound == DOOR_SOUND_NORMAL)
        return SE_DOOR;
    else if (sound == DOOR_SOUND_SLIDING)
        return SE_SLIDING_DOOR;
    else if (sound == DOOR_SOUND_ARENA)
        return SE_REPEL;
    else
        return SE_RG_DOOR;
}

// Opens the Battle Tower multi partner's door in sync with the player's door
static bool8 ShouldUseMultiCorridorDoor(void)
{
    if (FlagGet(FLAG_ENABLE_MULTI_CORRIDOR_DOOR))
    {
        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(BATTLE_FRONTIER_BATTLE_TOWER_MULTI_CORRIDOR)
            && gSaveBlock1Ptr->location.mapNum == MAP_NUM(BATTLE_FRONTIER_BATTLE_TOWER_MULTI_CORRIDOR))
        {
            return TRUE;
        }
    }
    return FALSE;
}
