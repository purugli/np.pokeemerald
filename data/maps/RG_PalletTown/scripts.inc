RG_PalletTown_MapScripts::
	.byte 0

RG_PalletTown_EventScript_OaksLabSign::
	lockall
	msgbox RG_PalletTown_Text_OakPokemonResearchLab
	releaseall
	end

RG_PalletTown_EventScript_PlayersHouseSign::
	msgbox RG_PalletTown_Text_PlayersHouse, MSGBOX_SIGN
	end

RG_PalletTown_EventScript_RivalsHouseSign::
	msgbox RG_PalletTown_Text_RivalsHouse, MSGBOX_SIGN
	end

RG_PalletTown_EventScript_TownSign::
	msgbox RG_PalletTown_Text_TownSign, MSGBOX_SIGN
	end

RG_PalletTown_EventScript_TrainerTips::
	lockall
	msgbox RG_PalletTown_Text_PressStartToOpenMenu
	releaseall
	end

RG_PalletTown_Text_OakPokemonResearchLab::
    .string "OAK POKéMON RESEARCH LAB$"

RG_PalletTown_Text_PlayersHouse::
    .string "{PLAYER}'s house$"

RG_PalletTown_Text_RivalsHouse::
    .string "{RIVAL}'s house$"

RG_PalletTown_Text_TownSign::
    .string "PALLET TOWN\n"
    .string "Shades of your journey await!$"

RG_PalletTown_Text_PressStartToOpenMenu::
	.string "TRAINER TIPS\p"
	.string "Press START to open the MENU!$"
