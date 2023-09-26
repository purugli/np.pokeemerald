#include "global.h"
#include "pokemon.h"
#include "pokemon_icon.h"

#include "data/pokemon/form_species_tables.h"
#include "data/pokemon/form_species_table_pointers.h"

u16 GetUnownSpecies(u32 personality)
{
    return gUnownFormSpecies[GetUnownLetterByPersonality(personality)];
}
