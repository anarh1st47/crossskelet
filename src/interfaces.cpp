#include "interfaces.h"



IBaseClientDLL* client;
ISurface* surface;
IVPanel* panel;
IEngineClient* engine;
IClientEntityList* entityList;
IVDebugOverlay* debugOverlay;
IVModelInfo* modelInfo;
IVModelRender* modelRender;
IClientMode* clientMode;
IEngineTrace* trace;
IInputSystem* inputSystem;
IInputInternal* inputInternal;
IMaterialSystem* material;
ICvar* cvar;
CGlobalVars* globalVars;
CEffects* effects;
IGameEventManager2* gameEvents;
IPhysicsSurfaceProps* physics;
CViewRender* viewRender;
IPrediction* prediction;
IGameMovement* gameMovement;
IMoveHelper* moveHelper;
ILauncherMgr* launcherMgr;
C_CSPlayerResource** csPlayerResource;
C_CSGameRules** csGameRules;
IEngineVGui* engineVGui;
IEngineSound* sound;
ILocalize* localize;
ICommandLine* commandline;
CInput* input;
IPanoramaUIEngine* panoramaEngine;

VMT* panelVMT;
VMT* clientVMT;
VMT* clientModeVMT;
VMT* inputInternalVMT;
VMT* materialVMT;
VMT* surfaceVMT;
VMT* launcherMgrVMT;
VMT* engineVGuiVMT;

InitKeyValuesFn InitKeyValues;
LoadFromBufferFn LoadFromBuffer;

SetNamedSkyBoxFn SetNamedSkyBox;

int* nPredictionRandomSeed;
CMoveData* g_MoveData;
bool* s_bOverridePostProcessingDisable;

#ifdef WIN32
CreateInterfaceFn get_module_factory(HMODULE module)
{
    return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
}

template <typename T>
T* get_interface(CreateInterfaceFn f, const char* szInterfaceVersion)
{
    auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

    if (!result)
    {
	throw std::runtime_error(std::string("[get_interface] Failed to GetOffset interface: ") + szInterfaceVersion);
    }

    return result;
}
#endif

