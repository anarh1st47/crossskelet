#pragma once

#include <unordered_map>
#ifndef WIN32
#include <zconf.h>
#include <dirent.h>
#endif
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "SDK/SDK.h"
#include "Utils/draw.h"
#include "Utils/util.h"
#include "Utils/util_sdk.h"

namespace Settings
{

	namespace SkyBox
	{
		inline bool enabled;
		inline const char* name = NULL;
	}

	namespace DisablePostProcessing
	{
		inline bool enabled;
	}

	namespace NoFog
	{
		inline bool enabled;
	}
	bool RegisterConVars();
	}
