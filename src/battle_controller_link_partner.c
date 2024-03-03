#include "global.h"
#include "battle.h"
#include "battle_ai_script_commands.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_message.h"
#include "battle_setup.h"
#include "battle_tower.h"
#include "battle_tv.h"
#include "bg.h"
#include "data.h"
#include "link.h"
#include "main.h"
#include "m4a.h"
#include "palette.h"
#include "pokeball.h"
#include "pokemon.h"
#include "recorded_battle.h"
#include "reshow_battle_screen.h"
#include "sound.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "util.h"
#include "window.h"
#include "constants/battle_anim.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "recorded_battle.h"

static void LinkPartnerHandleGetMonData(void);
static void LinkPartnerHandleSetMonData(void);
static void LinkPartnerHandleSetRawMonData(void);
static void LinkPartnerHandleLoadMonSprite(void);
static void LinkPartnerHandleSwitchInAnim(void);
static void LinkPartnerHandleReturnMonToBall(void);
static void LinkPartnerHandleDrawTrainerPic(void);
static void LinkPartnerHandleTrainerSlideBack(void);
static void LinkPartnerHandleFaintAnimation(void);
static void LinkPartnerHandleMoveAnimation(void);
static void LinkPartnerHandlePrintString(void);
static void LinkPartnerHandleHealthBarUpdate(void);;
static void LinkPartnerHandleStatusIconUpdate(void);
static void LinkPartnerHandleStatusAnimation(void);
static void LinkPartnerHandleHitAnimation(void);
static void LinkPartnerHandlePlaySE(void);
static void LinkPartnerHandlePlayFanfareOrBGM(void);
static void LinkPartnerHandleFaintingCry(void);
static void LinkPartnerHandleIntroSlide(void);
static void LinkPartnerHandleIntroTrainerBallThrow(void);
static void LinkPartnerHandleDrawPartyStatusSummary(void);
static void LinkPartnerHandleHidePartyStatusSummary(void);
static void LinkPartnerHandleSpriteInvisibility(void);
static void LinkPartnerHandleBattleAnimation(void);
static void LinkPartnerHandleLinkStandbyMsg(void);
static void LinkPartnerHandleEndLinkBattle(void);

static void LinkPartnerBufferRunCommand(void);
static void LinkPartnerBufferExecCompleted(void);
static void SwitchIn_WaitAndEnd(void);
static void StartSendOutAnim(u8 battlerId, bool8 dontClearSubstituteBit);
static void DoSwitchOutAnimation(void);
static void LinkPartnerDoMoveAnimation(void);
static void Task_StartSendOutAnim(u8 taskId);
static void EndDrawPartyStatusSummary(void);

