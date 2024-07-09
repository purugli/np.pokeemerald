#ifndef GUARD_DATA_H
#define GUARD_DATA_H

#include "constants/moves.h"

#define SPECIES_SHINY_TAG 500

#define MAX_TRAINER_ITEMS 4

#define TRAINER_PIC_WIDTH 64
#define TRAINER_PIC_HEIGHT 64
#define TRAINER_PIC_SIZE (TRAINER_PIC_WIDTH * TRAINER_PIC_HEIGHT / 2)

// Red and Leaf's back pics have 5 frames, but this is presumably irrelevant in the places this is used.
#define MAX_TRAINER_PIC_FRAMES 4

enum {
    BATTLER_AFFINE_NORMAL,
    BATTLER_AFFINE_EMERGE,
    BATTLER_AFFINE_RETURN,
};

struct MonCoords
{
    // This would use a bitfield, but some function
    // uses it as a u8 and casting won't match.
    u8 size; // u8 width:4, height:4;
    u8 y_offset;
};

#define MON_COORDS_SIZE(width, height)(DIV_ROUND_UP(width, 8) << 4 | DIV_ROUND_UP(height, 8))
#define GET_MON_COORDS_WIDTH(size)((size >> 4) * 8)
#define GET_MON_COORDS_HEIGHT(size)((size & 0xF) * 8)

struct TrainerMon
{
    u32 iv;
    u8 lvl;
    bool8 gender:2;
    bool8 isShiny:1;
    bool8 abilityNum:1;
    u16 species;
    u16 heldItem;
    u16 moves[MAX_MON_MOVES];
    u16 nature:11;
    u16 pokeball:5;
    const u8 *ev;
};

#define TRAINER_PARTY(party) party, .partySize = ARRAY_COUNT(party)

struct Trainer
{
    /*0x00*/ u8 partySize:7;
             bool8 doubleBattle:1;
    /*0x01*/ u8 trainerClass;
    /*0x02*/ u8 encounterMusic:5;
             bool8 gender:1;
             bool8 doubleBattle:1;
             bool8 mugshotEnabled:1;
    /*0x03*/ u8 trainerPic;
    /*0x04*/ const u8 *trainerName;
    union
    {
        struct
        {
            /*0x08*/ u16 items[MAX_TRAINER_ITEMS];
            /*0x10*/ u32 aiFlags;
        } trainer;
        struct
        {
            /*0x08*/ u32 otId;
            /*0x0C*/ u8 padding[4];
            /*0x10*/ u32 aiFlags;
        } partner;
        struct
        {
            /*0x08*/ u8 streakAppearances[4];
            // Flags to change the conversation when the Frontier Brain is encountered for a battle
            // First bit is has battled them before and not won yet, second bit is has battled them and won (obtained a Symbol)
            /*0x0C*/ u16 battledBrainBitFlags[2];
            /*0x10*/ u8 objectEventGfxId;
            /*0x11*/ u8 padding[3];
        } frontierBrain;
    } trainerType;
    /*0x14*/ const struct TrainerMon *party;
};

struct TrainerFrontPic
{
    struct CompressedSpriteSheet sprite;
    const u16 *palette;
    const struct Coords16 mugshotCoords;
    s16 mugshotRotation;
};

struct TrainerBackPic
{
    const union AnimCmd *const *anims;
    const struct SpriteFrameImage *images;
    const u16 *palette;
};

extern const u16 gMinigameDigits_Pal[];
extern const u32 gMinigameDigits_Gfx[];

extern const struct SpriteFrameImage gBattlerPicTable_PlayerLeft[];
extern const struct SpriteFrameImage gBattlerPicTable_OpponentLeft[];
extern const struct SpriteFrameImage gBattlerPicTable_PlayerRight[];
extern const struct SpriteFrameImage gBattlerPicTable_OpponentRight[];

extern const union AffineAnimCmd *const gAffineAnims_BattleSpritePlayerSide[];
extern const union AffineAnimCmd *const gAffineAnims_BattleSpriteOpponentSide[];
extern const union AffineAnimCmd *const gAffineAnims_BattleSpriteContest[];

extern const union AnimCmd *const gAnims_MonPic[];
extern const struct MonCoords gMonFrontPicCoords[];
extern const struct MonCoords gMonBackPicCoords[];
extern const u32 *const gMonBackPicTable[];
extern const struct SpritePalette gMonPaletteTable[];
extern const struct SpritePalette gMonShinyPaletteTable[];
extern const union AnimCmd *const gAnims_None[];
extern const struct TrainerFrontPic gTrainerFrontPicTable[];
extern const struct TrainerBackPic gTrainerBackPicTable[];

extern const u8 gEnemyMonElevation[NUM_SPECIES];

extern const union AnimCmd *const *const gMonFrontAnimsPtrTable[];
extern const u32 *const gMonFrontPicTable[];

extern const struct Trainer gTrainers[];
extern const u8 gTrainerClassNames[][13];
extern const u8 gJapaneseSpeciesNames[][JAPANESE_NAME_LENGTH + 1];
extern const u8 gSpeciesNames[][POKEMON_NAME_LENGTH + 1];
extern const u8 gFrenchSpeciesNames[][POKEMON_NAME_LENGTH + 1];
extern const u8 gGermanSpeciesNames[][POKEMON_NAME_LENGTH + 1];
extern const u8 gMoveNames[MOVES_COUNT][MOVE_NAME_LENGTH + 1];

#include "trainer_control.h"
#include "trainer_name_strings.h"

#endif // GUARD_DATA_H
