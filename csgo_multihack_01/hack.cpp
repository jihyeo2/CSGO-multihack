#include "pch.h"
#include "includes.h"

#define KEYUP 0
#define KEYDOWN 0x8000
#define KEYPRESSED 1

Hack::~Hack() {
	this->FontF->Release();
	this->LineL->Release();
}

void Hack::Init() {
	client = (uintptr_t)GetModuleHandle(L"client.dll");
	engine = (uintptr_t)GetModuleHandle(L"engine.dll");
	entList = (EntList*)(client + hazedumper::signatures::dwEntityList);
	localPlayer = LocalPlayer::Get();

	tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress((HMODULE)engine, "CreateInterface");
	EngineTrace = (IEngineTrace*)GetInterface(CreateInterface, "EngineTraceClient004");
}

// Responsible for keeping our view matrix updated
void Hack::Update() {
	memcpy(&viewMatrix, (BYTE**)(client + hazedumper::signatures::dwViewMatrix), sizeof(viewMatrix)); // is BYTE** correct? I think it should be float**, but it still works like that.
	this->CheckButtons();
}

void Hack::CheckButtons() {
	settings.aimbot = GetAsyncKeyState(buttons.aimbotBtn) & KEYDOWN; 
	settings.triggerbot = GetAsyncKeyState(buttons.triggerbotBtn) & KEYDOWN;

	if (GetAsyncKeyState(buttons.showMenuBtn) & KEYPRESSED)
		settings.showMenu = !settings.showMenu;
	//if (GetAsyncKeyState(buttons.aimbotBtn) & KEYDOWN)
	//	settings.aimbot = !settings.aimbot;
	//if (GetAsyncKeyState(buttons.triggerbotBtn) & KEYDOWN)
	//	settings.triggerbot = !settings.triggerbot;
	if (GetAsyncKeyState(buttons.rcsBtn) & KEYPRESSED)
		settings.rcs = !settings.rcs;
	if (GetAsyncKeyState(buttons.radarBtn) & KEYPRESSED)
		settings.radar = !settings.radar;
	if (GetAsyncKeyState(buttons.glowBtn) & KEYPRESSED)
		settings.glow = !settings.glow;
	if (GetAsyncKeyState(buttons.showTeammatesBtn) & KEYPRESSED)
		settings.showTeammates = !settings.showTeammates;
	if (GetAsyncKeyState(buttons.snaplinesBtn) & KEYPRESSED)
		settings.snaplines = !settings.snaplines;
	if (GetAsyncKeyState(buttons.box2DBtn) & KEYPRESSED)
		settings.box2D = !settings.box2D;
	if (GetAsyncKeyState(buttons.status2DBtn) & KEYPRESSED)
		settings.status2D = !settings.status2D;
	if (GetAsyncKeyState(buttons.statusTextBtn) & KEYPRESSED)
		settings.statusText = !settings.statusText;
	if (GetAsyncKeyState(buttons.box3DBtn) & KEYPRESSED)
		settings.box3D = !settings.box3D;
	if (GetAsyncKeyState(buttons.velEspBtn) & KEYPRESSED)
		settings.velEsp = !settings.velEsp;
	if (GetAsyncKeyState(buttons.headlineEspBtn) & KEYPRESSED)
		settings.headlineEsp = !settings.headlineEsp;
	if (GetAsyncKeyState(buttons.rcsCrosshairBtn) & KEYPRESSED)
		settings.rcsCrosshair = !settings.rcsCrosshair;
}

bool Hack::CheckValidEnt(Player* player) {
	if (!Player::IsValidPlayer(player))
		return false;
	if ((uint32_t)player->GetEnt() == (uint32_t)localPlayer->GetEnt())
		return false;
	if (*player->IsDormant())
		return false;
	return true;
}

bool Hack::WorldToScreen(Vec3 pos, Vec2& screen) {
	Vec4 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	// If less than 0.1, coord is behind us
	if (clipCoords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

// cosf returns float; float is less precise than double but often used in games and faster to process
Vec3 Hack::TransformVec(Vec3 src, Vec3 ang, float d)
{
	Vec3 newPos;
	newPos.x = src.x + (cosf(TORAD(ang.y)) * d);
	newPos.y = src.y + (sinf(TORAD(ang.y)) * d);
	newPos.z = src.z + (tanf(TORAD(ang.x)) * d);
	return newPos;
}
