#if defined _Dynamic_Vehicle_
	#endinput
#endif
/******************************** Defines *************************************/
#if !defined _samp_included
	#include <a_samp>
#endif

#if !defined V_STREAM_DIS
	#define V_STREAM_DIS 200.0
#endif
#if !defined V_CHECK_INTERVAL
    #define V_CHECK_INTERVAL 250
#endif

#define _Dynamic_Vehicle_
/******************************** Natives *************************************/
#if !defined IsValidVehicle
	native IsValidVehicle(vehicleid);
#endif
native CreateDynamicVehicle(modelid, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren=0, virtualworld = 0, interior = 0, Float: streamdis = V_STREAM_DIS);
native DestroyDynamicVehicle(vehicleid);
native SetStreamDistance(Float:streamdistance);
native SetVehicleCheckDelay(milliseconds);
native GetDynamicVehiclePos(vehicleid, &Float:x, &Float:y, &Float:z);
native SetDynamicVehiclePos(vehicleid, Float:x, Float:y, Float:z);
native SetDynamicVehicleZAngle(vehicleid, Float:z_angle);
native GetDynamicVehicleZAngle(vehicleid, &Float:z_angle);
native GetDynamicVehicleRotationQuat(vehicleid, &Float:w, &Float:x, &Float:y, &Float:z);
native Float:GetDynVehicleDistanceFromPoint(vehicleid, Float:X, Float:Y, Float:Z);
native SetDynamicVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
native GetDynamicVehicleParamsEx(vehicleid, &engine, &lights, &alarm, &doors, &bonnet, &boot, &objective);
native SetDynamicVehicleToRespawn(vehicleid);
native LinkDynamicVehicleToInterior(vehicleid, interiorid);
native GetDynamicVehicleInterior(vehicleid); // New
native AddDynamicVehicleComponent(vehicleid, componentid);
native RemoveDynamicVehicleComponent(vehicleid, componentid);
native GetDynVehicleComponentInSlot(vehicleid, slot); // There is 1 slot for each CARMODTYPE_*
native ChangeDynamicVehicleColor(vehicleid, color1, color2);
native GetDynamicVehicleColor(vehicleid, &color1, &color2); // New
native ChangeDynamicVehiclePaintjob(vehicleid, paintjobid);
native GetDynamicVehiclePaintjob(vehicleid);
native SetDynamicVehicleHealth(vehicleid, Float:health);
native GetDynamicVehicleHealth(vehicleid, &Float:health);
native SetDynamicVehicleNumberPlate(vehicleid, numberplate[]);
native GetDynamicVehicleNumberPlate(vehicleid, numberplate[], len = sizeof numberplate); // new
native SetDynamicVehicleModel(vehicleid, model, bool:bultinrespawn = false); // New requires respawn
native GetDynamicVehicleModel(vehicleid);
native RepairDynamicVehicle(vehicleid); // Repairs the damage model and resets the health
native GetDynamicVehicleVelocity(vehicleid, &Float:X, &Float:Y, &Float:Z);
native SetDynamicVehicleVelocity(vehicleid, Float:X, Float:Y, Float:Z);
native SetDVehicleAngularVelocity(vehicleid, Float:X, Float:Y, Float:Z);
native GetDynamicVehicleDamageStatus(vehicleid, &panels, &doors, &lights, &tires);
native UpdateDynVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
native SetDynamicVehicleVirtualWorld(vehicleid, worldid);
native GetDynamicVehicleVirtualWorld(vehicleid);
native IsDynamicVehicleStreamedIn(vehicleid, forplayerid);
native IsPlayerInDynamicVehicle(playerid, vehicleid);
native PlayerSpectateDynamicVehicle(playerid, targetvehicleid, mode = SPECTATE_MODE_NORMAL);
native IsValidDynamicVehicle(vehicleid);
native PutPlayerInDynamicVehicle(playerid, vehicleid, seatid);
native GetPlayerDynamicVehicleID(playerid);
native GetPlayerSurfingDVehicleID(playerid);
native UpdateTheVehicles();
native SetDVehicleParamsForPlayer(vehicleid, playerid, objective, doorslocked);
native GetVehicleDynamicID(vehicleid);
native MoveVehicleDynamicID(fromid, toid = -1);

#if defined _streamer_included
	native AttachDynamicObjectToDVehicle(objectid, vehicleid, Float:offsetx, Float:offsety, Float:offsetz, Float:rx, Float:ry, Float:rz);
