#ifndef GUARD_DAY_NIGHT_H
#define GUARD_DAY_NIGHT_H

#define PALOVER_LIST_TERM 0xFF

struct PaletteOverride
{
    u8 slot;
    u8 startHour;
    u8 endHour;
    const u16 *palette;
};

extern u16 ALIGNED(4) gPlttBufferPreDN[PLTT_BUFFER_SIZE];
extern const struct PaletteOverride *gPaletteOverrides[];

u32 GetTimeOfDay(void);
u8 LoadSpritePaletteWithDNSTint(const struct SpritePalette *palette);
void LoadCompressedPaletteWithDNSTint(const u32 *src, u32 offset, u32 size);
void LoadPaletteWithDNSTint(const void *src, u32 offset, u32 size);
void CheckClockForImmediateTimeEvents(void);
void ProcessImmediateTimeEvents(void);
void FillDNPlttBufferWithBlack(u32 offset, u16 size);
void LoadCompressedPalette_HandleDNSTint(const u32 *src, u16 offset, u16 size, bool32 applyDNSTint);
void LoadPalette_HandleDNSTint(const void *src, u16 offset, u16 size, bool32 applyDNSTint);
void LoadPaletteFast_HandleDNSTint(const void *src, u16 offset, u16 size, bool32 applyDNSTint);

#endif // GUARD_DAY_NIGHT_H