static void (*const sLinkPartnerBufferCommands[CONTROLLER_CMDS_COUNT])(void) =
{
    [CONTROLLER_GETMONDATA]               = LinkPartnerHandleGetMonData,
    [CONTROLLER_GETRAWMONDATA]            = LinkPartnerBufferExecCompleted,
    [CONTROLLER_SETMONDATA]               = LinkPartnerHandleSetMonData,
    [CONTROLLER_SETRAWMONDATA]            = LinkPartnerHandleSetRawMonData,
    [CONTROLLER_LOADMONSPRITE]            = LinkPartnerHandleLoadMonSprite,
    [CONTROLLER_SWITCHINANIM]             = LinkPartnerHandleSwitchInAnim,
    [CONTROLLER_RETURNMONTOBALL]          = LinkPartnerHandleReturnMonToBall,
    [CONTROLLER_DRAWTRAINERPIC]           = LinkPartnerHandleDrawTrainerPic,
    [CONTROLLER_TRAINERSLIDE]             = LinkPartnerBufferExecCompleted,
    [CONTROLLER_TRAINERSLIDEBACK]         = LinkPartnerHandleTrainerSlideBack,
    [CONTROLLER_FAINTANIMATION]           = LinkPartnerHandleFaintAnimation,
    [CONTROLLER_PALETTEFADE]              = LinkPartnerBufferExecCompleted,
    [CONTROLLER_SUCCESSBALLTHROWANIM]     = LinkPartnerBufferExecCompleted,
    [CONTROLLER_BALLTHROWANIM]            = LinkPartnerBufferExecCompleted,
    [CONTROLLER_PAUSE]                    = LinkPartnerBufferExecCompleted,
    [CONTROLLER_MOVEANIMATION]            = LinkPartnerHandleMoveAnimation,
    [CONTROLLER_PRINTSTRING]              = LinkPartnerHandlePrintString,
    [CONTROLLER_PRINTSTRINGPLAYERONLY]    = LinkPartnerBufferExecCompleted,
    [CONTROLLER_CHOOSEACTION]             = LinkPartnerBufferExecCompleted,
    [CONTROLLER_YESNOBOX]                 = LinkPartnerBufferExecCompleted,
    [CONTROLLER_CHOOSEMOVE]               = LinkPartnerBufferExecCompleted,
    [CONTROLLER_OPENBAG]                  = LinkPartnerBufferExecCompleted,
    [CONTROLLER_CHOOSEPOKEMON]            = LinkPartnerBufferExecCompleted,
    [CONTROLLER_23]                       = LinkPartnerBufferExecCompleted,
    [CONTROLLER_HEALTHBARUPDATE]          = LinkPartnerHandleHealthBarUpdate,
    [CONTROLLER_EXPUPDATE]                = LinkPartnerBufferExecCompleted,
    [CONTROLLER_STATUSICONUPDATE]         = LinkPartnerHandleStatusIconUpdate,
    [CONTROLLER_STATUSANIMATION]          = LinkPartnerHandleStatusAnimation,
    [CONTROLLER_STATUSXOR]                = LinkPartnerBufferExecCompleted,
    [CONTROLLER_DATATRANSFER]             = LinkPartnerBufferExecCompleted,
    [CONTROLLER_DMA3TRANSFER]             = LinkPartnerBufferExecCompleted,
    [CONTROLLER_PLAYBGM]                  = LinkPartnerBufferExecCompleted,
    [CONTROLLER_32]                       = LinkPartnerBufferExecCompleted,
    [CONTROLLER_TWORETURNVALUES]          = LinkPartnerBufferExecCompleted,
    [CONTROLLER_CHOSENMONRETURNVALUE]     = LinkPartnerBufferExecCompleted,
    [CONTROLLER_ONERETURNVALUE]           = LinkPartnerBufferExecCompleted,
    [CONTROLLER_ONERETURNVALUE_DUPLICATE] = LinkPartnerBufferExecCompleted,
    [CONTROLLER_CLEARUNKVAR]              = LinkPartnerBufferExecCompleted,
    [CONTROLLER_SETUNKVAR]                = LinkPartnerBufferExecCompleted,
    [CONTROLLER_CLEARUNKFLAG]             = LinkPartnerBufferExecCompleted,
    [CONTROLLER_TOGGLEUNKFLAG]            = LinkPartnerBufferExecCompleted,
    [CONTROLLER_HITANIMATION]             = LinkPartnerHandleHitAnimation,
    [CONTROLLER_CANTSWITCH]               = LinkPartnerBufferExecCompleted,
    [CONTROLLER_PLAYSE]                   = LinkPartnerHandlePlaySE,
    [CONTROLLER_PLAYFANFAREORBGM]         = LinkPartnerHandlePlayFanfareOrBGM,
    [CONTROLLER_FAINTINGCRY]              = LinkPartnerHandleFaintingCry,
    [CONTROLLER_INTROSLIDE]               = LinkPartnerHandleIntroSlide,
    [CONTROLLER_INTROTRAINERBALLTHROW]    = LinkPartnerHandleIntroTrainerBallThrow,
    [CONTROLLER_DRAWPARTYSTATUSSUMMARY]   = LinkPartnerHandleDrawPartyStatusSummary,
    [CONTROLLER_HIDEPARTYSTATUSSUMMARY]   = LinkPartnerHandleHidePartyStatusSummary,
    [CONTROLLER_ENDBOUNCE]                = LinkPartnerBufferExecCompleted,
    [CONTROLLER_SPRITEINVISIBILITY]       = LinkPartnerHandleSpriteInvisibility,
    [CONTROLLER_BATTLEANIMATION]          = LinkPartnerHandleBattleAnimation,
    [CONTROLLER_LINKSTANDBYMSG]           = LinkPartnerHandleLinkStandbyMsg,
    [CONTROLLER_RESETACTIONMOVESELECTION] = LinkPartnerBufferExecCompleted,
    [CONTROLLER_ENDLINKBATTLE]            = LinkPartnerHandleEndLinkBattle,
    [CONTROLLER_TERMINATOR_NOP]           = BattleControllerDummy
};

void SetControllerToLinkPartner(void)
{
    gBattlerControllerFuncs[gActiveBattler] = LinkPartnerBufferRunCommand;
}

static void LinkPartnerBufferRunCommand(void)
{
    if (gBattleControllerExecFlags & gBitTable[gActiveBattler])
    {
        if (gBattleBufferA[gActiveBattler][0] < ARRAY_COUNT(sLinkPartnerBufferCommands))
            sLinkPartnerBufferCommands[gBattleBufferA[gActiveBattler][0]]();
        else
            LinkPartnerBufferExecCompleted();
    }
}

