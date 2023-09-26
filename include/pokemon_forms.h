#ifndef GUARD_POKEMON_FORM_CHANGE_H
#define GUARD_POKEMON_FORM_CHANGE_H

#include "constants/species.h"

#define FORM_SPECIES_END 0xFFFF

extern const u16 *const gFormSpeciesTables[NUM_SPECIES];
extern const u16 gUnownFormSpecies[];

u16 GetUnownSpecies(u32 personality);

#endif // GUARD_POKEMON_FORM_CHANGE_H
