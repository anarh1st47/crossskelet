#pragma once

#include <string>
#ifndef WIN32
#include <sys/mman.h>
#include <link.h>
#include <zconf.h>
#endif
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <locale>
#include <codecvt>
#include <sstream>

#include "../SDK/SDK.h"

namespace Util
{
	std::wstring StringToWstring(std::string str);
	long GetEpochTime();
}