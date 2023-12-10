static const u16 sTilesetPalOverride_General01_05[] = INCBIN_U16("data/tilesets/primary/general/override_palettes/01_05.gbapal");
static const u16 sTilesetPalOverride_Petalburg06_08[] = INCBIN_U16("data/tilesets/secondary/petalburg/override_palettes/06_08.gbapal");

#define OVERRIDES_END { .slot = PALOVER_LIST_TERM }

static const struct PaletteOverride sTilesetPalOverrides_General[] =
{
    {
        .slot = 1,
        .startHour = NIGHT_HOUR_BEGIN,
        .endHour = NIGHT_HOUR_END,
        .palette = sTilesetPalOverride_General01_05,
    },
    {
        .slot = 5,
        .startHour = NIGHT_HOUR_BEGIN,
        .endHour = NIGHT_HOUR_END,
        .palette = sTilesetPalOverride_General01_05,
    },
    OVERRIDES_END
};

static const struct PaletteOverride sTilesetPalOverrides_Petalburg[] =
{
    {
        .slot = 6,
        .startHour = NIGHT_HOUR_BEGIN,
        .endHour = NIGHT_HOUR_END,
        .palette = sTilesetPalOverride_Petalburg06_08,
    },
    {
        .slot = 8,
        .startHour = NIGHT_HOUR_BEGIN,
        .endHour = NIGHT_HOUR_END,
        .palette = sTilesetPalOverride_Petalburg06_08,
    },
    OVERRIDES_END
};
