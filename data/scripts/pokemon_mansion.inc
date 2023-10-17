RG_PokemonMansion_EventScript_SecretSwitch::
	msgbox RG_PokemonMansion_Text_PressSecretSwitch, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RG_PokemonMansion_EventScript_DontPressSwitch
	msgbox RG_PokemonMansion_Text_WhoWouldnt
	goto_if_set FLAG_POKEMON_MANSION_SWITCH_STATE, RG_PokemonMansion_EventScript_ResetSwitch
	setflag FLAG_POKEMON_MANSION_SWITCH_STATE
	switch VAR_0x8004
	case 0, RG_PokemonMansion_EventScript_PressSwitch_1F
	case 1, RG_PokemonMansion_EventScript_PressSwitch_2F
	case 2, RG_PokemonMansion_EventScript_PressSwitch_3F
	case 3, RG_PokemonMansion_EventScript_PressSwitch_B1F
	end

RG_PokemonMansion_EventScript_ResetSwitch::
	clearflag FLAG_POKEMON_MANSION_SWITCH_STATE
	switch VAR_0x8004
	case 0, RG_PokemonMansion_EventScript_ResetSwitch_1F
	case 1, RG_PokemonMansion_EventScript_ResetSwitch_2F
	case 2, RG_PokemonMansion_EventScript_ResetSwitch_3F
	case 3, RG_PokemonMansion_EventScript_ResetSwitch_B1F
	end

RG_PokemonMansion_EventScript_DontPressSwitch::
	msgbox RG_PokemonMansion_Text_NotQuiteYet
	releaseall
	end

RG_PokemonMansion_EventScript_PressSwitch_1F::
	setmetatile 22, 10, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 23, 10, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 24, 10, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 22, 11, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 23, 11, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 24, 11, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 27, 25, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 28, 25, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 29, 25, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 27, 26, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 28, 26, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 29, 26, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 32, 25, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 33, 25, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 34, 25, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 32, 26, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 33, 26, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 34, 26, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 31, 18, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopLeft, TRUE
	setmetatile 32, 18, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopMid, TRUE
	setmetatile 33, 18, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopRight, TRUE
	setmetatile 31, 19, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomLeft, TRUE
	setmetatile 32, 19, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomMid, TRUE
	setmetatile 33, 19, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomRight, TRUE
	setmetatile  5,  4, METATILE_RG_PokemonMansion_Statue_RedEyes, FALSE
	return

RG_PokemonMansion_EventScript_ResetSwitch_1F::
	setmetatile 22, 10, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopLeft, TRUE
	setmetatile 23, 10, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopMid, TRUE
	setmetatile 24, 10, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopRight, TRUE
	setmetatile 22, 11, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomLeft, TRUE
	setmetatile 23, 11, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomMid, TRUE
	setmetatile 24, 11, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomRight, TRUE
	setmetatile 27, 25, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopLeft, TRUE
	setmetatile 28, 25, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopMid, TRUE
	setmetatile 29, 25, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopRight, TRUE
	setmetatile 27, 26, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomLeft, TRUE
	setmetatile 28, 26, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomMid, TRUE
	setmetatile 29, 26, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomRight, TRUE
	setmetatile 32, 25, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopLeft, TRUE
	setmetatile 33, 25, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopMid, TRUE
	setmetatile 34, 25, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopRight, TRUE
	setmetatile 32, 26, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomLeft, TRUE
	setmetatile 33, 26, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomMid, TRUE
	setmetatile 34, 26, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomRight, TRUE
	setmetatile 31, 18, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 32, 18, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 33, 18, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 31, 19, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 32, 19, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 33, 19, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile  5,  4, METATILE_RG_PokemonMansion_Statue_BlackEyes, FALSE
	return

RG_PokemonMansion_EventScript_PressSwitch_2F::
	setmetatile 24, 14, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 25, 14, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 26, 14, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 24, 15, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 25, 15, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 26, 15, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 10, 28, METATILE_RG_PokemonMansion_Wall_EndPost_Mid, TRUE
	setmetatile 10, 29, METATILE_RG_PokemonMansion_Wall_EndPost_Bottom, TRUE
	setmetatile 10, 30, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 10, 31, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 10, 32, METATILE_RG_PokemonMansion_Wall_EndCap, FALSE
	setmetatile 12,  4, METATILE_RG_PokemonMansion_Barrier_Vertical_TopWall, TRUE
	setmetatile 12,  5, METATILE_RG_PokemonMansion_Barrier_Vertical_TopBase, TRUE
	setmetatile 12,  6, METATILE_RG_PokemonMansion_Barrier_Vertical_MidShadow, TRUE
	setmetatile 12,  7, METATILE_RG_PokemonMansion_Barrier_Vertical_Mid, TRUE
	setmetatile 12,  8, METATILE_RG_PokemonMansion_Barrier_Vertical_Bottom, TRUE
	setmetatile  2, 15, METATILE_RG_PokemonMansion_Statue_RedEyes, FALSE
	return

