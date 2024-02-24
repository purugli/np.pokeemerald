#include "global.h"
#include "battle_pyramid.h"
#include "data.h"
#include "decompress.h"
#include "dma3.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "field_effect.h"
#include "field_weather.h"
#include "graphics.h"
#include "main.h"
#include "malloc.h"
#include "m4a.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "rtc.h"
#include "sound.h"
#include "sprite.h"
#include "trainer_hill.h"
#include "task.h"
#include "util.h"
#include "constants/event_objects.h"
#include "constants/rgb.h"

u8 gTimeOfDay;
struct TimeBlendSettings gCurrentTimeBlend;
u16 gTimeUpdateCounter; // playTimeVBlanks will eventually overflow, so this is used to update TOD

#define TINT_NIGHT Q_8_8(0.456) | Q_8_8(0.456) << 8 | Q_8_8(0.615) << 16

const struct BlendSettings gTimeOfDayBlend[] =
{
    [TIME_DAY] = {.coeff = 0, .blendColor = 0},
    [TIME_TWILIGHT] = {.coeff = 4, .blendColor = 0xA8B0E0, .isTint = TRUE},
    [TIME_NIGHT] = {.coeff = 10, .blendColor = TINT_NIGHT, .isTint = TRUE},
};

void UpdateTimeOfDay(void)
{
    s32 hours, minutes;

    RtcCalcLocalTime();
    hours = gLocalTime.hours;
    minutes = gLocalTime.minutes;

    if (hours < 4) // night
    {
        gCurrentTimeBlend.weight = 256;
        gCurrentTimeBlend.altWeight = 0;
        gTimeOfDay = gCurrentTimeBlend.time0 = gCurrentTimeBlend.time1 = TIME_NIGHT;
    }
    else if (hours < 7) // night->twilight
    {
        gCurrentTimeBlend.time0 = TIME_NIGHT;
        gCurrentTimeBlend.time1 = TIME_TWILIGHT;
        gCurrentTimeBlend.weight = 256 - 256 * ((hours - 4) * 60 + minutes) / ((7 - 4) * 60);
        gCurrentTimeBlend.altWeight = (256 - gCurrentTimeBlend.weight) / 2;
        gTimeOfDay = TIME_NIGHT;
    }
    else if (hours < 10) // twilight->day
    {
        gCurrentTimeBlend.time0 = TIME_TWILIGHT;
        gCurrentTimeBlend.time1 = TIME_DAY;
        gCurrentTimeBlend.weight = 256 - 256 * ((hours - 7) * 60 + minutes) / ((10 - 7) * 60);
        gCurrentTimeBlend.altWeight = (256 - gCurrentTimeBlend.weight) / 2 + 128;
        gTimeOfDay = TIME_DAY;
    }
    else if (hours < 18) // day
    {
        gCurrentTimeBlend.weight = gCurrentTimeBlend.altWeight = 256;
        gTimeOfDay = gCurrentTimeBlend.time0 = gCurrentTimeBlend.time1 = TIME_DAY;
    }
    else if (hours < 20) // day->twilight
    {
        gCurrentTimeBlend.time0 = TIME_DAY;
        gCurrentTimeBlend.time1 = TIME_TWILIGHT;
        gCurrentTimeBlend.weight = 256 - 256 * ((hours - 18) * 60 + minutes) / ((20 - 18) * 60);
        gCurrentTimeBlend.altWeight = gCurrentTimeBlend.weight / 2 + 128;
        gTimeOfDay = TIME_DAY;
    }
    else if (hours < 22) // twilight->night
    {
        gCurrentTimeBlend.time0 = TIME_TWILIGHT;
        gCurrentTimeBlend.time1 = TIME_NIGHT;
        gCurrentTimeBlend.weight = 256 - 256 * ((hours - 20) * 60 + minutes) / ((22 - 20) * 60);
        gCurrentTimeBlend.altWeight = gCurrentTimeBlend.weight / 2;
        gTimeOfDay = TIME_NIGHT;
    }
    else // 22-24, night
    {
        gCurrentTimeBlend.weight = 256;
        gCurrentTimeBlend.altWeight = 0;
        gTimeOfDay = gCurrentTimeBlend.time0 = gCurrentTimeBlend.time1 = TIME_NIGHT;
    }
}

