#pragma once

#include "hook.h"
#include "Signatures.h"

void* PatternScan(char* base, size_t size, const char* pattern, const char* mask);
void* PatternScanWrapper(uintptr_t begin, uintptr_t end, const char* pattern, const char* mask);
//char* GenerateMask(const char* pattern, char* mask);
void* PatternScanModule(const wchar_t* moduleName, const Pattern& pattern);