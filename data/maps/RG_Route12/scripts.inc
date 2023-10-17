RG_Route12_MapScripts::
	.byte 0

RG_Route12_EventScript_RouteSign::
	msgbox RG_Route12_Text_RouteSign, MSGBOX_SIGN
	end

RG_Route12_EventScript_FishingSign::
	msgbox RG_Route12_Text_SportfishingArea, MSGBOX_SIGN
	end

RG_Route12_Text_RouteSign::
    .string "ROUTE 12 \n"
    .string "North to LAVENDER$"

RG_Route12_Text_SportfishingArea::
    .string "SPORTFISHING AREA$"
