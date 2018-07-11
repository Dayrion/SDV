#pragma once

#include "natives\trailer.h"

static cell AMX_NATIVE_CALL CreateDynamicVehicle(AMX *amx, cell *params)
{
	logprintf("CDV is called");
	CHECK_PARAMS(12, "CreateDynamicVehicle");
	try
	{
		int
			vmodel = params[1];
		if (vmodel < 400 || vmodel > 611)
			return 0;
		int
			color1 = params[6],
			color2 = params[7],
			respawn = params[8],
			addsirn = params[9],
			virtualworID = params[10],
			interiorID = params[11],
			vdynamicslot = GetFreeVehicleSlot();

		float
			vvx = amx_ctof(params[2]),
			vvy = amx_ctof(params[3]),
			vvz = amx_ctof(params[4]),
			vvr = amx_ctof(params[5]),
			vstreamdis = amx_ctof(params[12]);

		VehiclesData[vdynamicslot].Model = vmodel;
		VehiclesData[vdynamicslot].Color1 = color1;
		VehiclesData[vdynamicslot].Color2 = color2;
		VehiclesData[vdynamicslot].Respawn_Delay = respawn;
		VehiclesData[vdynamicslot].Addsiren = addsirn;
		VehiclesData[vdynamicslot].vx = vvx;
		VehiclesData[vdynamicslot].vy = vvy;
		VehiclesData[vdynamicslot].vz = vvz;
		VehiclesData[vdynamicslot].defvx = vvx;
		VehiclesData[vdynamicslot].defvy = vvy;
		VehiclesData[vdynamicslot].defvz = vvz;
		VehiclesData[vdynamicslot].v_zangle = vvr;
		VehiclesData[vdynamicslot].World = virtualworID;
		ResetVehicleData(vdynamicslot);
		VehiclesData[vdynamicslot].InteriorID = interiorID;
		VehiclesData[vdynamicslot].streamdis = powf(vstreamdis, 2);
		VehicleCreated[vdynamicslot] = false;
		VehiclesData[vdynamicslot].ShouldBeCreated = false;
		WhoNearMe(vdynamicslot, VehiclesData[vdynamicslot], 1, true);
		return ( vdynamicslot + 1 );
	}
	catch (...)
	{
		logprintf("Error occured in CreateDynamicVehicle native");
		return -1;
	}
}

static cell AMX_NATIVE_CALL DestroyDynamicVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "DestroyDynamicVehicle");
	int dvid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(dvid))
	{
		if (VehicleCreated[dvid])
		{
			DestroyVehicle(VehiclesData[dvid].vID);
			vector<int>::iterator it = find_if(CreatedVehicles.begin(), CreatedVehicles.end(), [dvid](const int &item) { return (dvid == item); });
			if (it != CreatedVehicles.end())
				CreatedVehicles.erase(it);
			VDynamicID[VehiclesData[dvid].vID] = -1;
			VehiclesData[dvid].vID = 0;
			VehicleCreated[dvid] = false;
		}
		VehiclesData[dvid].plate = "";
		VehiclesData[dvid].AttachObject.clear();
		VehiclesData[dvid].ShouldBeCreated = false;
		VehiclesData[dvid].Model = -1;
		if (lastputid == -1)
			lastputid = dvid;
		else
		{
			if (lastputid > dvid)
				sortbeforeuse = true;
			else
				lastputid = dvid;
		}
		FreeVehicleSlots.push_back(dvid);
		return 1;
	}
	return 0;
}