static void CompleteOnBattlerSpriteCallbackDummy(void)
{
    if (gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
        LinkPartnerBufferExecCompleted();
}

static void FreeTrainerSpriteAfterSlide(void)
{
    if (gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
    {
        FreeSpriteOamMatrix(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        DestroySprite(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        LinkPartnerBufferExecCompleted();
    }
}

static void Intro_DelayAndEnd(void)
{
    if (--gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].introEndDelay == (u8)-1)
    {
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].introEndDelay = 0;
        LinkPartnerBufferExecCompleted();
    }
}

static void Intro_WaitForHealthbox(void)
{
    bool32 finished = FALSE;

    if (!IsDoubleBattle() || (IsDoubleBattle() && (gBattleTypeFlags & BATTLE_TYPE_MULTI)))
    {
        if (gSprites[gHealthboxSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
            finished = TRUE;
    }
    else
    {
        if (gSprites[gHealthboxSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy
            && gSprites[gHealthboxSpriteIds[BATTLE_PARTNER(gActiveBattler)]].callback == SpriteCallbackDummy)
        {
            finished = TRUE;
        }
    }

    if (IsCryPlayingOrClearCrySongs())
        finished = FALSE;

    if (finished)
    {
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].introEndDelay = 3;
        gBattlerControllerFuncs[gActiveBattler] = Intro_DelayAndEnd;
    }
}

static void Intro_ShowHealthbox(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].ballAnimActive
        && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(gActiveBattler)].ballAnimActive
        && gSprites[gBattleControllerData[gActiveBattler]].callback == SpriteCallbackDummy
        && gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy
        && ++gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].introEndDelay != 1)
    {
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].introEndDelay = 0;

        if (IsDoubleBattle() && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
        {
            DestroySprite(&gSprites[gBattleControllerData[BATTLE_PARTNER(gActiveBattler)]]);
            UpdateHealthboxAttribute(gHealthboxSpriteIds[BATTLE_PARTNER(gActiveBattler)], &gPlayerParty[gBattlerPartyIndexes[BATTLE_PARTNER(gActiveBattler)]], HEALTHBOX_ALL);
            StartHealthboxSlideIn(BATTLE_PARTNER(gActiveBattler));
            SetHealthboxSpriteVisible(gHealthboxSpriteIds[BATTLE_PARTNER(gActiveBattler)]);
        }

        DestroySprite(&gSprites[gBattleControllerData[gActiveBattler]]);
        UpdateHealthboxAttribute(gHealthboxSpriteIds[gActiveBattler], &gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], HEALTHBOX_ALL);
        StartHealthboxSlideIn(gActiveBattler);
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[gActiveBattler]);

        gBattleSpritesDataPtr->animationData->introAnimActive = FALSE;

        gBattlerControllerFuncs[gActiveBattler] = Intro_WaitForHealthbox;
    }
}

static void WaitForMonAnimAfterLoad(void)
{
    if (gSprites[gBattlerSpriteIds[gActiveBattler]].animEnded && gSprites[gBattlerSpriteIds[gActiveBattler]].x2 == 0)
        LinkPartnerBufferExecCompleted();
}

static void CompleteOnHealthbarDone(void)
{
    s16 hpValue = MoveBattleBar(gActiveBattler, gHealthboxSpriteIds[gActiveBattler], HEALTH_BAR);

    SetHealthboxSpriteVisible(gHealthboxSpriteIds[gActiveBattler]);

    if (hpValue != -1)
    {
        UpdateHpTextInHealthbox(gHealthboxSpriteIds[gActiveBattler], hpValue, gBattleMons[gActiveBattler].maxHP);
    }
    else
    {
        HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], gActiveBattler);
        LinkPartnerBufferExecCompleted();
    }
}

static void FreeMonSpriteAfterFaintAnim(void)
{
    if (gSprites[gBattlerSpriteIds[gActiveBattler]].y + gSprites[gBattlerSpriteIds[gActiveBattler]].y2 > DISPLAY_HEIGHT)
    {
        FreeOamMatrix(gSprites[gBattlerSpriteIds[gActiveBattler]].oam.matrixNum);
        DestroySprite(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[gActiveBattler]);
        LinkPartnerBufferExecCompleted();
    }
}

static void FreeMonSpriteAfterSwitchOutAnim(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive)
    {
        FreeSpriteOamMatrix(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        DestroySprite(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[gActiveBattler]);
        LinkPartnerBufferExecCompleted();
    }
}

static void CompleteOnInactiveTextPrinter(void)
{
    if (!IsTextPrinterActive(B_WIN_MSG))
        LinkPartnerBufferExecCompleted();
}

static void DoHitAnimBlinkSpriteEffect(void)
{
    u8 spriteId = gBattlerSpriteIds[gActiveBattler];

    if (gSprites[spriteId].data[1] == 32)
    {
        gSprites[spriteId].data[1] = 0;
        gSprites[spriteId].invisible = FALSE;
        gDoingBattleAnim = FALSE;
        LinkPartnerBufferExecCompleted();
    }
    else
    {
        if ((gSprites[spriteId].data[1] % 4) == 0)
            gSprites[spriteId].invisible ^= 1;
        gSprites[spriteId].data[1]++;
    }
}

static void SwitchIn_ShowSubstitute(void)
{
    if (gSprites[gHealthboxSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
    {
        CopyBattleSpriteInvisibility(gActiveBattler);
        if (gBattleSpritesDataPtr->battlerData[gActiveBattler].behindSubstitute)
            InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_MON_TO_SUBSTITUTE);

        gBattlerControllerFuncs[gActiveBattler] = SwitchIn_WaitAndEnd;
    }
}

static void SwitchIn_WaitAndEnd(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive
        && gSprites[gBattlerSpriteIds[gActiveBattler]].callback == SpriteCallbackDummy)
    {
        LinkPartnerBufferExecCompleted();
    }
}

