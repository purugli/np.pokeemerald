#include "constants/day_night.h"

#define OVERRIDES_END { .slot = PALOVER_LIST_TERM }

static const struct PaletteOverride sTilesetPalOverrides_General[] =
{
    {
        .slot = 1,
        .startHour = NIGHT_HOUR_BEGIN,
        .endHour = NIGHT_HOUR_END,
        .palette = gTilesetPalettes_General[7],
    },
    {
        .slot = 5,
        .startHour = NIGHT_HOUR_BEGIN,
        .endHour = NIGHT_HOUR_END,
        .palette = gTilesetPalettes_General[7],
    },
    OVERRIDES_END
};

static const struct PaletteOverride sTilesetPalOverrides_Petalburg[] =
{
    {
        .slot = 6,
        .startHour = NIGHT_HOUR_BEGIN,
        .endHour = NIGHT_HOUR_END,
        .palette = gTilesetPalettes_Petalburg[0],
    },
    {
        .slot = 8,
        .startHour = NIGHT_HOUR_BEGIN,
        .endHour = NIGHT_HOUR_END,
        .palette = gTilesetPalettes_Petalburg[0],
    },
    OVERRIDES_END
};
