RG_Route2_MapScripts::
	.byte 0

RG_Route2_EventScript_RouteSign::
	msgbox RG_Route2_Text_RouteSign, MSGBOX_SIGN
	end

RG_Route2_EventScript_DiglettsCaveSign::
	msgbox RG_Route2_Text_DiglettsCave, MSGBOX_SIGN
	end

RG_Route2_Text_RouteSign::
    .string "ROUTE 2\n"
    .string "VIRIDIAN CITY - PEWTER CITY$"

RG_Route2_Text_DiglettsCave::
    .string "DIGLETT'S CAVE$"