RG_PokemonMansion_EventScript_ResetSwitch_2F::
	setmetatile 24, 14, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopLeft, TRUE
	setmetatile 25, 14, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopMid, TRUE
	setmetatile 26, 14, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopRight, TRUE
	setmetatile 24, 15, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomLeft, TRUE
	setmetatile 25, 15, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomMid, TRUE
	setmetatile 26, 15, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomRight, TRUE
	setmetatile 10, 28, METATILE_RG_PokemonMansion_Barrier_Vertical_TopWall, TRUE
	setmetatile 10, 29, METATILE_RG_PokemonMansion_Barrier_Vertical_TopBase, TRUE
	setmetatile 10, 30, METATILE_RG_PokemonMansion_Barrier_Vertical_MidShadow, TRUE
	setmetatile 10, 31, METATILE_RG_PokemonMansion_Barrier_Vertical_Mid, TRUE
	setmetatile 10, 32, METATILE_RG_PokemonMansion_Barrier_Vertical_Bottom, TRUE
	setmetatile 12,  4, METATILE_RG_PokemonMansion_Wall_EndPost_Mid, TRUE
	setmetatile 12,  5, METATILE_RG_PokemonMansion_Wall_EndPost_Bottom, TRUE
	setmetatile 12,  6, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 12,  7, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 12,  8, METATILE_RG_PokemonMansion_Wall_EndCap, FALSE
	setmetatile  2, 15, METATILE_RG_PokemonMansion_Statue_BlackEyes, FALSE
	return

RG_PokemonMansion_EventScript_PressSwitch_3F::
	setmetatile 17, 11, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 18, 11, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 19, 11, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 17, 12, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 18, 12, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 19, 12, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 21,  4, METATILE_RG_PokemonMansion_Barrier_Vertical_TopWall, TRUE
	setmetatile 21,  5, METATILE_RG_PokemonMansion_Barrier_Vertical_TopBase, TRUE
	setmetatile 21,  6, METATILE_RG_PokemonMansion_Barrier_Vertical_MidShadow, TRUE
	setmetatile 21,  7, METATILE_RG_PokemonMansion_Barrier_Vertical_Mid, TRUE
	setmetatile 21,  8, METATILE_RG_PokemonMansion_Barrier_Vertical_Bottom, TRUE
	setmetatile 12,  4, METATILE_RG_PokemonMansion_Statue_RedEyes, FALSE
	return

RG_PokemonMansion_EventScript_ResetSwitch_3F::
	setmetatile 17, 11, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopLeft, TRUE
	setmetatile 18, 11, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopMid, TRUE
	setmetatile 19, 11, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopRight, TRUE
	setmetatile 17, 12, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomLeft, TRUE
	setmetatile 18, 12, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomMid, TRUE
	setmetatile 19, 12, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomRight, TRUE
	setmetatile 21,  4, METATILE_RG_PokemonMansion_Wall_EndPost_Mid, TRUE
	setmetatile 21,  5, METATILE_RG_PokemonMansion_Wall_EndPost_Bottom, TRUE
	setmetatile 21,  6, METATILE_RG_PokemonMansion_Floor_ShadeFull, FALSE
	setmetatile 21,  7, METATILE_RG_PokemonMansion_Floor, FALSE
	setmetatile 21,  8, METATILE_RG_PokemonMansion_Wall_EndCap, FALSE
	setmetatile 12,  4, METATILE_RG_PokemonMansion_Statue_BlackEyes, FALSE
	return

