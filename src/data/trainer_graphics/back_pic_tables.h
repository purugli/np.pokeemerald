#define TRAINER_BACK_SPRITE(id, anim, pic, pal) \
    [TRAINER_BACK_PIC_##id] =                   \
    {                                           \
        .anims = sBackAnims_##anim,             \
        .images = sTrainerBackPicTable_##pic,   \
        .palette = gTrainer##pal,               \
    }

static const union AnimCmd sAnimCmd_4Frames[] =
{
    ANIMCMD_FRAME(1, 24),
    ANIMCMD_FRAME(2, 9),
    ANIMCMD_FRAME(3, 24),
    ANIMCMD_FRAME(1, 9),
    ANIMCMD_FRAME(0, 50),
    ANIMCMD_END,
};

static const union AnimCmd sAnimCmd_5Frames[] =
{
    ANIMCMD_FRAME(1, 20),
    ANIMCMD_FRAME(2, 6),
    ANIMCMD_FRAME(3, 6),
    ANIMCMD_FRAME(4, 24),
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sBackAnims_4Frames[] =
{
    sAnim_GeneralFrame0,
    sAnimCmd_4Frames,
};

static const union AnimCmd *const sBackAnims_5Frames[] =
{
    sAnim_GeneralFrame0,
    sAnimCmd_5Frames,
};

const struct TrainerBackPic gTrainerBackPicTable[] =
{
    TRAINER_BACK_SPRITE(BRENDAN, 4Frames, Brendan, Palette_Brendan),
    TRAINER_BACK_SPRITE(MAY, 4Frames, May, Palette_May),
    TRAINER_BACK_SPRITE(FRLG_RED, 5Frames, FRLGRed, BackPicPalette_FRLGRedLeaf),
    TRAINER_BACK_SPRITE(FRLG_LEAF, 5Frames, FRLGLeaf, BackPicPalette_FRLGRedLeaf),
    TRAINER_BACK_SPRITE(RUBY_SAPPHIRE_BRENDAN, 4Frames, RubySapphireBrendan, Palette_RubySapphireBrendan),
    TRAINER_BACK_SPRITE(RUBY_SAPPHIRE_MAY, 4Frames, RubySapphireMay, Palette_RubySapphireMay),
    TRAINER_BACK_SPRITE(WALLY, 4Frames, Wally, Palette_Wally),
    TRAINER_BACK_SPRITE(STEVEN, 4Frames, Steven, Palette_Steven),
    TRAINER_BACK_SPRITE(FRLG_POKEDUDE, 4Frames, FRLGPokedude, BackPicPalette_FRLGPokedude),
    TRAINER_BACK_SPRITE(FRLG_OLD_MAN, 4Frames, FRLGOldMan, BackPicPalette_FRLGOldMan),
};
