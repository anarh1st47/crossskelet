#include "interfaces.h"

void Interfaces::FindInterfaces()
{
	client = GetInterface<IBaseClientDLL>(CLIENT_FILE, "VClient018");
	engine = GetInterface<IEngineClient>(ENGINE_FILE, "VEngineClient014");
	entityList = GetInterface<IClientEntityList>(CLIENT_FILE, "VClientEntityList003");
	surface = GetInterface<ISurface>(VGUI_FILE, "VGUI_Surface031");
	panel = GetInterface<IVPanel>(VGUI2_FILE, "VGUI_Panel009");
	debugOverlay = GetInterface<IVDebugOverlay>(ENGINE_FILE, "VDebugOverlay004");
	modelInfo = GetInterface<IVModelInfo>(ENGINE_FILE, "VModelInfoClient004");
	modelRender = GetInterface<IVModelRender>(ENGINE_FILE, "VEngineModel016");
	trace = GetInterface<IEngineTrace>(ENGINE_FILE, "EngineTraceClient004");
	inputSystem = GetInterface<IInputSystem>(INSYS_FILE, "InputSystemVersion001");
	inputInternal = GetInterface<IInputInternal>(VGUI2_FILE, "VGUI_InputInternal001");
	material = GetInterface<IMaterialSystem>(MATSYS_FILE, "VMaterialSystem080");
	cvar = GetInterface<ICvar>(MATSYS_FILE, "VEngineCvar007");
	gameEvents = GetInterface<IGameEventManager2>(ENGINE_FILE, "GAMEEVENTSMANAGER002");
	physics = GetInterface<IPhysicsSurfaceProps>(VPHYS_FILE, "VPhysicsSurfaceProps001");
	gameMovement = GetInterface<IGameMovement>(CLIENT_FILE, "GameMovement001");
	engineVGui = GetInterface<IEngineVGui>(ENGINE_FILE, "VEngineVGui001");
	sound = GetInterface<IEngineSound>(ENGINE_FILE, "IEngineSoundClient003");
	localize = GetInterface<ILocalize>(LOCALIZE_FILE, "Localize_001");
	panoramaEngine = GetInterface<IPanoramaUIEngine>(PANORAMA_FILE, "PanoramaUIEngine001");

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
