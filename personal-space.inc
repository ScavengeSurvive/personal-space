#if defined _personal_space_included
	#endinput
#endif
#define _personal_space_included

#include <logger>
#include <YSI\y_iterate>
#include <YSI\y_timers>
#include <YSI\y_hooks>
#include <streamer>
#include <mathutil>


enum E_APPROACH_DIRECTION {
	APPROACH_FROM_FRONT,
	APPROACH_FROM_LEFT,
	APPROACH_FROM_BACK,
	APPROACH_FROM_RIGHT,
}

#define PSPACE_STREAMER_AREA_IDENTIFIER 1098

forward IsPlayerNextToPlayer(playerid, targetid);
/*
# Description
Checks if a player is standing next to another player.
*/

forward GetPlayersNextToPlayer(playerid, players[], len = sizeof players);
/*
# Description
Returns a list of players stood next to the player.
*/

forward E_APPROACH_DIRECTION:GetApproachDirection(Float:angle);
/*
# Description
Returns a direction from the given angle.
*/


forward OnPlayerApproachPlayer(playerid, targetid, E_APPROACH_DIRECTION:fromDirection);
/*
# Called
When a player walks within 2m of another player.
*/

forward OnPlayerLeavePlayer(playerid, targetid);
/*
# Called
When a player leaves the 2m boundary of another player.
*/


static
	Area[MAX_PLAYERS],
	Iterator:NearbyPlayers[MAX_PLAYERS]<MAX_PLAYERS>;


hook OnGameModeInit() {
	Iter_Init(NearbyPlayers);
}

#if defined FILTERSCRIPT
hook OnFilterScriptInit()
{
	for(new i; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i))
			_pspace_connect(i);
	}
}

hook OnFilterScriptExit()
{
	for(new i; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i))
			_pspace_disconnect(i);
	}
}
#endif

hook OnPlayerConnect(playerid) {
	_pspace_connect(playerid);
}

hook OnPlayerDisconnect(playerid) {
	_pspace_disconnect(playerid);
}

_pspace_connect(playerid) {
	new data[2];

	data[0] = PSPACE_STREAMER_AREA_IDENTIFIER;
	data[1] = playerid;

	Area[playerid] = CreateDynamicSphere(0.0, 0.0, 0.0, 2.0);
	Streamer_SetArrayData(STREAMER_TYPE_AREA, Area[playerid], E_STREAMER_EXTRA_ID, data, 2);
}

_pspace_disconnect(playerid) {
	DestroyDynamicArea(Area[playerid]);
	Area[playerid] = -1;
}

hook OnPlayerSpawn(playerid) {
	AttachDynamicAreaToPlayer(Area[playerid], playerid);
}

hook OnPlayerEnterDynArea(playerid, areaid) {
	if(GetPlayerState(playerid) != PLAYER_STATE_SPECTATING) {
		new data[2];

		Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, data, 2);

		if(data[0] == PSPACE_STREAMER_AREA_IDENTIFIER) {
			if(data[1] == playerid) {
				return Y_HOOKS_CONTINUE_RETURN_0;
			}

			if(GetPlayerState(data[1]) == PLAYER_STATE_SPECTATING) {
				return Y_HOOKS_CONTINUE_RETURN_0;
			}

			if(areaid != Area[data[1]]) {
				err("player area id mismatch");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}

			Iter_Add(NearbyPlayers[playerid], data[1]);
			Iter_Add(NearbyPlayers[data[1]], playerid);

			new
				Float:px,
				Float:py,
				Float:pz,
				Float:tx,
				Float:ty,
				Float:tz,
				Float:angle,
				E_APPROACH_DIRECTION:direction;
			
			GetPlayerPos(playerid, px, py, pz);
			GetPlayerPos(playerid, tx, ty, tz);

			angle = GetAbsoluteAngle(GetAngleToPoint(tx, ty, px, py));
			direction = GetApproachDirection(angle);

			CallLocalFunction("OnPlayerApproachPlayer", "ddd", playerid, data[1], _:direction);
		}
	}

	return Y_HOOKS_CONTINUE_RETURN_0;
}

hook OnPlayerLeaveDynArea(playerid, areaid) {
	if(GetPlayerState(playerid) != PLAYER_STATE_SPECTATING) {
		new data[2];

		Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, data, 2);

		if(data[0] == PSPACE_STREAMER_AREA_IDENTIFIER) {
			if(data[1] == playerid) {
				return Y_HOOKS_CONTINUE_RETURN_0;
			}

			if(GetPlayerState(data[1]) == PLAYER_STATE_SPECTATING) {
				return Y_HOOKS_CONTINUE_RETURN_0;
			}

			if(areaid != Area[data[1]]) {
				err("player area id mismatch");
			} else {
				CallLocalFunction("OnPlayerLeavePlayer", "dd", playerid, data[1]);
			}

			Iter_Remove(NearbyPlayers[playerid], data[1]);
			Iter_Remove(NearbyPlayers[data[1]], playerid);
		}
	}

	return Y_HOOKS_CONTINUE_RETURN_0;
}

stock IsPlayerNextToPlayer(playerid, targetid) {
	if(playerid == targetid) {
		return 0;
	}
	
	if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING) {
		return 0;
	}

	if(GetPlayerState(targetid) == PLAYER_STATE_SPECTATING) {
		return 0;
	}

	return IsPlayerInDynamicArea(playerid, Area[targetid]);
}

stock GetPlayersNextToPlayer(playerid, players[], len = sizeof players) {
	new idx;

	foreach(new i : NearbyPlayers[playerid]) {
		if(idx == len - 1) {
			break;
		}
		players[idx++] = i;
	}

	return idx;
}

stock E_APPROACH_DIRECTION:GetApproachDirection(Float:angle) {
	if(155.0 < angle < 205.0) {
		return APPROACH_FROM_BACK;
	} else if(225.0 < angle < 315.0) {
		return APPROACH_FROM_RIGHT;
	} else if(45.0 < angle < 155.0) {
		return APPROACH_FROM_LEFT;
	}
	return APPROACH_FROM_FRONT;
}
