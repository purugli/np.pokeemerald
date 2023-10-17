RG_Route17_MapScripts::
	.byte 0

RG_Route17_EventScript_ItemsNotice::
	msgbox RG_Route17_Text_WatchOutForDiscardedItems, MSGBOX_SIGN
	end

RG_Route17_EventScript_TrainerTips1::
	msgbox RG_Route17_Text_SameSpeciesGrowDifferentRates, MSGBOX_SIGN
	end

RG_Route17_EventScript_TrainerTips2::
	msgbox RG_Route17_Text_PressBToStayInPlace, MSGBOX_SIGN
	end

RG_Route17_EventScript_RouteSign::
	msgbox RG_Route17_Text_RouteSign, MSGBOX_SIGN
	end

RG_Route17_EventScript_BallsNotice::
	msgbox RG_Route17_Text_DontThrowGameThrowBalls, MSGBOX_SIGN
	end

RG_Route17_EventScript_CyclingRoadSign::
	msgbox RG_Route17_Text_CyclingRoadSign, MSGBOX_SIGN
	end

RG_Route17_Text_WatchOutForDiscardedItems::
    .string "It's a notice.\p"
    .string "Watch out for discarded items.$"

RG_Route17_Text_SameSpeciesGrowDifferentRates::
    .string "TRAINER TIPS\p"
    .string "All POKéMON are unique.\p"
    .string "Even POKéMON of the same species\n"
    .string "and level grow at different rates.$"

RG_Route17_Text_PressBToStayInPlace::
    .string "TRAINER TIPS\p"
    .string "Press the B Button to stay in place\n"
    .string "while on a slope.$"

RG_Route17_Text_RouteSign::
    .string "ROUTE 17\n"
    .string "CELADON CITY - FUCHSIA CITY$"

RG_Route17_Text_DontThrowGameThrowBalls::
    .string "It's a notice!\p"
    .string "Don't throw the game, throw POKé\n"
    .string "BALLS instead!$"

RG_Route17_Text_CyclingRoadSign::
    .string "CYCLING ROAD\n"
    .string "Slope ends here!$"
