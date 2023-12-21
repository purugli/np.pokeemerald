#include "global.h"
#include "palette.h"
#include "util.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "task.h"
#include "constants/rgb.h"

enum
{
    NORMAL_FADE,
    FAST_FADE,
    HARDWARE_FADE,
};

static u8 UpdateNormalPaletteFade(void);
static void UpdateBlendRegisters(void);
static bool8 IsSoftwarePaletteFadeFinishing(void);
static void Task_BlendPalettesGradually(u8 taskId);

// palette buffers require alignment with agbcc because
// unaligned word reads are issued in BlendPalette otherwise
ALIGNED(4) EWRAM_DATA u16 gPlttBufferUnfaded[PLTT_BUFFER_SIZE] = {0};
ALIGNED(4) EWRAM_DATA u16 gPlttBufferFaded[PLTT_BUFFER_SIZE] = {0};
EWRAM_DATA struct PaletteFadeControl gPaletteFade = {0};
static EWRAM_DATA u32 sPlttBufferTransferPending = 0;
EWRAM_DATA u8 ALIGNED(2) gPaletteDecompressionBuffer[PLTT_SIZE] = {0};

static const u8 sRoundedDownGrayscaleMap[] = {
     0,  0,  0,  0,  0,
     5,  5,  5,  5,  5,
    11, 11, 11, 11, 11,
    16, 16, 16, 16, 16,
    21, 21, 21, 21, 21,
    27, 27, 27, 27, 27,
    31, 31
};

void LoadCompressedPalette(const u32 *src, u16 offset, u16 size)
{
    LoadCompressedPalette_HandleDNSTint(src, offset, size, FALSE);
}

void LoadPalette(const void *src, u16 offset, u16 size)
{
    LoadPalette_HandleDNSTint(src, offset, size, FALSE);
}

void FillPalette(u16 value, u16 offset, u16 size)
{
    FillDNPlttBufferWithBlack(offset, size);
    CpuFill16(value, &gPlttBufferUnfaded[offset], size);
    CpuFill16(value, &gPlttBufferFaded[offset], size);
}

void TransferPlttBuffer(void)
{
    if (!gPaletteFade.bufferTransferDisabled)
    {
        void *src = gPlttBufferFaded;
        void *dest = (void *)PLTT;
        DmaCopy16(3, src, dest, PLTT_SIZE);
        sPlttBufferTransferPending = FALSE;
        if (gPaletteFade.mode == HARDWARE_FADE && gPaletteFade.active)
            UpdateBlendRegisters();
    }
}

u8 UpdatePaletteFade(void)
{
    u8 result;
    u8 dummy = 0;

    if (sPlttBufferTransferPending)
        return PALETTE_FADE_STATUS_LOADING;

    if (gPaletteFade.mode == NORMAL_FADE)
    {
        result = UpdateNormalPaletteFade();
    }
    else if (gPaletteFade.mode == FAST_FADE)
    {
        if (!gPaletteFade.active)
        {
            result = PALETTE_FADE_STATUS_DONE;
        }
        else
        {
            if (IsSoftwarePaletteFadeFinishing())
            {
                result = gPaletteFade.active;
            }
            else
            {
                u32 i;
                u16 paletteOffsetStart;
                u16 paletteOffsetEnd;
                s8 r0, g0, b0;
                s8 r, g, b;

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
                {
                    result = PALETTE_FADE_STATUS_ACTIVE;
                }
                else
                {
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

                    result = PALETTE_FADE_STATUS_ACTIVE;
                }
            }
        }
    }
    else
    {
        if (!gPaletteFade.active)
        {
            result = PALETTE_FADE_STATUS_DONE;
        }
        else
        {
            if (gPaletteFade.delayCounter < gPaletteFade_delay)
            {
                gPaletteFade.delayCounter++;
                result = PALETTE_FADE_STATUS_DELAY;
            }
            else
            {
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

                result = PALETTE_FADE_STATUS_ACTIVE;
            }
        }
    }

    sPlttBufferTransferPending = gPaletteFade.multipurpose1 | dummy;

    return result;
}

