RG_Route4_MapScripts::
	.byte 0

RG_Route4_EventScript_MtMoonSign::
	msgbox RG_Route4_Text_MtMoonEntrance, MSGBOX_SIGN
	end

RG_Route4_EventScript_RouteSign::
	msgbox RG_Route4_Text_RouteSign, MSGBOX_SIGN
	end

RG_Route4_Text_MtMoonEntrance::
    .string "MT. MOON\n"
    .string "Tunnel Entrance$"

RG_Route4_Text_RouteSign::
    .string "ROUTE 4\n"
    .string "MT. MOON - CERULEAN CITY$"