static void SwitchIn_ShowHealthbox(void)
{
    if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].finishedShinyMonAnim)
    {
        struct Pokemon *mon;
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].triedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].finishedShinyMonAnim = FALSE;

        FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
        FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);

        CreateTask(Task_PlayerController_RestoreBgmAfterCry, 10);
        mon = &gPlayerParty[gBattlerPartyIndexes[gActiveBattler]];
        HandleLowHpMusicChange(mon, gActiveBattler);
        StartSpriteAnim(&gSprites[gBattlerSpriteIds[gActiveBattler]], 0);
        UpdateHealthboxAttribute(gHealthboxSpriteIds[gActiveBattler], mon, HEALTHBOX_ALL);
        StartHealthboxSlideIn(gActiveBattler);
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[gActiveBattler]);

        gBattlerControllerFuncs[gActiveBattler] = SwitchIn_ShowSubstitute;
    }
}

static void SwitchIn_TryShinyAnim(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].triedShinyMonAnim
        && !gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].ballAnimActive)
    {
        TryShinyAnimation(gActiveBattler, &gPlayerParty[gBattlerPartyIndexes[gActiveBattler]]);
    }

    if (gSprites[gBattleControllerData[gActiveBattler]].callback == SpriteCallbackDummy
     && !gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].ballAnimActive)
    {
        DestroySprite(&gSprites[gBattleControllerData[gActiveBattler]]);
        gBattlerControllerFuncs[gActiveBattler] = SwitchIn_ShowHealthbox;
    }
}

static void LinkPartnerBufferExecCompleted(void)
{
    gBattlerControllerFuncs[gActiveBattler] = LinkPartnerBufferRunCommand;
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        u8 playerId = GetMultiplayerId();

        PrepareBufferDataTransferLink(2, 4, &playerId);
        gBattleBufferA[gActiveBattler][0] = CONTROLLER_TERMINATOR_NOP;
    }
    else
    {
        gBattleControllerExecFlags &= ~gBitTable[gActiveBattler];
    }
}

static void CompleteOnFinishedStatusAnimation(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].statusAnimActive)
        LinkPartnerBufferExecCompleted();
}

static void CompleteOnFinishedBattleAnimation(void)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animFromTableActive)
        LinkPartnerBufferExecCompleted();
}

static void LinkPartnerHandleGetMonData(void)
{
    u8 monData[sizeof(struct Pokemon) * 2 + 56]; // this allows to get full data of two Pokémon, trying to get more will result in overwriting data
    u32 size = 0;
    u8 monToCheck;
    s32 i;

    if (gBattleBufferA[gActiveBattler][2] == 0)
    {
        size += CopyPlayerMonData(gBattlerPartyIndexes[gActiveBattler], monData);
    }
    else
    {
        monToCheck = gBattleBufferA[gActiveBattler][2];
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (monToCheck & 1)
                size += CopyPlayerMonData(i, monData + size);
            monToCheck >>= 1;
        }
    }
    BtlController_EmitDataTransfer(BUFFER_B, size, monData);
    LinkPartnerBufferExecCompleted();
}

static void LinkPartnerHandleSetMonData(void)
{
    u8 monToCheck;
    u32 i;

    if (gBattleBufferA[gActiveBattler][2] == 0)
    {
        SetPlayerMonData(gBattlerPartyIndexes[gActiveBattler]);
    }
    else
    {
        monToCheck = gBattleBufferA[gActiveBattler][2];
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (monToCheck & 1)
                SetPlayerMonData(i);
            monToCheck >>= 1;
        }
    }
    LinkPartnerBufferExecCompleted();
}

static void LinkPartnerHandleSetRawMonData(void)
{
    u8 *dst = (u8 *)&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]] + gBattleBufferA[gActiveBattler][1];
    u32 i;

    for (i = 0; i < gBattleBufferA[gActiveBattler][2]; i++)
        dst[i] = gBattleBufferA[gActiveBattler][3 + i];

    LinkPartnerBufferExecCompleted();
}

static void LinkPartnerHandleLoadMonSprite(void)
{
    u16 species;
    struct Pokemon *mon = &gPlayerParty[gBattlerPartyIndexes[gActiveBattler]];

    BattleLoadMonSpriteGfx(mon, gActiveBattler);
    species = GetMonData(mon, MON_DATA_SPECIES);
    SetMultiuseSpriteTemplateToPokemon(species, GetBattlerPosition(gActiveBattler));

    gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gMultiuseSpriteTemplate,
                                               GetBattlerSpriteCoord(gActiveBattler, BATTLER_COORD_X_2),
                                               GetBattlerSpriteDefault_Y(gActiveBattler),
                                               GetBattlerSpriteSubpriority(gActiveBattler));
    gSprites[gBattlerSpriteIds[gActiveBattler]].x2 = -DISPLAY_WIDTH;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = gActiveBattler;
    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = gActiveBattler;
    StartSpriteAnim(&gSprites[gBattlerSpriteIds[gActiveBattler]], gBattleMonForms[gActiveBattler]);
    gBattlerControllerFuncs[gActiveBattler] = WaitForMonAnimAfterLoad;
}

