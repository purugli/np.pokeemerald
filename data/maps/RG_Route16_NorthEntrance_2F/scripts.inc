RG_Route16_NorthEntrance_2F_MapScripts::
	.byte 0

RG_Route16_NorthEntrance_2F_EventScript_LeftBinoculars::
	msgbox RG_Route16_NorthEntrance_2F_Text_ItsCeladonDeptStore, MSGBOX_SIGN
	end

RG_Route16_NorthEntrance_2F_EventScript_RightBinoculars::
	msgbox RG_Route16_NorthEntrance_2F_Text_LongPathOverWater, MSGBOX_SIGN
	end

RG_Route16_NorthEntrance_2F_Text_ItsCeladonDeptStore::
    .string "Let's see what the binoculars have\n"
    .string "to show…\p"
    .string "It's the CELADON DEPT. STORE!$"

RG_Route16_NorthEntrance_2F_Text_LongPathOverWater::
    .string "Let's see what the binoculars have\n"
    .string "to show…\p"
    .string "There's a long path over water far\n"
    .string "away.$"
