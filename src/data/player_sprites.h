static const u8 sPlayerSprite_RegionMapIcon_BrendanMayGfx[] = INCBIN_U8("graphics/pokenav/region_map/brendan_icon.4bpp",
                                                                        "graphics/pokenav/region_map/may_icon.4bpp");
static const u8 sPlayerSprite_RegionMapIcon_RSBrendanRSMayGfx[] = INCBIN_U8("graphics/pokenav/region_map/rs_brendan_icon.4bpp",
                                                                            "graphics/pokenav/region_map/rs_may_icon.4bpp");
static const u8 sPlayerSprite_RegionMapIcon_FRLGRedFRLGLeafGfx[] = INCBIN_U8("graphics/pokenav/region_map/frlg_red_icon.4bpp",
                                                                             "graphics/pokenav/region_map/frlg_leaf_icon.4bpp");

static const u32 sPlayerSprite_FrontierPassIcon_BrendanMayGfx[] = INCBIN_U32("graphics/frontier_pass/brendan_may_icons.4bpp.lz");
static const u32 sPlayerSprite_FrontierPassIcon_RSBrendanRSMayGfx[] = INCBIN_U32("graphics/frontier_pass/rs_brendan_may_icons.4bpp.lz");
static const u32 sPlayerSprite_FrontierPassIcon_FRLGRedFRLGLeafGfx[] = INCBIN_U32("graphics/frontier_pass/frlg_red_leaf_icons.4bpp.lz");

#define OBJ_EVENT_GFX_RS_BRENDAN_UNDERWATER OBJ_EVENT_GFX_BRENDAN_UNDERWATER

#define TRAINER_BACK_PIC_RS_BRENDAN TRAINER_BACK_PIC_RUBY_SAPPHIRE_BRENDAN
#define TRAINER_BACK_PIC_RS_MAY TRAINER_BACK_PIC_RUBY_SAPPHIRE_MAY

#define PLAYER_TRAINER_GFX(male, female)                                               \
{                                                                                      \
    [PLAYER_TRAINER_BACK_PIC]  = {TRAINER_BACK_PIC_##male, TRAINER_BACK_PIC_##female}, \
    [PLAYER_TRAINER_FRONT_PIC] = {TRAINER_PIC_##male,      TRAINER_PIC_##female}       \
}

#define PLAYER_AVATAR_STATE_GFX(male, female)                                                                   \
{                                                                                                               \
    [PLAYER_AVATAR_STATE_NORMAL]     = {OBJ_EVENT_GFX_##male##_NORMAL,     OBJ_EVENT_GFX_##female##_NORMAL},    \
    [PLAYER_AVATAR_STATE_BIKE]       = {OBJ_EVENT_GFX_##male##_BIKE,       OBJ_EVENT_GFX_##female##_BIKE},      \
    [PLAYER_AVATAR_STATE_SURFING]    = {OBJ_EVENT_GFX_##male##_SURFING,    OBJ_EVENT_GFX_##female##_SURFING},   \
    [PLAYER_AVATAR_STATE_UNDERWATER] = {OBJ_EVENT_GFX_##male##_UNDERWATER, OBJ_EVENT_GFX_##female##_UNDERWATER} \
}

#define PLAYER_AVATAR_ANIM_GFX(male, female)                                                                                 \
{                                                                                                                            \
    [PLAYER_AVATAR_GFX_FIELD_MOVE]      = {OBJ_EVENT_GFX_##male##_FIELD_MOVE,      OBJ_EVENT_GFX_##female##_FIELD_MOVE},     \
    [PLAYER_AVATAR_GFX_FISHING]         = {OBJ_EVENT_GFX_##male##_FISHING,         OBJ_EVENT_GFX_##female##_FISHING},        \
    [PLAYER_AVATAR_GFX_WATERING]        = {OBJ_EVENT_GFX_##male##_WATERING,        OBJ_EVENT_GFX_##female##_WATERING},       \
    [PLAYER_AVATAR_GFX_DECORATING]      = {OBJ_EVENT_GFX_##male##_DECORATING,      OBJ_EVENT_GFX_##female##_DECORATING},     \
    [PLAYER_AVATAR_GFX_FIELD_MOVE_BIKE] = {OBJ_EVENT_GFX_##male##_FIELD_MOVE_BIKE, OBJ_EVENT_GFX_##female##_FIELD_MOVE_BIKE} \
}

#define REGION_MAP_ICONS(male, female)                                  \
{                                                                       \
    [MALE]   = sPlayerSprite_RegionMapIcon_##male####female##Gfx,       \
    [FEMALE] = sPlayerSprite_RegionMapIcon_##male####female##Gfx + 0x80 \
}

#define PLAYER_SPRITE(version, male, female, malePtr, femalePtr)                        \
    [PLAYER_##version] =                                                                \
    {                                                                                   \
        .trainerPics = PLAYER_TRAINER_GFX(male, female),                                \
        .avatarGraphics = PLAYER_AVATAR_STATE_GFX(male, female),                        \
        .avatarAnimGraphics = PLAYER_AVATAR_ANIM_GFX(male, female),                     \
        .regionMapIcons = REGION_MAP_ICONS(malePtr, femalePtr),                         \
        .frontierPassIcons = sPlayerSprite_FrontierPassIcon_##malePtr####femalePtr##Gfx \
    }

const struct PlayerSprite gPlayerSpriteTable[PLAYER_VERSION_COUNT] =
{
    PLAYER_SPRITE(EMERALD, BRENDAN, MAY, Brendan, May),
    PLAYER_SPRITE(RS, BRENDAN, MAY, RSBrendan, RSMay),
    PLAYER_SPRITE(FRLG, FRLG_RED, FRLG_LEAF, FRLGRed, FRLGLeaf),
};