static void LinkPartnerHandleSwitchInAnim(void)
{
    ClearTemporarySpeciesSpriteData(gActiveBattler, gBattleBufferA[gActiveBattler][2]);
    gBattlerPartyIndexes[gActiveBattler] = gBattleBufferA[gActiveBattler][1];
    BattleLoadMonSpriteGfx(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], gActiveBattler);
    StartSendOutAnim(gActiveBattler, gBattleBufferA[gActiveBattler][2]);
    gBattlerControllerFuncs[gActiveBattler] = SwitchIn_TryShinyAnim;
}

static void StartSendOutAnim(u8 battlerId, bool8 dontClearSubstituteBit)
{
    u16 species;

    ClearTemporarySpeciesSpriteData(battlerId, dontClearSubstituteBit);
    gBattlerPartyIndexes[battlerId] = gBattleBufferA[battlerId][1];
    species = GetMonData(&gPlayerParty[gBattlerPartyIndexes[battlerId]], MON_DATA_SPECIES);
    gBattleControllerData[battlerId] = CreateInvisibleSpriteWithCallback(SpriteCB_WaitForBattlerBallReleaseAnim);
    SetMultiuseSpriteTemplateToPokemon(species, GetBattlerPosition(battlerId));

    gBattlerSpriteIds[battlerId] = CreateSprite(
      &gMultiuseSpriteTemplate,
      GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X_2),
      GetBattlerSpriteDefault_Y(battlerId),
      GetBattlerSpriteSubpriority(battlerId));

    gSprites[gBattleControllerData[battlerId]].data[1] = gBattlerSpriteIds[battlerId];
    gSprites[gBattleControllerData[battlerId]].data[2] = battlerId;

    gSprites[gBattlerSpriteIds[battlerId]].data[0] = battlerId;
    gSprites[gBattlerSpriteIds[battlerId]].data[2] = species;
    gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = battlerId;

    StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], gBattleMonForms[battlerId]);

    gSprites[gBattlerSpriteIds[battlerId]].invisible = TRUE;
    gSprites[gBattlerSpriteIds[battlerId]].callback = SpriteCallbackDummy;

    gSprites[gBattleControllerData[battlerId]].data[0] = DoPokeballSendOutAnimation(0, POKEBALL_PLAYER_SENDOUT);
}

static void LinkPartnerHandleReturnMonToBall(void)
{
    if (gBattleBufferA[gActiveBattler][1] == 0)
    {
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 0;
        gBattlerControllerFuncs[gActiveBattler] = DoSwitchOutAnimation;
    }
    else
    {
        FreeSpriteOamMatrix(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        DestroySprite(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[gActiveBattler]);
        LinkPartnerBufferExecCompleted();
    }
}

static void DoSwitchOutAnimation(void)
{
    switch (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState)
    {
    case 0:
        if (gBattleSpritesDataPtr->battlerData[gActiveBattler].behindSubstitute)
            InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_SUBSTITUTE_TO_MON);

        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 1;
        break;
    case 1:
        if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive)
        {
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 0;
            InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_SWITCH_OUT_PLAYER_MON);
            gBattlerControllerFuncs[gActiveBattler] = FreeMonSpriteAfterSwitchOutAnim;
        }
        break;
    }
}

#define sSpeedX data[0]

static void LinkPartnerHandleDrawTrainerPic(void)
{
    s16 xPos = 80;
    u32 trainerPicId;

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if ((GetBattlerPosition(gActiveBattler) & BIT_FLANK) != 0) // second mon
            xPos = 90;
        else // first mon
            xPos = 32;
    }

    trainerPicId = GetLinkPlayerBackTrainerPic(GetBattlerMultiplayerId(gActiveBattler));
    DecompressTrainerBackPic(trainerPicId, gActiveBattler);
    SetMultiuseSpriteTemplateToTrainerBack(trainerPicId, GetBattlerPosition(gActiveBattler));
    gBattlerSpriteIds[gActiveBattler] = CreateSprite(&gMultiuseSpriteTemplate, xPos, 80, GetBattlerSpriteSubpriority(gActiveBattler));

    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = gActiveBattler;
    gSprites[gBattlerSpriteIds[gActiveBattler]].x2 = DISPLAY_WIDTH;
    gSprites[gBattlerSpriteIds[gActiveBattler]].sSpeedX = -2;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = SpriteCB_TrainerSlideIn;

    gBattlerControllerFuncs[gActiveBattler] = CompleteOnBattlerSpriteCallbackDummy;
}

#undef sSpeedX

