#include "pch.h"
#include "includes.h"

#include <iostream>

extern Hack* hack;

Player* GetClosestEnemy()
{
	LocalPlayer* localPlayer = hack->localPlayer;

	float closestDistance = 1000000;
	int closestDistanceIndex = -1;

	for (int i = 1; i < *Player::GetMaxPlayer(); i++)
	{
		Player* currentPlayer = Player::GetPlayer(i);

		if (!Player::IsValidPlayer(currentPlayer))
			continue;

		if ((uint32_t)currentPlayer->GetEnt() == (uint32_t)localPlayer->GetEnt())
		{
			continue;
		}

		if (*currentPlayer->GetTeam() == *localPlayer->GetTeam())
		{
			continue;
		}

		if (*localPlayer->GetHealth() < 1)
		{
			continue;
		}

		if (*currentPlayer->IsDormant())
		{
			continue;
		}

		if (!localPlayer->CanSeeEntity(currentPlayer))
		{
			continue;
		}

		float currentDistance = localPlayer->GetDistance(currentPlayer->GetOrigin());
		if (currentDistance < closestDistance)
		{
			closestDistance = currentDistance;
			closestDistanceIndex = i;
		}
	}

	if (closestDistanceIndex == -1)
	{
		return NULL;
	}

	return Player::GetPlayer(closestDistanceIndex);
}

void RunAimbot()
{
	Player* closestEnemy = GetClosestEnemy();

	if (closestEnemy)
		hack->localPlayer->AimAt(closestEnemy->GetBonePos(8)); // 8: head
}

void RunRadarhack()
{
	for (int i = 1; i < *Player::GetMaxPlayer(); i++)
	{
		Player* currentPlayer = Player::GetPlayer(i);

		if (!Player::IsValidPlayer(currentPlayer))
		{
			continue;
		}

		currentPlayer->setSpotted();
	}
}

static Vector3 oPunch = Vector3( 0.f, 0.f, 0.f );

void RunRCShack()
{
	LocalPlayer* localplayer = hack->localPlayer;

	Vector3* viewAngles = (Vector3*)(*(uintptr_t*)(hack->engine + hazedumper::signatures::dwClientState) + hazedumper::signatures::dwClientState_ViewAngles);
	Vector3* aimPunchAngle = localplayer->GetAimPunchAngle();
	int* iShotsFired = localplayer->GetNumShotsFired();

	if (*iShotsFired > 1) {
		Vector3 punchAngle = *aimPunchAngle * 2;
		Vector3 newAngle = { 0, 0, 0 };
		newAngle = *viewAngles + (oPunch - punchAngle); // smoother recoil compensation
		newAngle.Normalize();
		*viewAngles = newAngle;
		oPunch = punchAngle;
	}
	else {
		oPunch = Vector3();
	}
}

void TeleportToEnemy()
{
	const double PI = 3.14159265358;

	Player* closestEnemy = GetClosestEnemy();
	if (closestEnemy)
	{
		Vector3* enemyOrigin = closestEnemy->GetOrigin();
		Vector3* enemyVelocity = closestEnemy->GetVelocity();

		// separate when vel is zero and not

		if (!enemyOrigin || !enemyVelocity)
		{
			return;
		}

		float distanceBehind = 50.0f;
		
		float yawRad = atan2(enemyVelocity->y, enemyVelocity->x);

		Vector3 delta;
		delta.x = -cos(yawRad) * distanceBehind;
		delta.y = -sin(yawRad) * distanceBehind;
		delta.z = 50.0f;

		Vector3 newPos = *enemyOrigin + delta;

		LocalPlayer* localPlayer = hack->localPlayer;
		localPlayer->Teleport(&newPos);
	}
}

typedef int(__fastcall* OnTakeDamage_t)(void* thisptr, void* edx, void* CTakeDamageInfo);
OnTakeDamage_t oOnTakeDamage;

int __fastcall hOnTakeDamage(void* thisptr, void* edx, void* CTakeDamageInfo) {
	std::cout << "hooked dmg func for target: " << thisptr << std::endl;
	return oOnTakeDamage(thisptr, edx, CTakeDamageInfo);
}

// not working; turns out I don't have access to the server where OnTakeDamage() is		called even in a bot match
// How the control flow looks like:
// game calls OnTakeDamage() -> hOnTakeDamage() -> oOnTakeDamage() (gateway)
void RunGodmode() {
	std::cout << "Running God Mode" << std::endl;
	// 1. pattern scan CBasePlayer::OnTakeDamage
	void* addr = PatternScanModule(L"server.dll", (const Pattern&)SIG_PLAYER_ONTAKEDMG);

	if (!addr)
	{
		std::cout << "Pattern scan for OnTakeDamage failed" << std::endl;
		return;
	}

	// 2. Hook with a simple function
	oOnTakeDamage = (OnTakeDamage_t)addr;
	oOnTakeDamage = (OnTakeDamage_t)TrampHook((BYTE*)oOnTakeDamage, (BYTE*)hOnTakeDamage, 6);

	// 3. Add my function
	// a) check who the target player is
	// b) return early if the target player is me
}