.set LOCALID_DEOXYS, 2

@ Note: LOCALID_BIRTH_ISLAND_EXTERIOR_ROCK is a local id for this map used elsewhere. It's defined in event_objects.h

RG_BirthIsland_Exterior_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RG_BirthIsland_Exterior_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, RG_BirthIsland_Exterior_OnResume
	map_script MAP_SCRIPT_ON_RETURN_TO_FIELD, RG_BirthIsland_Exterior_OnReturnToField
	.byte 0

RG_BirthIsland_Exterior_OnReturnToField:
	special SetDeoxysRockPalette
	end

RG_BirthIsland_Exterior_OnTransition:
	setflag FLAG_MAP_SCRIPT_CHECKED_DEOXYS
	setvar VAR_DEOXYS_ROCK_STEP_COUNT, 0
	setvar VAR_DEOXYS_ROCK_LEVEL, 0
	call_if_set FLAG_BATTLED_DEOXYS, RG_BirthIsland_Exterior_EventScript_HideDeoxysAndPuzzle
	call_if_unset FLAG_BATTLED_DEOXYS, RG_BirthIsland_Exterior_EventScript_TryShowDeoxysPuzzle
	end

RG_BirthIsland_Exterior_EventScript_HideDeoxysAndPuzzle::
	setflag FLAG_HIDE_DEOXYS
	setflag FLAG_HIDE_BIRTH_ISLAND_DEOXYS_TRIANGLE
	return

RG_BirthIsland_Exterior_EventScript_TryShowDeoxysPuzzle::
	goto_if_set FLAG_DEFEATED_DEOXYS, Common_EventScript_NopReturn
	clearflag FLAG_HIDE_BIRTH_ISLAND_DEOXYS_TRIANGLE
	clearflag FLAG_DEOXYS_ROCK_COMPLETE
	return

RG_BirthIsland_Exterior_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, RG_BirthIsland_Exterior_EventScript_TryRemoveDeoxys
	end

RG_BirthIsland_Exterior_EventScript_TryRemoveDeoxys::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject LOCALID_DEOXYS
	return

RG_BirthIsland_Exterior_EventScript_Triangle::
	lock
	faceplayer
	special DoDeoxysRockInteraction
	waitstate
	switch VAR_RESULT
	case DEOXYS_ROCK_FAILED,     RG_BirthIsland_Exterior_EventScript_Failed
	case DEOXYS_ROCK_PROGRESSED, RG_BirthIsland_Exterior_EventScript_Progressed
	case DEOXYS_ROCK_SOLVED,     RG_BirthIsland_Exterior_EventScript_Deoxys
	case DEOXYS_ROCK_COMPLETE,   RG_BirthIsland_Exterior_EventScript_Complete
	end

@ The actual rock triangle movement is handled by DoDeoxysRockInteraction.
@ Unless the player has solved the puzzle and needs to encounter Deoxys,
@ there's nothing else the script needs to do.
RG_BirthIsland_Exterior_EventScript_Failed::
	release
	end

RG_BirthIsland_Exterior_EventScript_Progressed::
	release
	end

RG_BirthIsland_Exterior_EventScript_Complete::
	release
	end

RG_BirthIsland_Exterior_EventScript_Deoxys::
	waitse
	setfieldeffectargument 0, LOCALID_BIRTH_ISLAND_EXTERIOR_ROCK
	setfieldeffectargument 1, MAP_NUM(RG_BIRTH_ISLAND_EXTERIOR)
	setfieldeffectargument 2, MAP_GROUP(RG_BIRTH_ISLAND_EXTERIOR)
	dofieldeffect FLDEFF_DESTROY_DEOXYS_ROCK
	playbgm MUS_RG_ENCOUNTER_DEOXYS, FALSE
	waitfieldeffect FLDEFF_DESTROY_DEOXYS_ROCK
	addobject LOCALID_DEOXYS
	applymovement LOCALID_DEOXYS, RG_BirthIsland_Exterior_Movement_DeoxysApproach
	waitmovement 0
	waitse
	playmoncry SPECIES_DEOXYS, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_DEOXYS
	seteventmon SPECIES_DEOXYS, 30
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RG_BirthIsland_Exterior_EventScript_DefeatedDeoxys
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, RG_BirthIsland_Exterior_EventScript_RanFromDeoxys
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, RG_BirthIsland_Exterior_EventScript_RanFromDeoxys
	setflag FLAG_BATTLED_DEOXYS
	release
	end

RG_BirthIsland_Exterior_EventScript_DefeatedDeoxys::
	setflag FLAG_DEFEATED_DEOXYS
	setvar VAR_0x8004, SPECIES_DEOXYS
	goto Common_EventScript_LegendaryFlewAway
	end

RG_BirthIsland_Exterior_EventScript_RanFromDeoxys::
	setvar VAR_0x8004, SPECIES_DEOXYS
	goto Common_EventScript_LegendaryFlewAway
	end

RG_BirthIsland_Exterior_Movement_DeoxysApproach:
	walk_slow_down
	walk_slow_down
	walk_slow_down
	walk_slow_down
	walk_slow_down
	walk_slow_down
	walk_slow_down
	step_end
