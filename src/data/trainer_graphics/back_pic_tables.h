#define TRAINER_BACK_PAL(trainerPic, pal) [TRAINER_BACK_PIC_##trainerPic] = pal

const u16 *const gTrainerBackPicPaletteTable[] =
{
    TRAINER_BACK_PAL(BRENDAN, gTrainerPalette_Brendan),
    TRAINER_BACK_PAL(MAY, gTrainerPalette_May),
    TRAINER_BACK_PAL(RG_RED, gTrainerBackPicPalette_RG_RedLeaf),
    TRAINER_BACK_PAL(RG_LEAF, gTrainerBackPicPalette_RG_RedLeaf),
    TRAINER_BACK_PAL(RUBY_SAPPHIRE_BRENDAN, gTrainerPalette_RubySapphireBrendan),
    TRAINER_BACK_PAL(RUBY_SAPPHIRE_MAY, gTrainerPalette_RubySapphireMay),
    TRAINER_BACK_PAL(WALLY, gTrainerPalette_Wally),
    TRAINER_BACK_PAL(STEVEN, gTrainerPalette_Steven),
    TRAINER_BACK_PAL(RG_POKEDUDE, gTrainerBackPicPalette_RG_Pokedude),
    TRAINER_BACK_PAL(RG_OLD_MAN, gTrainerBackPicPalette_RG_OldMan),
};
