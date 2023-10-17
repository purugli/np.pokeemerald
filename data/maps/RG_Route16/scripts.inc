RG_Route16_MapScripts::
	.byte 0

RG_Route16_EventScript_CyclingRoadSign::
	msgbox RG_Route16_Text_CyclingRoadSign, MSGBOX_SIGN
	end

RG_Route16_EventScript_RouteSign::
	msgbox RG_Route16_Text_RouteSign, MSGBOX_SIGN
	end

RG_Route16_Text_CyclingRoadSign::
    .string "Enjoy the slope!\n"
    .string "CYCLING ROAD$"

RG_Route16_Text_RouteSign::
    .string "ROUTE 16\n"
    .string "CELADON CITY - FUCHSIA CITY$"
