#include "hooker.h"
#ifdef WIN32
#include <Windows.h>
#include <tlhelp32.h>
auto GetModuleSize(const char* m)
{
    HANDLE hSnap;
    auto processID = GetCurrentProcessId();
    MODULEENTRY32 xModule;
    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
    xModule.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(hSnap, &xModule))
    {
	while (Module32Next(hSnap, &xModule))
	{
	    if (!strncmp((char*)xModule.szModule, m, 8))
	    {
		CloseHandle(hSnap);
		return (DWORD)xModule.modBaseSize;
	    }
	}
    }
    CloseHandle(hSnap);
    return (DWORD)0;
};
#endif

constexpr auto KEYVALUES_PATTERN =
#ifdef WIN32
    "55 8B EC 51 33 C0 C7 45 ? ? ? ? ? 56 8B F1 81 26 ? ? ? ? C6 46 03 ? 89 46 10 89 46 18 89 46 14 89 46 1C 89 46 04 89 46 08 89 46 0C FF 15 ? ? ? ? 6A 01 FF 75 08 8D 4D FC 8B 10 51 8B C8 FF 52 24 8B 0E 33 4D FC 81 E1 ? ? ? ? 31 0E 88 46 03"
#else
    "81 27 ?? ?? ?? FF 55 31 C0 48 89 E5 5D"
#endif
;

RecvVarProxyFn fnSequenceProxyFn;

std::vector<dlinfo_t> libraries;

panorama::PanelArray* panelArray;

// taken form aixxe's cstrike-basehook-linux
bool Hooker::GetLibraryInformation(const char* library, uintptr_t* address, size_t* size)
{
#ifdef WIN32

    *address = (uintptr_t)GetModuleHandleA(library);
    *size = GetModuleSize(library);
    return *address && *size;
#else

    if (libraries.size() == 0)
    {
	dl_iterate_phdr([](struct dl_phdr_info* info, size_t, void*) {
	    dlinfo_t library_info = {};

	    library_info.library = info->dlpi_name;
	    library_info.address = info->dlpi_addr + info->dlpi_phdr[0].p_vaddr;
	    library_info.size = info->dlpi_phdr[0].p_memsz;

	    libraries.push_back(library_info);

	    return 0;
	},
	    nullptr);
    }

    for (const dlinfo_t& current : libraries)
    {
	if (!strcasestr(current.library, library))
	    continue;

	if (address)
	    *address = current.address;

	if (size)
	    *size = current.size;

	return true;
    }

    return false;
#endif
}

void Hooker::InitializeVMHooks()
{
    panelVMT = new VMT(panel);
    clientVMT = new VMT(client);
    inputInternalVMT = new VMT(inputInternal);
    materialVMT = new VMT(material);
    surfaceVMT = new VMT(surface);
    //launcherMgrVMT = new VMT(launcherMgr);//null
    engineVGuiVMT = new VMT(engineVGui);
}

bool Hooker::HookRecvProp(const char* className, const char* propertyName, std::unique_ptr<RecvPropHook>& recvPropHook)
{
    // FIXME: Does not search recursively.. yet.
    // Recursion is a meme, stick to reddit mcswaggens.
    for (auto pClass = client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
    {
	if (strcmp(pClass->m_pNetworkName, className) == 0)
	{
	    auto pClassTable = pClass->m_pRecvTable;

	    for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++)
	    {
		auto pProp = &pClassTable->m_pProps[nIndex];

		if (!pProp || strcmp(pProp->m_pVarName, propertyName) != 0)
		    continue;

		recvPropHook = std::make_unique<RecvPropHook>(pProp);

		return true;
	    }

	    break;
	}
    }

    return false;
}

void Hooker::FindIClientMode()
{
#ifdef WIN32
    clientMode = *(IClientMode**)(PatternFinder::PatternScan("client_panorama.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? 5D") + 1);
#else
    uintptr_t hudprocessinput = reinterpret_cast<uintptr_t>(getvtable(client)[10]);
    GetClientModeFn GetClientMode = reinterpret_cast<GetClientModeFn>(GetAbsoluteAddress(hudprocessinput + 11, 1, 5));

    clientMode = GetClientMode();

#endif
    clientModeVMT = new VMT(clientMode);
}

void Hooker::FindGlobalVars()
{

#ifdef WIN32
    globalVars = **(CGlobalVars***)(PatternFinder::PatternScan("client_panorama.dll", "A1 ? ? ? ? 5E 8B 40 10") + 1);
#else
    uintptr_t HudUpdate = reinterpret_cast<uintptr_t>(getvtable(client)[11]);

    globalVars = *reinterpret_cast<CGlobalVars**>(GetAbsoluteAddress(HudUpdate + 13, 3, 7));
#endif
}

void Hooker::FindCInput()
{
#ifdef WIN32
    input = *(CInput**)(PatternFinder::PatternScan("client_panorama.dll", "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85") + 1);
#else
    uintptr_t IN_ActivateMouse = reinterpret_cast<uintptr_t>(getvtable(client)[16]);

    input = **reinterpret_cast<CInput***>(GetAbsoluteAddress(IN_ActivateMouse, 3, 7));
#endif
}

void Hooker::FindViewRender()
{
#ifndef WIN32
    auto func_address = PatternFinder::PatternScan("client_panorama_client.so", "48 8D 05 ?? ?? ?? ?? 55 F3 0F 10 ?? ?? ?? ?? ?? 48 89 05 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 89 E5 48 8D 0D ?? ?? ?? ?? 0F 57 C9 C6");
    viewRender = reinterpret_cast<CViewRender*>(GetAbsoluteAddress(func_address + 50, 3, 7));
#else
    viewRender = *(CViewRender**)(PatternFinder::PatternScan("client_panorama.dll", "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);
#endif
}

void Hooker::FindGetLocalClient()
{
    auto GetLocalPlayer = reinterpret_cast<uintptr_t>(getvtable(engine)[12]);
    GetLocalClient = reinterpret_cast<GetLocalClientFn>(GetAbsoluteAddress(GetLocalPlayer + 9, 1, 5));
}

void Hooker::FindInitKeyValues()
{
    auto func_address = PatternFinder::PatternScan(CLIENT_FILE,	KEYVALUES_PATTERN);
    InitKeyValues = reinterpret_cast<InitKeyValuesFn>(func_address);
}

void Hooker::FindPanelArrayOffset()
{
    /*
	 * CUIEngine::IsValidPanelPointer()
	 *
	   55                      push    rbp
	   48 81 C7 B8 01 00 00    add     rdi, 1B8h <--------
	 */
    auto IsValidPanelPointer = reinterpret_cast<uintptr_t>(getvtable(panoramaEngine->AccessUIEngine())[35]);
    panelArray = *(panorama::PanelArray**)(((uintptr_t)panoramaEngine->AccessUIEngine()) + *(unsigned int*)(IsValidPanelPointer + 4) + 8); // +8 for vtable
}
