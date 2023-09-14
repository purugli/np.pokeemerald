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

const union AnimCmd *const gBackAnims_4Frames[] =
{
    sAnim_GeneralFrame0,
    sAnimCmd_4Frames,
};

const union AnimCmd *const gBackAnims_5Frames[] =
{
    sAnim_GeneralFrame0,
    sAnimCmd_5Frames,
};
