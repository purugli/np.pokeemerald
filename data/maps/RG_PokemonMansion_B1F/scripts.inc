RG_PokemonMansion_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, RG_PokemonMansion_B1F_OnLoad
	.byte 0

RG_PokemonMansion_B1F_OnLoad::
	call_if_set FLAG_POKEMON_MANSION_SWITCH_STATE, RG_PokemonMansion_EventScript_PressSwitch_B1F
	end

RG_PokemonMansion_B1F_EventScript_Statue::
	lockall
	setvar VAR_0x8004, 3
	call RG_PokemonMansion_EventScript_SecretSwitch
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	releaseall
	end

RG_PokemonMansion_B1F_EventScript_DiarySep1st::
	msgbox RG_PokemonMansion_B1F_Text_MewtwoIsFarTooPowerful, MSGBOX_SIGN
	end

RG_PokemonMansion_B1F_Text_MewtwoIsFarTooPowerful::
    .string "Diary: Sept. 1\n"
    .string "MEWTWO is far too powerful.\p"
    .string "We have failed to curb its vicious\n"
    .string "tendencies…$"
