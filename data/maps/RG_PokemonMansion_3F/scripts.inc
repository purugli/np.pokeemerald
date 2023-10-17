RG_PokemonMansion_3F_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, RG_PokemonMansion_3F_OnLoad
	.byte 0

RG_PokemonMansion_3F_OnLoad::
	call_if_set FLAG_POKEMON_MANSION_SWITCH_STATE, RG_PokemonMansion_EventScript_PressSwitch_3F
	end

RG_PokemonMansion_3F_EventScript_Statue::
	lockall
	setvar VAR_0x8004, 2
	call RG_PokemonMansion_EventScript_SecretSwitch
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	releaseall
	end

RG_PokemonMansion_3F_EventScript_DiaryFeb6th::
	msgbox RG_PokemonMansion_1F_Text_MewGaveBirthToMewtwo, MSGBOX_SIGN
	end

RG_PokemonMansion_1F_Text_MewGaveBirthToMewtwo::
    .string "Diary: Feb. 6\n"
    .string "MEW gave birth.\p"
    .string "We named the newborn MEWTWO.$"

