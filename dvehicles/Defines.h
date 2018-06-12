#pragma once

#include <vector>
#include <map>
#include <string>
#include <math.h>
#include <thread>
#include <algorithm>

#define DV_DEBUG

#define CHECK_PARAMS(m, n) \
	if (params[0] != (m * 4)) \
	{ \
		logprintf("  SA-MP Features: %s: Expecting %d parameter(s), but found %d", n, m, params[0] / sizeof(cell)); \
		return 0; \
	}

#define BULLET_HIT_TYPE_VEHICLE	2

struct PlayerData
{
	int 
		world,
		vehicleID
		;
	float
		PX,
		PY,
		PZ
		;
};

struct VehiclePlayerParams
{
	int
		objective,
		doorslocked
		;
};

struct VehicleData
{
	int
		Model,
		Color1,
		Color2,
		Paintjob,
		Respawn_Delay,
		Addsiren,
		vID,
		Components[14],
		World,
		InteriorID,
		Damage_panels,
		Damage_doors,
		Damage_lights,
		Damage_tires,
		Vengine, 
		Vlights, 
		Valarm, 
		Vdoors, 
		Vbonnet, 
		Vboot, 
		Vobjective
		;
	float
		defvx,
		defvy,
		defvz,
		vx,
		vy,
		vz,
		v_zangle,
		Quatw,
		Quatx,
		Quaty,
		Quatz,
		Health,
		streamdis,
		nearestplayer
		;
	string
		plate
		;
	bool
		ShouldBeCreated,
		nearestone
		;
	map<int, VehiclePlayerParams> PlayerParams;
};

vector<VehicleData>
	VehiclesData
;
vector<int>
	FreeVehicleSlots
;
vector<int>
	CreatedVehicles
;
vector<int>
	ConnectedIds,
	TakeAction
;
map<int, bool>
	VehicleCreated
;
map<int, int>
	VDynamicID
;
map<int, PlayerData>
	PlayersData
;
float streamdistance = 200.0;
int
	VehiclesCount = 0, 
	MAX_PLAYERS = 100,
	connected_players = 0,
	lastputid = -1,
	VehicleCheckDelay = 500
;
bool sortbeforeuse = false, threadrunning = false;
int ExecuteFunction(AMX *amx, string fname, vector<int> parameters, vector<float> fparameters);
int ExecuteFunction(string fname, vector<int> parameters, vector<float> fparameters);
float AnyPlayerNearCar(int world, float vx, float vy, float vz, float streamdis, bool nearest);
float GetDistanceBetweenTwoPoints(float x1, float y1, float z1, float x2, float y2, float z2);
int GetFreeVehicleSlot();
void WhoNearMe(int dynamicid, VehicleData &vehicle, int newcar = 0, bool mainthread = false, int exclude = -1);
bool RemoveFarestCar(float nearestdis, bool mainthread = false);
bool IsValidDynamicVehicleEx(int vehicleid);
void ResetVehicleData(int dynamicid);
void CheckTheCars(AMX *amx);
int GetFarestCarID(float nearestdis, bool mainthread = false);
void UnStreamDynamicVehicle(int dynamicid, int vehicleid);
void ChangeVehicleModel(int dynamicid, int vehicleid, VehicleData &vehicledata);
int CountAllVehicles();
bool IsDynamicVehicle(int vehicleid);
void StreamVehicle(int dynamicid, VehicleData &vehicle);
void UpdateTheCache(int exclude = -1);
AMX *globalamx = nullptr;