void ResetPaletteFade(void)
{
    gPaletteFade.multipurpose1 = 0;
    gPaletteFade.multipurpose2 = 0;
    gPaletteFade.delayCounter = 0;
    gPaletteFade.y = 0;
    gPaletteFade.targetY = 0;
    gPaletteFade.blendColor = 0;
    gPaletteFade.active = FALSE;
    gPaletteFade.yDec = 0;
    gPaletteFade.bufferTransferDisabled = FALSE;
    gPaletteFade.shouldResetBlendRegisters = FALSE;
    gPaletteFade.hardwareFadeFinishing = FALSE;
    gPaletteFade.softwareFadeFinishingCounter = 0;
    gPaletteFade.objPaletteToggle = 0;
    gPaletteFade.deltaY = 2;
}

bool8 BeginNormalPaletteFade(u32 selectedPalettes, s8 delay, u8 startY, u8 targetY, u16 blendColor)
{
    u16 denominator;

    if (delay >= 0)
    {
        u32 diff = startY - targetY;

        if (startY < targetY)
            diff = targetY - startY;

        if (delay > 63)
            delay = 63;
        denominator = ((diff + 1) / 2) * (delay + 2) + 1;
    }
    else
    {
        u32 i;
        u8 deltaY;
        s8 y;

        if (delay < -14)
            delay = -14;

        deltaY = 2 + (delay * -1);
        y = startY;

        if (y < targetY)
        {
            for (i = 0; y < targetY; i++)
                y += deltaY;
        }
        else
        {
            for (i = 0; y > targetY; i++)
                y -= deltaY;
        }

        denominator = i * 2 + 1;
    }

    if (gPaletteFade.active)
    {
        return FALSE;
    }
    else
    {
        u8 temp;

        startY *= 2;
        targetY *= 2;
        gPaletteFade_selectedPalettes = selectedPalettes;
        gPaletteFade.y = startY * denominator;
        gPaletteFade.targetY = targetY;
        gPaletteFade.denominator = denominator;
        gPaletteFade.blendColor = blendColor;
        gPaletteFade.active = TRUE;
        gPaletteFade.mode = NORMAL_FADE;
        gPaletteFade.objPaletteToggle = 0;
        gPaletteFade.yChanged = TRUE;
        gPaletteFade.doEndDelay = TRUE;

        if (startY < targetY)
        {
            gPaletteFade.deltaY = targetY - startY;
            gPaletteFade.yDec = 0;
        }
        else
        {
            gPaletteFade.deltaY = startY - targetY;
            gPaletteFade.yDec = 1;
        }

        UpdatePaletteFade();

        temp = gPaletteFade.bufferTransferDisabled;
        gPaletteFade.bufferTransferDisabled = FALSE;
        CpuCopy32(gPlttBufferFaded, (void *)PLTT, PLTT_SIZE);
        sPlttBufferTransferPending = FALSE;
        if (gPaletteFade.mode == HARDWARE_FADE && gPaletteFade.active)
            UpdateBlendRegisters();
        gPaletteFade.bufferTransferDisabled = temp;
        return TRUE;
    }
}

static void BlendPalettesFine(u32 selectedPalettes, u32 coeff, u32 blendColor)
{
    if (selectedPalettes)
    {
        s32 newR = (blendColor << 27) >> 27;
        s32 newG = (blendColor << 22) >> 27;
        s32 newB = (blendColor << 17) >> 27;

        u16 *palDataSrc = gPlttBufferUnfaded;
        u16 *palDataDst = gPlttBufferFaded;

        do 
        {
            if (selectedPalettes & 1)
            {
                u16 *palDataSrcEnd = &palDataSrc[16];
                while (palDataSrc != palDataSrcEnd)
                {
                    u32 palDataSrcColor = *palDataSrc;

                    s32 r = (palDataSrcColor << 27) >> 27;
                    s32 g = (palDataSrcColor << 22) >> 27;
                    s32 b = (palDataSrcColor << 17) >> 27;

                    *palDataDst = RGB(r + (((newR - r) * coeff) >> 5),
                                      g + (((newG - g) * coeff) >> 5),
                                      b + (((newB - b) * coeff) >> 5));

                    palDataSrc++;
                    palDataDst++;
                }
            }
            else
            {
                palDataSrc += 16;
                palDataDst += 16;
            }
            selectedPalettes >>= 1;
        } while (selectedPalettes);
    }
}