static void LinkPartnerHandleTrainerSlideBack(void)
{
    SetSpritePrimaryCoordsFromSecondaryCoords(&gSprites[gBattlerSpriteIds[gActiveBattler]]);
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = 35;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[2] = -40;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[4] = gSprites[gBattlerSpriteIds[gActiveBattler]].y;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(&gSprites[gBattlerSpriteIds[gActiveBattler]], SpriteCallbackDummy);
    gBattlerControllerFuncs[gActiveBattler] = FreeTrainerSpriteAfterSlide;
}

#define sSpeedX data[1]
#define sSpeedY data[2]

static void LinkPartnerHandleFaintAnimation(void)
{
    if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState == 0)
    {
        if (gBattleSpritesDataPtr->battlerData[gActiveBattler].behindSubstitute)
            InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_SUBSTITUTE_TO_MON);
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState++;
    }
    else
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive)
        {
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 0;
            HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], gActiveBattler);
            PlaySE12WithPanning(SE_FAINT, SOUND_PAN_ATTACKER);
            gSprites[gBattlerSpriteIds[gActiveBattler]].sSpeedX = 0;
            gSprites[gBattlerSpriteIds[gActiveBattler]].sSpeedY = 5;
            gSprites[gBattlerSpriteIds[gActiveBattler]].callback = SpriteCB_FaintSlideAnim;
            gBattlerControllerFuncs[gActiveBattler] = FreeMonSpriteAfterFaintAnim;
        }
    }
}

#undef sSpeedX
#undef sSpeedY

static void LinkPartnerHandleMoveAnimation(void)
{
    if (!IsBattleSEPlaying(gActiveBattler))
    {
        u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);

        gAnimMoveTurn = gBattleBufferA[gActiveBattler][3];
        gAnimMovePower = gBattleBufferA[gActiveBattler][4] | (gBattleBufferA[gActiveBattler][5] << 8);
        gAnimMoveDmg = gBattleBufferA[gActiveBattler][6] | (gBattleBufferA[gActiveBattler][7] << 8) | (gBattleBufferA[gActiveBattler][8] << 16) | (gBattleBufferA[gActiveBattler][9] << 24);
        gAnimFriendship = gBattleBufferA[gActiveBattler][10];
        gWeatherMoveAnim = gBattleBufferA[gActiveBattler][12] | (gBattleBufferA[gActiveBattler][13] << 8);
        gAnimDisableStructPtr = (struct DisableStruct *)&gBattleBufferA[gActiveBattler][16];
        gTransformedPersonalities[gActiveBattler] = gAnimDisableStructPtr->transformedMonPersonality;
        if (IsMoveWithoutAnimation(move, gAnimMoveTurn)) // always returns FALSE
        {
            LinkPartnerBufferExecCompleted();
        }
        else
        {
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 0;
            gBattlerControllerFuncs[gActiveBattler] = LinkPartnerDoMoveAnimation;
            BattleTv_SetDataBasedOnMove(move, gWeatherMoveAnim, gAnimDisableStructPtr);
        }
    }
}

static void LinkPartnerDoMoveAnimation(void)
{
    u16 move = gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8);
    u8 multihit = gBattleBufferA[gActiveBattler][11];

    switch (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState)
    {
    case 0:
        if (gBattleSpritesDataPtr->battlerData[gActiveBattler].behindSubstitute
            && !gBattleSpritesDataPtr->battlerData[gActiveBattler].flag_x8)
        {
            gBattleSpritesDataPtr->battlerData[gActiveBattler].flag_x8 = 1;
            InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_SUBSTITUTE_TO_MON);
        }
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 1;
        break;
    case 1:
        if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive)
        {
            SetBattlerSpriteAffineMode(ST_OAM_AFFINE_OFF);
            DoMoveAnim(move);
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 2;
        }
        break;
    case 2:
        gAnimScriptCallback();
        if (!gAnimScriptActive)
        {
            SetBattlerSpriteAffineMode(ST_OAM_AFFINE_NORMAL);
            if (gBattleSpritesDataPtr->battlerData[gActiveBattler].behindSubstitute && multihit < 2)
            {
                InitAndLaunchSpecialAnimation(gActiveBattler, gActiveBattler, gActiveBattler, B_ANIM_MON_TO_SUBSTITUTE);
                gBattleSpritesDataPtr->battlerData[gActiveBattler].flag_x8 = 0;
            }
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 3;
        }
        break;
    case 3:
        if (!gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].specialAnimActive)
        {
            CopyAllBattleSpritesInvisibilities();
            TrySetBehindSubstituteSpriteBit(gActiveBattler, gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8));
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].animationState = 0;
            LinkPartnerBufferExecCompleted();
        }
        break;
    }
}

static void LinkPartnerHandlePrintString(void)
{
    u16 *stringId;

    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    stringId = (u16 *)(&gBattleBufferA[gActiveBattler][2]);
    BufferStringBattle(*stringId);
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
    gBattlerControllerFuncs[gActiveBattler] = CompleteOnInactiveTextPrinter;
    BattleTv_SetDataBasedOnString(*stringId);
}

