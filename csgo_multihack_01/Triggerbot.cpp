#include "pch.h"
#include "includes.h"

extern Hack* hack;

std::array<int, 6> grenadeWeaponIDs = { 43, 44, 45, 46, 47, 48 };

struct variables
{
    int tbDelay;
    int myweaponID;
    float weaponAccuracyPenalty;
} val;

void setTBDelay(float distance)
{
    float delay;
    switch (val.myweaponID)
    {
    case 262204:
        delay = 3;
        break;
    case 7:
        delay = 3.3;
        break;
    case 40:
    case 9:
        delay = 0.15;
        break;
    default:
        delay = 0;
    }
    val.tbDelay = delay * distance;
}

void getMyWeapon()
{
    int weaponID = *hack->localPlayer->GetWeapon();
    DWORD weaponEntity = *(DWORD*)(hack->client + hazedumper::signatures::dwEntityList + (weaponID & 0xFFF) * 0x10);
    if (weaponEntity)
    {
        val.myweaponID = *(int*)(weaponEntity + hazedumper::netvars::m_iItemDefinitionIndex);
        val.weaponAccuracyPenalty = *(float*)(weaponEntity + hazedumper::netvars::m_fAccuracyPenalty);
    }
}

void shoot()
{
    Sleep(val.tbDelay);
    *(int*)(hack->client + hazedumper::signatures::dwForceLMB) = 5;
    Sleep(20);
    *(int*)(hack->client + hazedumper::signatures::dwForceLMB) = 4;
}

bool checkTriggerbot()
{
    LocalPlayer* localplayer = hack->localPlayer;

    int targetID = *localplayer->GetObjectIDInCrosshair() - 1;

    if (targetID != 0 && targetID < 64)
    {
        Player* targetPlayer = Player::GetPlayer(targetID);

        if (!Player::IsValidPlayer(targetPlayer))
            return false;

        if ((uint32_t)targetPlayer->GetEnt() == (uint32_t)localplayer->GetEnt())
            return false;

        if (*targetPlayer->GetTeam() == *localplayer->GetTeam())
            return false;

        if (*localplayer->GetHealth() < 1)
            return false;

        float distance = localplayer->GetDistance(targetPlayer->GetOrigin());
        getMyWeapon();
        setTBDelay(distance);

        // grenade validation check
        for (int i = 0; i < grenadeWeaponIDs.size(); i++) {
            if (val.myweaponID == grenadeWeaponIDs[i])
                return false;
        }

        if (val.weaponAccuracyPenalty < 0.1f) {
            if (val.myweaponID == 40 || val.myweaponID == 9)
                return *localplayer->IsScoped();
            else
                return true;
        }
        else
            return false;
    }
    return false;
}

void RunTriggerbot()
{
    if (checkTriggerbot())
        shoot();
}
