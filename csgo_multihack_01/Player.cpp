#include "pch.h"
#include "includes.h"

// Player:: prefix tells the compiler that these are not global functions and guides to correctly scope them to Player class
Player* Player::GetPlayer(int index)
{
	static uint32_t clientModule = (uint32_t)GetModuleHandle(L"client.dll");
	static EntList* entList = (EntList*)(clientModule + hazedumper::signatures::dwEntityList);
	Ent* ent = entList->ents[index].ent;

	if (!ent)
		return nullptr;

	// new keyword creates a new instance of Player class on the heap, not on the stack (temporary bc only valid in this function), and returns a pointer, not an object, to it
	return new Player(ent);
}

int* Player::GetMaxPlayer()
{
	static uint32_t moduleBase = (uintptr_t)GetModuleHandle(L"engine.dll");
	return (int*)(*(uint32_t*)(moduleBase + hazedumper::signatures::dwClientState) + hazedumper::signatures::dwClientState_MaxPlayer);
}

bool Player::IsValidPlayer(Player* player)
{
	if (!player || !player->GetEnt() || !(*(uint32_t*)player->GetEnt()))
		return false;
	if (*player->GetHealth() <= 0)
		return false;
	return true;
}

Vector3* Player::GetOrigin()
{
	return &ent->m_vecOrigin;
}

void Player::SetOrigin(Vector3* origin)
{
	ent->m_vecOrigin = *origin;
}

Vector3* Player::GetVelocity()
{
	return &ent->m_vecVelocity;
}

void Player::SetVelocity(Vector3* velocity)
{
	ent->m_vecVelocity = *velocity;
}

Vector3* Player::GetViewOffset()
{
	return &ent->m_vecViewOffset; // m_vecViewOffset:  offset from m_vecOrigin to the player's eye position
}

float* Player::GetEyeAnglesX()
{
	return &ent->m_angEyeAnglesX;
}

float* Player::GetEyeAnglesY()
{
	return &ent->m_angEyeAnglesY;
}

Vector3* Player::GetAimPunchAngle()
{
	return &ent->m_aimPunchAngle;
}

Vector3 Player::GetBonePos(int boneID)
{
	uint32_t boneMatrix = ent->m_dwBoneMatrix;
	Vector3 bonePos;
	bonePos.x = *(float*)(boneMatrix + 0x30 * boneID + 0x0C);
	bonePos.y = *(float*)(boneMatrix + 0x30 * boneID + 0x1C);
	bonePos.z = *(float*)(boneMatrix + 0x30 * boneID + 0x2C);
	return bonePos;
}

int* Player::GetHealth()
{
	return &ent->m_iHealth;
}

void Player::SetHealth(int health)
{
	ent->m_iHealth = health;
}

int* Player::GetArmorValue()
{
	return &ent->m_ArmorValue;
}

int* Player::GetTeam()
{
	return &ent->m_iTeamNum;
}

int* Player::GetNumShotsFired()
{
	return &ent->m_iShotsFired;
}

int* Player::GetObjectIDInCrosshair()
{
	return &ent->m_iCrosshairId;
}

int* Player::GetWeapon()
{
	return &ent->m_hActiveWeapon;
}

int* Player::GetGlowIndex()
{
	return &ent->m_iGlowIndex;
}

// m_bDormant is set to True when the target enters the PVS (Potentially Visible Set), not necessarily in the direct field of view, or when the game client receives udpates about them (ex) your ally dies) 
bool* Player::IsDormant()
{
	return &ent->m_bDormant;
}

bool* Player::IsSpotted()
{
	return &ent->m_bSpotted;
}

void Player::setSpotted()
{
	ent->m_bSpotted = true;
}

bool* Player::IsScoped()
{
	return &ent->m_bIsScoped;
}

bool* Player::IsDefusing()
{
	return &ent->m_bIsDefusing;
}

bool* Player::hasHelmet()
{
	return &ent->m_bHasHelmet;
}

void Player::setClrRender(ClrRender c)
{
	ent->m_clrRender = c;
}

float Player::GetDistance(Vector3* other)
{
	Vector3* myPos = GetOrigin();
	Vector3 delta = Vector3(other->x - myPos->x, other->y - myPos->y, other->z - myPos->z);
	return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}