#ifndef GUARD_tilesets_H
#define GUARD_tilesets_H

extern const u32 gTilesetTiles_General[];
extern const u16 gTilesetPalettes_General[][16];

extern const struct Tileset gTileset_SecretBase;
extern const struct Tileset gTileset_SecretBaseRedCave;

struct MetatileMapping
{
    const struct Tileset *tileset;
    u16 metatileIds[2];
};

extern const struct Tileset gTileset_General;
extern const struct Tileset gTileset_Petalburg;
extern const struct Tileset gTileset_Rustboro;
extern const struct Tileset gTileset_Dewford;
extern const struct Tileset gTileset_Slateport;
extern const struct Tileset gTileset_Mauville;
extern const struct Tileset gTileset_Lavaridge;
extern const struct Tileset gTileset_Fallarbor;
extern const struct Tileset gTileset_Fortree;
extern const struct Tileset gTileset_Lilycove;
extern const struct Tileset gTileset_Mossdeep;
extern const struct Tileset gTileset_EverGrande;
extern const struct Tileset gTileset_Pacifidlog;
extern const struct Tileset gTileset_Sootopolis;
extern const struct Tileset gTileset_BattleFrontierOutsideWest;
extern const struct Tileset gTileset_BattleFrontierOutsideEast;
extern const struct Tileset gTileset_Building;
extern const struct Tileset gTileset_Shop;
extern const struct Tileset gTileset_PokemonCenter;
extern const struct Tileset gTileset_RG_General;
extern const struct Tileset gTileset_PetalburgGym;
extern const struct Tileset gTileset_InsideShip;
extern const struct Tileset gTileset_BattleFrontier;
extern const struct Tileset gTileset_BattlePalace;
extern const struct Tileset gTileset_BattleDome;
extern const struct Tileset gTileset_BattleArena;
extern const struct Tileset gTileset_BattlePyramid;
extern const struct Tileset gTileset_TrainerHill;
extern const struct Tileset gTileset_BattleTent;
extern const struct Tileset gTileset_RG_PalletTown;
extern const struct Tileset gTileset_RG_ViridianCity;
extern const struct Tileset gTileset_RG_PewterCity;
extern const struct Tileset gTileset_RG_CeruleanCity;
extern const struct Tileset gTileset_RG_LavenderTown;
extern const struct Tileset gTileset_RG_VermilionCity;
extern const struct Tileset gTileset_RG_CeladonCity;
extern const struct Tileset gTileset_RG_FuchsiaCity;
extern const struct Tileset gTileset_RG_CinnabarIsland;
extern const struct Tileset gTileset_RG_IndigoPlateau;
extern const struct Tileset gTileset_RG_SaffronCity;
extern const struct Tileset gTileset_RG_Building;
extern const struct Tileset gTileset_RG_PokemonCenter;
extern const struct Tileset gTileset_RG_SSAnne;
extern const struct Tileset gTileset_RG_DepartmentStore;
extern const struct Tileset gTileset_RG_SeaCottage;
extern const struct Tileset gTileset_RG_SilphCo;
extern const struct Tileset gTileset_RG_SeviiIslands123;
extern const struct Tileset gTileset_RG_SeviiIslands45;
extern const struct Tileset gTileset_RG_SeviiIslands67;
extern const struct Tileset gTileset_RG_TrainerTower;

#endif //GUARD_tilesets_H
