#pragma once // a header guard in C++ that prevents multiple inclusions of the same header file during compilation

#include "GlowConfig.h"
#include "Vector3.h"

// Forward Declaration
// basically telling the compiler that there is a class called Vector3, but not telling what's inside it yet.
// This is enough in the header because only a pointer--not a full object--is used, and pointers have a fixed size.
// - useful when only pointers or references to Vector3 (e.g., Vector3* or Vector3&) are used.
// -  if you use Vector3 by value (e.g., as a member variable or a function parameter or return type by value), then you need the full definition because the compiler needs to know its size and layout.
class Ent;

class Player
{
	// private - only the class itself (and friends) can access
	// protected - the class itself and derived classes can access
	// public - exposed to everyone
private:
	Ent* ent;

protected:
	// explicit - prevents implicit conversion
	// initializer list - used to initialize member variables instead of setting the member variable to a dummy then assigning them in the constructor body; more efficient
	explicit Player(Ent* entity) : ent(entity) {}
public:
	// static: class specific methods; no access to 'this' instance or non-static variables; can be called by itself
	static int* GetMaxPlayer();
	static Player* GetPlayer(int index);
	static bool IsValidPlayer(Player* player);

	// non-static: instance-specific methods; has access to 'this' instance or both static and non-static variables; always need an instance though, cannot be called by itself
	Ent* GetEnt() {
		return ent;
	}

	int* GetHealth();
	void SetHealth(int health);
	int* GetArmorValue();
	int* GetTeam();
	int* GetGlowIndex();
	bool* IsDormant();
	bool* IsSpotted();
	void setSpotted();
	bool* IsScoped();
	bool* IsDefusing();
	bool* hasHelmet();
	void setClrRender(ClrRender c);
	Vector3* GetOrigin();
	void SetOrigin(Vector3* origin);
	Vector3* GetVelocity();
	void SetVelocity(Vector3* velocity);
	float* GetEyeAnglesX();
	float* GetEyeAnglesY();
	Vector3* GetAimPunchAngle();
	Vector3* GetViewOffset();
	Vector3 GetBonePos(int boneID);
	float GetDistance(Vector3* other);
	int* GetNumShotsFired();
	int* GetObjectIDInCrosshair();
	int* GetWeapon();
};
