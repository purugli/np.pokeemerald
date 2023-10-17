RG_Route10_MapScripts::
	.byte 0

RG_Route10_EventScript_NorthRockTunnelSign::
	msgbox RG_Route10_Text_RockTunnelDetourToLavender, MSGBOX_SIGN
	end

RG_Route10_EventScript_SouthRockTunnelSign::
	msgbox RG_Route10_Text_RockTunnel, MSGBOX_SIGN
	end

RG_Route10_EventScript_PowerPlantSign::
	msgbox RG_Route10_Text_PowerPlant, MSGBOX_SIGN
	end

RG_Route10_Text_RockTunnelDetourToLavender::
    .string "ROCK TUNNEL\n"
    .string "Detour to LAVENDER TOWN$"

RG_Route10_Text_RockTunnel::
    .string "ROCK TUNNEL$"

RG_Route10_Text_PowerPlant::
    .string "POWER PLANT$"
