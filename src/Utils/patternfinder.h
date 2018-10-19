#pragma once

#include <cstdint>

namespace PatternFinder
{
	uintptr_t FindPattern(uintptr_t dwAddress, uintptr_t dwLen, unsigned char* bMask, const char* szMask);
uintptr_t FindPatternInModule(const char* moduleName, unsigned char* bMask, const char* szMask);
std::uint8_t* PatternScan(void* module, const char* signature);
}