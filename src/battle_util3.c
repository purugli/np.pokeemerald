#include "global.h"
#include "battle.h"
#include "battle_controllers.h"
#include "util.h"

bool32 IsBattlerMarkedForControllerExec(u8 battler)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        return (gBattleControllerExecFlags & (gBitTable[battler] << (32 - MAX_BATTLERS_COUNT))) != 0;
    else
        return (gBattleControllerExecFlags & (gBitTable[battler])) != 0;
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
