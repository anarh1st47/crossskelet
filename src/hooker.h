#pragma once

#include <memory>
#include <unordered_map>
#ifndef WIN32
#include <sys/mman.h>
#include <link.h>
#define __thiscall
#endif
#include "Utils/patternfinder.h"
#include "Utils/recvproxyhook.h"
#include "SDK/SDK.h"
#include "Utils/vmt.h"
#include "Utils/util.h"
#include "interfaces.h"

struct dlinfo_t
{
	const char* library = nullptr;
	uintptr_t address = 0;
	size_t size = 0;
};

namespace Hooker
{
	inline GetLocalClientFn GetLocalClient;
	bool GetLibraryInformation(const char* library, uintptr_t* address, size_t* size);
	void InitializeVMHooks();
	bool HookRecvProp(const char* className, const char* propertyName, std::unique_ptr<RecvPropHook>& recvPropHook);
	void FindIClientMode();
	void FindGlobalVars();
	void FindCInput();
	void FindViewRender();
	void FindGetLocalClient();
	void FindInitKeyValues();
	void FindPanelArrayOffset();
}
