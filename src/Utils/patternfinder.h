#pragma once

#include <cstdint>

namespace PatternFinder
{
	std::uint8_t* PatternScan(char* module, const char* signature);
}