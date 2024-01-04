#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_ai_script_commands.h"
#include "battle_controllers.h"
#include "constants/abilities.h"
#include "constants/battle_string_ids.h"
#include "constants/moves.h"
#include "util.h"

/*
    This file has various battle utility functions ported
    from RHH's pokeemerald-expansion and adapted for use
    with gen 3.
*/

void MulByTypeEffectiveness(uq4_12_t *modifier, u32 moveType, u32 battlerDef, u32 defType)
{
    uq4_12_t mod = GetTypeModifier(moveType, defType);

    if ((moveType == TYPE_FIGHTING || moveType == TYPE_NORMAL) && defType == TYPE_GHOST && gBattleMons[battlerDef].status2 & STATUS2_FORESIGHT && mod == TYPE_MOD_NO_EFFECT)
        mod = TYPE_MOD_NORMAL;

    *modifier = uq4_12_multiply(*modifier, mod);
}

uq4_12_t CalcPartyMonTypeEffectivenessMultiplier(u32 move, u32 speciesDef, u32 abilityDef)
{
    uq4_12_t modifier = TYPE_MOD_NORMAL;
    u32 moveType = gBattleMoves[move].type;

    if (move != MOVE_STRUGGLE && moveType != TYPE_MYSTERY)
    {
        MulByTypeEffectiveness(&modifier, moveType, 0, gSpeciesInfo[speciesDef].types[0]);
        if (gSpeciesInfo[speciesDef].types[1] != gSpeciesInfo[speciesDef].types[0])
            MulByTypeEffectiveness(&modifier, moveType, 0, gSpeciesInfo[speciesDef].types[1]);

        if (abilityDef == ABILITY_WONDER_GUARD && modifier <= TYPE_MOD_NORMAL && gBattleMoves[move].power)
            modifier = TYPE_MOD_NO_EFFECT;
    }

    return modifier;
}

uq4_12_t GetTypeModifier(u32 atkType, u32 defType)
{
    return gTypeEffectiveness[atkType][defType];
}

void GetAIPartyIndexes(s32 *firstId, s32 *lastId)
{
    if (gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_TOWER_LINK_MULTI))
    {
        if ((gActiveBattler & BIT_FLANK) == B_FLANK_LEFT)
            *firstId = 0, *lastId = PARTY_SIZE / 2;
        else
            *firstId = PARTY_SIZE / 2, *lastId = PARTY_SIZE;
    }
    else
    {
        *firstId = 0, *lastId = PARTY_SIZE;
    }
}
