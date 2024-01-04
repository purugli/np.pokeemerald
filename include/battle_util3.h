#ifndef GUARD_BATTLE_UTIL3_H
#define GUARD_BATTLE_UTIL3_H

void MulByTypeEffectiveness(uq4_12_t *modifier, u32 moveType, u32 battlerDef, u32 defType);
uq4_12_t CalcPartyMonTypeEffectivenessMultiplier(u32 move, u32 speciesDef, u32 abilityDef);
uq4_12_t GetTypeModifier(u32 atkType, u32 defType);
void GetAIPartyIndexes(s32 *firstId, s32 *lastId);

#endif // GUARD_BATTLE_UTIL3_H
