RG_SevenIsland_TrainerTower_MapScripts::
	.byte 0

RG_SevenIsland_TrainerTower_EventScript_TrainerTowerSign::
	msgbox RG_SevenIsland_TrainerTower_Text_TrainerTowerSign, MSGBOX_SIGN
	end

RG_SevenIsland_TrainerTower_EventScript_TrainerTowerAheadSign::
	msgbox RG_SevenIsland_TrainerTower_Text_TrainerTowerAhead, MSGBOX_SIGN
	end

RG_SevenIsland_TrainerTower_Text_TrainerTowerSign::
    .string "TRAINER TOWER ENTRANCE\n"
    .string "Rise to the Challenge, TRAINERS!$"

RG_SevenIsland_TrainerTower_Text_TrainerTowerAhead::
    .string "TRAINER TOWER AHEAD$"
