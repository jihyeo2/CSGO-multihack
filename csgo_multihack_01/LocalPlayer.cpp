#include "pch.h"
#include "includes.h"

extern Hack* hack;

LocalPlayer* LocalPlayer::Get()
{
	// role of static keyword here
	// any variables marked with static keyword are only initialized once on the first call to Get() but not on subsequent Get() calls. After the first initialization, the variables will be saved in the memory.
	static uint32_t clientModule = (uint32_t)(GetModuleHandle(L"client.dll"));
	static EntList* entList = (EntList*)(clientModule + hazedumper::signatures::dwEntityList);
	Ent* ent = entList->ents[0].ent;

	if (!ent)
		return nullptr;

	return new LocalPlayer(ent);
}

bool LocalPlayer::CanSeeEntity(Player* player)
{
	// Get start & end positions of a ray
	vec3 eyepos = *GetOrigin() + *GetViewOffset();

	vec3 targeteyepos = *player->GetOrigin() + *player->GetViewOffset();
	vec3 targetheadpos = player->GetBonePos(8);

	CGameTrace trace;
	Ray_t ray;
	CTraceFilter tracefilter;
	tracefilter.pSkip = (void*)this->GetEnt();

	ray.Init(eyepos, targetheadpos);
	// other possible mask value: 0x4600400B, MASK_SHOT_HULL | CONTENTS_HITBOX
	hack->EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &tracefilter, &trace);

	// fraction of a ray: how far it travelled before hitting anything (always valid)
	// ex) 1.f -> travelled the whole distance unobstructed, 0.5f -> hit something half way
	return (trace.fraction > 0.97f && (Ent*)player->GetEnt() == (Ent*)trace.hit_entity);
}

void LocalPlayer::AimAt(Vector3 target) // target's head position vector
{
	const double PI = 3.14159265358;
	static Vector3* viewAngles = (Vector3*)(*(uint32_t*)(hack->engine +
		hazedumper::signatures::dwClientState) + hazedumper::signatures::dwClientState_ViewAngles); // dwClientState_ViewAngles: current player's view angle; saved in engine.dll because it affects rendering, which is controlled by engine.dll, and has to be updated in real-time

	Vector3 origin = *GetOrigin();
	Vector3 viewOffset = *GetViewOffset();
	Vector3 sum = (origin + viewOffset); // localPlayer's eye position
	Vector3* myPos = &sum;

	Vector3 deltaVec = { target.x - myPos->x, target.y - myPos->y, target.z - myPos->z };
	float deltaVecLength = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);

	// because asin and atan2 functions return in radian, we have to convert the return values into degrees
	float pitch = -asin(deltaVec.z / deltaVecLength) * (180 / PI); // added negative because when positive the character looks below not upwards to the target's head
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);
	// smoothing added
	float smoothing = 4.0f;
	Vector3 angleDiff = { (pitch - viewAngles->x) / smoothing, (yaw - viewAngles->y) / smoothing, 0 };
	Vector3 updatedViewAngles = { viewAngles->x + angleDiff.x, viewAngles->y + angleDiff.y, 0 };

	if (updatedViewAngles.x >= -89 && updatedViewAngles.x <= 89 && updatedViewAngles.y >= -180 && updatedViewAngles.y <= 180)
	{
		viewAngles->x = updatedViewAngles.x;
		viewAngles->y = updatedViewAngles.y;
	}
}

// accumulatedExtraSamples: represents how much extra time (in seconds) to simulate movement ex) 0.0f means "don't simulate any extra time — just process a single tick
// bFinalTick: passing true means "This is the final tick for this frame.Go ahead and send the movement command to the server.”
// It forces the engine to apply all usercmds, and sync the new player state to the server.
typedef void(__cdecl* CL_Move_t)(float accumuldatedExtraSamples, bool bFinalTick);

// not working; de-sync with the server
void LocalPlayer::Teleport(Vector3* destination)
{
	SetOrigin(destination);
	Vector3 vec(0.f, 0.f, 5.f);
	SetVelocity(&vec);

	//static CL_Move_t CL_Move = nullptr;
	//if (!CL_Move)
	//{
	//	/* static_cast<> vs reinterpret_cast<>
	//	// 1. static_cast<> is only for safe, compile - time checked CONVERSIONS like:
	//	//	ex) int <-> float / up/downcasting in inheritance trees / void* -> object
	//	// 2. reinterpret_cast<> is used for low-level, bitwise reinterpretation (NOT A CONVERSION) of one type as another:
	//	//	ex) cast between completely unrelated types like a void* and a function pointer
	//	*/
	//	void* addr = PatternScanModule(L"engine.dll", (const Pattern&)SIG_CL_MOVE);
	//	CL_Move = reinterpret_cast<CL_Move_t>(addr);
	//}

	//if (CL_Move)
	//	CL_Move(0.f, true);
}