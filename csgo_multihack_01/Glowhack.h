#pragma once
#include "Player.h"
#include "LocalPlayer.h"
#include "GlowConfig.h"

void SetGlowColor(GlowStruct& glow, Player* player);
void SetTeamGlow(Player* player, int glowIndex);
void SetEnemyGlow(Player* player, int glowIndex);

void RunGlowhack();

