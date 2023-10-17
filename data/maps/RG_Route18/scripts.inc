RG_Route18_MapScripts::
	.byte 0

RG_Route18_EventScript_RouteSign::
	msgbox RG_Route18_Text_RouteSign, MSGBOX_SIGN
	end

RG_Route18_EventScript_CyclingRoadSign::
	msgbox RG_Route18_Text_CyclingRoadSign, MSGBOX_SIGN
	end

RG_Route18_Text_RouteSign::
    .string "ROUTE 18\n"
    .string "CELADON CITY - FUCHSIA CITY$"

RG_Route18_Text_CyclingRoadSign::
    .string "CYCLING ROAD\n"
    .string "No pedestrians permitted!$"
