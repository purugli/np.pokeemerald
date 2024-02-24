#ifndef GUARD_PALETTE_INLINE_H
#define GUARD_PALETTE_INLINE_H

static inline u8 UpdateTimeOfDayPaletteFade(void)
{
    u16 paletteOffset;
    u16 selectedPalettes;
    u16 timePalettes, copyPalettes;
    u16 *src;
    u16 *dst;

    if (!gPaletteFade.active)
        return PALETTE_FADE_STATUS_DONE;

    if (IsSoftwarePaletteFadeFinishing())
    {
        return gPaletteFade.active;
    }
    else
    {
        // First pply TOD blend to relevant subset of palettes
        if (!gPaletteFade.objPaletteToggle)
        {
            if (gPaletteFade.delayCounter < gPaletteFade_delay)
            {
                gPaletteFade.delayCounter++;
                return PALETTE_FADE_STATUS_DELAY;
            }
            gPaletteFade.delayCounter = 0;

            paletteOffset = 0;

            selectedPalettes = gPaletteFade_selectedPalettes;

            // tile palettes, don't blend [13, 15]
            timePalettes = selectedPalettes & 0x1FFF;
        }
        else
        {
            u8 i;
            u16 j;

            selectedPalettes = gPaletteFade_selectedPalettes >> 16;

            paletteOffset = OBJ_PLTT_OFFSET;

            j = 1;
            timePalettes = 0; // palettes passed to the time-blender

            for (i = 0; i < 16; i++, j <<= 1) // Mask out palettes that should not be light blended
            {
                if ((selectedPalettes & j) && !(GetSpritePaletteTagByPaletteNum(i) >> 15))
                    timePalettes |= j;
            }
        }

        src = &gPlttBufferUnfaded[paletteOffset];
        dst = &gPlttBufferFaded[paletteOffset];

        TimeMixPalettes(timePalettes, src, dst);

        // palettes that were not blended above must be copied through
        if ((copyPalettes = ~timePalettes))
        {
            u16 *src1 = src;
            u16 *dst1 = dst;

            while (copyPalettes)
            {
                if (copyPalettes & 1)
                    CpuFastCopy(src1, dst1, PLTT_SIZE_4BPP);
                copyPalettes >>= 1;
                src1 += 16;
                dst1 += 16;
            }
        }

        // Then, blend from faded->faded with native BlendPalettes
        BlendPalettesFine(selectedPalettes, dst, dst, gPaletteFade.y, gPaletteFade.blendColor);

        gPaletteFade.objPaletteToggle ^= 1;

        if (!gPaletteFade.objPaletteToggle)
        {
            if ((gPaletteFade.yDec && gPaletteFade.y == 0) || (!gPaletteFade.yDec && gPaletteFade.y == gPaletteFade.targetY))
            {
                gPaletteFade_selectedPalettes = 0;
                gPaletteFade.softwareFadeFinishingCounter = 1;
            }
            else
            {
                s32 newY;

                if (!gPaletteFade.yDec)
                {
                    newY = gPaletteFade.y + gPaletteFade.deltaY;
                    if (newY > gPaletteFade.targetY)
                        newY = gPaletteFade.targetY;
                    gPaletteFade.y = newY;
                }
                else
                {
                    newY = gPaletteFade.y - gPaletteFade.deltaY;
                    if (newY < 0)
                        newY = 0;
                    gPaletteFade.y = newY;
                }
            }
        }

        return PALETTE_FADE_STATUS_ACTIVE;
    }
}

