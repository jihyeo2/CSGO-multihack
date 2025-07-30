#include "pch.h"
#include "Signatures.h"

// CL_Move 
Pattern SIG_CL_MOVE = {
	(const char*)"\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\x53\x56\x8A\xF9",
	"xxxxx????xxxx",
	13
};

// CBasePlayer::OnTakeDamage
Pattern SIG_PLAYER_ONTAKEDMG = {
	(const char*)"\x55\x8B\xEC\x83\xE4\x00\x81\xEC\x00\x00\x00\x00\x8B\x45\x00\x53\x56\x57\x8B\x58",
	"xxxxx?xx????xx?xxxxx",
	20
};