static void LinkPartnerHandleHealthBarUpdate(void)
{
    s16 hpVal;
    u32 maxHP;
    struct Pokemon *mon;

    LoadBattleBarGfx();
    hpVal = gBattleBufferA[gActiveBattler][2] | (gBattleBufferA[gActiveBattler][3] << 8);

    mon = &gPlayerParty[gBattlerPartyIndexes[gActiveBattler]];
    maxHP = GetMonData(mon, MON_DATA_MAX_HP);
    if (hpVal != INSTANT_HP_BAR_DROP)
    {
        u32 curHP = GetMonData(mon, MON_DATA_HP);

        SetBattleBarStruct(gActiveBattler, gHealthboxSpriteIds[gActiveBattler], maxHP, curHP, hpVal);
    }
    else
    {
        SetBattleBarStruct(gActiveBattler, gHealthboxSpriteIds[gActiveBattler], maxHP, 0, hpVal);
    }

    gBattlerControllerFuncs[gActiveBattler] = CompleteOnHealthbarDone;
}

static void LinkPartnerHandleStatusIconUpdate(void)
{
    if (!IsBattleSEPlaying(gActiveBattler))
    {
        u8 battlerId;

        UpdateHealthboxAttribute(gHealthboxSpriteIds[gActiveBattler], &gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], HEALTHBOX_STATUS_ICON);
        battlerId = gActiveBattler;
        gBattleSpritesDataPtr->healthBoxesData[battlerId].statusAnimActive = 0;
        gBattlerControllerFuncs[gActiveBattler] = CompleteOnFinishedStatusAnimation;
    }
}

static void LinkPartnerHandleStatusAnimation(void)
{
    if (!IsBattleSEPlaying(gActiveBattler))
    {
        InitAndLaunchChosenStatusAnimation(gBattleBufferA[gActiveBattler][1],
                        gBattleBufferA[gActiveBattler][2] | (gBattleBufferA[gActiveBattler][3] << 8) | (gBattleBufferA[gActiveBattler][4] << 16) | (gBattleBufferA[gActiveBattler][5] << 24));
        gBattlerControllerFuncs[gActiveBattler] = CompleteOnFinishedStatusAnimation;
    }
}

static void LinkPartnerHandleHitAnimation(void)
{
    if (gSprites[gBattlerSpriteIds[gActiveBattler]].invisible == TRUE)
    {
        LinkPartnerBufferExecCompleted();
    }
    else
    {
        gDoingBattleAnim = TRUE;
        gSprites[gBattlerSpriteIds[gActiveBattler]].data[1] = 0;
        DoHitAnimHealthboxEffect(gActiveBattler);
        gBattlerControllerFuncs[gActiveBattler] = DoHitAnimBlinkSpriteEffect;
    }
}

static void LinkPartnerHandlePlaySE(void)
{
    s8 pan;

    if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        pan = SOUND_PAN_ATTACKER;
    else
        pan = SOUND_PAN_TARGET;

    PlaySE12WithPanning(gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8), pan);
    LinkPartnerBufferExecCompleted();
}

static void LinkPartnerHandlePlayFanfareOrBGM(void)
{
    if (gBattleBufferA[gActiveBattler][3])
    {
        BattleStopLowHpSound();
        PlayBGM(gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8));
    }
    else
    {
        PlayFanfare(gBattleBufferA[gActiveBattler][1] | (gBattleBufferA[gActiveBattler][2] << 8));
    }

    LinkPartnerBufferExecCompleted();
}

static void LinkPartnerHandleFaintingCry(void)
{
    u16 species = GetMonData(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES);

    PlayCry_ByMode(species, -25, CRY_MODE_FAINT);
    LinkPartnerBufferExecCompleted();
}

static void LinkPartnerHandleIntroSlide(void)
{
    HandleIntroSlide(gBattleBufferA[gActiveBattler][1]);
    gIntroSlideFlags |= 1;
    LinkPartnerBufferExecCompleted();
}

static void LinkPartnerHandleIntroTrainerBallThrow(void)
{
    u8 paletteNum;
    u8 taskId;
    u32 trainerPicId;

    SetSpritePrimaryCoordsFromSecondaryCoords(&gSprites[gBattlerSpriteIds[gActiveBattler]]);

    gSprites[gBattlerSpriteIds[gActiveBattler]].data[0] = 50;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[2] = -40;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[4] = gSprites[gBattlerSpriteIds[gActiveBattler]].y;
    gSprites[gBattlerSpriteIds[gActiveBattler]].callback = StartAnimLinearTranslation;
    gSprites[gBattlerSpriteIds[gActiveBattler]].data[5] = gActiveBattler;

    StoreSpriteCallbackInData6(&gSprites[gBattlerSpriteIds[gActiveBattler]], SpriteCB_FreePlayerSpriteLoadMonSprite);
    StartSpriteAnim(&gSprites[gBattlerSpriteIds[gActiveBattler]], 1);

    paletteNum = AllocSpritePalette(0xD6F9);

    trainerPicId = GetLinkPlayerBackTrainerPic(GetBattlerMultiplayerId(gActiveBattler));

    DecompressTrainerBackPic(trainerPicId, paletteNum);

    gSprites[gBattlerSpriteIds[gActiveBattler]].oam.paletteNum = paletteNum;

    taskId = CreateTask(Task_StartSendOutAnim, 5);
    gTasks[taskId].data[0] = gActiveBattler;

    if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].partyStatusSummaryShown)
        gTasks[gBattlerStatusSummaryTaskId[gActiveBattler]].func = Task_HidePartyStatusSummary;

    gBattleSpritesDataPtr->animationData->introAnimActive = TRUE;
    gBattlerControllerFuncs[gActiveBattler] = BattleControllerDummy;
}

