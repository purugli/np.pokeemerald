#include "global.h"
#include "pokemon.h"
#include "pokemon_icon.h"

#include "data/pokemon/form_species_tables.h"
#include "data/pokemon/form_species_table_pointers.h"

u16 GetUnownSpecies(u32 personality)
{
    return gUnownFormSpecies[GetUnownLetterByPersonality(personality)];
}

// Returns TRUE if formSpecies is in the form table for formTableSpecies.
// If *formIndex is not NULL, it will be set to the form index if one was found or 0 if it wasn't.
bool32 FormIfExists(u16 formTableSpecies, u16 formSpecies, u8 *formIndex)
{
    const u16 *formTable;
    bool32 formExists = FALSE;

    if (formIndex != NULL)
        *formIndex = 0;

    formTable = gFormSpeciesTables[formTableSpecies];
    if (formTable != NULL)
    {
        u32 targetForm;
        for (targetForm = 0; formTable[targetForm] != FORM_SPECIES_END; targetForm++)
        {
            if (formSpecies == formTable[targetForm])
            {
                if (formIndex != NULL)
                    *formIndex = targetForm;
                formExists = TRUE;
            }
        }
    }
    return formExists;
}
