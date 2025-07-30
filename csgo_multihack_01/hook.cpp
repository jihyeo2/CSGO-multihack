#include "pch.h"
#include "includes.h"

void Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oProc;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oProc);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oProc, &oProc);
}

bool Hook(BYTE* src, BYTE* dst, int len)
{
	if (len < 5) return false;
	DWORD oProc;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProc);
	memset(src, 0x90, len);
	// to my function
	// dst - (src + 5)
	uintptr_t relAddy = (uintptr_t)(dst - src - 5);
	*src = (char)0xE9;
	*(uintptr_t*)(src + 1) = (uintptr_t)relAddy;
	VirtualProtect(src, len, oProc, &oProc);
}

BYTE* TrampHook(BYTE* src, BYTE* dst, unsigned int len)
{
	if (len < 5) return 0;

	//Create Gateway
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	//write the stolen bytes to the gateway
	memcpy(gateway, src, len);

	//Get the gateway to destination address
	// gateway is allocated with VirtualAlloc — usually this lands at a random (and high) address due to ASLR (Address Space Layout Randomization)
	// (gateway + len) + 5 + relative address = (src + len)
	// relative address = (src + len) - (gateway + len) - 5
	uintptr_t jumpAddy = (uintptr_t)(src - gateway - 5);

	// add the jmp opcode to the end of the gateway
	*(gateway + len) = (char)0xE9;

	//Write the address of the gateway to the jmp
	*(uintptr_t*)(gateway + len + 1) = jumpAddy;

	if (Hook(src, dst, len))
		return gateway;
	else
		return nullptr;
}

uintptr_t GetModuleEnd(const wchar_t* moduleName)
{
	uintptr_t base = (uintptr_t)GetModuleHandle(moduleName);
	if (!base)
		return 0;

	/*
	+------------------------+
	| DOS Header (MZ)       |
	| - e_lfanew -> 0x80    | e_lfanew: points to the NT header
	+------------------------+
	| ... padding ...       |
	+------------------------+
	| NT Headers (PE)       |
	+------------------------+.

	*/
	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)base;
	IMAGE_NT_HEADERS* ntHeaders = (IMAGE_NT_HEADERS*)(base + dosHeader->e_lfanew);

	return base + ntHeaders->OptionalHeader.SizeOfImage;
}
