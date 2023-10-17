RG_Route12_NorthEntrance_2F_MapScripts::
	.byte 0

RG_Route12_NorthEntrance_2F_EventScript_LeftBinoculars::
	msgbox RG_Route12_NorthEntrance_2F_Text_TheresManFishing, MSGBOX_SIGN
	end

RG_Route12_NorthEntrance_2F_EventScript_RightBinoculars::
	msgbox RG_Route12_NorthEntrance_2F_Text_ItsPokemonTower, MSGBOX_SIGN
	end

RG_Route12_NorthEntrance_2F_Text_TheresManFishing::
    .string "Let's see what the binoculars have\n"
    .string "to show…\p"
    .string "There's a man fishing!$"

RG_Route12_NorthEntrance_2F_Text_ItsPokemonTower::
    .string "Let's see what the binoculars have\n"
    .string "to show…\p"
    .string "It's POKéMON TOWER!$"
