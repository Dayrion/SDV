#pragma once

static cell AMX_NATIVE_CALL AttachTrailerToDynamicVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "AttachTrailerToDynamicVehicle");
	int trailerid = params[1] - 1,
		vehicleid = params[2] - 1;
	bool player_found = false;

	if (!IsValidDynamicVehicleEx(vehicleid) || !IsValidDynamicVehicleEx(trailerid))
		return 0;

	for (int i = 0; i < connected_players; i++)
	{
		if (!IsPlayerConnected(i))
			continue;
		// if (!IsVehicleStreamedIn(VehiclesData[vehicleid].vID, i) || !IsVehicleStreamedIn(VehiclesData[trailerid].vID, i)) // VehicleCreated[vehicleid] ?

		if (!VehicleCreated[vehicleid] || !VehicleCreated[trailerid])
			continue;
		player_found = true;
		break;
	}

	if (!player_found)
		return 0;

	VehiclesData[vehicleid].attached_trailer = trailerid;
	AttachTrailerToVehicle(VehiclesData[trailerid].vID, VehiclesData[vehicleid].vID);
	return 1;
}

static cell AMX_NATIVE_CALL GetDynamicVehicleTrailer(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "GetDynamicVehicleTrailer");
	int vehicleid = params[1] - 1;

	if (!IsValidDynamicVehicleEx(vehicleid))
		return 0;

	return (VehiclesData[VehiclesData[vehicleid].attached_trailer].vID);
}

static cell AMX_NATIVE_CALL IsTrailerAttachedToDynVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "IsTrailerAttachedToDynVehicle");
	int vehicleid = params[1] - 1;

	if (!IsValidDynamicVehicleEx(vehicleid))
		return 0;

	return !!(VehiclesData[vehicleid].attached_trailer);
}

static cell AMX_NATIVE_CALL DetachTrailerFromDynamicVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "DetachTrailerFromDynamicVehicle");
	int vehicleid = params[1] - 1;

	if (!IsValidDynamicVehicleEx(vehicleid) || !VehiclesData[vehicleid].attached_trailer)
		return 0;

	DetachTrailerFromVehicle(VehiclesData[vehicleid].vID);
	VehiclesData[vehicleid].attached_trailer = 0;
	return 1;
}