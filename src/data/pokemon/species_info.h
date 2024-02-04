// Maximum value for a female Pokémon is 254 (MON_FEMALE) which is 100% female.
// 255 (MON_GENDERLESS) is reserved for genderless Pokémon.
#define PERCENT_FEMALE(percent) min(254, ((percent * 255) / 100))

#define OLD_UNOWN_SPECIES_INFO                                                          \
    {                                                                                   \
        .baseHP = 50,                                                                   \
        .baseAttack = 150,                                                              \
        .baseDefense = 50,                                                              \
        .baseSpeed = 150,                                                               \
        .baseSpAttack = 150,                                                            \
        .baseSpDefense = 50,                                                            \
        .types = { TYPE_NORMAL, TYPE_NORMAL},                                           \
        .catchRate = 3,                                                                 \
        .expYield = 1,                                                                  \
        .evYield_HP = 2,                                                                \
        .evYield_Attack = 2,                                                            \
        .evYield_Defense = 2,                                                           \
        .evYield_Speed = 2,                                                             \
        .evYield_SpAttack = 2,                                                          \
        .evYield_SpDefense = 2,                                                         \
        .itemCommon = ITEM_NONE,                                                        \
        .itemRare   = ITEM_NONE,                                                        \
        .genderRatio = MON_GENDERLESS,                                                  \
        .eggCycles = 120,                                                               \
        .friendship = 0,                                                                \
        .growthRate = GROWTH_MEDIUM_FAST,                                               \
        .eggGroups = { EGG_GROUP_NO_EGGS_DISCOVERED, EGG_GROUP_NO_EGGS_DISCOVERED, },   \
        .abilities = {ABILITY_NONE, ABILITY_NONE},                                      \
        .safariZoneFleeRate = 0,                                                        \
        .bodyColor = BODY_COLOR_BLACK,                                                  \
        .noFlip = FALSE,                                                                \
    }

const struct SpeciesInfo gSpeciesInfo[] =
{
    [SPECIES_NONE] = {0},

    #include "species_info/gen_1.h"
    #include "species_info/gen_2.h"
    #include "species_info/old_unown.h"
    #include "species_info/gen_3.h"
};
