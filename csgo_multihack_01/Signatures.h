#pragma once

struct Pattern {
	const char* bytes;
	const char* mask;
	size_t length;
};

extern Pattern SIG_CL_MOVE;
extern Pattern SIG_PLAYER_ONTAKEDMG;