// Apply weighted average to palettes, preserving high bits of dst throughout
static inline void AvgPaletteWeighted(u16 *src0, u16 *src1, u16 *dst, u16 weight)
{
    u16 *srcEnd = &src0[16];
    src0++;
    src1++;
    dst++; // leave dst transparency unchanged
    while (src0 != srcEnd)
    {
        u32 src0Color = *src0++;
        s32 r0 = (src0Color << 27) >> 27;
        s32 g0 = (src0Color << 22) >> 27;
        s32 b0 = (src0Color << 17) >> 27;
        u32 src1Color = *src1++;
        s32 r1 = (src1Color << 27) >> 27;
        s32 g1 = (src1Color << 22) >> 27;
        s32 b1 = (src1Color << 17) >> 27;

        // Average and bitwise-OR
        r0 = r1 + (((r0 - r1) * weight) >> 8);
        g0 = g1 + (((g0 - g1) * weight) >> 8);
        b0 = b1 + (((b0 - b1) * weight) >> 8);
        *dst = (*dst & RGB_ALPHA) | RGB2(r0, g0, b0); // preserve high bit of dst
        dst++;
    }
}

// Update & mix day / night bg palettes (into unfaded)
void UpdateAltBgPalettes(u16 palettes)
{
    if (IsMapTypeOutdoors(gMapHeader.mapType))
    {
        const struct Tileset *primary = gMapHeader.mapLayout->primaryTileset;
        const struct Tileset *secondary = gMapHeader.mapLayout->secondaryTileset;
        u32 numPrimaryPals = NUM_PALS_IN_PRIMARY_EMERALD + secondary->dontUsePal7;

        palettes &= ~((1 << numPrimaryPals) - 1) | primary->swapPalettes;
        palettes &= ((1 << numPrimaryPals) - 1) | (secondary->swapPalettes << numPrimaryPals);
        palettes &= 0x1FFE; // don't blend palette 0, [13,15]
        palettes >>= 1; // start at palette 1
        if (palettes != 0)
        {
	        u32 i = 1;
            while (palettes != 0)
            {
                if (palettes & 1)
                {
                    u16 *palette = (u16 *)secondary->palettes;
                    if (i < numPrimaryPals)
                        palette = (u16 *)primary->palettes;
                    AvgPaletteWeighted(&palette[PLTT_ID(i)], &palette[PLTT_ID((i + 9) % 16)], &gPlttBufferUnfaded[PLTT_ID(i)], gCurrentTimeBlend.altWeight);
                }
                i++;
                palettes >>= 1;
            }
        }
    }
}

void UpdatePalettesWithTime(u32 palettes)
{
    if (IsMapTypeOutdoors(gMapHeader.mapType))
    {
        u32 i;
        u32 mask = 1 << 16;
        if (palettes >= 0x10000)
        {
            for (i = 0; i < 16; i++, mask <<= 1)
            {
                if (GetSpritePaletteTagByPaletteNum(i) >> 15) // Don't blend special sprite palette tags
                    palettes &= ~(mask);
            }
        }

        palettes &= 0xFFFF1FFF; // Don't blend UI BG palettes [13,15]
        if (palettes != 0)
            TimeMixPalettes(palettes, gPlttBufferUnfaded, gPlttBufferFaded);
    }
}

u8 UpdateSpritePaletteWithTime(u8 paletteNum)
{
    if (IsMapTypeOutdoors(gMapHeader.mapType))
    {
        u16 offset;
        if (GetSpritePaletteTagByPaletteNum(paletteNum) >> 15)
            return paletteNum;
        offset = (paletteNum + 16) << 4;
        TimeMixPalettes(1, &gPlttBufferUnfaded[offset], &gPlttBufferFaded[offset]);
    }
    return paletteNum;
}

#define DEFAULT_LIGHT_COLOR 0x3F9F

