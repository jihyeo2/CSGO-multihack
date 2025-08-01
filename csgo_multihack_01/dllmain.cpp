// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "includes.h"
#include <sstream>
#include <string.h>

void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice)
{
    // pDevice doesn't get reset (i.e. stays the same) for the lifetime of the game session
    if (!pDevice)
        pDevice = o_pDevice;

    // 1. Print menu

    DrawText("Jerry's fart stinks", windowWidth / 2, windowHeight - 20, D3DCOLOR_ARGB(255, 255, 255, 255));

    int menuOffX = windowWidth - 100;
    int menuOffY = 50;
    D3DCOLOR enabled = D3DCOLOR_ARGB(255, 0, 255, 0);
    D3DCOLOR disabled = D3DCOLOR_ARGB(255, 255, 0, 0);

    if (!hack->settings.showMenu) {
        DrawText("Show Menu (INS)", menuOffX, menuOffY, D3DCOLOR_ARGB(255, 255, 255, 255));
    }
    else {
        DrawText("Aimbot (Mouse Side Button-Front)", menuOffX, menuOffY + 0 * 12, hack->settings.aimbot ? enabled : disabled);
        DrawText("Triggerbot (Mouse Side Button-Back)", menuOffX, menuOffY + 1 * 12, hack->settings.triggerbot ? enabled : disabled);
        DrawText("Recoil (F1)", menuOffX, menuOffY + 2 * 12, hack->settings.rcs ? enabled : disabled);

		DrawText("-------- ESP --------", menuOffX, menuOffY + 3 * 12, D3DCOLOR_ARGB(255, 255, 255, 255));

        DrawText("Radar (F2)", menuOffX, menuOffY + 4 * 12, hack->settings.radar ? enabled : disabled);
        DrawText("Glow (F3)", menuOffX, menuOffY + 5 * 12, hack->settings.glow ? enabled : disabled);
        DrawText("Show Teammates (F4)", menuOffX, menuOffY + 6 * 12, hack->settings.showTeammates ? enabled : disabled);
        DrawText("Snaplines (F5)", menuOffX, menuOffY + 7 * 12, hack->settings.snaplines ? enabled : disabled);
        DrawText("2D Box (F6)", menuOffX, menuOffY + 8 * 12, hack->settings.box2D ? enabled : disabled);
        DrawText("2D Status Bars (F7)", menuOffX, menuOffY + 9 * 12, hack->settings.status2D ? enabled : disabled);
        DrawText("Status Text (F8)", menuOffX, menuOffY + 10 * 12, hack->settings.statusText ? enabled : disabled);
        DrawText("3D Box (F9)", menuOffX, menuOffY + 11 * 12, hack->settings.box3D ? enabled : disabled);
        DrawText("Velocity Esp (F11)", menuOffX, menuOffY + 12 * 12, hack->settings.velEsp ? enabled : disabled);
        DrawText("Headline Esp (Home)", menuOffX, menuOffY + 13 * 12, hack->settings.headlineEsp ? enabled : disabled);
        DrawText("Recoil crosshair (Delete)", menuOffX, menuOffY + 14 * 12, hack->settings.rcsCrosshair ? enabled : disabled);

        DrawText("Hide Menu (INS)", menuOffX, menuOffY + 15 * 12, D3DCOLOR_ARGB(255, 255, 255, 255));
    }

    // 2. Hacks (aimbot, triggerbot, recoil, radar, glow)
    if (hack->settings.aimbot) {
        RunAimbot();
        Sleep(1);
    }

    if (hack->settings.triggerbot) {
        RunTriggerbot();
        Sleep(10);
    }

    if (hack->settings.rcs)
        RunRCShack();

    if (hack->settings.radar)
		RunRadarhack();

    if (hack->settings.glow)
		RunGlowhack();

    // 3. ESP

    // rectangle
    // DrawFilledRect(25, 25, 100, 100, D3DCOLOR_ARGB(255, 255, 255, 255));

    for (int i = 1; i < 32; i++) {
        Player* currPlayer = Player::GetPlayer(i);

        if (!hack->CheckValidEnt(currPlayer))
            continue;

        D3DCOLOR espColor, snaplineColor, velocityColor, headlineColor;
        if (*currPlayer->GetTeam() == *(hack->localPlayer->GetTeam())) {
            espColor = hack->colors.team.esp;
            snaplineColor = hack->colors.team.snapline;
            velocityColor = hack->colors.team.velocity;
            headlineColor = hack->colors.team.headline;
            //color = D3DCOLOR_ARGB(255, 0, 255, 0); // green for teammates
        }
        else {
            espColor = hack->colors.enemy.esp;
            snaplineColor = hack->colors.enemy.snapline;
            velocityColor = hack->colors.enemy.velocity;
            headlineColor = hack->colors.enemy.headline;
            //color = D3DCOLOR_ARGB(255, 255, 0, 0); // red for enemies
        }

        if (!hack->settings.showTeammates && (*currPlayer->GetTeam() == *(hack->localPlayer->GetTeam())))
            continue;

        Vec3 entHead3D = currPlayer->GetBonePos(8);
        entHead3D.z += 8; // to cover the head but penetrate it with the esp box border
        Vec2 entPos2D, entHead2D;

        if (hack->WorldToScreen(*currPlayer->GetOrigin(), entPos2D)) {

            if (hack->settings.velEsp)
            {
                Vec3 velOff = *currPlayer->GetOrigin() + (*currPlayer->GetVelocity() * 0.25); // .25 to scale it down
                Vec2 velOff2D;

                if (hack->WorldToScreen(velOff, velOff2D)) {
                    DrawLine(entPos2D, velOff2D, 2, velocityColor);
                    DrawFilledRect(velOff2D.x - 2, velOff2D.y - 2, 4, 4, espColor); // draws a small rectangle at the end of the line
                }
            }

            if (hack->settings.snaplines)
                DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, snaplineColor);

            if (hack->WorldToScreen(entHead3D, entHead2D)) {
                if (hack->settings.box2D)
                    DrawEspBox2D(entPos2D, entHead2D, 2, espColor);

                if (hack->settings.box3D)
                    DrawEspBox3D(entHead3D, *currPlayer->GetOrigin(), *currPlayer->GetEyeAnglesY(), 25, 2, espColor);

                if (hack->settings.status2D) {

                    //  Creating a health bar and an armor bar that follow the direction of the body (dX) and scale its length based on health.
                    int height = ABS(entPos2D.y - entHead2D.y);
                    int dX = (entPos2D.x - entHead2D.x);

                    float healthPerc = *currPlayer->GetHealth() / 100.0f;
                    float armorPerc = *currPlayer->GetArmorValue() / 100.0f;

                    Vec2 botHealth, topHealth, botArmor, topArmor;
                    int healthHeight = height * healthPerc;
                    int armorHeight = height * armorPerc;

                    botHealth.y = botArmor.y = entPos2D.y;

                    botHealth.x = entPos2D.x - (height / 4) - 2;
                    botArmor.x = entPos2D.x + (height / 4) + 2;

                    topHealth.y = entHead2D.y + height - healthHeight;
                    topArmor.y = entHead2D.y + height - armorHeight;

                    topHealth.x = entPos2D.x - (height / 4) - 2 - (dX * healthPerc);
                    topArmor.x = entPos2D.x + (height / 4) + 2 - (dX * armorPerc);

                    DrawLine(botHealth, topHealth, 2, hack->colors.health);
                    DrawLine(botArmor, topArmor, 2, hack->colors.armor);
                }

                if (hack->settings.headlineEsp) {
                    Vec3 head3D = currPlayer->GetBonePos(8);
                    Vec3 entAngles;
                    entAngles.x = *currPlayer->GetEyeAnglesX();
                    entAngles.y = *currPlayer->GetEyeAnglesY();
                    entAngles.z = 0;
                    Vec3 endPoint = hack->TransformVec(head3D, entAngles, 60);
                    Vec2 endPoint2D, head2D;
                    hack->WorldToScreen(head3D, head2D);
                    if (hack->WorldToScreen(endPoint, endPoint2D))
                        DrawLine(head2D, endPoint2D, 2, headlineColor);
                }

                if (hack->settings.statusText) {

                    //std::stringstream s1, s2;
                    //s1 << currEnt->m_iHealth;
                    //s2 << currEnt->m_ArmorValue;
                    //std::string t1 = "hp: " + s1.str();
                    //std::string t2 = "ap: " + s2.str();

                    std::string t1 = "hp: " + std::to_string(*currPlayer->GetHealth());
                    std::string t2 = "ap: " + std::to_string(*currPlayer->GetArmorValue());
                    const char* healthMsg = (char*)t1.c_str();
                    const char* armorMsg = (char*)t2.c_str();

                    DrawText(healthMsg, entPos2D.x, entPos2D.y, D3DCOLOR_ARGB(255, 255, 255, 255));
                    DrawText(armorMsg, entPos2D.x, entPos2D.y + 12, D3DCOLOR_ARGB(255, 255, 255, 255));

                    if (!(*currPlayer->hasHelmet()))
                        DrawText("easy headshot", entPos2D.x, entPos2D.y + 24, D3DCOLOR_ARGB(255, 255, 255, 255));
                }
            }
        }

    }

    // crosshair

    // old implementation
    //DrawFilledRect(windowWidth / 2 - 2, windowHeight / 2 - 2, 4, 4, D3DCOLOR_ARGB(255, 255, 255, 255));
    if (hack->settings.rcsCrosshair) {
        Vec2 l, r, t, b;

        l = r = t = b = hack->crosshair2D;
        l.x -= hack->crosshairSize;
        r.x += hack->crosshairSize;
        b.y += hack->crosshairSize;
        t.y -= hack->crosshairSize;

        DrawLine(l, r, 2, hack->colors.crosshair);
        DrawLine(t, b, 2, hack->colors.crosshair);
    }

    oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule)
{
    if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
    {
        // d3d9Device[42]: EndScene()
        memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

        oEndScene = (tEndScene)TrampHook((BYTE*)d3d9Device[42], (BYTE*)hkEndScene, 7);
    }

    hack = new Hack();
    hack->Init();

    while (!GetAsyncKeyState(VK_END)) {
        hack->Update();

        Vec3 pAng = *(hack->localPlayer->GetAimPunchAngle());

        // ptich(x): positive when looking down, negative when up
        // yaw(y): positive towards right, negative towards left
        // windowWidth / 90: pixel-per-degree measure
        hack->crosshair2D.x = windowWidth / 2 - (windowWidth / 90 * pAng.y);
        hack->crosshair2D.y = windowHeight / 2 + (windowHeight / 90 * pAng.x);
    }

    Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

    Sleep(1000);

    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  reason,
    LPVOID lpr
)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        // Closing the handle not the thread; the thread will still be running
        CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
    }
    return TRUE;
}