static u8 UpdateNormalPaletteFade(void)
{
    if (!gPaletteFade.active)
        return PALETTE_FADE_STATUS_DONE;

    if (IsSoftwarePaletteFadeFinishing())
    {
        return gPaletteFade.active;
    }
    else
    {
        u16 targetY = gPaletteFade.targetY * gPaletteFade.denominator;
        u16 yWholeValue = gPaletteFade.y / gPaletteFade.denominator;
        if (gPaletteFade.yChanged)
            BlendPalettesFine(gPaletteFade_selectedPalettes, yWholeValue, gPaletteFade.blendColor);

        if (gPaletteFade.y == targetY)
        {
            gPaletteFade_selectedPalettes = 0;
            gPaletteFade.softwareFadeFinishingCounter = 1;
        }
        else
        {
            u16 newY;

            if (!gPaletteFade.yDec)
            {
                newY = gPaletteFade.y + gPaletteFade.deltaY;

                if (newY > targetY)
                    newY = targetY;
            }
            else
            {
                newY = gPaletteFade.y - gPaletteFade.deltaY;

                if (newY < targetY)
                    newY = targetY;
            }

            gPaletteFade.yChanged = (newY / gPaletteFade.denominator) != yWholeValue;
            gPaletteFade.y = newY;
        }

        return PALETTE_FADE_STATUS_ACTIVE;
    }
}

void InvertPlttBuffer(u32 selectedPalettes)
{
    u16 paletteOffset = 0;

    while (selectedPalettes)
    {
        if (selectedPalettes & 1)
        {
            u32 i;
            for (i = 0; i < 16; i++)
                gPlttBufferFaded[paletteOffset + i] = ~gPlttBufferFaded[paletteOffset + i];
        }
        selectedPalettes >>= 1;
        paletteOffset += 16;
    }
}

void BeginFastPaletteFade(u8 submode)
{
    gPaletteFade.deltaY = 2;
    gPaletteFade.y = 31;
    gPaletteFade_submode = submode & 0x3F;
    gPaletteFade.active = TRUE;
    gPaletteFade.mode = FAST_FADE;
    gPaletteFade.doEndDelay = TRUE;

    if (submode == FAST_FADE_IN_FROM_BLACK)
        CpuFill16(RGB_BLACK, gPlttBufferFaded, PLTT_SIZE);

    if (submode == FAST_FADE_IN_FROM_WHITE)
        CpuFill16(RGB_WHITE, gPlttBufferFaded, PLTT_SIZE);

    UpdatePaletteFade();
}

void BeginHardwarePaletteFade(u8 blendCnt, u8 delay, u8 y, u8 targetY, u8 shouldResetBlendRegisters)
{
    gPaletteFade_blendCnt = blendCnt;
    gPaletteFade.delayCounter = delay;
    gPaletteFade_delay = delay;
    gPaletteFade.y = y;
    gPaletteFade.targetY = targetY;
    gPaletteFade.active = TRUE;
    gPaletteFade.mode = HARDWARE_FADE;
    gPaletteFade.shouldResetBlendRegisters = shouldResetBlendRegisters & 1;
    gPaletteFade.hardwareFadeFinishing = FALSE;

    if (y < targetY)
        gPaletteFade.yDec = 0;
    else
        gPaletteFade.yDec = 1;
}

static void UpdateBlendRegisters(void)
{
    SetGpuReg(REG_OFFSET_BLDCNT, (u16)gPaletteFade_blendCnt);
    SetGpuReg(REG_OFFSET_BLDY, gPaletteFade.y);
    if (gPaletteFade.hardwareFadeFinishing)
    {
        gPaletteFade.hardwareFadeFinishing = FALSE;
        gPaletteFade.mode = 0;
        gPaletteFade_blendCnt = 0;
        gPaletteFade.y = 0;
        gPaletteFade.active = FALSE;
    }
}

