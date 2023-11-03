#ifndef GUARD_TRAINER_POKEMON_SPRITES_H
#define GUARD_TRAINER_POKEMON_SPRITES_H

bool16 ResetAllPicSprites(void);
u16 CreateMonPicSprite(u16 species, u32 otId, u32 personality, bool8 useAffine, s16 x, s16 y, u8 paletteSlot, u16 paletteTag);
u16 FreeAndDestroyPicSprite(u16 spriteId);
u16 CreateTrainerPicSprite(u16 trainerPicId, s16 x, s16 y, u8 paletteSlot, u16 paletteTag);
u16 FreeAndDestroyTrainerPicSprite(u16 spriteId);
u16 CreateTrainerCardTrainerPicSprite(u16 trainerPicId, u16 destX, u16 destY);

#endif // GUARD_TRAINER_POKEMON_SPRITES_H
