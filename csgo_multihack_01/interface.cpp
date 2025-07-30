#include "pch.h"
#include "interface.h"

void* GetInterface(tCreateInterface fn, const char* name)
{
	return (fn)(name, 0);
}