static bool8 IsSoftwarePaletteFadeFinishing(void)
{
    if (gPaletteFade.softwareFadeFinishingCounter > 0)
    {
        if (gPaletteFade.doEndDelay == FALSE || gPaletteFade.softwareFadeFinishingCounter == 5)
        {
            gPaletteFade.active = FALSE;
            gPaletteFade.softwareFadeFinishingCounter = 0;
        }
        else
        {
            gPaletteFade.softwareFadeFinishingCounter++;
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void BlendPalettes(u32 selectedPalettes, u8 coeff, u16 color)
{
    BlendPalettesFine(selectedPalettes, coeff * 2, color);
}

void BlendPalettesUnfaded(u32 selectedPalettes, u8 coeff, u16 color)
{
    void *src = gPlttBufferUnfaded;
    void *dest = gPlttBufferFaded;
    DmaCopy32(3, src, dest, PLTT_SIZE);
    BlendPalettes(selectedPalettes, coeff, color);
}

static void TintPalette_GrayScaleRoundDown(u16 *palette, u16 count, bool32 roundDown)
{
    s32 r, g, b, i;
    u32 gray;

    for (i = 0; i < count; i++)
    {
        r = GET_R(*palette);
        g = GET_G(*palette);
        b = GET_B(*palette);

        gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;

        if (roundDown)
        {
            if (gray > 31)
                gray = 31;

            gray = sRoundedDownGrayscaleMap[gray];
        }
        *palette++ = RGB2(gray, gray, gray);
    }
}

void TintPalette_GrayScale(u16 *palette, u16 count)
{
    TintPalette_GrayScaleRoundDown(palette, count, FALSE);
}

void TintPalette_GrayScale2(u16 *palette, u16 count)
{
    TintPalette_GrayScaleRoundDown(palette, count, TRUE);
}

void TintPalette_SepiaTone(u16 *palette, u16 count)
{
    s32 r, g, b, i;
    u32 gray;

    for (i = 0; i < count; i++)
    {
        r = GET_R(*palette);
        g = GET_G(*palette);
        b = GET_B(*palette);

        gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;

        r = (u16)((Q_8_8(1.2) * gray)) >> 8;
        g = (u16)((Q_8_8(1.0) * gray)) >> 8;
        b = (u16)((Q_8_8(0.94) * gray)) >> 8;

        if (r > 31)
            r = 31;

        *palette++ = RGB2(r, g, b);
    }
}

void TintPalette_CustomTone(u16 *palette, u16 count, u16 rTone, u16 gTone, u16 bTone)
{
    s32 r, g, b, i;
    u32 gray;

    for (i = 0; i < count; i++)
    {
        r = GET_R(*palette);
        g = GET_G(*palette);
        b = GET_B(*palette);

        gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;

        r = (u16)((rTone * gray)) >> 8;
        g = (u16)((gTone * gray)) >> 8;
        b = (u16)((bTone * gray)) >> 8;

        if (r > 31)
            r = 31;
        if (g > 31)
            g = 31;
        if (b > 31)
            b = 31;

        *palette++ = RGB2(r, g, b);
    }
}

#define tCoeff       data[0]
#define tCoeffTarget data[1]
#define tCoeffDelta  data[2]
#define tDelay       data[3]
#define tDelayTimer  data[4]
#define tPalettes    5 // data[5] and data[6], set/get via Set/GetWordTaskArg
#define tColor       data[7]
#define tId          data[8]

// Blend the selected palettes in a series of steps toward or away from the color.
// Only used by the Groudon/Kyogre fight scene to flash the screen for lightning.
// One call is used to fade the bg from white, while another fades the duo from black
void BlendPalettesGradually(u32 selectedPalettes, s8 delay, u8 coeff, u8 coeffTarget, u16 color, u8 priority, u8 id)
{
    u8 taskId;

    taskId = CreateTask((void *)Task_BlendPalettesGradually, priority);
    gTasks[taskId].tCoeff = coeff;
    gTasks[taskId].tCoeffTarget = coeffTarget;

    if (delay >= 0)
    {
        gTasks[taskId].tDelay = delay;
        gTasks[taskId].tCoeffDelta = 1;
    }
    else
    {
        gTasks[taskId].tDelay = 0;
        gTasks[taskId].tCoeffDelta = -delay + 1;
    }

    if (coeffTarget < coeff)
        gTasks[taskId].tCoeffDelta *= -1;

    SetWordTaskArg(taskId, tPalettes, selectedPalettes);
    gTasks[taskId].tColor = color;
    gTasks[taskId].tId = id;
    gTasks[taskId].func(taskId);
}

static void Task_BlendPalettesGradually(u8 taskId)
{
    u32 palettes;
    s16 *data;
    s16 target;

    data = gTasks[taskId].data;
    palettes = GetWordTaskArg(taskId, tPalettes);

    if (++tDelayTimer > tDelay)
    {
        tDelayTimer = 0;
        BlendPalettes(palettes, tCoeff, tColor);
        target = tCoeffTarget;
        if (tCoeff == target)
        {
            DestroyTask(taskId);
        }
        else
        {
            tCoeff += tCoeffDelta;
            if (tCoeffDelta >= 0)
            {
                if (tCoeff < target)
                    return;
            }
            else if (tCoeff > target)
            {
                return;
            }
            tCoeff = target;
        }
    }
}
