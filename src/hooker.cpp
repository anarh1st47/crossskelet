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

RecvVarProxyFn fnSequenceProxyFn;

StartDrawingFn StartDrawing;
FinishDrawingFn FinishDrawing;

GetLocalClientFn GetLocalClient;

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

void Hooker::FindPlayerResource()
{
    uintptr_t instruction_addr = PatternFinder::FindPatternInModule("client_panorama_client.so",
	(unsigned char*)"\x48\x8B\x05\x00\x00\x00\x00\x55\x48\x89\xE5\x48\x85\xC0\x74\x10\x48",
	"xxx????xxxxxxxxxx");

    csPlayerResource = reinterpret_cast<C_CSPlayerResource**>(GetAbsoluteAddress(instruction_addr, 3, 7));
}

void Hooker::FindGameRules()
{
    auto instruction_addr = PatternFinder::FindPatternInModule("client_panorama_client.so",
	(unsigned char*)"\x48\x8B\x05"
			"\x00\x00\x00\x00"
			"\x48\x8B\x38\x0F\x84",
	"xxx????xxxxx");

    csGameRules = *reinterpret_cast<C_CSGameRules***>(GetAbsoluteAddress(instruction_addr, 3, 7));
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

void Hooker::FindPrediction()
{
    auto seed_instruction_addr = PatternFinder::FindPatternInModule("client_panorama_client.so",
	(unsigned char*)"\x48\x8B\x05"
			"\x00\x00\x00\x00"
			"\x8B\x38\xE8"
			"\x00\x00\x00\x00"
			"\x89\xC7",
	"xxx????xxx????xx");
    auto helper_instruction_addr = PatternFinder::FindPatternInModule("client_panorama_client.so",
	(unsigned char*)"\x00\x48\x89\x3D\x00\x00\x00\x00\xC3",
	"xxxx????x");
    auto movedata_instruction_addr = PatternFinder::FindPatternInModule("client_panorama_client.so",
	(unsigned char*)"\x48\x8B\x0D"
			"\x00\x00\x00\x00"
			"\x4C\x89\xEA",
	"xxx????xxx");

    nPredictionRandomSeed = *reinterpret_cast<int**>(GetAbsoluteAddress(seed_instruction_addr, 3, 7));
    moveHelper = *reinterpret_cast<IMoveHelper**>(GetAbsoluteAddress(helper_instruction_addr + 1, 3, 7));
    g_MoveData = **reinterpret_cast<CMoveData***>(GetAbsoluteAddress(movedata_instruction_addr, 3, 7));
}

void Hooker::FindSurfaceDrawing()
{
    auto start_func_address = PatternFinder::FindPatternInModule("vguimatsurface_client.so",
	(unsigned char*)"\x55\x48\x89\xE5\x53\x48\x89\xFB\x48\x83\xEC\x28\x80\x3D",
	"xxxxxxxxxxxxxx");
    StartDrawing = reinterpret_cast<StartDrawingFn>(start_func_address);

    auto finish_func_address = PatternFinder::FindPatternInModule("vguimatsurface_client.so",
	(unsigned char*)"\x55\x31\xFF\x48\x89\xE5\x53",
	"xxxxxxx");
    FinishDrawing = reinterpret_cast<FinishDrawingFn>(finish_func_address);
}

void Hooker::FindGetLocalClient()
{
    auto GetLocalPlayer = reinterpret_cast<uintptr_t>(getvtable(engine)[12]);
    GetLocalClient = reinterpret_cast<GetLocalClientFn>(GetAbsoluteAddress(GetLocalPlayer + 9, 1, 5));
}

void Hooker::FindInitKeyValues()
{
    auto func_address = PatternFinder::FindPatternInModule("client_panorama_client.so",
	(unsigned char*)"\x81\x27\x00\x00\x00\xFF\x55\x31\xC0\x48\x89\xE5\x5D",
	"xxxxxxxxxxxxx");
    InitKeyValues = reinterpret_cast<InitKeyValuesFn>(func_address);
}

void Hooker::FindLoadFromBuffer()
{
    auto func_address = PatternFinder::FindPatternInModule("client_panorama_client.so",
	(unsigned char*)"\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x55\x41\x54\x49\x89\xD4\x53\x48\x83\xEC\x78\x48",
	"xxxxxxxxxxxxxxxxxxxxx");
    LoadFromBuffer = reinterpret_cast<LoadFromBufferFn>(func_address);
}

void Hooker::FindOverridePostProcessingDisable()
{
    auto bool_address = PatternFinder::FindPatternInModule("client_panorama_client.so",
	(unsigned char*)"\x80\x3D"
			"\x00\x00\x00\x00\x00"
			"\x89\xB5"
			"\x00\x00"
			"\xFF\xFF",
	"xx?????xx??xx");
    bool_address = GetAbsoluteAddress(bool_address, 2, 7);

    s_bOverridePostProcessingDisable = reinterpret_cast<bool*>(bool_address);
}

void Hooker::FindSDLInput()
{
    /*
        0F 95 83 AC 01 00 00    setnz   byte ptr [rbx+1ACh]
        E8 E2 B7 FF FF          call    _memcpy
        E8 FD D8 02 00          call    LauncherMgrCreateFunc <------
     */
    uintptr_t startAddr = PatternFinder::FindPatternInModule("launcher_client.so",
	(unsigned char*)"\x0F\x95\x83"
			"\x00\x00\x00\x00"
			"\xE8"
			"\x00\x00\x00\x00"
			"\xE8",
	"xxx????x????x");
    auto createFunc = reinterpret_cast<ILauncherMgrCreateFn>(GetAbsoluteAddress(startAddr + 12, 1, 5));
    launcherMgr = createFunc();
}

void Hooker::FindSetNamedSkybox()
{
    //55 4C 8D 05 ?? ?? ?? ?? 48 89 E5
    // xref for "skybox/%s%s"
    auto func_address = PatternFinder::FindPatternInModule("engine_client.so",
	(unsigned char*)"\x55\x4C\x8D\x05"
			"\x00\x00\x00\x00" //??
			"\x48\x89\xE5",
	"xxxx????xxx");

    SetNamedSkyBox = reinterpret_cast<SetNamedSkyBoxFn>(func_address);
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
