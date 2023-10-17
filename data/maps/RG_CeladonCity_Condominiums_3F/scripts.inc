RG_CeladonCity_Condominiums_3F_MapScripts::
	.byte 0

RG_CeladonCity_Condominiums_3F_EventScript_DevelopmentRoomSign::
	msgbox RG_CeladonCity_Condominiums_3F_Text_GameFreakDevelopmentRoom, MSGBOX_SIGN
	end

RG_CeladonCity_Condominiums_3F_EventScript_Computer1::
	msgbox RG_CeladonCity_Condominiums_3F_Text_ItsTheGameProgram, MSGBOX_SIGN
	end

RG_CeladonCity_Condominiums_3F_EventScript_Computer2::
	msgbox RG_CeladonCity_Condominiums_3F_Text_SomeonesPlayingGame, MSGBOX_SIGN
	end

RG_CeladonCity_Condominiums_3F_EventScript_Computer3::
	msgbox RG_CeladonCity_Condominiums_3F_Text_ItsScriptBetterNotLookAtEnding, MSGBOX_SIGN
	end

RG_CeladonCity_Condominiums_3F_Text_ItsTheGameProgram::
    .string "It's the game program! Messing with\n"
    .string "it could bug out the game!$"

RG_CeladonCity_Condominiums_3F_Text_SomeonesPlayingGame::
    .string "Someone's playing a game instead of\n"
    .string "working!$"

RG_CeladonCity_Condominiums_3F_Text_ItsScriptBetterNotLookAtEnding::
    .string "It's the script!\n"
    .string "Better not look at the ending!$"

RG_CeladonCity_Condominiums_3F_Text_GameFreakDevelopmentRoom::
    .string "GAME FREAK Development Room$"