static inline u8 UpdateFastPaletteFade(void)
{
    u32 i;
    u16 paletteOffsetStart;
    u16 paletteOffsetEnd;
    s8 r0;
    s8 g0;
    s8 b0;
    s8 r;
    s8 g;
    s8 b;

    if (!gPaletteFade.active)
        return PALETTE_FADE_STATUS_DONE;

    if (IsSoftwarePaletteFadeFinishing())
        return gPaletteFade.active;


    if (gPaletteFade.objPaletteToggle)
    {
        paletteOffsetStart = OBJ_PLTT_OFFSET;
        paletteOffsetEnd = PLTT_BUFFER_SIZE;
    }
    else
    {
        paletteOffsetStart = 0;
        paletteOffsetEnd = OBJ_PLTT_OFFSET;
    }

    switch (gPaletteFade_submode)
    {
    case FAST_FADE_IN_FROM_WHITE:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; i++)
        {
            struct PlttData *unfaded;
            struct PlttData *faded;

            unfaded = (struct PlttData *)&gPlttBufferUnfaded[i];
            r0 = unfaded->r;
            g0 = unfaded->g;
            b0 = unfaded->b;

            faded = (struct PlttData *)&gPlttBufferFaded[i];
            r = faded->r - 2;
            g = faded->g - 2;
            b = faded->b - 2;

            if (r < r0)
                r = r0;
            if (g < g0)
                g = g0;
            if (b < b0)
                b = b0;

            gPlttBufferFaded[i] = RGB(r, g, b);
        }
        break;
    case FAST_FADE_OUT_TO_WHITE:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; i++)
        {
            struct PlttData *data = (struct PlttData *)&gPlttBufferFaded[i];
            r = data->r + 2;
            g = data->g + 2;
            b = data->b + 2;

            if (r > 31)
                r = 31;
            if (g > 31)
                g = 31;
            if (b > 31)
                b = 31;

            gPlttBufferFaded[i] = RGB(r, g, b);
        }
        break;
    case FAST_FADE_IN_FROM_BLACK:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; i++)
        {
            struct PlttData *unfaded;
            struct PlttData *faded;

            unfaded = (struct PlttData *)&gPlttBufferUnfaded[i];
            r0 = unfaded->r;
            g0 = unfaded->g;
            b0 = unfaded->b;

            faded = (struct PlttData *)&gPlttBufferFaded[i];
            r = faded->r + 2;
            g = faded->g + 2;
            b = faded->b + 2;

            if (r > r0)
                r = r0;
            if (g > g0)
                g = g0;
            if (b > b0)
                b = b0;

            gPlttBufferFaded[i] = RGB(r, g, b);
        }
        break;
    case FAST_FADE_OUT_TO_BLACK:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; i++)
        {
            struct PlttData *data = (struct PlttData *)&gPlttBufferFaded[i];
            r = data->r - 2;
            g = data->g - 2;
            b = data->b - 2;

            if (r < 0)
                r = 0;
            if (g < 0)
                g = 0;
            if (b < 0)
                b = 0;

            gPlttBufferFaded[i] = RGB(r, g, b);
        }
    }

    gPaletteFade.objPaletteToggle ^= 1;

    if (gPaletteFade.objPaletteToggle)
        return PALETTE_FADE_STATUS_ACTIVE;

    if (gPaletteFade.y - gPaletteFade.deltaY < 0)
        gPaletteFade.y = 0;
    else
        gPaletteFade.y -= gPaletteFade.deltaY;

    if (gPaletteFade.y == 0)
    {
        switch (gPaletteFade_submode)
        {
        case FAST_FADE_IN_FROM_WHITE:
        case FAST_FADE_IN_FROM_BLACK:
            CpuCopy32(gPlttBufferUnfaded, gPlttBufferFaded, PLTT_SIZE);
            break;
        case FAST_FADE_OUT_TO_WHITE:
            CpuFill32(0xFFFFFFFF, gPlttBufferFaded, PLTT_SIZE);
            break;
        case FAST_FADE_OUT_TO_BLACK:
            CpuFill32(0x00000000, gPlttBufferFaded, PLTT_SIZE);
            break;
        }

        gPaletteFade.mode = NORMAL_FADE;
        gPaletteFade.softwareFadeFinishingCounter = 1;
    }

    return PALETTE_FADE_STATUS_ACTIVE;
}

static inline u8 UpdateHardwarePaletteFade(void)
{
    if (!gPaletteFade.active)
        return PALETTE_FADE_STATUS_DONE;

    if (gPaletteFade.delayCounter < gPaletteFade_delay)
    {
        gPaletteFade.delayCounter++;
        return PALETTE_FADE_STATUS_DELAY;
    }

    gPaletteFade.delayCounter = 0;

    if (!gPaletteFade.yDec)
    {
        gPaletteFade.y++;
        if (gPaletteFade.y > gPaletteFade.targetY)
        {
            gPaletteFade.hardwareFadeFinishing++;
            gPaletteFade.y--;
        }
    }
    else
    {
        s32 y = gPaletteFade.y--;
        if (y - 1 < gPaletteFade.targetY)
        {
            gPaletteFade.hardwareFadeFinishing++;
            gPaletteFade.y++;
        }
    }

    if (gPaletteFade.hardwareFadeFinishing)
    {
        if (gPaletteFade.shouldResetBlendRegisters)
        {
            gPaletteFade_blendCnt = 0;
            gPaletteFade.y = 0;
        }
        gPaletteFade.shouldResetBlendRegisters = FALSE;
    }

    return PALETTE_FADE_STATUS_ACTIVE;
}

#endif // GUARD_PALETTE_INLINE_H
