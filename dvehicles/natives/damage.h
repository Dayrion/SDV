#pragma once


static cell AMX_NATIVE_CALL UpdateSpecificDynVehDmgStatus(AMX *amx, cell *params)
{
	if (params[0] > (9 * 4))
	{
		logprintf("Too much parameters for the function UpdateSpecificDynVehDmgStatus");
		return 0;
	}

	int vehicleid = params[1] - 1,
		damage_type = params[2],
		right_front = params[3],
		left_front = params[4],
		right_back = params[5],
		left_back = params[6],
		windshield = params[7],
		front_bumper = params[8],
		rear_bumper = params[9];

	cell* addr = nullptr;

	amx_GetAddr(amx, params[3], &addr);
	if (addr != nullptr)
		right_front = *addr;

	amx_GetAddr(amx, params[4], &addr);
	if (addr != nullptr)
		left_front = *addr;

	amx_GetAddr(amx, params[5], &addr);
	if (addr != nullptr)
		right_back = *addr;

	amx_GetAddr(amx, params[6], &addr);
	if (addr != nullptr)
		left_back = *addr;

	amx_GetAddr(amx, params[7], &addr);
	if (addr != nullptr)
		windshield = *addr;

	amx_GetAddr(amx, params[8], &addr);
	if (addr != nullptr)
		front_bumper = *addr;

	amx_GetAddr(amx, params[9], &addr);
	if (addr != nullptr)
		rear_bumper = *addr;

	if (!IsValidDynamicVehicleEx(vehicleid))
		return 0;
	
	switch (damage_type)
	{
		case 0: // panels
		{
			if (VehicleCreated[vehicleid])
				UpdateVehicleDamageStatus(VehiclesData[vehicleid].vID, right_front | (left_front << 4) | (right_back << 8) | (left_back << 12) | (windshield << 16) | (front_bumper << 20) | (rear_bumper << 24), VehiclesData[vehicleid].Damage_doors, VehiclesData[vehicleid].Damage_lights, VehiclesData[vehicleid].Damage_tires);
			VehiclesData[vehicleid].Damage_panels = right_front | (left_front << 4) | (right_back << 8) | (left_back << 12) | (windshield << 16) | (front_bumper << 20) | (rear_bumper << 24);
			break;
		}
		
		case 1: // doors
		{
			/*
				hood: left_front
				trunk: right_front
				drivers doors: left_back
				co-drivers door: right_back*/
			
			if (VehicleCreated[vehicleid])
				UpdateVehicleDamageStatus(VehiclesData[vehicleid].vID, VehiclesData[vehicleid].Damage_panels, (right_back) | (left_back << 8) | (right_front << 16) | (left_front << 24), VehiclesData[vehicleid].Damage_lights, VehiclesData[vehicleid].Damage_tires);
			VehiclesData[vehicleid].Damage_doors = (right_back) | (left_back << 8) | (right_front << 16) | (left_front << 24);
			break;
		}

		case 2: // lights
		{
			if (VehicleCreated[vehicleid])
				UpdateVehicleDamageStatus(VehiclesData[vehicleid].vID, VehiclesData[vehicleid].Damage_panels, VehiclesData[vehicleid].Damage_doors, (left_back << 6) | (right_front << 2) | (left_front), VehiclesData[vehicleid].Damage_tires);
			VehiclesData[vehicleid].Damage_lights = (left_back << 6) | (right_front << 2) | (left_front);
			break;
		}

		case 3: // tires
		{
			if (VehicleCreated[vehicleid])
				UpdateVehicleDamageStatus(VehiclesData[vehicleid].vID, VehiclesData[vehicleid].Damage_panels, VehiclesData[vehicleid].Damage_doors, VehiclesData[vehicleid].Damage_lights, left_front | (left_back << 1) | (right_front << 2) | (right_back << 3));
			VehiclesData[vehicleid].Damage_tires = left_front | (left_back << 1) | (right_front << 2) | (right_back << 3);
			break;
		}
	}

	return 1;
}


static cell AMX_NATIVE_CALL UpdateDynVehicleDamageStatus(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "UpdateDynVehicleDamageStatus");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{

		VehiclesData[vehicleid].Damage_panels = params[2],
		VehiclesData[vehicleid].Damage_doors = params[3],
		VehiclesData[vehicleid].Damage_lights = params[4],
		VehiclesData[vehicleid].Damage_tires = params[5];
		
		if (VehicleCreated[vehicleid])
			UpdateVehicleDamageStatus(VehiclesData[vehicleid].vID, VehiclesData[vehicleid].Damage_panels, VehiclesData[vehicleid].Damage_doors, VehiclesData[vehicleid].Damage_lights, VehiclesData[vehicleid].Damage_tires);
		return 1;
	}
	else
		return 0;
}