static cell AMX_NATIVE_CALL SetDynamicVehiclePos(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "SetDynamicVehiclePos");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{
			return SetVehiclePos(VehiclesData[vehicleid].vID, amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
		}
		else
		{
			VehiclesData[vehicleid].vx = amx_ctof(params[2]);
			VehiclesData[vehicleid].vy = amx_ctof(params[3]);
			VehiclesData[vehicleid].vz = amx_ctof(params[4]);
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehiclePos(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "GetDynamicVehiclePos");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{
			cell* poses[3];
			amx_GetAddr(amx, params[2], &poses[0]);
			amx_GetAddr(amx, params[3], &poses[1]);
			amx_GetAddr(amx, params[4], &poses[2]);
			float
				X, Y, Z 
			;
			GetVehiclePos(VehiclesData[vehicleid].vID, &X, &Y, &Z);
			*poses[0] = amx_ftoc(X);
			*poses[1] = amx_ftoc(Y);
			*poses[2] = amx_ftoc(Z);
		}
		else
		{
			cell* poses[3];
			amx_GetAddr(amx, params[2], &poses[0]);
			amx_GetAddr(amx, params[3], &poses[1]);
			amx_GetAddr(amx, params[4], &poses[2]);
			*poses[0] = amx_ftoc(VehiclesData[vehicleid].vx);
			*poses[1] = amx_ftoc(VehiclesData[vehicleid].vy);
			*poses[2] = amx_ftoc(VehiclesData[vehicleid].vz);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}


static cell AMX_NATIVE_CALL SetDynamicVehicleZAngle(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "SetDynamicVehicleZAngle");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{
			return SetVehicleZAngle(VehiclesData[vehicleid].vID, amx_ctof(params[2]));
		}
		else
		{
			VehiclesData[vehicleid].v_zangle = amx_ctof(params[2]);
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehicleZAngle(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "GetDynamicVehicleZAngle");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{
			cell* poses;
			amx_GetAddr(amx, params[2], &poses);
			float z_angle;
			GetVehicleZAngle(VehiclesData[vehicleid].vID, &z_angle);
			*poses = amx_ftoc(z_angle);
		}
		else
		{
			cell* poses;
			amx_GetAddr(amx, params[2], &poses);
			*poses = amx_ftoc(VehiclesData[vehicleid].v_zangle);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehicleRotationQuat(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "GetDynamicVehicleRotationQuat");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{
			cell* quatpos[4];
			amx_GetAddr(amx, params[2], &quatpos[0]);
			amx_GetAddr(amx, params[3], &quatpos[1]);
			amx_GetAddr(amx, params[4], &quatpos[2]);
			amx_GetAddr(amx, params[5], &quatpos[3]);
			float quatw, quatx, quaty, quatz;
			GetVehicleRotationQuat(VehiclesData[vehicleid].vID, &quatw, &quatx, &quaty, &quatz);
			*quatpos[0] = amx_ftoc(quatw);
			*quatpos[1] = amx_ftoc(quatx);
			*quatpos[2] = amx_ftoc(quaty);
			*quatpos[3] = amx_ftoc(quatz);
		}
		else
		{
			cell* quatpos[4];
			amx_GetAddr(amx, params[2], &quatpos[0]);
			amx_GetAddr(amx, params[3], &quatpos[1]);
			amx_GetAddr(amx, params[4], &quatpos[2]);
			amx_GetAddr(amx, params[5], &quatpos[3]);
			*quatpos[0] = amx_ftoc(VehiclesData[vehicleid].Quatw);
			*quatpos[1] = amx_ftoc(VehiclesData[vehicleid].Quatx);
			*quatpos[2] = amx_ftoc(VehiclesData[vehicleid].Quaty);
			*quatpos[3] = amx_ftoc(VehiclesData[vehicleid].Quatz);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDVehicleDistanceFromPoint(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "GetDVehicleDistanceFromPoint");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{

			float
				PX = amx_ctof(params[2]),
				PY = amx_ctof(params[3]),
				PZ = amx_ctof(params[4]),
				distanc = GetVehicleDistanceFromPoint(VehiclesData[vehicleid].vID, PX, PY, PZ)
				;
			return amx_ftoc(distanc);
		}
		else
		{
			float
				PX = amx_ctof(params[2]),
				PY = amx_ctof(params[3]),
				PZ = amx_ctof(params[4])
				;
			float d = sqrtf((powf(VehiclesData[vehicleid].vx - PX, 2) + powf(VehiclesData[vehicleid].vy - PY, 2) + powf(VehiclesData[vehicleid].vz - PZ, 2)));
			return amx_ftoc(d);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL SetDynamicVehicleParamsEx(AMX *amx, cell *params)
{
	CHECK_PARAMS(8, "SetDynamicVehicleParamsEx");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		int
			vengine = params[2],
			vlights = params[3],
			valarm = params[4],
			vdoors = params[5],
			vbonnet = params[6],
			vboot = params[7],
			vobjective = params[8]
			;
		VehiclesData[vehicleid].Vengine = vengine;
		VehiclesData[vehicleid].Vlights = vlights;
		VehiclesData[vehicleid].Valarm = valarm;
		VehiclesData[vehicleid].Vdoors = vdoors;
		VehiclesData[vehicleid].Vbonnet = vbonnet;
		VehiclesData[vehicleid].Vboot = vboot;
		VehiclesData[vehicleid].Vobjective = vobjective;
		if (VehicleCreated[vehicleid])
		{
			return SetVehicleParamsEx(VehiclesData[vehicleid].vID, VehiclesData[vehicleid].Vengine, VehiclesData[vehicleid].Vlights, VehiclesData[vehicleid].Valarm, VehiclesData[vehicleid].Vdoors, VehiclesData[vehicleid].Vbonnet, VehiclesData[vehicleid].Vboot, VehiclesData[vehicleid].Vobjective);
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL SetDVehicleParamsForPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "SetDVehicleParamsForPlayer");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		int playerid = params[2];
		if (!IsPlayerConnected(playerid))
			return 0;
		VehiclePlayerParams vparams;
		vparams.objective = params[3];
		vparams.doorslocked = params[4];
		VehiclesData[vehicleid].PlayerParams[playerid] = vparams;
		if (VehicleCreated[vehicleid])
		{
			return SetVehicleParamsForPlayer(VehiclesData[vehicleid].vID, playerid, vparams.objective, vparams.doorslocked);
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetVehicleDynamicID(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "GetVehicleDynamicID");
	int vehicleid = (params[1]);	
	if (IsDynamicVehicle(vehicleid))
	{
		return (VDynamicID[vehicleid] + 1);
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehicleParamsEx(AMX *amx, cell *params)
{
	CHECK_PARAMS(8, "GetDynamicVehicleParamsEx");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{
			cell* paramsex[7];
			amx_GetAddr(amx, params[2], &paramsex[0]);
			amx_GetAddr(amx, params[3], &paramsex[1]);
			amx_GetAddr(amx, params[4], &paramsex[2]);
			amx_GetAddr(amx, params[5], &paramsex[3]);
			amx_GetAddr(amx, params[6], &paramsex[4]);
			amx_GetAddr(amx, params[7], &paramsex[5]);
			amx_GetAddr(amx, params[8], &paramsex[6]);
			int vengine, vlights, valarm, vdoors, vbonnet, vboot, vobjective;
			GetVehicleParamsEx(VehiclesData[vehicleid].vID, &vengine, &vlights, &valarm, &vdoors, &vbonnet, &vboot, &vobjective);
			*paramsex[0] = vengine;
			*paramsex[1] = vlights;
			*paramsex[2] = valarm;
			*paramsex[3] = vdoors;
			*paramsex[4] = vbonnet;
			*paramsex[5] = vboot;
			*paramsex[6] = vobjective;
		}
		else
		{
			cell* paramsex[7];
			amx_GetAddr(amx, params[2], &paramsex[0]);
			amx_GetAddr(amx, params[3], &paramsex[1]);
			amx_GetAddr(amx, params[4], &paramsex[2]);
			amx_GetAddr(amx, params[5], &paramsex[3]);
			amx_GetAddr(amx, params[6], &paramsex[4]);
			amx_GetAddr(amx, params[7], &paramsex[5]);
			amx_GetAddr(amx, params[8], &paramsex[6]);
			*paramsex[0] = VehiclesData[vehicleid].Vengine;
			*paramsex[1] = VehiclesData[vehicleid].Vlights;
			*paramsex[2] = VehiclesData[vehicleid].Valarm;
			*paramsex[3] = VehiclesData[vehicleid].Vdoors;
			*paramsex[4] = VehiclesData[vehicleid].Vbonnet;
			*paramsex[5] = VehiclesData[vehicleid].Vboot;
			*paramsex[6] = VehiclesData[vehicleid].Vobjective;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL SetDynamicVehicleToRespawn(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "SetDynamicVehicleToRespawn");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		ResetVehicleData(vehicleid);
		if (VehicleCreated[vehicleid])
		{
			return SetVehicleToRespawn(VehiclesData[vehicleid].vID);
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL LinkDynamicVehicleToInterior(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "LinkDynamicVehicleToInterior");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		int interior = params[2];
		VehiclesData[vehicleid].InteriorID = interior;
		if (VehicleCreated[vehicleid])
		{
			return LinkVehicleToInterior(VehiclesData[vehicleid].vID, interior);
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehicleInterior(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "GetDynamicVehicleInterior");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		return VehiclesData[vehicleid].InteriorID;
	}
	else
	{
		return -1;
	}
}

static cell AMX_NATIVE_CALL AddDynamicVehicleComponent(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "AddDynamicVehicleComponent");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		int componentid = params[2];
		if (VehicleCreated[vehicleid])
		{
			return AddVehicleComponent(VehiclesData[vehicleid].vID, componentid);
		}
		else
		{
			int slotid = GetVehicleComponentType(componentid);
			if (slotid != -1)
			{
				VehiclesData[vehicleid].Components[slotid] = componentid;
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}

static cell AMX_NATIVE_CALL RemoveDynamicVehicleComponent(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "RemoveDynamicVehicleComponent");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		int componentid = params[2];
		if (VehicleCreated[vehicleid])
		{
			return RemoveVehicleComponent(VehiclesData[vehicleid].vID, componentid);
		}
		else
		{
			bool found = false;
			for (int i = 0; i < 14; i++)
			{
				if (VehiclesData[vehicleid].Components[i] == componentid)
				{
					found = true;
					VehiclesData[vehicleid].Components[i] = 0;
					break;
				}
			}
			if (found)
				return 1;
			else
				return 0;
		}
	}
	else
	{
		return -1;
	}
}

static cell AMX_NATIVE_CALL ChangeDynamicVehicleColor(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "ChangeDynamicVehicleColor");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		int color1 = params[2], color2 = params[3];
		VehiclesData[vehicleid].Color1 = color1;
		VehiclesData[vehicleid].Color2 = color2;
		if (VehicleCreated[vehicleid])
		{
			return ChangeVehicleColor(VehiclesData[vehicleid].vID, color1, color2);
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehicleColor(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "GetDynamicVehicleColor");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		cell* colors[2];
		amx_GetAddr(amx, params[2], &colors[0]);
		amx_GetAddr(amx, params[3], &colors[1]);
		*colors[0] = VehiclesData[vehicleid].Color1;
		*colors[1] = VehiclesData[vehicleid].Color2;
		return 1;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL ChangeDynamicVehiclePaintjob(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "ChangeDynamicVehiclePaintjob");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		int paintjobid = params[2];
		if (paintjobid > -1 && paintjobid < 4)
			VehiclesData[vehicleid].Paintjob = paintjobid;
		else
			VehiclesData[vehicleid].Paintjob = 3;
		if (VehicleCreated[vehicleid])
		{
			return ChangeVehiclePaintjob(VehiclesData[vehicleid].vID, paintjobid);
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehiclePaintjob(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "GetDynamicVehiclePaintjob");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		return VehiclesData[vehicleid].Paintjob;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL SetDynamicVehicleHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "SetDynamicVehicleHealth");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		float health = amx_ctof(params[2]);
		if (health < 0.0)
			health = 0.0;
		if (VehicleCreated[vehicleid])
		{
			return SetVehicleHealth(VehiclesData[vehicleid].vID, health);
		}
		else
		{
			VehiclesData[vehicleid].Health = health;
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehicleHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "GetDynamicVehicleHealth");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		cell* health;
		amx_GetAddr(amx, params[2], &health);
		if (VehicleCreated[vehicleid])
		{
			float vhealth;
			GetVehicleHealth(VehiclesData[vehicleid].vID, &vhealth);
			*health = amx_ftoc(vhealth);
		}
		else
		{
			*health = amx_ftoc(VehiclesData[vehicleid].Health);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL SetDynamicVehicleNumberPlate(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "SetDynamicVehicleNumberPlate");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{	
		int len = NULL;
		cell *addr = NULL;
		amx_GetAddr(amx, params[2], &addr);
		amx_StrLen(addr, &len);
		if (len)
		{
			len++;
			char* text = new char[len];
			amx_GetString(text, addr, 0, len);
			VehiclesData[vehicleid].plate = text;			
			if (VehicleCreated[vehicleid])
			{
				int ret = SetVehicleNumberPlate(VehiclesData[vehicleid].vID, text);
				delete[] text;
				return ret;
			}
			else
			{
				delete[] text;
				return 1;
			}
		}
		else
		{
			VehiclesData[vehicleid].plate = "";
			if (VehicleCreated[vehicleid])
			{
				return SetVehicleNumberPlate(VehiclesData[vehicleid].vID, "");
			}
			else
			{
				return 1;
			}
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehicleNumberPlate(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "GetDynamicVehicleNumberPlate");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		string platetext = VehiclesData[vehicleid].plate;
		cell* addr;
		amx_GetAddr(amx, params[2], &addr);
		amx_SetString(addr, platetext.c_str(), 0, 0, params[3]);
		return 1;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL SetDynamicVehicleModel(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "SetDynamicVehicleModel");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		int model = params[2];
		if (model < 400 || model > 611)
			return 0;
		VehiclesData[vehicleid].Model = model;
		ResetVehicleData(vehicleid);
		if (VehicleCreated[vehicleid])
		{
			bool respawn = (params[3] != 0);
			if (respawn)
			{
				ChangeVehicleModel(vehicleid, VehiclesData[vehicleid].vID, VehiclesData[vehicleid]);
			}
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehicleModel(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "GetDynamicVehicleModel");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		return VehiclesData[vehicleid].Model;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDVehicleComponentInSlot(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "GetDVehicleComponentInSlot");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		int slot = params[2];
		if (slot < 0 || slot > 13)
			return 0;
		if (VehicleCreated[vehicleid])
		{
			return GetVehicleComponentInSlot(VehiclesData[vehicleid].vID, slot);
		}
		else
		{
			return VehiclesData[vehicleid].Components[slot];
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL RepairDynamicVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "RepairDynamicVehicle");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{
			return RepairVehicle(VehiclesData[vehicleid].vID);
		}
		else
		{
			VehiclesData[vehicleid].Health = 1000.0;
			VehiclesData[vehicleid].Damage_doors = 0;
			VehiclesData[vehicleid].Damage_lights = 0;
			VehiclesData[vehicleid].Damage_panels = 0;
			VehiclesData[vehicleid].Damage_tires = 0;
			VehiclesData[vehicleid].Damage_tires = 0;
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL SetDynamicVehicleVirtualWorld(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "SetDynamicVehicleVirtualWorld");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		int world = params[2];
		VehiclesData[vehicleid].World = world;
		if (VehicleCreated[vehicleid])
		{
			return SetVehicleVirtualWorld(VehiclesData[vehicleid].vID, world);
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehicleVirtualWorld(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "GetDynamicVehicleVirtualWorld");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		return VehiclesData[vehicleid].World;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL SetDynamicVehicleVelocity(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "SetDynamicVehicleVelocity");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{
			float
				VX = amx_ctof(params[2]),
				VY = amx_ctof(params[3]),
				VZ = amx_ctof(params[4])
				;
			return SetVehicleVelocity(VehiclesData[vehicleid].vID, VX, VY, VZ);
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehicleVelocity(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "GetDynamicVehicleVelocity");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{
			cell* Velocitys[3];
			amx_GetAddr(amx, params[2], &Velocitys[0]);
			amx_GetAddr(amx, params[3], &Velocitys[1]);
			amx_GetAddr(amx, params[4], &Velocitys[2]);
			float VX, VY, VZ;
			GetVehicleVelocity(VehiclesData[vehicleid].vID, &VX, &VY, &VZ);
			*Velocitys[0] = amx_ftoc(VX);
			*Velocitys[1] = amx_ftoc(VY);
			*Velocitys[2] = amx_ftoc(VZ);
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return 0;
	}
}


static cell AMX_NATIVE_CALL SetDVehicleAngularVelocity(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "SetDVehicleAngularVelocity");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{
			float
				VX = amx_ctof(params[2]),
				VY = amx_ctof(params[3]),
				VZ = amx_ctof(params[4])
				;
			return SetVehicleAngularVelocity(VehiclesData[vehicleid].vID, VX, VY, VZ);
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL GetDynamicVehicleDamageStatus(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "GetDynamicVehicleDamageStatus");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		cell* Damages[4];
		amx_GetAddr(amx, params[2], &Damages[0]);
		amx_GetAddr(amx, params[3], &Damages[1]);
		amx_GetAddr(amx, params[4], &Damages[2]);
		amx_GetAddr(amx, params[5], &Damages[3]);
		if (VehicleCreated[vehicleid])
		{
			int Panels, Doors, Lights, Tires;
			GetVehicleDamageStatus(VehiclesData[vehicleid].vID, &Panels, &Doors, &Lights, &Tires);
			*Damages[0] = Panels;
			*Damages[1] = Doors;
			*Damages[2] = Lights;
			*Damages[3] = Tires;
		}
		else
		{
			*Damages[0] = VehiclesData[vehicleid].Damage_panels;
			*Damages[1] = VehiclesData[vehicleid].Damage_doors;
			*Damages[2] = VehiclesData[vehicleid].Damage_lights;
			*Damages[3] = VehiclesData[vehicleid].Damage_tires;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL PopDynamicVehicleTires(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "PopDynamicVehicleTires");
	int vehicleid = params[1] - 1,
		right_front = params[2],
		left_front = params[3],
		right_back = params[4],
		left_back = params[5];

	if (!IsValidDynamicVehicleEx(vehicleid))
		return 0;

	int panels, doors, lights, tires;
	GetVehicleDamageStatus(vehicleid, &panels, &doors, &lights, &tires);

	if (VehicleCreated[vehicleid])
		return UpdateVehicleDamageStatus(VehiclesData[vehicleid].vID, panels, doors, lights, left_front | (left_back << 1) | (right_front << 2) | (right_back << 3));
	else
	{
		VehiclesData[vehicleid].Damage_panels = panels;
		VehiclesData[vehicleid].Damage_doors = doors;
		VehiclesData[vehicleid].Damage_lights = lights;
		VehiclesData[vehicleid].Damage_tires = left_front | (left_back << 1) | (right_front << 2) | (right_back << 3);
		return 1;
	}
	return 1;
}



static cell AMX_NATIVE_CALL UpdateDVehicleDamageStatus(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "UpdateDVehicleDamageStatus");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		int
			panels = params[2],
			doors = params[3],
			lights = params[4],
			tires = params[5];
		;
		if (VehicleCreated[vehicleid])
		{
			return UpdateVehicleDamageStatus(VehiclesData[vehicleid].vID, panels, doors, lights, tires);
		}
		else
		{
			VehiclesData[vehicleid].Damage_panels = panels;
			VehiclesData[vehicleid].Damage_doors = doors;
			VehiclesData[vehicleid].Damage_lights = lights;
			VehiclesData[vehicleid].Damage_tires = tires;
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL IsDynamicVehicleStreamedIn(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "IsDynamicVehicleStreamedIn");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid] && IsPlayerConnected(params[2]))
		{
			return IsVehicleStreamedIn(VehiclesData[vehicleid].vID, params[2]);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

static cell AMX_NATIVE_CALL IsPlayerInDynamicVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "IsPlayerInDynamicVehicle");
	int vehicleid = (params[2]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (VehicleCreated[vehicleid])
		{
			if (!IsPlayerConnected(params[1]))
			{
				return 0;
			}
			else
			{
				return IsPlayerInVehicle(params[1], VehiclesData[vehicleid].vID);
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

static cell AMX_NATIVE_CALL PlayerSpectateDynamicVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "PlayerSpectateDynamicVehicle");
	int vehicleid = (params[2]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (!IsPlayerConnected(params[1]))
			return 0;
		if (VehicleCreated[vehicleid])
		{
			return PlayerSpectateVehicle(params[1], VehiclesData[vehicleid].vID, params[3]);
		}
		else
		{
			PlayersData[params[1]].PX = VehiclesData[vehicleid].vx;
			PlayersData[params[1]].PY = VehiclesData[vehicleid].vy;
			PlayersData[params[1]].PZ = VehiclesData[vehicleid].vz;
			WhoNearMe(vehicleid, VehiclesData[vehicleid], 0, true, params[1]);
			if (VehicleCreated[vehicleid])
			{
				return PlayerSpectateVehicle(params[1], VehiclesData[vehicleid].vID, params[3]);
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		return -1;
	}
}

static cell AMX_NATIVE_CALL IsValidDynamicVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "IsValidDynamicVehicle");
	int vehicleid = (params[1]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL PutPlayerInDynamicVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "PutPlayerInDynamicVehicle");
	int vehicleid = (params[2]) - 1;
	if (IsValidDynamicVehicleEx(vehicleid))
	{
		if (!IsPlayerConnected(params[1]))
			return 0;
		if (VehicleCreated[vehicleid])
		{
			return PutPlayerInVehicle(params[1], VehiclesData[vehicleid].vID, params[3]);
		}
		else
		{
			PlayersData[params[1]].PX = VehiclesData[vehicleid].vx;
			PlayersData[params[1]].PY = VehiclesData[vehicleid].vy;
			PlayersData[params[1]].PZ = VehiclesData[vehicleid].vz;
			WhoNearMe(vehicleid, VehiclesData[vehicleid], 0, true, params[1]);
			if (VehicleCreated[vehicleid])
			{
				return PutPlayerInVehicle(params[1], VehiclesData[vehicleid].vID, params[3]);
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		return -1;
	}
}

static cell AMX_NATIVE_CALL GetPlayerDynamicVehicleID(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "GetPlayerDynamicVehicleID");
	int playerid = (params[1]);
	if (IsPlayerConnected(playerid))
	{
		if (IsPlayerInAnyVehicle(playerid))
		{
			int vid = GetPlayerVehicleID(playerid);
			if (IsDynamicVehicle(vid))
			{
				return ( VDynamicID[vid] + 1 );
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

static cell AMX_NATIVE_CALL GetPlayerSurfingDVehicleID(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "GetPlayerSurfingDVehicleID");
	int playerid = (params[1]);
	if (IsPlayerConnected(playerid))
	{
		int vid = GetPlayerSurfingVehicleID(playerid);
		if (vid != 0xFFFF)
		{
			if (IsDynamicVehicle(vid))
			{
				return ( VDynamicID[vid] + 1 );
			}
			else
			{
				return -2;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

static cell AMX_NATIVE_CALL MoveVehicleDynamicID(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "MoveVehicleDynamicID");
	int 
		fromid = (params[1]) - 1, 
		toid = (params[2]) - 1
		;
	if (IsValidDynamicVehicleEx(fromid))
	{
		if (toid < 0)
			toid = GetFreeVehicleSlot();
		if (!IsValidDynamicVehicleEx(toid))
		{
			VehiclesData[toid] = VehiclesData[fromid];
			VehicleCreated[toid] = true;
			if (VehicleCreated[fromid])
			{
				VDynamicID[VehiclesData[toid].vID] = toid;
				VehiclesData[fromid].vID = 0;
				VehicleCreated[fromid] = false;
			}
			VehiclesData[fromid].ShouldBeCreated = false;
			VehiclesData[fromid].Model = -1;
			if (lastputid == -1)
				lastputid = fromid;
			else
			{
				if (lastputid > fromid)
					sortbeforeuse = true;
				else
					lastputid = fromid;
			}
			FreeVehicleSlots.push_back(fromid);
			return (toid + 1);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

static cell AMX_NATIVE_CALL AttachDynamicObjectToDVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(8, "AttachDynamicObjectToDVehicle");
	int
		objectid = params[1],
		vehicleid = ( params[2] - 1 )
		;
	if (IsValidDynamicObject(objectid))
	{
		if (IsValidDynamicVehicleEx(vehicleid))
		{
			float
				offsetx = amx_ctof(params[3]),
				offsety = amx_ctof(params[4]),
				offsetz = amx_ctof(params[5]),
				rx = amx_ctof(params[6]),
				ry = amx_ctof(params[7]),
				rz = amx_ctof(params[8])
				;
			AttachedObject object;
			object.offsetx = offsetx;
			object.offsety = offsety;
			object.offsetz = offsetz;
			object.rx = rx;
			object.ry = ry;
			object.rz = rz;
			VehiclesData[vehicleid].AttachObject[objectid] = object;
			if (VehicleCreated[vehicleid])
			{
				return AttachDynamicObjectToVehicle(objectid, VehiclesData[vehicleid].vID, offsetx, offsety, offsetz, rx, ry, rz);
			}
			else
			{				
				// 0 objecttype, 48 Virtual World data
				Streamer_SetIntData(0, objectid, 48, 6621);
				// 3 Attached vehicle data
				Streamer_SetIntData(0, objectid, 3, 65535);
				return 1;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
	return 1;
}

static cell AMX_NATIVE_CALL SetStreamDistance(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "SetStreamDistance");
	float streamd = amx_ctof(params[1]);
	streamdistance = powf(streamd, 2);
	return 1;
}

static cell AMX_NATIVE_CALL SetVehicleCheckDelay(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "SetVehicleCheckDelay");
	int checkdelay = (params[1]);
	VehicleCheckDelay = checkdelay;
	return 1;
}

static cell AMX_NATIVE_CALL UpdateTheVehicles(AMX *amx, cell *params)
{
	CHECK_PARAMS(0, "UpdateTheVehicles");
	CheckTheCars(amx);
	return 1;
}

void CheckTheCars(AMX *amx)
{
	if (connected_players)
	{
		if (threadrunning)
		{
			ExecuteFunction(amx, "UpdateTheVehiclesAgain", {}, {});
			return;
		}		
		UpdateTheCache();
		int j = TakeAction.size();
		if (j)
		{
			for (int i = 0; i < j; i++)
			{
				int vid = TakeAction[i];
				if (!VehiclesData[vid].ShouldBeCreated)
				{
					if (VehicleCreated[vid] && VehiclesData[vid].Model >= 400)
						UnStreamDynamicVehicle(vid, VehiclesData[vid].vID);
					TakeAction.erase(TakeAction.begin() + i);
					j--;
					i--;
				}
			}
			if (j)
			{
				for (int i = 0; i < j; i++)
				{
					int vid = TakeAction[0];
					VehiclesData[vid].ShouldBeCreated = false;
					if (!VehicleCreated[vid] && VehiclesData[vid].Model >= 400)
						StreamVehicle(vid, VehiclesData[vid]);
					TakeAction.erase(TakeAction.begin());
				}
			}
		}
		threadrunning = true;
		thread vcheck([]
		{
			int nj = VehiclesData.size();
			if (nj)
			{
				for (int i = 0; i < nj; i++)
				{
					if (!VehicleCreated[i] && VehiclesData[i].Model > 399)
					{
						if (!VehiclesData[i].ShouldBeCreated)
							WhoNearMe(i, VehiclesData[i]);
					}
				}
			}
			threadrunning = false;
		});
		vcheck.detach();
		ExecuteFunction(amx, "UpdateTheVehiclesAgain", {}, {});
	}
	else
	{
		ExecuteFunction(amx, "UpdateTheVehiclesAgain", {}, {});
	}
}

int ExecuteFunction(AMX *amx, string fname, vector<cell> parameters, vector<float> fparameters)
{	
	int idx;
	if (!amx_FindPublic(amx, fname.c_str(), &idx))
	{
		cell ret;
		for (int i = fparameters.size() - 1; i > -1; i--)
		{
			amx_Push(amx, amx_ftoc(fparameters[i]));
		}
		for (int i = parameters.size() - 1; i > -1; i--)
		{
			amx_Push(amx, parameters[i]);
		}
		amx_Exec(amx, &ret, idx);
		return ret;
	}
	else
	{
		return -2;
	}
}

int ExecuteFunction(string fname, vector<int> parameters, vector<float> fparameters)
{
	int j = globalamx.size();
	int retval = -1;
	for (int z = 0; z < j; z++)
	{
		int idx;
		if (!amx_FindPublic(globalamx[z], fname.c_str(), &idx))
		{
			cell ret;
			for (int i = fparameters.size() - 1; i > -1; i--)
			{
				amx_Push(globalamx[z], amx_ftoc(fparameters[i]));
			}
			for (int i = parameters.size() - 1; i > -1; i--)
			{
				amx_Push(globalamx[z], parameters[i]);
			}
			amx_Exec(globalamx[z], &ret, idx);
			if (ret == 0)
			{
				return ret;
			}
			else
			{
				retval = ret;
			}
		}
	}
	return retval;
}

int GetFreeVehicleSlot()
{
	int j = FreeVehicleSlots.size();
	if (j)
	{	
		if (sortbeforeuse)
		{
			sort(FreeVehicleSlots.begin(), FreeVehicleSlots.end());
			sortbeforeuse = false;
		}
		int index = FreeVehicleSlots[0];
		if (index == lastputid)
			lastputid = -1;
		FreeVehicleSlots.erase(FreeVehicleSlots.begin());
		return index;
	}
	else
	{
		VehiclesData.push_back(VehicleData());
		return (VehiclesData.size() - 1);
	}
}

void WhoNearMe(int dynamicid, VehicleData &vehicle, int newcar, bool mainthread, int exclude)
{
	if (mainthread)
	{
		UpdateTheCache(exclude);
	}
	bool shouldgetnearest = (VehiclesCount >= 1999);
	float isnear = AnyPlayerNearCar(vehicle.World, vehicle.vx, vehicle.vy, vehicle.vz, vehicle.streamdis, shouldgetnearest);
	if (isnear != -1.0)
	{
		vehicle.nearestone = shouldgetnearest;
		vehicle.nearestplayer = isnear;
		bool removedone = false;
		if (shouldgetnearest/*VehiclesCount >= 1999*/)
		{
			if (RemoveFarestCar(isnear, mainthread))
				removedone = true;
		}
		else
		{
			removedone = true;
		}
		if (removedone)
		{
			if (mainthread)
			{
				StreamVehicle(dynamicid, VehiclesData[dynamicid]);
				//VehiclesCount++;
			}
			else
			{
				vehicle.ShouldBeCreated = true;
				TakeAction.push_back(dynamicid);
			}			
		}
	}
}

bool RemoveFarestCar(float nearestdis, bool mainthread)
{
	int vid = GetFarestCarID(nearestdis, mainthread);
	if (vid != -1)
	{
		vector<int>::iterator it = find_if(CreatedVehicles.begin(), CreatedVehicles.end(), [vid](const int &item) { return (vid == item); });
		if(it != CreatedVehicles.end())
			CreatedVehicles.erase(it);
		return true;
	}
	else
	{
		return false;
	}
}

bool IsValidDynamicVehicleEx(int vehicleid)
{
	int len = VehiclesData.size();
	if (vehicleid < 0 || vehicleid >= len || VehiclesData[vehicleid].Model < 400)
	{
		return false;
	}
	return true;
}

void ResetAllVehicleData(int dynamicid)
{
	ResetVehicleData(dynamicid);
	VehiclesData[dynamicid].Model = -1;
	VehiclesData[dynamicid].Color1 =
	VehiclesData[dynamicid].Color2 =
	VehiclesData[dynamicid].Respawn_Delay =
	VehiclesData[dynamicid].Addsiren =
	VehiclesData[dynamicid].InteriorID = 
	VehiclesData[dynamicid].World = 0;
	VehiclesData[dynamicid].vx =
	VehiclesData[dynamicid].vy =
	VehiclesData[dynamicid].vz =
	VehiclesData[dynamicid].defvx =
	VehiclesData[dynamicid].defvy =
	VehiclesData[dynamicid].defvz =
	VehiclesData[dynamicid].v_zangle =
	VehiclesData[dynamicid].streamdis = 0.0;
	VehicleCreated[dynamicid] = false;
	VehiclesData[dynamicid].ShouldBeCreated = false;
}

void ResetVehicleData(int dynamicid)
{
	VehiclesData[dynamicid].Damage_doors =
	VehiclesData[dynamicid].Damage_lights =
	VehiclesData[dynamicid].Damage_panels =
	VehiclesData[dynamicid].Damage_tires =
	VehiclesData[dynamicid].attached_trailer =
	VehiclesData[dynamicid].Damage_tires = 0;
	VehiclesData[dynamicid].Quatw =
	VehiclesData[dynamicid].Quatx =
	VehiclesData[dynamicid].Quaty =
	VehiclesData[dynamicid].Quatz = 0.5;
	VehiclesData[dynamicid].Vengine =
	VehiclesData[dynamicid].Vlights =
	VehiclesData[dynamicid].Valarm =
	VehiclesData[dynamicid].Vdoors =
	VehiclesData[dynamicid].Vbonnet =
	VehiclesData[dynamicid].Vboot =
	VehiclesData[dynamicid].Vobjective = -1;
	VehiclesData[dynamicid].Paintjob = 3;	
	for (int i = 0; i < 14; i++)
		VehiclesData[dynamicid].Components[i] = 0;
	VehiclesData[dynamicid].Health = 1000.0;
	VehiclesData[dynamicid].PlayerParams.clear();
}

float AnyPlayerNearCar(int world, float vx, float vy, float vz, float streamdis, bool nearest)
{
	int j = ConnectedIds.size();
	if (j > 0)
	{
		float NearestDist = -1.0;
		for (int i = 0; i < j; i++)
		{
			int pid = ConnectedIds[i];
			if (PlayersData[pid].world != world)
				continue;
			float pdist = GetDistanceBetweenTwoPoints(PlayersData[pid].PX, PlayersData[pid].PY, PlayersData[pid].PZ, vx, vy, vz);
			if (pdist <= streamdis && (NearestDist == -1 || NearestDist > pdist))
			{
				if (!nearest)
					return pdist;
				NearestDist = pdist;
			}
		}
		return NearestDist;
	}
	else
	{
		return -1.0;
	}
}

int GetFarestCarID(float nearestdis, bool mainthread)
{
	int s = CreatedVehicles.size();
	vector<bool>
		CanRemove(s, true),
		PlayerIsIn(s, false)
		;
	bool foundone = false;
	int vidtoremove = -1;
	vector<float> NearestPlayer(s, -1.0);
	int z = ConnectedIds.size();
	for (int i = 0; i < s/*(MAX_VEHICLES)*/; i++)
	{
		int vid = CreatedVehicles[i];
		if (!VehicleCreated[vid])
			continue;
		for (int j = 0; j < z; j++)
		{
			int pid = ConnectedIds[j];
			if (PlayersData[pid].vehicleID == VehiclesData[vid].vID)
			{
				PlayerIsIn[i] = true;
				CanRemove[i] = false;
				break;
			}
			float Pdis = GetDistanceBetweenTwoPoints(PlayersData[pid].PX, PlayersData[pid].PY, PlayersData[pid].PZ, VehiclesData[vid].vx, VehiclesData[vid].vy, VehiclesData[vid].vz);
			if (Pdis <= streamdistance)
			{
				CanRemove[i] = false;
				if (NearestPlayer[i] == -1.0)
				{
					NearestPlayer[i] = Pdis;
				}
				else if (Pdis < NearestPlayer[i])
				{
					NearestPlayer[i] = Pdis;
				}
			}

		}
		if (CanRemove[i])
		{
			foundone = true;
			vidtoremove = vid;
			break;
		}
	}
	if (foundone)
	{
		if(mainthread)
			UnStreamDynamicVehicle(vidtoremove, VehiclesData[vidtoremove].vID);
		else
		{
			VehiclesData[vidtoremove].ShouldBeCreated = false;
			TakeAction.push_back(vidtoremove);
		}
		return vidtoremove;
	}
	else
	{
		float
			FDistance = -1.0;
		int FarestID = -1;
		for (int i = 0; i < s/*(MAX_VEHICLES)*/; i++)
		{
			if (PlayerIsIn[i])
				continue;
			if (FarestID == -1 || FDistance < NearestPlayer[i])
			{
				FDistance = NearestPlayer[i];
				FarestID = i;
			}
		}
		if (FDistance > nearestdis)
		{
			int dynamicid = CreatedVehicles[FarestID];
			if(mainthread)
				UnStreamDynamicVehicle(dynamicid, VehiclesData[dynamicid].vID);
			else
			{
				VehiclesData[dynamicid].ShouldBeCreated = false;
				TakeAction.push_back(dynamicid);
			}
			return dynamicid;
		}
		else
		{
			return -1;
		}
	}
}

float GetDistanceBetweenTwoPoints(float x1, float y1, float z1, float x2, float y2, float z2)
{
	return (powf((x1 - x2), 2) + powf((y1 - y2), 2) + powf((z1 - z2), 2));
}

void UnStreamDynamicVehicle(int dynamicid, int vehicleid)
{
	int comp[14];
	for (int i = 0; i < 14; i++)
	{
		comp[i] = GetVehicleComponentInSlot(vehicleid, i);
	}
	float
		vvx, vvy, vvz,
		vvr,
		w, x, y, z,
		health
		;
	int
		panels, doors, lights, tires,
		engine, vlights, alarm, vdoors, bonnet, boot, objective
		;
	/************** Components ************/
	for (int i = 0; i < 14; i++)
		VehiclesData[dynamicid].Components[i] = comp[i];
	/************** Position ************/
	GetVehiclePos(vehicleid, &vvx, &vvy, &vvz);
	VehiclesData[dynamicid].vx = vvx;
	VehiclesData[dynamicid].vy = vvy;
	VehiclesData[dynamicid].vz = vvz;
	/************** Angle ************/
	GetVehicleZAngle(vehicleid, &vvr);
	VehiclesData[dynamicid].v_zangle = vvr;
	/************** Damage ************/
	GetVehicleDamageStatus(vehicleid, &panels, &doors, &lights, &tires);
	VehiclesData[dynamicid].Damage_panels = panels;
	VehiclesData[dynamicid].Damage_doors = doors;
	VehiclesData[dynamicid].Damage_lights = lights;
	VehiclesData[dynamicid].Damage_tires = tires;
	/************** Quat pos ************/
	GetVehicleRotationQuat(vehicleid, &w, &x, &y, &z);
	VehiclesData[dynamicid].Quatw = w;
	VehiclesData[dynamicid].Quatx = x;
	VehiclesData[dynamicid].Quaty = y;
	VehiclesData[dynamicid].Quatz = z;
	/************** ParamsEx ************/
	GetVehicleParamsEx(vehicleid, &engine, &vlights, &alarm, &vdoors, &bonnet, &boot, &objective);
	VehiclesData[dynamicid].Vengine = engine;
	VehiclesData[dynamicid].Vlights = vlights;
	VehiclesData[dynamicid].Valarm = alarm;
	VehiclesData[dynamicid].Vdoors = vdoors;
	VehiclesData[dynamicid].Vbonnet = bonnet;
	VehiclesData[dynamicid].Vboot = boot;
	VehiclesData[dynamicid].Vobjective = objective;
	/************** Health ************/
	GetVehicleHealth(vehicleid, &health);
	VehiclesData[dynamicid].Health = health;
	/******** Attached Objects *******/
	for (map<int, AttachedObject>::iterator it = VehiclesData[dynamicid].AttachObject.begin(); it != VehiclesData[dynamicid].AttachObject.end(); ++it)
	{
		int objectid = it->first;
		if (!IsValidDynamicObject(objectid) || Streamer_GetIntData(0, objectid, 3) != vehicleid) // object is destroyed or something, not the vehicle we attached to before!
		{
			VehiclesData[dynamicid].AttachObject.erase(objectid);
			continue;
		}
		else
		{
			Streamer_SetIntData(0, objectid, 48, 6621);
			Streamer_SetIntData(0, objectid, 3, 65535);
		}
	}
	VDynamicID[vehicleid] = -1;	
	VehicleCreated[dynamicid] = false;
	DestroyVehicle(vehicleid);
	VehiclesCount--;
}

void ChangeVehicleModel(int dynamicid, int vehicleid, VehicleData &vehicledata)
{
	float vx, vy, vz, vz_angle, speedx, speedy, speedz;
	GetVehiclePos(vehicleid, &vx, &vy, &vz);
	GetVehicleZAngle(vehicleid, &vz_angle);
	GetVehicleVelocity(vehicleid, &speedx, &speedy, &speedz);
	vector<int> PlayerSeat(MAX_PLAYERS, -1);
	int j = ConnectedIds.size();
	for (int i = 0; i < j; i++)
	{
		int pid = ConnectedIds[i];
		if (!IsPlayerConnected(pid))
			continue;
		if (IsPlayerInVehicle(pid, vehicleid))
			PlayerSeat[pid] = GetPlayerVehicleSeat(pid);
	}
	DestroyVehicle(vehicleid);
	VDynamicID[vehicleid] = -1;
	int carid = CreateVehicle(vehicledata.Model, vehicledata.vx, vehicledata.vy, vehicledata.vz, vehicledata.v_zangle, vehicledata.Color1, vehicledata.Color2, vehicledata.Respawn_Delay, (vehicledata.Addsiren == 1));
	vehicledata.vID = carid;
	SetVehiclePos(carid, vx, vy, vz);
	for (int i = 0; i < j; i++)
	{
		int pid = ConnectedIds[i];
		if (!IsPlayerConnected(pid))
			continue;
		if (PlayerSeat[pid] != -1)
		{
			PutPlayerInVehicle(pid, carid, PlayerSeat[pid]);
		}
	}
	VDynamicID[carid] = dynamicid;
	SetVehicleZAngle(carid, vz_angle);
	SetVehicleVelocity(vehicleid, speedx, speedy, speedz);
}

int CountAllVehicles()
{
	int t = 0;
	for (int i = 1; i < 2000/*MAX_VEHICLES*/; i++)
	{
		if (IsValidVehicle(i))
		{
			t++;
		}
	}
	return t;
}

bool IsDynamicVehicle(int vehicleid)
{
	if (VDynamicID.find(vehicleid) != VDynamicID.end() && VDynamicID[vehicleid] != -1)
	{	
		return true;
	}
	else
	{
		return false;
	}
}

void StreamVehicle(int dynamicid, VehicleData &vehicle)
{
	int vid = sampgdk_CreateVehicle(vehicle.Model, vehicle.vx, vehicle.vy, vehicle.vz, vehicle.v_zangle, vehicle.Color1, vehicle.Color2, vehicle.Respawn_Delay, (vehicle.Addsiren == 1));
	if (vid == 65535)
	{
		//logprintf("0. A invalid vehicle tried to create and I prevented that %d | %d | VehCount: %d", dynamicid, vid, VehiclesCount);
		if (VehiclesCount < 1999)
			VehiclesCount = CountAllVehicles();
		if (VehiclesCount >= 1999)
		{
			if (!vehicle.nearestone)
			{
				vehicle.nearestone = true;
				vehicle.nearestplayer = AnyPlayerNearCar(vehicle.World, vehicle.vx, vehicle.vy, vehicle.vz, vehicle.streamdis, true);
				if (vehicle.nearestplayer == -1.0)
					return;
			}
			if (!RemoveFarestCar(vehicle.nearestplayer, true))
			{
				//logprintf("1. A invalid vehicle tried to create and I prevented that %d | %d | VehCount: %d | %f", dynamicid, vid, VehiclesCount, vehicle.nearestplayer);
				return;
			}
			else
			{
				vid = sampgdk_CreateVehicle(vehicle.Model, vehicle.vx, vehicle.vy, vehicle.vz, vehicle.v_zangle, vehicle.Color1, vehicle.Color2, vehicle.Respawn_Delay, (vehicle.Addsiren == 1));
				if (vid == 65535)
				{
					//logprintf("2. A invalid vehicle tried to create and I prevented that %d | %d | VehCount: %d", dynamicid, vid, VehiclesCount);
					return;
				}
			}
		}
		else
		{
			vid = sampgdk_CreateVehicle(vehicle.Model, vehicle.vx, vehicle.vy, vehicle.vz, vehicle.v_zangle, vehicle.Color1, vehicle.Color2, vehicle.Respawn_Delay, (vehicle.Addsiren == 1));
			if (vid == 65535)
			{
				//logprintf("3. A invalid vehicle tried to create and I prevented that %d | %d | VehCount: %d", dynamicid, vid, VehiclesCount);
				return;
			}
		}
	}
	vehicle.vID = vid;
	VDynamicID[vid] = dynamicid;
	VehicleCreated[dynamicid] = true;
	CreatedVehicles.push_back(dynamicid);
		
	/*************Components******************/
	for (int i = 0; i < 14; i++)
	{
		if (vehicle.Components[i] != 0)
		{
			AddVehicleComponent(vid, vehicle.Components[i]);
		}
	}
	/*************Damages******************/
	UpdateVehicleDamageStatus(vid, vehicle.Damage_panels, vehicle.Damage_doors, vehicle.Damage_lights, vehicle.Damage_tires);
	/*************Paramses******************/
	if(vehicle.Vengine != -1)
		SetVehicleParamsEx(vid, vehicle.Vengine, vehicle.Vlights, vehicle.Valarm, vehicle.Vdoors, vehicle.Vbonnet, vehicle.Vboot, vehicle.Vobjective);
	/*************World******************/
	SetVehicleVirtualWorld(vid, vehicle.World);
	/**************Interior****************/
	LinkVehicleToInterior(vid, vehicle.InteriorID);
	/****************PaintJob******************/
	ChangeVehiclePaintjob(vid, vehicle.Paintjob);
	/****************Health********************/
	SetVehicleHealth(vid, vehicle.Health);
	/*************Player Params****************/
	for (map<int, VehiclePlayerParams>::iterator it = vehicle.PlayerParams.begin(); it != vehicle.PlayerParams.end(); ++it)
	{
		int playerid = it->first, objective = it->second.objective, doorslocked = it->second.doorslocked;
		SetVehicleParamsForPlayer(vid, playerid, objective, doorslocked);
	}
	for (map<int, AttachedObject>::iterator it = vehicle.AttachObject.begin(); it != vehicle.AttachObject.end(); ++it)
	{
		int objectid = it->first;
		if (!IsValidDynamicObject(objectid) || Streamer_GetIntData(0, objectid, 48) != 6621) // object is destroied or something, not the world we set before!
		{
			vehicle.AttachObject.erase(objectid);
			continue;
		}
		float
			offsetx = it->second.offsetx,
			offsety = it->second.offsety,
			offsetz = it->second.offsetz,
			rx = it->second.rx,
			ry = it->second.ry,
			rz = it->second.rz
			;
		AttachDynamicObjectToVehicle(objectid, vid, offsetx, offsety, offsetz, rx, ry, rz);
	}
	if (vehicle.plate.length() > 0)
	{
		SetVehicleNumberPlate(vid, vehicle.plate.c_str());
		SetVehicleToRespawn(vid);
	}
	else
	{
		SetVehicleNumberPlate(vid, "");
	}
	VehiclesCount++;
}

void UpdateTheCache(int exclude)
{
	VehiclesCount = CountAllVehicles();
	for (int i = 1; i < 2000/*MAX_VEHICLES*/; i++)
	{
		if (IsValidVehicle(i) && IsDynamicVehicle(i))
		{
			float X, Y, Z;
			GetVehiclePos(i, &X, &Y, &Z);
			int vehicleid = VDynamicID[i];
			VehiclesData[vehicleid].vx = X;
			VehiclesData[vehicleid].vy = Y;
			VehiclesData[vehicleid].vz = Z;
		}
	}
	int j = ConnectedIds.size();
	for (int i = 0; i < j; i++)
	{
		int pid = ConnectedIds[i];
		if (!IsPlayerConnected(pid))
			continue;
		int pvid = GetPlayerVehicleID(pid);
		PlayersData[pid].world = GetPlayerVirtualWorld(pid);
		PlayersData[pid].vehicleID = pvid;
		if (exclude != pid)
		{
			float X, Y, Z;
			GetPlayerPos(pid, &X, &Y, &Z);
			PlayersData[pid].PX = X;
			PlayersData[pid].PY = Y;
			PlayersData[pid].PZ = Z;
		}
	}
}

bool IsValidDynamicObject(int objectid)
{
	AMX_NATIVE native = sampgdk::FindNative("IsValidDynamicObject");
	if (native != nullptr)
	{
		return (sampgdk::InvokeNative(native, "i", objectid) == 1);
	}
	else
	{
		logprintf("You need streamer plugin to use this fucntion");
		return false;
	}
}

int Streamer_SetIntData(int type, int id, int data, int value)
{
	AMX_NATIVE native = sampgdk::FindNative("Streamer_SetIntData");
	if (native != nullptr)
	{
		return sampgdk::InvokeNative(native, "iiii", type, id, data, value);
	}
	else
	{
		logprintf("You need streamer plugin to use this fucntion");
		return -1;
	}
}

int Streamer_GetIntData(int type, int id, int data)
{
	AMX_NATIVE native = sampgdk::FindNative("Streamer_GetIntData");
	if(native != nullptr)
	{
		return sampgdk::InvokeNative(native, "iii", type, id, data);
	}
	else
	{
		logprintf("You need streamer plugin to use this fucntion");
		return -1;
	}
}

int AttachDynamicObjectToVehicle(int objectid, int vehicleid, float offsetx, float offsety, float offsetz, float rx, float ry, float rz)
{
	AMX_NATIVE native = sampgdk::FindNative("AttachDynamicObjectToVehicle");
	if (native != nullptr)
	{
		return sampgdk::InvokeNative(native, "iiffffff", objectid, vehicleid, offsetx, offsety, offsetz, rx, ry, rz);
	}
	else
	{
		logprintf("You need streamer plugin to use this fucntion");
		return -1;
	}
}