#endif

// Trailer
native AttachTrailerToDynamicVehicle(trailerid, vehicleid);
native DetachTrailerFromDynamicVehicle(vehicleid);
native bool:IsTrailerAttachedToDynVehicle(vehicleid);
native GetDynamicVehicleTrailer(vehicleid);

native Attach3DTextLabelToDynVehicle(Text3D:id, vehicleid, Float:OffsetX, Float:OffsetY, Float:OffsetZ); // Label gotta be dynamic

/*
native AttachPlayerObjectToDynVehicle(playerid, objectid, vehicleid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:RotZ);
native Update3DTextLabelText(Text3D:id, color, text[]);
native PlayerText3D:CreatePlayer3DTextLabel(playerid, text[], color, Float:X, Float:Y, Float:Z, Float:DrawDistance, attachedplayer=INVALID_PLAYER_ID, attachedvehicle=INVALID_VEHICLE_ID, testLOS=0);
native DeletePlayer3DTextLabel(playerid, PlayerText3D:id);
native UpdatePlayer3DTextLabelText(playerid, PlayerText3D:id, color, text[]);
^ To be made
*/

enum E_VEHICLE_DMG_TYPE
{
	DAMAGE_PANELS,
	DAMAGE_DOORS,
	DAMAGE_LIGHTS,
	DAMAGE_TIRES
};

enum _:E_VEHICLE_DMG_STATE
{
	DOOR_HEALTHY_CLOSED,
	DOOR_HEALTHY_OPENED,
	DOOR_DAMAGED_CLOSED,
	DOOR_DAMAGED_OPENED,
	DOOR_MISSING,

	TIRES_FIXED = 0,
	TIRES_POPPED,

	LIGHT_FIXED = 0,
	LIGHT_DESTROYED,

	PANEL_FIXED = 0,
	PANEL_DESTROYED,
};

/*
	Door:
		right_front, left_front, right_back, left_back

	Tires:
		right_front, left_front, right_back, left_back

	Light:
		right_front, left_front, back

	Panel:
		front_left_panel, front_right_panel, rear_left_panel, rear_right_panel, windshield, front_bumper, rear_bumper
*/
native UpdateSpecificDynVehDmgStatus(vehicleid, E_VEHICLE_DMG_TYPE:damage_type, ...);

/***************************Functions Replacer*********************************/

#if defined RVFunc

/******************************************************************************/
#if defined _ALS_OnVehicleSpawn
	#undef OnVehicleSpawn
#else
	#define _ALS_OnVehicleSpawn
#endif
/******************************************************************************/
#if defined _ALS_OnVehicleDeath
	#undef OnVehicleDeath
#else
	#define _ALS_OnVehicleDeath
#endif
/******************************************************************************/
#if defined _ALS_OnPlayerEnterVehicle
	#undef OnPlayerEnterVehicle
#else
	#define _ALS_OnPlayerEnterVehicle
#endif
/******************************************************************************/
#if defined _ALS_OnPlayerExitVehicle
	#undef OnPlayerExitVehicle
#else
	#define _ALS_OnPlayerExitVehicle
#endif
/******************************************************************************/
#if defined _ALS_OnVehicleMod
	#undef OnVehicleMod
#else
	#define _ALS_OnVehicleMod
#endif
/******************************************************************************/
#if defined _ALS_OnVehiclePaintjob
	#undef OnVehiclePaintjob
#else
	#define _ALS_OnVehiclePaintjob
#endif
/******************************************************************************/
#if defined _ALS_OnVehicleRespray
	#undef OnVehicleRespray
#else
	#define _ALS_OnVehicleRespray
#endif
/******************************************************************************/
#if defined _ALS_OnVehicleDamageStatusUpdat
	#undef OnVehicleDamageStatusUpdate
#else
	#define _ALS_OnVehicleDamageStatusUpdat
#endif
/******************************************************************************/
#if defined _ALS_OnUnoccupiedVehicleUpdate
	#undef OnUnoccupiedVehicleUpdate
#else
	#define _ALS_OnUnoccupiedVehicleUpdate
#endif
/******************************************************************************/
#if defined _ALS_OnVehicleStreamIn
	#undef OnVehicleStreamIn
#else
	#define _ALS_OnVehicleStreamIn
