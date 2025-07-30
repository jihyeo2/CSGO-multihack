#pragma once

#include <Windows.h>
#include "Vector3.h"
#include "Offsets.h"
#include "GlowConfig.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

class Ent {
public:
	union {
		DEFINE_MEMBER_N(bool, m_bDormant, hazedumper::signatures::m_bDormant);
		DEFINE_MEMBER_N(int, m_iHealth, hazedumper::netvars::m_iHealth);
		DEFINE_MEMBER_N(int, m_iTeamNum, hazedumper::netvars::m_iTeamNum);
		DEFINE_MEMBER_N(Vec3, m_vecOrigin, hazedumper::netvars::m_vecOrigin);
		DEFINE_MEMBER_N(uint32_t, m_dwBoneMatrix, hazedumper::netvars::m_dwBoneMatrix);
		DEFINE_MEMBER_N(int, m_ArmorValue, hazedumper::netvars::m_ArmorValue);
		DEFINE_MEMBER_N(Vec3, m_aimPunchAngle, hazedumper::netvars::m_aimPunchAngle);
		DEFINE_MEMBER_N(float, m_angEyeAnglesX, hazedumper::netvars::m_angEyeAnglesX);
		DEFINE_MEMBER_N(float, m_angEyeAnglesY, hazedumper::netvars::m_angEyeAnglesY);
		DEFINE_MEMBER_N(Vec3, m_vecVelocity, hazedumper::netvars::m_vecVelocity);
		DEFINE_MEMBER_N(bool, m_bHasHelmet, hazedumper::netvars::m_bHasHelmet);
		DEFINE_MEMBER_N(Vec3, m_vecViewOffset, hazedumper::netvars::m_vecViewOffset);
		DEFINE_MEMBER_N(int, m_iGlowIndex, hazedumper::netvars::m_iGlowIndex);
		DEFINE_MEMBER_N(bool, m_bSpotted, hazedumper::netvars::m_bSpotted);
		DEFINE_MEMBER_N(bool, m_bIsDefusing, hazedumper::netvars::m_bIsDefusing);
		DEFINE_MEMBER_N(ClrRender, m_clrRender, hazedumper::netvars::m_clrRender);
		DEFINE_MEMBER_N(int, m_iShotsFired, hazedumper::netvars::m_iShotsFired);
		DEFINE_MEMBER_N(int, m_iCrosshairId, hazedumper::netvars::m_iCrosshairId);
		DEFINE_MEMBER_N(int, m_hActiveWeapon, hazedumper::netvars::m_hActiveWeapon);
		DEFINE_MEMBER_N(bool, m_bIsScoped, hazedumper::netvars::m_bIsScoped);
	};
};

class EntListObj {
public:
	Ent* ent;
	char padding[12];
};

class EntList {
public:
	EntListObj ents[32]; // 32 =  max number of players in CSGO
};