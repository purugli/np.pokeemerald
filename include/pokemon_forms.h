#ifndef GUARD_POKEMON_FORM_CHANGE_H
#define GUARD_POKEMON_FORM_CHANGE_H

#include "constants/species.h"

#define FORM_SPECIES_END 0xFFFF

extern const u16 *const gFormSpeciesTables[NUM_SPECIES];
extern const u16 gUnownFormSpecies[];

u16 GetUnownSpecies(u32 personality);
bool32 FormIfExists(u16 formTableSpecies, u16 formSpecies, u8 *formIndex);

#endif // GUARD_POKEMON_FORM_CHANGE_H
