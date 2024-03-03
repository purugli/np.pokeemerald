#ifndef GUARD_PLAYER_SPRITES_H
#define GUARD_PLAYER_SPRITES_H

enum
{
    PLAYER_TRAINER_BACK_PIC,
    PLAYER_TRAINER_FRONT_PIC,
    PLAYER_TRAINER_PIC_COUNT,
};

struct PlayerSprite
{
    // Trainer front and back pic index (see include/constants/trainers.h)
    u16 trainerPics[PLAYER_TRAINER_PIC_COUNT][GENDER_COUNT];

    // Overworld avatars, consisting of: walking, cycling,
    // surfing, and underwater (see include/constants/event_object.h)
    u16 avatarGraphics[PLAYER_AVATAR_STATE_COUNT - 3][GENDER_COUNT];

    // Overworld anims, consisting of: field move, fishing,
    // water, and decorating (see include/constants/event_object.h)
    u16 avatarAnimGraphics[PLAYER_AVATAR_GFX_COUNT][GENDER_COUNT];

    // Head icons for region map
    const void *regionMapIcons[GENDER_COUNT];

    // Head icons for frontier pass map.
    // Note that frontier pass needs to be in one sprite instead of two, unlike region map
    const u32 *frontierPassIcons;
};

extern const struct PlayerSprite gPlayerSpriteTable[PLAYER_VERSION_COUNT];

void SetPlayerAnimation(u16 graphicsId, u8 animNum);
void SetPlayerAvatarAnimation(u8 playerAnimId, u8 animNum);
const u16 *GetObjectEventPaletteFromGraphicsId(u16 graphicsId);
u32 GetLinkPlayerVersionId(u16 linkVersion);
u16 GetLinkPlayerAvatarGraphics(u16 version, u8 gender);
u16 GetLinkPlayerFrontTrainerPic(u8 multiplayerId);
u16 GetLinkPlayerBackTrainerPic(u8 multiplayerId);

static inline u16 PlayerSprites_GetTrainerPic(u32 playerVersion, u8 which, u8 gender)
{
    return gPlayerSpriteTable[playerVersion].trainerPics[which][gender];
}

static inline u16 PlayerSprites_GetAvatarGraphics(u32 playerVersion, u8 state, u8 gender)
{
    return gPlayerSpriteTable[playerVersion].avatarGraphics[state][gender];
}

static inline u16 PlayerSprites_GetAvatarAnimGraphics(u32 playerVersion, u8 animId, u8 gender)
{
    return gPlayerSpriteTable[playerVersion].avatarAnimGraphics[animId][gender];
}

static inline const void *PlayerSprites_GetRegionMapIcons(u32 playerVersion, u8 gender)
{
    return gPlayerSpriteTable[playerVersion].regionMapIcons[gender];
}

static inline const u32 *PlayerSprites_GetFrontierPassIcons(u32 playerVersion)
{
    return gPlayerSpriteTable[playerVersion].frontierPassIcons;
}

#endif // GUARD_PLAYER_SPRITES_H
