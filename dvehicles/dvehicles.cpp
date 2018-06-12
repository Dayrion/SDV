using namespace std;
#define HAVE_STDINT_H
#define __STDC__ 1
//#include <sampgdk\sdk.h>
#include <sampgdk\core.h>
#include <sampgdk\a_vehicles.h>
#include <sampgdk\a_players.h>
//#include <SDK/plugin.h>

typedef void
	(*logprintf_t)(char* format, ...)
;

logprintf_t
	logprintf
;

void
	**ppPluginData
;

extern void
	*pAMXFunctions
;

#include "Defines.h"
#include "Natives.h"

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	return sampgdk::Load(ppData);;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	sampgdk::Unload();
}

AMX_NATIVE_INFO projectNatives[] =
{
	{ "CreateDynamicVehicle", CreateDynamicVehicle },
	{ "DestroyDynamicVehicle", DestroyDynamicVehicle },
	{ "SetStreamDistance", SetStreamDistance },
	{ "SetVehicleCheckDelay", SetVehicleCheckDelay },
	{ "GetDynamicVehiclePos", GetDynamicVehiclePos },
	{ "SetDynamicVehiclePos", SetDynamicVehiclePos },
	{ "SetDynamicVehicleZAngle", SetDynamicVehicleZAngle },
	{ "GetDynamicVehicleZAngle", GetDynamicVehicleZAngle },
	{ "GetDynamicVehicleRotationQuat", GetDynamicVehicleRotationQuat },
	{ "GetDVehicleDistanceFromPoint", GetDVehicleDistanceFromPoint },
	{ "SetDynamicVehicleParamsEx", SetDynamicVehicleParamsEx },
	{ "GetDynamicVehicleParamsEx", GetDynamicVehicleParamsEx },
	{ "SetDynamicVehicleToRespawn", SetDynamicVehicleToRespawn },
	{ "LinkDynamicVehicleToInterior", LinkDynamicVehicleToInterior },
	{ "GetDynamicVehicleInterior", GetDynamicVehicleInterior },
	{ "AddDynamicVehicleComponent", AddDynamicVehicleComponent },
	{ "RemoveDynamicVehicleComponent", RemoveDynamicVehicleComponent },
	{ "GetDVehicleComponentInSlot", GetDVehicleComponentInSlot },
	{ "ChangeDynamicVehicleColor", ChangeDynamicVehicleColor },
	{ "GetDynamicVehicleColor", GetDynamicVehicleColor },
	{ "ChangeDynamicVehiclePaintjob", ChangeDynamicVehiclePaintjob },
	{ "GetDynamicVehiclePaintjob", GetDynamicVehiclePaintjob },
	{ "SetDynamicVehicleHealth", SetDynamicVehicleHealth },
	{ "GetDynamicVehicleHealth", GetDynamicVehicleHealth },
	{ "SetDynamicVehicleNumberPlate", SetDynamicVehicleNumberPlate },
	{ "GetDynamicVehicleNumberPlate", GetDynamicVehicleNumberPlate },
	{ "SetDynamicVehicleModel", SetDynamicVehicleModel },
	{ "GetDynamicVehicleModel", GetDynamicVehicleModel },
	{ "RepairDynamicVehicle", RepairDynamicVehicle },
	{ "GetDynamicVehicleVelocity", GetDynamicVehicleVelocity },
	{ "SetDynamicVehicleVelocity", SetDynamicVehicleVelocity },
	{ "SetDVehicleAngularVelocity", SetDVehicleAngularVelocity },
	{ "GetDynamicVehicleDamageStatus", GetDynamicVehicleDamageStatus },
	{ "UpdateDVehicleDamageStatus", UpdateDVehicleDamageStatus },
	{ "SetDynamicVehicleVirtualWorld", SetDynamicVehicleVirtualWorld },
	{ "GetDynamicVehicleVirtualWorld", GetDynamicVehicleVirtualWorld },
	{ "IsDynamicVehicleStreamedIn", IsDynamicVehicleStreamedIn },
	{ "IsPlayerInDynamicVehicle", IsPlayerInDynamicVehicle },
	{ "PlayerSpectateDynamicVehicle", PlayerSpectateDynamicVehicle },
	{ "IsValidDynamicVehicle", IsValidDynamicVehicle },
	{ "PutPlayerInDynamicVehicle", PutPlayerInDynamicVehicle },
	{ "GetPlayerDynamicVehicleID", GetPlayerDynamicVehicleID },
	{ "GetPlayerSurfingDVehicleID", GetPlayerSurfingDVehicleID },
	{ "UpdateTheVehicles", UpdateTheVehicles },
	{ "SetDVehicleParamsForPlayer", SetDVehicleParamsForPlayer },
	{ "GetVehicleDynamicID", GetVehicleDynamicID },
	{ "MoveVehicleDynamicID", MoveVehicleDynamicID },
	{ 0, 0 }
};


PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	globalamx = amx;
	return amx_Register(amx, projectNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid)
{
	ConnectedIds.push_back(playerid);
	connected_players++;
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
{
	vector<int>::iterator it = find_if(ConnectedIds.begin(), ConnectedIds.end(), [playerid](const int &item) { return (item == playerid); });
	if (it != ConnectedIds.end())
		ConnectedIds.erase(it);
	connected_players--;
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnVehicleSpawn(int vehicleid)
{
	if (IsDynamicVehicle(vehicleid))
	{
		ExecuteFunction("OnDynamicVehicleSpawn", { (VDynamicID[vehicleid] + 1) }, {});
	}
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL OnVehicleDeath(int vehicleid, int killerid)
{
	if (IsDynamicVehicle(vehicleid))
	{
		ExecuteFunction("OnDynamicVehicleDeath", { (VDynamicID[vehicleid] + 1), killerid }, {});
	}
}

PLUGIN_EXPORT void PLUGIN_CALL OnPlayerEnterVehicle(int playerid, int vehicleid, bool ispassenger)
{
	if (IsDynamicVehicle(vehicleid))
	{
		ExecuteFunction("OnPlayerEnterDynamicVehicle", { playerid, (VDynamicID[vehicleid] + 1), ispassenger }, {});
	}
}

PLUGIN_EXPORT void PLUGIN_CALL OnPlayerExitVehicle(int playerid, int vehicleid)
{
	if (IsDynamicVehicle(vehicleid))
	{
		ExecuteFunction("OnPlayerExitDynamicVehicle", { playerid, (VDynamicID[vehicleid] + 1) }, {});
	}
}

PLUGIN_EXPORT bool PLUGIN_CALL OnVehicleMod(int playerid, int vehicleid, int componentid)
{
	if (IsDynamicVehicle(vehicleid))
	{
		ExecuteFunction("OnDynamicVehicleMod", { playerid, (VDynamicID[vehicleid] + 1), componentid }, {});
	}
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL OnVehiclePaintjob(int playerid, int vehicleid, int paintjobid)
{
	if (IsDynamicVehicle(vehicleid))
	{
		VehiclesData[VDynamicID[vehicleid]].Paintjob = paintjobid;
		ExecuteFunction("OnDynamicVehiclePaintjob", { playerid, (VDynamicID[vehicleid] + 1), paintjobid }, {});
	}
}

PLUGIN_EXPORT bool PLUGIN_CALL OnVehicleRespray(int playerid, int vehicleid, int color1, int color2)
{
	if (IsDynamicVehicle(vehicleid))
	{
		VehiclesData[VDynamicID[vehicleid]].Color1 = color1;
		VehiclesData[VDynamicID[vehicleid]].Color2 = color2;
		ExecuteFunction("OnDynamicVehicleRespray", { playerid, (VDynamicID[vehicleid] + 1), color1, color2 }, {});
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnVehicleDamageStatusUpdate(int vehicleid, int playerid)
{
	if (IsDynamicVehicle(vehicleid))
	{
		ExecuteFunction("OnDVehicleDamageStatusUpdate", { (VDynamicID[vehicleid] + 1), playerid }, {});
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnUnoccupiedVehicleUpdate(int vehicleid, int playerid, int passenger_seat, float new_x, float new_y, float new_z, float vel_x, float vel_y, float vel_z)
{
	if (IsDynamicVehicle(vehicleid))
	{
		ExecuteFunction("OnDUnoccupiedVehicleUpdate", { (VDynamicID[vehicleid] + 1), playerid, passenger_seat }, { new_x, new_y, new_z, vel_x, vel_y, vel_z });
	}
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL OnVehicleStreamIn(int vehicleid, int forplayerid)
{
	if (IsDynamicVehicle(vehicleid))
	{
		ExecuteFunction("OnDynamicVehicleStreamIn", { (VDynamicID[vehicleid] + 1), forplayerid }, {});
	}
}

PLUGIN_EXPORT void PLUGIN_CALL OnVehicleStreamOut(int vehicleid, int forplayerid)
{
	if (IsDynamicVehicle(vehicleid))
	{
		ExecuteFunction("OnDynamicVehicleStreamOut", { (VDynamicID[vehicleid] + 1), forplayerid }, {});
	}
}

PLUGIN_EXPORT bool PLUGIN_CALL OnTrailerUpdate(int playerid, int vehicleid)
{
	if (IsDynamicVehicle(vehicleid))
	{
		ExecuteFunction("OnDynamicTrailerUpdate", { playerid, (VDynamicID[vehicleid] + 1) }, {});
	}
	return true;
}

PLUGIN_EXPORT int PLUGIN_CALL OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ)
{
	if (hittype == BULLET_HIT_TYPE_VEHICLE && IsDynamicVehicle(hitid))
	{
		return ExecuteFunction("OnPlayerShotDynamicVehicle", { playerid, weaponid, (VDynamicID[hitid] + 1) }, { fX, fY, fZ});
	}
	else
		return 1;
}

//$(SolutionDir)$(Configuration)\