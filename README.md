## SDV ( Semi Dynamic Vehicle ) Plugin

A plugin to make as much vehicles as you want in your server and it will handle the destroy and create when necessary to be able to create more than 2 thousand vehicles in your server.

## Documentation

Documentation can be found on the [wiki](https://github.com/Jlalt/SDV/wiki).

## Download

The latest binaries for Windows and Linux can be found on the [releases page](https://github.com/Jlalt/SDV/releases).

## List of natives
```pawn
native CreateDynamicVehicle(modelid, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren=0, virtualworld = 0, interior = 0, Float: streamdis = V_STREAM_DIS);
native DestroyDynamicVehicle(vehicleid);
native SetStreamDistance(Float:streamdistance);
native SetVehicleCheckDelay(milliseconds);
native GetDynamicVehiclePos(vehicleid, &Float:x, &Float:y, &Float:z);
native SetDynamicVehiclePos(vehicleid, Float:x, Float:y, Float:z);
native SetDynamicVehicleZAngle(vehicleid, Float:z_angle);
native GetDynamicVehicleZAngle(vehicleid, &Float:z_angle);
native GetDynamicVehicleRotationQuat(vehicleid, &Float:w, &Float:x, &Float:y, &Float:z);
native Float:GetDVehicleDistanceFromPoint(vehicleid, Float:X, Float:Y, Float:Z);
native SetDynamicVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
native GetDynamicVehicleParamsEx(vehicleid, &engine, &lights, &alarm, &doors, &bonnet, &boot, &objective);
native SetDynamicVehicleToRespawn(vehicleid);
native LinkDynamicVehicleToInterior(vehicleid, interiorid);
native GetDynamicVehicleInterior(vehicleid); // New
native AddDynamicVehicleComponent(vehicleid, componentid);
native RemoveDynamicVehicleComponent(vehicleid, componentid);
native GetDVehicleComponentInSlot(vehicleid, slot); // There is 1 slot for each CARMODTYPE_*
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
native UpdateDVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
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

native PopDynamicVehicleTires(vehicleid, right_front = 0, left_front = 0, right_back = 0, left_back = 0); // New

// Trailer
native AttachTrailerToDynamicVehicle(trailerid, vehicleid);
native DetachTrailerFromDynamicVehicle(vehicleid);
native bool:IsTrailerAttachedToDynVehicle(vehicleid);
native GetDynamicVehicleTrailer(vehicleid);

native Attach3DTextLabelToDynVehicle(Text3D:id, vehicleid, Float:OffsetX, Float:OffsetY, Float:OffsetZ); // Label gotta be dynamic

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

```

## Credits
- Kurta999 for his c++ tutorials and YSF plugin. [ used ysf makefile to compile on linux ]
- Streamer plugin, took this readme design from its readme file.
- SA-MP Developers & Zeex for making samp, its SDK & GDK avaliable.
- Jlalt for programming.

## Special thank to
- Blackturbo, for allowing this system to ran on his server with good player base to test it.
- Neutro, for beta testing.
- [KOD]roak[BiA], for beta testing.
- Saints, for beta testing.
- Ninja, for beta testing.
- Pinkbird, for beta testing.
- [ Sorry if I forgot your name tell me to add it here ]
- [ Sorry if I forgot your name tell me to add it here ]
- [ Sorry if I forgot your name tell me to add it here ]
- [ Sorry if I forgot your name tell me to add it here ]
