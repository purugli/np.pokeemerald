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
    TIME_OF_DAY_FADE,
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
    LZ77UnCompWram(src, gPaletteDecompressionBuffer);
    LoadPalette(gPaletteDecompressionBuffer, offset, size);
}

void LoadPalette(const void *src, u16 offset, u16 size)
{
    CpuCopy16(src, &gPlttBufferUnfaded[offset], size);
    CpuCopy16(&gPlttBufferUnfaded[offset], &gPlttBufferFaded[offset], size);
}

// Drop in replacement for LoadPalette, uses CpuFastCopy, size must be 0 % 32
void LoadPaletteFast(const void *src, u16 offset, u16 size)
{
    if ((u32)src & 3) // In case palette is not 4 byte aligned
    {
        LoadPalette(src, offset, size);
    }
    else
    {
        CpuFastCopy(src, &gPlttBufferUnfaded[offset], size);
        // Copying from EWRAM->EWRAM is faster than ROM->EWRAM
        CpuFastCopy(&gPlttBufferUnfaded[offset], &gPlttBufferFaded[offset], size);
    }
}

void FillPalette(u16 value, u16 offset, u16 size)
{
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

#include "palette_inline.h"

u32 UpdatePaletteFade(void)
{
    u32 result;

    if (sPlttBufferTransferPending)
        return PALETTE_FADE_STATUS_LOADING;

    if (gPaletteFade.mode == NORMAL_FADE)
        result = UpdateNormalPaletteFade();
    else if (gPaletteFade.mode == FAST_FADE)
        result = UpdateFastPaletteFade();
    else if (gPaletteFade.mode == TIME_OF_DAY_FADE)
        result = UpdateTimeOfDayPaletteFade();
    else
        result = UpdateHardwarePaletteFade();

    sPlttBufferTransferPending = gPaletteFade.multipurpose1;

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

void BeginNormalPaletteFade(u32 selectedPalettes, s8 delay, u8 startY, u8 targetY, u16 blendColor)
{
    // all this is to make fades more smooth
    // while keeping the same delay as vanilla
    // in case some code relies on timing
    u32 denominator;
    bool16 savedBufferTransferDisabled;

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
        u32 i, deltaY;
        s32 y;

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

    if (!gPaletteFade.active)
    {
        startY <<= 1;
        targetY <<= 1;
        gPaletteFade_selectedPalettes = selectedPalettes;
        gPaletteFade.y = startY * denominator;
        gPaletteFade.targetY = targetY;
        gPaletteFade_denominator = denominator;
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

        savedBufferTransferDisabled = gPaletteFade.bufferTransferDisabled;
        gPaletteFade.bufferTransferDisabled = FALSE;
        CpuCopy32(gPlttBufferFaded, (void *)PLTT, PLTT_SIZE);
        sPlttBufferTransferPending = FALSE;
        gPaletteFade.bufferTransferDisabled = savedBufferTransferDisabled;
    }
}

static u8 UpdateNormalPaletteFade(void)
{
    u32 targetY;

    if (gPaletteFade.active && IsSoftwarePaletteFadeFinishing() == FALSE)
    {
        u32 yWholeValue = gPaletteFade.y / gPaletteFade_denominator;

        if (gPaletteFade.yChanged)
            BlendPalettesFine(gPaletteFade_selectedPalettes, gPlttBufferUnfaded, gPlttBufferFaded, yWholeValue / 2, gPaletteFade.blendColor);

        targetY = gPaletteFade.targetY * gPaletteFade_denominator;

        if (gPaletteFade.y == targetY)
        {
            gPaletteFade_selectedPalettes = 0;
            gPaletteFade.softwareFadeFinishingCounter = 1;
        }
        else
        {
            u32 newY;

            if (!gPaletteFade.yDec)
            {
                newY = gPaletteFade.y + gPaletteFade.deltaY;
                if (newY > targetY)
                    newY = targetY;
                gPaletteFade.yChanged = (newY / gPaletteFade_denominator) != yWholeValue;
                gPaletteFade.y = newY;
            }
            else
            {
                newY = gPaletteFade.y - gPaletteFade.deltaY;
                if (newY < targetY)
                    newY = targetY;
                gPaletteFade.yChanged = (newY / gPaletteFade_denominator) != yWholeValue;
                gPaletteFade.y = newY;
            }
        }
    }
    return gPaletteFade.active;
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

void BeginFastPaletteFade(u32 submode)
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

void BeginHardwarePaletteFade(u32 blendCnt, u32 delay, u32 y, u32 targetY, u32 shouldResetBlendRegisters)
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

// Like normal palette fade but respects sprite/tile palettes immune to time of day fading
void BeginTimeOfDayPaletteFade(u32 selectedPalettes, s8 delay, u8 startY, u8 targetY, u16 blendColor)
{
    bool16 savedBufferTransferDisabled;

    if (!gPaletteFade.active)
    {
        gPaletteFade.deltaY = 2;

        if (delay < 0)
        {
            gPaletteFade.deltaY += (delay * -1);
            delay = 0;
        }

        gPaletteFade_selectedPalettes = selectedPalettes;
        gPaletteFade.delayCounter = delay;
        gPaletteFade_delay = delay;
        gPaletteFade.y = startY;
        gPaletteFade.targetY = targetY;
        gPaletteFade.blendColor = blendColor;
        gPaletteFade.active = TRUE;
        gPaletteFade.mode = TIME_OF_DAY_FADE;

        if (startY < targetY)
            gPaletteFade.yDec = 0;
        else
            gPaletteFade.yDec = 1;

        UpdatePaletteFade();

        savedBufferTransferDisabled = gPaletteFade.bufferTransferDisabled;
        gPaletteFade.bufferTransferDisabled = FALSE;
        CpuCopy32(gPlttBufferFaded, (void *)PLTT, PLTT_SIZE);
        sPlttBufferTransferPending = FALSE;
        gPaletteFade.bufferTransferDisabled = savedBufferTransferDisabled;
    }
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
    BlendPalettesFine(selectedPalettes, gPlttBufferUnfaded, gPlttBufferFaded, coeff, color);
}

// Optimized based on lucktyphlosion's BlendPalettesFine
void BlendPalettesFine(u32 selectedPalettes, u16 *palDataSrc, u16 *palDataDst, u32 coeff, u32 blendColor)
{
    s32 newR, newG, newB;

    if (selectedPalettes)
    {
        coeff <<= 1;
        newR = (blendColor << 27) >> 27;
        newG = (blendColor << 22) >> 27;
        newB = (blendColor << 17) >> 27;

        do
        {
            if (selectedPalettes & 1)
            {
                u16 *palDataSrcEnd = &palDataSrc[16];
                while (palDataSrc != palDataSrcEnd) // Transparency is blended (for backdrop reasons)
                {
                    u32 palDataSrcColor = *palDataSrc;
                    s32 r = (palDataSrcColor << 27) >> 27;
                    s32 g = (palDataSrcColor << 22) >> 27;
                    s32 b = (palDataSrcColor << 17) >> 27;

                    *palDataDst++ = RGB(r + (((newR - r) * (s32)coeff) >> 5),
                                        g + (((newG - g) * (s32)coeff) >> 5),
                                        b + (((newB - (b & 31)) * (s32)coeff) >> 5));
                    palDataSrc++;
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
