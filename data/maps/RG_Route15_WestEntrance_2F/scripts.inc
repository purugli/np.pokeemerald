RG_Route15_WestEntrance_2F_MapScripts::
	.byte 0

RG_Route15_WestEntrance_2F_EventScript_LeftBinoculars::
	lockall
	msgbox RG_Route15_WestEntrance_2F_Text_LargeShiningBird
	showmonpic SPECIES_ARTICUNO, 10, 3
	delay 20
	waitbuttonpress
	hidemonpic
	releaseall
	end

RG_Route15_WestEntrance_2F_EventScript_RightBinoculars::
	msgbox RG_Route15_WestEntrance_2F_Text_SmallIslandOnHorizon, MSGBOX_SIGN
	end

RG_Route15_WestEntrance_2F_Text_LargeShiningBird::
    .string "Let's see what the binoculars have\n"
    .string "to show…\p"
    .string "A large, shining bird is flying\n"
    .string "toward the sea.$"

RG_Route15_WestEntrance_2F_Text_SmallIslandOnHorizon::
    .string "Let's see what the binoculars have\n"
    .string "to show…\p"
    .string "It looks like a small island on\n"
    .string "the horizon!$"
