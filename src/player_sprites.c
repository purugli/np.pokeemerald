#include "global.h"
#include "data.h"
#include "event_object_movement.h"
#include "frontier_pass.h"
#include "graphics.h"
#include "link.h"
#include "region_map.h"
#include "constants/event_objects.h"
#include "constants/trainers.h"

#include "data/player_sprites.h"

void SetPlayerAnimation(u16 graphicsId, u8 animNum)
{
    ObjectEventSetGraphicsId(&gObjectEvents[gPlayerAvatar.objectEventId], graphicsId);
    StartSpriteAnim(&gSprites[gPlayerAvatar.spriteId], animNum);
}

void SetPlayerAvatarAnimation(u8 playerAnimId, u8 animNum)
{
    SetPlayerAnimation(PlayerSprites_GetAvatarAnimGraphics(PLAYER_VERSION, playerAnimId, gSaveBlock2Ptr->playerGender), animNum);
}

const u16 *GetObjectEventPaletteFromGraphicsId(u16 graphicsId)
{
    u32 paletteIndex = FindObjectEventPaletteIndexByTag(GetObjectEventGraphicsInfo(graphicsId)->paletteTag);
    return gObjectEventSpritePalettes[paletteIndex].data;
}

// Link player sprite functions

u32 GetLinkPlayerVersionId(u16 linkVersion)
{
    // Because a player's link version value is gGameVersion + 0x4000,
    // we need to mask to get the correct value.
    // (u8)linkVersion should also work.
    u8 actualVersion = linkVersion & 0xFF;

    if (actualVersion <= VERSION_RUBY)
        return PLAYER_RS;
    else if (actualVersion >= VERSION_FIRE_RED)
        return PLAYER_FRLG;
    else
        return PLAYER_EMERALD;
}

u16 GetLinkPlayerAvatarGraphics(u16 version, u8 gender)
{
    return PlayerSprites_GetAvatarGraphics(GetLinkPlayerVersionId(version), PLAYER_AVATAR_STATE_NORMAL, gender);
}

u16 GetLinkPlayerBackTrainerPic(u8 multiplayerId)
{
    struct LinkPlayer *linkPlayer = &gLinkPlayers[multiplayerId];
    return PlayerSprites_GetTrainerPic(GetLinkPlayerVersionId(linkPlayer->version), PLAYER_TRAINER_BACK_PIC, linkPlayer->gender);
}

u16 GetLinkPlayerFrontTrainerPic(u8 multiplayerId)
{
    struct LinkPlayer *linkPlayer = &gLinkPlayers[multiplayerId];
    return PlayerSprites_GetTrainerPic(GetLinkPlayerVersionId(linkPlayer->version), PLAYER_TRAINER_FRONT_PIC, linkPlayer->gender);
}
