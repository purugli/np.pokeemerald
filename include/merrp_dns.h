#ifndef GUARD_MERRP_DNS_H
#define GUARD_MERRP_DNS_H

#include "constants/merrp_dns.h"

struct __attribute__((packed)) TimeBlendSettings
{
    u16 weight:9;
    u16 time0:3;
    u16 time1:3;
    u16 unused:1;
    u16 altWeight;
};

struct BlendSettings
{
    u32 blendColor:24;
    u32 isTint:1;
    u32 coeff:5;
};

extern u8 gTimeOfDay;
extern u16 gTimeUpdateCounter;
extern struct TimeBlendSettings gCurrentTimeBlend;

extern const struct BlendSettings gTimeOfDayBlend[];

void UpdateTimeOfDay(void);
void UpdateAltBgPalettes(u16 palettes);
void UpdatePalettesWithTime(u32);
void TimeMixPalettes(u32 palettes, u16 *src, u16 *dst);
u8 UpdateSpritePaletteWithTime(u8 paletteNum);

#endif //GUARD_MERRP_DNS_H
