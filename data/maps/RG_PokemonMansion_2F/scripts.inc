RG_PokemonMansion_2F_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, RG_PokemonMansion_2F_OnLoad
	.byte 0

RG_PokemonMansion_2F_OnLoad::
	call_if_set FLAG_POKEMON_MANSION_SWITCH_STATE, RG_PokemonMansion_EventScript_PressSwitch_2F
	end

RG_PokemonMansion_2F_EventScript_Statue::
	lockall
	setvar VAR_0x8004, 1
	call RG_PokemonMansion_EventScript_SecretSwitch
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	releaseall
	end

RG_PokemonMansion_2F_EventScript_DiaryJuly5th::
	msgbox RG_PokemonMansion_1F_Text_NewMonDiscoveredInGuyanaJungle, MSGBOX_SIGN
	end

RG_PokemonMansion_2F_EventScript_DiaryJuly10th::
	msgbox RG_PokemonMansion_1F_Text_ChristenedDiscoveredMonMew, MSGBOX_SIGN
	end

RG_PokemonMansion_Text_PressSecretSwitch::
    .string "A secret switch!\p"
    .string "Press it?$"

RG_PokemonMansion_Text_WhoWouldnt::
    .string "Who wouldn't?$"

RG_PokemonMansion_Text_NotQuiteYet::
    .string "Not quite yet!$"

RG_PokemonMansion_1F_Text_NewMonDiscoveredInGuyanaJungle::
    .string "Diary: July 5\n"
    .string "Guyana, South America\p"
    .string "A new POKéMON was discovered deep\n"
    .string "in the jungle.$"

RG_PokemonMansion_1F_Text_ChristenedDiscoveredMonMew::
    .string "Diary: July 10\n"
    .string "We christened the newly discovered\l"
    .string "POKéMON, MEW.$"
