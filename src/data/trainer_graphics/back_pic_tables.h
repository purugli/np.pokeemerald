#define TRAINER_BACK_PAL(trainerPic, pal) [TRAINER_BACK_PIC_##trainerPic] = pal

const u16 *const gTrainerBackPicPaletteTable[] =
{
    TRAINER_BACK_PAL(BRENDAN, gTrainerPalette_Brendan),
    TRAINER_BACK_PAL(MAY, gTrainerPalette_May),
    TRAINER_BACK_PAL(FRLG_RED, gTrainerBackPicPalette_FRLGRedLeaf),
    TRAINER_BACK_PAL(FRLG_LEAF, gTrainerBackPicPalette_FRLGRedLeaf),
    TRAINER_BACK_PAL(RUBY_SAPPHIRE_BRENDAN, gTrainerPalette_RubySapphireBrendan),
    TRAINER_BACK_PAL(RUBY_SAPPHIRE_MAY, gTrainerPalette_RubySapphireMay),
    TRAINER_BACK_PAL(WALLY, gTrainerPalette_Wally),
    TRAINER_BACK_PAL(STEVEN, gTrainerPalette_Steven),
    TRAINER_BACK_PAL(FRLG_POKEDUDE, gTrainerBackPicPalette_FRLGPokedude),
    TRAINER_BACK_PAL(FRLG_OLD_MAN, gTrainerBackPicPalette_FRLGOldMan),
};