#endif
/******************************************************************************/
#if defined _ALS_OnVehicleStreamOut
	#undef OnVehicleStreamOut
#else
	#define _ALS_OnVehicleStreamOut
#endif
/******************************************************************************/
#if defined _ALS_OnTrailerUpdate
	#undef OnTrailerUpdate
#else
	#define _ALS_OnTrailerUpdate
#endif
/******************************************************************************/
#if defined _ALS_CreateVehicle
	#undef CreateVehicle
#else
	#define _ALS_CreateVehicle
#endif
/******************************************************************************/
#if defined _ALS_DestroyVehicle
	#undef DestroyVehicle
#else
	#define _ALS_DestroyVehicle
#endif
/******************************************************************************/
#if defined _ALS_IsVehicleStreamedIn
	#undef IsVehicleStreamedIn
#else
	#define _ALS_IsVehicleStreamedIn
#endif
/******************************************************************************/
#if defined _ALS_GetVehiclePos
	#undef GetVehiclePos
#else
	#define _ALS_GetVehiclePos
#endif
/******************************************************************************/
#if defined _ALS_SetVehiclePos
	#undef SetVehiclePos
#else
	#define _ALS_SetVehiclePos
#endif
/******************************************************************************/
#if defined _ALS_GetVehicleZAngle
	#undef GetVehicleZAngle
#else
	#define _ALS_GetVehicleZAngle
#endif
/******************************************************************************/
#if defined _ALS_SetVehicleZAngle
	#undef SetVehicleZAngle
#else
	#define _ALS_SetVehicleZAngle
#endif
/******************************************************************************/
#if defined _ALS_GetVehicleRotationQuat
	#undef GetVehicleRotationQuat
#else
	#define _ALS_GetVehicleRotationQuat
#endif
/******************************************************************************/
#if defined _ALS_GetVehicleDistanceFromPoin
	#undef GetVehicleDistanceFromPoint
#else
	#define _ALS_GetVehicleDistanceFromPoin
#endif
/******************************************************************************/
#if defined _ALS_SetVehicleParamsEx
	#undef SetVehicleParamsEx
#else
	#define _ALS_SetVehicleParamsEx
#endif
/******************************************************************************/
#if defined _ALS_GetVehicleParamsEx
	#undef GetVehicleParamsEx
#else
	#define _ALS_GetVehicleParamsEx
#endif
/******************************************************************************/
#if defined _ALS_SetVehicleParamsForPlayer
	#undef SetVehicleParamsForPlayer
#else
	#define _ALS_SetVehicleParamsForPlayer
#endif
/******************************************************************************/
#if defined _ALS_SetVehicleToRespawn
	#undef SetVehicleToRespawn
#else
	#define _ALS_SetVehicleToRespawn
#endif
/******************************************************************************/
#if defined _ALS_LinkVehicleToInterior
	#undef LinkVehicleToInterior
#else
	#define _ALS_LinkVehicleToInterior
#endif
/******************************************************************************/
#if defined _ALS_AddVehicleComponent
	#undef AddVehicleComponent
#else
	#define _ALS_AddVehicleComponent
#endif
/******************************************************************************/
#if defined _ALS_RemoveVehicleComponent
	#undef RemoveVehicleComponent
#else
	#define _ALS_RemoveVehicleComponent
#endif
/******************************************************************************/
#if defined _ALS_ChangeVehicleColor
	#undef ChangeVehicleColor
#else
	#define _ALS_ChangeVehicleColor
#endif
/******************************************************************************/
#if defined _ALS_ChangeVehiclePaintjob
	#undef ChangeVehiclePaintjob
#else
	#define _ALS_ChangeVehiclePaintjob
#endif
/******************************************************************************/
#if defined _ALS_SetVehicleHealth
	#undef SetVehicleHealth
#else
	#define _ALS_ChangeVehiclePaintjob
#endif
/******************************************************************************/
#if defined _ALS_GetVehicleHealth
	#undef GetVehicleHealth
#else
	#define _ALS_GetVehicleHealth
#endif
/******************************************************************************/
#if defined _ALS_SetVehicleNumberPlate
	#undef SetVehicleNumberPlate
#else
	#define _ALS_SetVehicleNumberPlate
#endif
/******************************************************************************/
#if defined _ALS_GetVehicleModel
	#undef GetVehicleModel
#else
	#define _ALS_GetVehicleModel
