#pragma once

#include <d3dx9.h>
#include "Vectors.h"
#include "Ent.h"

class LocalPlayer;
class Player;
class IEngineTrace;

#define ABS(x) ((x < 0) ? (-x) : (x))
#define TORAD(x) (x * 0.01745329252)
#define W2S(x, y) hack->WorldToScreen(x, y)

class Hack
{
public:
	uintptr_t engine;
	uintptr_t client;
	LocalPlayer* localPlayer;
	EntList* entList;
	float viewMatrix[16];

	IEngineTrace* EngineTrace;

	ID3DXLine* LineL;
	ID3DXFont* FontF;

	Vec2 crosshair2D;
	int crosshairSize = 4;

	~Hack();

	void Init();
	void Update();

	void CheckButtons();

	bool CheckValidEnt(Player* player);
	bool WorldToScreen(Vec3 pos, Vec2& screen);
	Vec3 TransformVec(Vec3 src, Vec3 ang, float d);

	struct Settings {
		bool showMenu = false;
		bool aimbot = false;
		bool triggerbot = false;
		bool rcs = false;
		bool radar = false;
		bool glow = false;
		bool showTeammates = false;
		bool snaplines = false;
		bool box2D = false;
		bool status2D = false;
		bool statusText = false;
		bool box3D = false;
		bool velEsp = false;
		bool headlineEsp = false;
		bool rcsCrosshair = false;
	} settings;

	struct Buttons {
		DWORD showMenuBtn = VK_INSERT;
		DWORD aimbotBtn = VK_F1;
		DWORD triggerbotBtn = VK_F2;
		DWORD rcsBtn = VK_F3;
		DWORD radarBtn = VK_F4;
		DWORD glowBtn = VK_F5;
		DWORD showTeammatesBtn = VK_F6;
		DWORD snaplinesBtn = VK_F7;
		DWORD box2DBtn = VK_F8;
		DWORD status2DBtn = VK_F9;
		DWORD statusTextBtn = VK_F11;
		DWORD box3DBtn = VK_HOME;
		DWORD velEspBtn = VK_DELETE;
		DWORD headlineEspBtn = VK_PRIOR;
		DWORD rcsCrosshairBtn = VK_NEXT;
	} buttons;

	struct Colors {
		D3DCOLOR health = D3DCOLOR_ARGB(255, 46, 139, 87);
		D3DCOLOR armor = D3DCOLOR_ARGB(255, 30, 144, 255);
		D3DCOLOR crosshair = D3DCOLOR_ARGB(255, 255, 255, 255);
		struct Team {
			D3DCOLOR esp = D3DCOLOR_ARGB(255, 0, 255, 0);
			D3DCOLOR snapline = D3DCOLOR_ARGB(255, 0, 255, 0);
			D3DCOLOR velocity = D3DCOLOR_ARGB(255, 0, 0, 255);
			D3DCOLOR headline = D3DCOLOR_ARGB(255, 0, 0, 255);
		} team;
		struct Enemy {
			D3DCOLOR esp = D3DCOLOR_ARGB(255, 255, 0, 0);
			D3DCOLOR snapline = D3DCOLOR_ARGB(255, 255, 0, 0);
			D3DCOLOR velocity = D3DCOLOR_ARGB(255, 0, 255, 255);
			D3DCOLOR headline = D3DCOLOR_ARGB(255, 0, 255, 255);
		} enemy;
	} colors;
};