static void Task_StartSendOutAnim(u8 taskId)
{
    if (gTasks[taskId].data[1] < 24)
    {
        gTasks[taskId].data[1]++;
    }
    else
    {
        u8 savedActiveBank = gActiveBattler;

        gActiveBattler = gTasks[taskId].data[0];
        if (!IsDoubleBattle() || (gBattleTypeFlags & BATTLE_TYPE_MULTI))
        {
            gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
            StartSendOutAnim(gActiveBattler, FALSE);
        }
        else
        {
            gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
            StartSendOutAnim(gActiveBattler, FALSE);
            gActiveBattler ^= BIT_FLANK;
            gBattleBufferA[gActiveBattler][1] = gBattlerPartyIndexes[gActiveBattler];
            BattleLoadMonSpriteGfx(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], gActiveBattler);
            StartSendOutAnim(gActiveBattler, FALSE);
            gActiveBattler ^= BIT_FLANK;
        }
        gBattlerControllerFuncs[gActiveBattler] = Intro_ShowHealthbox;
        gActiveBattler = savedActiveBank;
        DestroyTask(taskId);
    }
}

static void LinkPartnerHandleDrawPartyStatusSummary(void)
{
    if (gBattleBufferA[gActiveBattler][1] != 0 && GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
    {
        LinkPartnerBufferExecCompleted();
    }
    else
    {
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].partyStatusSummaryShown = 1;
        gBattlerStatusSummaryTaskId[gActiveBattler] = CreatePartyStatusSummarySprites(gActiveBattler, (struct HpAndStatus *)&gBattleBufferA[gActiveBattler][4], gBattleBufferA[gActiveBattler][1], gBattleBufferA[gActiveBattler][2]);
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].partyStatusDelayTimer = 0;

        if (gBattleBufferA[gActiveBattler][2] != 0)
            gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].partyStatusDelayTimer = 93;

        gBattlerControllerFuncs[gActiveBattler] = EndDrawPartyStatusSummary;
    }
}

static void EndDrawPartyStatusSummary(void)
{
    if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].partyStatusDelayTimer++ > 92)
    {
        gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].partyStatusDelayTimer = 0;
        LinkPartnerBufferExecCompleted();
    }
}

static void LinkPartnerHandleHidePartyStatusSummary(void)
{
    if (gBattleSpritesDataPtr->healthBoxesData[gActiveBattler].partyStatusSummaryShown)
        gTasks[gBattlerStatusSummaryTaskId[gActiveBattler]].func = Task_HidePartyStatusSummary;
    LinkPartnerBufferExecCompleted();
}

static void LinkPartnerHandleSpriteInvisibility(void)
{
    if (IsBattlerSpritePresent(gActiveBattler))
    {
        gSprites[gBattlerSpriteIds[gActiveBattler]].invisible = gBattleBufferA[gActiveBattler][1];
        CopyBattleSpriteInvisibility(gActiveBattler);
    }
    LinkPartnerBufferExecCompleted();
}

static void LinkPartnerHandleBattleAnimation(void)
{
    if (!IsBattleSEPlaying(gActiveBattler))
    {
        u8 animationId = gBattleBufferA[gActiveBattler][1];
        u16 argument = gBattleBufferA[gActiveBattler][2] | (gBattleBufferA[gActiveBattler][3] << 8);

        if (TryHandleLaunchBattleTableAnimation(gActiveBattler, gActiveBattler, gActiveBattler, animationId, argument))
            LinkPartnerBufferExecCompleted();
        else
            gBattlerControllerFuncs[gActiveBattler] = CompleteOnFinishedBattleAnimation;

        BattleTv_SetDataBasedOnAnimation(animationId);
    }
}

static void LinkPartnerHandleLinkStandbyMsg(void)
{
    RecordedBattle_RecordAllBattlerData(&gBattleBufferA[gActiveBattler][2]);
    LinkPartnerBufferExecCompleted();
}

static void LinkPartnerHandleEndLinkBattle(void)
{
    RecordedBattle_RecordAllBattlerData(&gBattleBufferA[gActiveBattler][4]);
    gBattleOutcome = gBattleBufferA[gActiveBattler][1];
    gSaveBlock2Ptr->frontier.disableRecordBattle = gBattleBufferA[gActiveBattler][2];
    FadeOutMapMusic(5);
    BeginFastPaletteFade(3);
    LinkPartnerBufferExecCompleted();
    gBattlerControllerFuncs[gActiveBattler] = SetBattleEndCallbacks;
}