#endif
/******************************************************************************/
#if defined _ALS_GetVehicleComponentInSlot
	#undef GetVehicleComponentInSlot
#else
	#define _ALS_GetVehicleComponentInSlot
#endif
/******************************************************************************/
#if defined _ALS_RepairVehicle
	#undef RepairVehicle
#else
	#define _ALS_RepairVehicle
#endif
/******************************************************************************/
#if defined _ALS_GetVehicleVelocity
	#undef GetVehicleVelocity
#else
	#define _ALS_GetVehicleVelocity
#endif
/******************************************************************************/
#if defined _ALS_SetVehicleVelocity
	#undef SetVehicleVelocity
#else
	#define _ALS_SetVehicleVelocity
#endif
/******************************************************************************/
#if defined _ALS_SetVehicleAngularVelocity
	#undef SetVehicleAngularVelocity
#else
	#define _ALS_SetVehicleAngularVelocity
#endif
/******************************************************************************/
#if defined _ALS_GetVehicleDamageStatus
	#undef GetVehicleDamageStatus
#else
	#define _ALS_GetVehicleDamageStatus
#endif
/******************************************************************************/
#if defined _ALS_UpdateVehicleDamageStatus
	#undef UpdateVehicleDamageStatus
#else
	#define _ALS_UpdateVehicleDamageStatus
#endif
/******************************************************************************/
#if defined _ALS_SetVehicleVirtualWorld
	#undef SetVehicleVirtualWorld
#else
	#define _ALS_SetVehicleVirtualWorld
#endif
/******************************************************************************/
#if defined _ALS_GetVehicleVirtualWorld
	#undef GetVehicleVirtualWorld
#else
	#define _ALS_GetVehicleVirtualWorld
#endif
/******************************************************************************/
#if defined _ALS_GetPlayerVehicleID
	#undef GetPlayerVehicleID
#else
	#define _ALS_GetPlayerVehicleID
#endif
/******************************************************************************/
#if defined _ALS_PutPlayerInVehicle
	#undef PutPlayerInVehicle
#else
	#define _ALS_PutPlayerInVehicle
#endif
/******************************************************************************/
#if defined _ALS_IsPlayerInVehicle
	#undef IsPlayerInVehicle
#else
	#define _ALS_IsPlayerInVehicle
#endif
/******************************************************************************/
#if defined _ALS_PlayerSpectateVehicle
	#undef PlayerSpectateVehicle
#else
	#define _ALS_PlayerSpectateVehicle
#endif
/******************************************************************************/
#if defined _ALS_IsValidVehicle
	#undef IsValidVehicle
#else
	#define _ALS_IsValidVehicle
#endif
/******************************************************************************/
#if defined _ALS_AttachDynamicObjectToVehic
	#undef AttachDynamicObjectToVehicle
#else
	#define _ALS_AttachDynamicObjectToVehic
