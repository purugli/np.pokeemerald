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
extern const struct Tileset gTileset_PetalburgGym;
extern const struct Tileset gTileset_InsideShip;
extern const struct Tileset gTileset_BattleFrontier;
extern const struct Tileset gTileset_BattlePalace;
extern const struct Tileset gTileset_BattleDome;
extern const struct Tileset gTileset_BattleArena;
extern const struct Tileset gTileset_BattlePyramid;
extern const struct Tileset gTileset_TrainerHill;
extern const struct Tileset gTileset_BattleTent;

#include "tilesets_fr.h"
#include "tilesets_custom.h"

#endif //GUARD_tilesets_H
