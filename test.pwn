#include <test-boilerplate>

#include "personal-space.inc"

main() {
	//
}

public OnPlayerApproachPlayer(playerid, targetid, E_APPROACH_DIRECTION:fromDirection, Float:angle) {
	new str[128];

	format(str, 128, "You approached player %d from direction %d %f", targetid, _:fromDirection, angle);
	SendClientMessage(playerid, -1, str);

	format(str, 128, "You were approached by player %d from direction %d %f", playerid, _:fromDirection, angle);
	SendClientMessage(targetid, -1, str);

	return 0;
}

public OnPlayerLeavePlayer(playerid, targetid) {
	new str[128];

	format(str, 128, "You left player %d", targetid);
	SendClientMessage(playerid, -1, str);

	format(str, 128, "You were left by player %d", playerid);
	SendClientMessage(targetid, -1, str);

	return 0;
}