#endif
/******************************************************************************/
#define OnVehicleSpawn OnDynamicVehicleSpawn
#define OnVehicleDeath OnDynamicVehicleDeath
#define OnPlayerEnterVehicle OnPlayerEnterDynamicVehicle
#define OnPlayerExitVehicle OnPlayerExitDynamicVehicle
#define OnVehicleMod OnDynamicVehicleMod
#define OnVehiclePaintjob OnDynamicVehiclePaintjob
#define OnVehicleRespray OnDynamicVehicleRespray
#define OnVehicleDamageStatusUpdate OnDynamicVehicleDamageStatusUpdate
#define OnUnoccupiedVehicleUpdate OnUnoccupiedDynamicVehicleUpdate
#define OnVehicleStreamIn OnDynamicVehicleStreamIn
#define OnVehicleStreamOut OnDynamicVehicleStreamOut
#define OnTrailerUpdate OnDynamicTrailerUpdate
#define CreateVehicle CreateDynamicVehicle
#define DestroyVehicle DestroyDynamicVehicle
#define IsVehicleStreamedIn IsDynamicVehicleStreamedIn
#define GetVehiclePos GetDynamicVehiclePos
#define SetVehiclePos SetDynamicVehiclePos
#define GetVehicleZAngle GetDynamicVehicleZAngle
#define SetVehicleZAngle SetDynamicVehicleZAngle
#define GetVehicleRotationQuat GetDynamicVehicleRotationQuat
#define GetVehicleDistanceFromPoint GetDynVehicleDistanceFromPoint
#define SetVehicleParamsForPlayer SetDVehicleParamsForPlayer
#define SetVehicleParamsEx SetDynamicVehicleParamsEx
#define GetVehicleParamsEx GetDynamicVehicleParamsEx
#define SetVehicleToRespawn SetDynamicVehicleToRespawn
#define LinkVehicleToInterior LinkDynamicVehicleToInterior
#define AddVehicleComponent AddDynamicVehicleComponent
#define RemoveVehicleComponent RemoveDynamicVehicleComponent
#define ChangeVehicleColor ChangeDynamicVehicleColor
#define ChangeVehiclePaintjob ChangeDynamicVehiclePaintjob
#define SetVehicleHealth SetDynamicVehicleHealth
#define GetVehicleHealth GetDynamicVehicleHealth
#define SetVehicleNumberPlate SetDynamicVehicleNumberPlate
#define GetVehicleModel GetDynamicVehicleModel
#define GetVehicleComponentInSlot GetDynVehicleComponentInSlot
#define RepairVehicle RepairDynamicVehicle
#define GetVehicleVelocity GetDynamicVehicleVelocity
#define SetVehicleVelocity SetDynamicVehicleVelocity
#define SetVehicleAngularVelocity SetDVehicleAngularVelocity
#define GetVehicleDamageStatus GetDynamicVehicleDamageStatus
#define UpdateVehicleDamageStatus UpdateDVehicleDamageStatus
#define SetVehicleVirtualWorld SetDynamicVehicleVirtualWorld
#define GetVehicleVirtualWorld GetDynamicVehicleVirtualWorld
#define GetPlayerVehicleID GetPlayerDynamicVehicleID
#define PutPlayerInVehicle PutPlayerInDynamicVehicle
#define IsPlayerInVehicle IsPlayerInDynamicVehicle
#define PlayerSpectateVehicle PlayerSpectateDynamicVehicle
#define IsValidVehicle IsValidDynamicVehicle
#if defined _streamer_included
	#define AttachDynamicObjectToVehicle AttachDynamicObjectToDVehicle
#endif
#endif

/***************************** Forwards **************************************/

forward OnDynamicVehicleSpawn(vehicleid);
/******************************************************************************/
forward OnDynamicVehicleDeath(vehicleid, killerid);
/******************************************************************************/
forward OnPlayerEnterDynamicVehicle(playerid, vehicleid, ispassenger);
/******************************************************************************/
/******************************************************************************/
forward OnPlayerExitDynamicVehicle(playerid, vehicleid);
/******************************************************************************/
forward OnDynamicVehicleMod(playerid, vehicleid, componentid);
/******************************************************************************/
forward OnDynamicVehiclePaintjob(playerid, vehicleid, paintjobid);
/******************************************************************************/
forward OnDynamicVehicleRespray(playerid, vehicleid, color1, color2);
/******************************************************************************/
forward OnDVehicleDamageStatusUpdate(vehicleid, playerid);
/******************************************************************************/
forward OnDUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z);
/******************************************************************************/
forward OnDynamicVehicleStreamIn(vehicleid, forplayerid);
/******************************************************************************/
forward OnDynamicVehicleStreamOut(vehicleid, forplayerid);
/******************************************************************************/
forward OnDynamicTrailerUpdate(playerid, vehicleid);
/******************************************************************************/
forward OnPlayerShotDynamicVehicle(playerid, weaponid, vehicleid, Float:fX, Float:fY, Float:fZ);
/****************************OnGamemodeInit hook*******************************/

public OnGameModeInit()
{
	SetTimer("UpdateTheVehiclesCallback", V_CHECK_INTERVAL, false);
	SetVehicleCheckDelay(V_CHECK_INTERVAL);
	CallLocalFunction("OnGameModeInitEx", "");
	return 1;
}

/******************************************************************************/
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit OnGameModeInitEx
forward OnGameModeInitEx();
/************************* Plugin - Server Communication **********************/

forward public UpdateTheVehiclesCallback();
public UpdateTheVehiclesCallback()
{
    UpdateTheVehicles();
    return 1;
}

forward public UpdateTheVehiclesAgain();
public UpdateTheVehiclesAgain()
{
    SetTimer("UpdateTheVehiclesCallback", V_CHECK_INTERVAL, false);
}

/******************************************************************************/
