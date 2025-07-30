#pragma once

#include "Player.h"
#include "Vector3.h"

class LocalPlayer : public Player
{
private:
	explicit LocalPlayer(Ent* entity) : Player(entity) {}

public:
	static LocalPlayer* Get(); // static because Get() belongs to the class, not to any instance of LocalPlayer

	bool CanSeeEntity(Player* player);
	void AimAt(Vector3 target);
	void Teleport(Vector3* destination);
};