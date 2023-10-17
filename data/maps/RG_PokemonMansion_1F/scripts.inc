RG_PokemonMansion_1F_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, RG_PokemonMansion_1F_OnLoad
	.byte 0

RG_PokemonMansion_1F_OnLoad::
	call_if_set FLAG_POKEMON_MANSION_SWITCH_STATE, RG_PokemonMansion_EventScript_PressSwitch_1F
	end

RG_PokemonMansion_1F_EventScript_Statue::
	lockall
	setvar VAR_0x8004, 0
	call RG_PokemonMansion_EventScript_SecretSwitch
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	releaseall
	end
