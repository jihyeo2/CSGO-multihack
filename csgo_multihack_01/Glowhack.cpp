#include "pch.h"
#include "includes.h"

GlowObjectManager* glowManager;

void SetGlowColor(GlowStruct& glow, Player* player)
{
    if (*player->IsDefusing())
    {
        glow.red = 1.0f;
        glow.green = 1.0f;
        glow.blue = 1.0f;
    }
    else
    {
        int health = *player->GetHealth();
        glow.red = health * -0.01 + 1;
        glow.green = health * 0.01;
    }
    glow.alpha = 1.0f;
    glow.renderWhenOccluded = true;
    glow.renderWhenUnOccluded = false;
}

void SetTeamGlow(Player* player, int glowIndex)
{
    GlowStruct& tGlow = glowManager->glowObjects[glowIndex];
    tGlow.blue = 1.0f;
    tGlow.alpha = 1.0f;
    tGlow.renderWhenOccluded = true;
    tGlow.renderWhenUnOccluded = false;
}

void SetEnemyGlow(Player* player, int glowIndex)
{
    GlowStruct& eGlow = glowManager->glowObjects[glowIndex];
    SetGlowColor(eGlow, player);
}

void RunGlowhack()
{
    LocalPlayer* localPlayer = LocalPlayer::Get();

    static uint32_t clientModule = (uint32_t)(GetModuleHandle(L"client.dll"));
    glowManager = (GlowObjectManager*)(clientModule + hazedumper::signatures::dwGlowObjectManager);

    int myTeam = *localPlayer->GetTeam();

    for (short int i = 1; i < *Player::GetMaxPlayer(); i++)
    {
        Player* currentPlayer = Player::GetPlayer(i);

        if (!Player::IsValidPlayer(currentPlayer))
            continue;

        if ((uint32_t)currentPlayer->GetEnt() == (uint32_t)localPlayer->GetEnt())
            continue;

        int glowIndex = *currentPlayer->GetGlowIndex();

        if (myTeam == *currentPlayer->GetTeam()) {
            currentPlayer->setClrRender(GlowColors::clrTeam);
            SetTeamGlow(currentPlayer, glowIndex);
        }
        else {
            currentPlayer->setClrRender(GlowColors::clrEnemy);
            SetEnemyGlow(currentPlayer, glowIndex);
        }
    }
}