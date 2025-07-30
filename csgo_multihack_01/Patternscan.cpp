#include "pch.h"
#include "includes.h"

void* PatternScan(char* base, size_t size, const char* pattern, const char* mask)
{
	size_t patternLength = strlen(mask);

	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(base + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
			return (void*)(base + i);
	}
	return nullptr;
}

void* PatternScanWrapper(uintptr_t begin, uintptr_t end, const char* pattern, const char* mask)
{
	uintptr_t currentChunk = begin;

	while (currentChunk < end)
	{
		const size_t chunkSize = 4096; // 4KB; 1 memory page size
		size_t bytesToScan = (end - currentChunk) >= chunkSize ? chunkSize : (end - currentChunk);

		DWORD oldprotect;
		VirtualProtect((void*)currentChunk, bytesToScan, PAGE_EXECUTE_READWRITE, &oldprotect);
		void* internalAddress = PatternScan((char*)currentChunk, bytesToScan, pattern, mask);
		VirtualProtect((void*)currentChunk, bytesToScan, oldprotect, &oldprotect);

		if (internalAddress != nullptr)
		{
			return internalAddress;
		}
		else {
			currentChunk += bytesToScan;
		}
	}
	return nullptr;
}

//char* GenerateMask(const char* pattern)
//{
//	size_t patternLength = strlen(pattern);
//	char* mask = new char[patternLength + 1];
//	for (size_t i = 0; i < patternLength; i++)
//	{
//		mask[i] = (pattern[i] == '\x00') ? '?' : 'x';
//	}
//	mask[patternLength] = '\0'; 
//	return mask;
//}

void* PatternScanModule(const wchar_t* moduleName, const Pattern& pattern)
{
	uintptr_t begin = (uintptr_t)(GetModuleHandle(moduleName));
	uintptr_t end = GetModuleEnd(moduleName);

	void* result = PatternScanWrapper(begin, end, pattern.bytes, pattern.mask);

	return result;
}