// Blends a weighted average of two blend parameters
// Parameters can be either blended (as in BlendPalettes) or tinted (as in TintPaletteRGB_Copy)
void TimeMixPalettes(u32 palettes, u16 *src, u16 *dst)
{
    if (palettes != 0)
    {
        struct BlendSettings *blend0 = (struct BlendSettings *)&gTimeOfDayBlend[gCurrentTimeBlend.time0];
        struct BlendSettings *blend1 = (struct BlendSettings *)&gTimeOfDayBlend[gCurrentTimeBlend.time1];
        u32 color0 = blend0->blendColor;
        bool8 tint0 = blend0->isTint;
        u32 coeff0 = tint0 ? 8 * 2 : blend0->coeff * 2;
        u32 color1 = blend1->blendColor;
        u32 tint1 = blend1->isTint;
        u32 coeff1 = tint1 ? 8 * 2 : blend1->coeff * 2;
        u32 defaultColor = DEFAULT_LIGHT_COLOR;
        s32 r0, g0, b0, r1, g1, b1, defR, defG, defB;
        u16 weight = gCurrentTimeBlend.weight;

        if (tint0)
        {
            r0 = (color0 << 24) >> 24;
            g0 = (color0 << 16) >> 24;
            b0 = (color0 << 8) >> 24;
        }
        else
        {
            r0 = (color0 << 27) >> 27;
            g0 = (color0 << 22) >> 27;
            b0 = (color0 << 17) >> 27;
        }

        if (tint1)
        {
            r1 = (color1 << 24) >> 24;
            g1 = (color1 << 16) >> 24;
            b1 = (color1 << 8) >> 24;
        }
        else
        {
            r1 = (color1 << 27) >> 27;
            g1 = (color1 << 22) >> 27;
            b1 = (color1 << 17) >> 27;
        }
        defR = (defaultColor << 27) >> 27;
        defG = (defaultColor << 22) >> 27;
        defB = (defaultColor << 17) >> 27;

        do
        {
            s32 altR, altG, altB;
            if (palettes & 1)
            {
                u16 *srcEnd = src + 16;
                u32 altBlendColor = *dst++ = *src++; // color 0 is copied through
                if (altBlendColor >> 15) // Transparency high bit set; alt blend color
                {
                    altR = (altBlendColor << 27) >> 27;
                    altG = (altBlendColor << 22) >> 27;
                    altB = (altBlendColor << 17) >> 27;
                }
                else
                {
                    altBlendColor = 0;
                }

                while (src != srcEnd)
                {
                    u32 srcColor = *src;
                    s32 r = (srcColor << 27) >> 27;
                    s32 g = (srcColor << 22) >> 27;
                    s32 b = (srcColor << 17) >> 27;
                    s32 r2, g2, b2;

                    if (srcColor >> 15)
                    {
                        if (altBlendColor) // Use alternate blend color
                        {
                            r2 = r + (((altR - r) * (s32)coeff1) >> 5);
                            g2 = g + (((altG - g) * (s32)coeff1) >> 5);
                            b2 = b + (((altB - b) * (s32)coeff1) >> 5);
                            r = r + (((altR - r) * (s32)coeff0) >> 5);
                            g = g + (((altG - g) * (s32)coeff0) >> 5);
                            b = b + (((altB - b) * (s32)coeff0) >> 5);
                        }
                        else // Use default blend color
                        {
                            r2 = r + (((defR - r) * (s32)coeff1) >> 5);
                            g2 = g + (((defG - g) * (s32)coeff1) >> 5);
                            b2 = b + (((defB - b) * (s32)coeff1) >> 5);
                            r = r + (((defR - r) * (s32)coeff0) >> 5);
                            g = g + (((defG - g) * (s32)coeff0) >> 5);
                            b = b + (((defB - b) * (s32)coeff0) >> 5);
                        }
                    }
                    else // Use provided blend colors
                    {
                        if (!tint1) // blend-based
                        {
                            r2 = (r + (((r1 - r) * (s32)coeff1) >> 5));
                            g2 = (g + (((g1 - g) * (s32)coeff1) >> 5));
                            b2 = (b + (((b1 - b) * (s32)coeff1) >> 5));
                        }
                        else // tint-based
                        {
                            r2 = (u16)((r1 * r)) >> 8;
                            g2 = (u16)((g1 * g)) >> 8;
                            b2 = (u16)((b1 * b)) >> 8;
                            if (r2 > 31)
                                r2 = 31;
                            if (g2 > 31)
                                g2 = 31;
                            if (b2 > 31)
                                b2 = 31;
                        }
                        if (!tint0) // blend-based
                        {
                            r = (r + (((r0 - r) * (s32)coeff0) >> 5));
                            g = (g + (((g0 - g) * (s32)coeff0) >> 5));
                            b = (b + (((b0 - b) * (s32)coeff0) >> 5));
                        }
                        else // tint-based
                        {
                            r = (u16)((r0 * r)) >> 8;
                            g = (u16)((g0 * g)) >> 8;
                            b = (u16)((b0 * b)) >> 8;
                            if (r > 31)
                                r = 31;
                            if (g > 31)
                                g = 31;
                            if (b > 31)
                                b = 31;
                        }
                    }
                    r = r2 + (((r - r2) * (s32)weight) >> 8);
                    g = g2 + (((g - g2) * (s32)weight) >> 8);
                    b = b2 + (((b - b2) * (s32)weight) >> 8);
                    *dst++ = RGB2(r, g, b);
                    // *dst++ = RGB2(r, g, b) | (srcColor >> 15) << 15;
                    src++;
                }
            }
            else
            {
                src += 16;
                dst += 16;
            }
            palettes >>= 1;
        } while (palettes);
    }
}
