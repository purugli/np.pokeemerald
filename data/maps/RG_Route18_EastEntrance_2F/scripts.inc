RG_Route18_EastEntrance_2F_MapScripts::
	.byte 0

RG_Route18_EastEntrance_2F_EventScript_LeftBinoculars::
	msgbox RG_Route18_EastEntrance_2F_Text_PalletTownInWest, MSGBOX_SIGN
	end

RG_Route18_EastEntrance_2F_EventScript_RightBinoculars::
	msgbox RG_Route18_EastEntrance_2F_Text_PeopleSwimming, MSGBOX_SIGN
	end

RG_Route18_EastEntrance_2F_Text_PalletTownInWest::
    .string "Let's see what the binoculars have\n"
    .string "to show…\p"
    .string "PALLET TOWN is in the west.$"

RG_Route18_EastEntrance_2F_Text_PeopleSwimming::
    .string "Let's see what the binoculars have\n"
    .string "to show…\p"
    .string "There are people swimming.$"