RG_PokemonMansion_EventScript_PressSwitch_B1F::
	setmetatile 33, 20, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopLeft, TRUE
	setmetatile 34, 20, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopMid, TRUE
	setmetatile 35, 20, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopRight, TRUE
	setmetatile 33, 21, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomLeft_Basement, TRUE
	setmetatile 34, 21, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomMid_Basement, TRUE
	setmetatile 35, 21, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomRight_Basement, TRUE
	setmetatile 16, 26, METATILE_RG_PokemonMansion_Barrier_Vertical_TopWall, TRUE
	setmetatile 16, 27, METATILE_RG_PokemonMansion_Barrier_Vertical_TopBase, TRUE
	setmetatile 16, 28, METATILE_RG_PokemonMansion_Barrier_Vertical_MidShadow_Basement, TRUE
	setmetatile 16, 29, METATILE_RG_PokemonMansion_Barrier_Vertical_Mid_Basement, TRUE
	setmetatile 16, 30, METATILE_RG_PokemonMansion_Barrier_Vertical_Bottom_Basement, TRUE
	setmetatile 12,  8, METATILE_RG_PokemonMansion_Wall_EndPost_Mid, TRUE
	setmetatile 12,  9, METATILE_RG_PokemonMansion_Wall_EndPost_Bottom, TRUE
	setmetatile 12, 10, METATILE_RG_PokemonMansion_BasementFloor_ShadeFull, FALSE
	setmetatile 12, 11, METATILE_RG_PokemonMansion_BasementFloor, FALSE
	setmetatile 12, 12, METATILE_RG_PokemonMansion_Wall_EndCap_Basement, FALSE
	setmetatile 20, 22, METATILE_RG_PokemonMansion_BasementFloor_ShadeFull, FALSE
	setmetatile 21, 22, METATILE_RG_PokemonMansion_BasementFloor, FALSE
	setmetatile 22, 22, METATILE_RG_PokemonMansion_BasementFloor, FALSE
	setmetatile 20, 23, METATILE_RG_PokemonMansion_BasementFloor_ShadeFull, FALSE
	setmetatile 21, 23, METATILE_RG_PokemonMansion_BasementFloor, FALSE
	setmetatile 22, 23, METATILE_RG_PokemonMansion_BasementFloor, FALSE
	setmetatile 24, 28, METATILE_RG_PokemonMansion_Statue_RedEyes_Basement, FALSE
	setmetatile 27,  4, METATILE_RG_PokemonMansion_Statue_RedEyes_Basement, FALSE
	return

RG_PokemonMansion_EventScript_ResetSwitch_B1F::
	setmetatile 33, 20, METATILE_RG_PokemonMansion_BasementFloor_ShadeFull, FALSE
	setmetatile 34, 20, METATILE_RG_PokemonMansion_BasementFloor, FALSE
	setmetatile 35, 20, METATILE_RG_PokemonMansion_BasementFloor, FALSE
	setmetatile 33, 21, METATILE_RG_PokemonMansion_BasementFloor_ShadeFull, FALSE
	setmetatile 34, 21, METATILE_RG_PokemonMansion_BasementFloor, FALSE
	setmetatile 35, 21, METATILE_RG_PokemonMansion_BasementFloor, FALSE
	setmetatile 16, 26, METATILE_RG_PokemonMansion_Wall_EndPost_Mid, TRUE
	setmetatile 16, 27, METATILE_RG_PokemonMansion_Wall_EndPost_Bottom, TRUE
	setmetatile 16, 28, METATILE_RG_PokemonMansion_BasementFloor_ShadeFull, FALSE
	setmetatile 16, 29, METATILE_RG_PokemonMansion_BasementFloor, FALSE
	setmetatile 16, 30, METATILE_RG_PokemonMansion_Wall_EndCap_Basement, FALSE
	setmetatile 12,  8, METATILE_RG_PokemonMansion_Barrier_Vertical_TopWall, TRUE
	setmetatile 12,  9, METATILE_RG_PokemonMansion_Barrier_Vertical_TopBase, TRUE
	setmetatile 12, 10, METATILE_RG_PokemonMansion_Barrier_Vertical_MidShadow_Basement, TRUE
	setmetatile 12, 11, METATILE_RG_PokemonMansion_Barrier_Vertical_Mid_Basement, TRUE
	setmetatile 12, 12, METATILE_RG_PokemonMansion_Barrier_Vertical_Bottom_Basement, TRUE
	setmetatile 20, 22, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopLeft, TRUE
	setmetatile 21, 22, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopMid, TRUE
	setmetatile 22, 22, METATILE_RG_PokemonMansion_Barrier_Horizontal_TopRight, TRUE
	setmetatile 20, 23, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomLeft_Basement, TRUE
	setmetatile 21, 23, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomMid_Basement, TRUE
	setmetatile 22, 23, METATILE_RG_PokemonMansion_Barrier_Horizontal_BottomRight_Basement, TRUE
	setmetatile 24, 28, METATILE_RG_PokemonMansion_Statue_BlackEyes_Basement, FALSE
	setmetatile 27,  4, METATILE_RG_PokemonMansion_Statue_BlackEyes_Basement, FALSE
	return