void Interfaces::FindInterfaces()
{
#ifndef WIN32
	client = GetInterface<IBaseClientDLL>("./csgo/bin/linux64/client_panorama_client.so", "VClient");
	engine = GetInterface<IEngineClient>("./bin/linux64/engine_client.so", "VEngineClient");
	entityList = GetInterface<IClientEntityList>("./csgo/bin/linux64/client_panorama_client.so", "VClientEntityList");
	surface = GetInterface<ISurface>("./bin/linux64/vguimatsurface_client.so", "VGUI_Surface");
	panel = GetInterface<IVPanel>("./bin/linux64/vgui2_client.so", "VGUI_Panel");
	debugOverlay = GetInterface<IVDebugOverlay>("./bin/linux64/engine_client.so", "VDebugOverlay");
	modelInfo = GetInterface<IVModelInfo>("./bin/linux64/engine_client.so", "VModelInfoClient");
	modelRender = GetInterface<IVModelRender>("./bin/linux64/engine_client.so", "VEngineModel");
	trace = GetInterface<IEngineTrace>("./bin/linux64/engine_client.so", "EngineTraceClient");
	inputSystem = GetInterface<IInputSystem>("./bin/linux64/inputsystem_client.so", "InputSystemVersion");
	inputInternal = GetInterface<IInputInternal>("./bin/linux64/vgui2_client.so", "VGUI_InputInternal");
	material = GetInterface<IMaterialSystem>("./bin/linux64/materialsystem_client.so", "VMaterialSystem");
	cvar = GetInterface<ICvar>("./bin/linux64/materialsystem_client.so", "VEngineCvar");
	effects = GetInterface<CEffects>("./bin/linux64/engine_client.so", "VEngineEffects");
	gameEvents = GetInterface<IGameEventManager2>("./bin/linux64/engine_client.so", "GAMEEVENTSMANAGER002", true);
	physics = GetInterface<IPhysicsSurfaceProps>("./bin/linux64/vphysics_client.so", "VPhysicsSurfaceProps");
    prediction = GetInterface<IPrediction>("./csgo/bin/linux64/client_client.so", "VClientPrediction001", true);
	gameMovement = GetInterface<IGameMovement>("./csgo/bin/linux64/client_panorama_client.so", "GameMovement");
	engineVGui = GetInterface<IEngineVGui>("./bin/linux64/engine_client.so", "VEngineVGui");
	sound = GetInterface<IEngineSound>("./bin/linux64/engine_client.so", "IEngineSoundClient");
	localize = GetInterface<ILocalize>("./bin/linux64/localize_client.so", "Localize_");
	commandline = GetSymbolAddress<CommandLineFn>("./bin/linux64/libtier0_client.so", "CommandLine")();
	panoramaEngine = GetInterface<IPanoramaUIEngine>("./bin/linux64/panorama_client.so", "PanoramaUIEngine001", true);
#else
    auto engineFactory = get_module_factory(GetModuleHandleW(L"engine.dll"));
    auto clientFactory = get_module_factory(GetModuleHandleW(L"client_panorama.dll"));
    auto valveStdFactory = get_module_factory(GetModuleHandleW(L"vstdlib.dll"));
    auto vguiFactory = get_module_factory(GetModuleHandleW(L"vguimatsurface.dll"));
    auto vgui2Factory = get_module_factory(GetModuleHandleW(L"vgui2.dll"));
    auto matSysFactory = get_module_factory(GetModuleHandleW(L"materialsystem.dll"));
    auto dataCacheFactory = get_module_factory(GetModuleHandleW(L"datacache.dll"));
    auto vphysicsFactory = get_module_factory(GetModuleHandleW(L"vphysics.dll"));
    auto inputFactory = get_module_factory(GetModuleHandleW(L"inputsystem.dll"));
    auto localizeFactory = get_module_factory(GetModuleHandleW(L"localize.dll"));
    auto PanoramaFactory = get_module_factory(GetModuleHandleW(L"panorama.dll"));


    client = get_interface<IBaseClientDLL>(clientFactory, "VClient018");
    engine = get_interface<IEngineClient>(engineFactory, "VEngineClient014");
    entityList = get_interface<IClientEntityList>(clientFactory, "VClientEntityList003");
    surface = get_interface<ISurface>(vguiFactory, "VGUI_Surface031");
    panel = get_interface<IVPanel>(vgui2Factory, "VGUI_Panel009");
    debugOverlay = get_interface<IVDebugOverlay>(engineFactory, "VDebugOverlay004");
    modelInfo = get_interface<IVModelInfo>(engineFactory, "VModelInfoClient004");
    modelRender = get_interface<IVModelRender>(engineFactory, "VEngineModel016");
    trace = get_interface<IEngineTrace>(engineFactory, "EngineTraceClient004");
    inputSystem = get_interface<IInputSystem>(inputFactory, "InputSystemVersion001");
    inputInternal = get_interface<IInputInternal>(vgui2Factory, "VGUI_InputInternal001");
    material = get_interface<IMaterialSystem>(matSysFactory, "VMaterialSystem080");
    cvar = get_interface<ICvar>(valveStdFactory, "VEngineCvar007");
    effects = GetInterface<CEffects>("./bin/linux64/engine_client.so", "VEngineEffects");
	gameEvents = get_interface<IGameEventManager2>(engineFactory, "GAMEEVENTSMANAGER002");
    physics = get_interface<IPhysicsSurfaceProps>(vphysicsFactory, "VPhysicsSurfaceProps001");
    prediction = get_interface<IPrediction>(clientFactory, "VClientPrediction001");
    gameMovement = get_interface<IGameMovement>(clientFactory, "GameMovement001");
    engineVGui = get_interface<IEngineVGui>(engineFactory, "VEngineVGui001");
    sound = get_interface<IEngineSound>(engineFactory, "IEngineSoundClient003");
    localize = get_interface<ILocalize>(localizeFactory, "Localize_001");
    //commandline = GetSymbolAddress<CommandLineFn>("./bin/linux64/libtier0_client.so", "CommandLine")();
    panoramaEngine = get_interface<IPanoramaUIEngine>(PanoramaFactory, "PanoramaUIEngine001");

    
#endif
}

void Interfaces::DumpInterfaces()
{
#ifdef WIN32
//lol
	#else
	std::stringstream ss;

	std::vector<const char*> modules;

	dl_iterate_phdr([](struct dl_phdr_info* info, size_t size, void* data) {
		reinterpret_cast<std::vector<const char*>*>(data)->push_back(info->dlpi_name);
		return 0;
	}, &modules);

	for (auto module : modules)
	{
		void* library = dlopen(module, RTLD_NOLOAD | RTLD_NOW);

		if (!library)
			continue;

		void* interfaces_sym = dlsym(library, "s_pInterfaceRegs");

		if (!interfaces_sym)
		{
			dlclose(library);
			continue;
		}

		dlclose(library);

		InterfaceReg* interfaces = *reinterpret_cast<InterfaceReg**>(interfaces_sym);

		InterfaceReg* cur_interface;

		std::set<const char*> interface_name;

		for (cur_interface = interfaces; cur_interface; cur_interface = cur_interface->m_pNext)
			interface_name.insert(cur_interface->m_pName);

		if (interface_name.empty())
			continue;

		ss << std::string(module) << "\n";

		for (auto interface : interface_name)
			ss << "\t" << interface << "\n";

		ss << '\n';
	}

	std::string interfacesPath = "/tmp/interfaces.txt";

	std::ofstream(interfacesPath) << ss.str();
#endif
}
