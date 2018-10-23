#include "Main.h"

void MainThread()
{
	Interfaces::FindInterfaces();
	//Interfaces::DumpInterfaces();

	//Hooker::FindSetNamedSkybox();
	Hooker::FindViewRender();
	//Hooker::FindSDLInput();
	Hooker::InitializeVMHooks();
	Hooker::FindIClientMode();
	Hooker::FindGlobalVars();
	Hooker::FindCInput();
	//Hooker::FindPlayerResource();
	//Hooker::FindGameRules();
	//Hooker::FindPrediction();
	//Hooker::FindSurfaceDrawing();
	Hooker::FindGetLocalClient();
	//Hooker::FindInitKeyValues();
	//Hooker::FindLoadFromBuffer();
	//Hooker::FindVstdlibFunctions();
	//Hooker::FindOverridePostProcessingDisable();
	Hooker::FindPanelArrayOffset();

	/*if (!Settings::RegisterConVars())
	{
	    cvar->ConsoleDPrintf("Error making Custom ConVars! Stopping...\n");
	    return;
	}
	cvar->ConsoleDPrintf("sensitivity @ %p\n", (void*)cvar->FindVar("sensitivity"));*/

	//clientVMT->HookVM( Hooks::FrameStageNotify, 37 );
	//clientVMT->ApplyVMT();

    //clientModeVMT->HookVM( Hooks::ShouldDrawFog, 18 );
    //clientModeVMT->ApplyVMT();

    //engineVGuiVMT->HookVM( Hooks::Paint, 41 );
    //engineVGuiVMT->ApplyVMT();

    inputInternalVMT->HookVM(Hooks::SetKeyCodeState, (int)Indexes::SetKeyCodeState);
    inputInternalVMT->ApplyVMT();
    //clientVMT->HookVM(Hooks::CreateMove, 22);
    //clientVMT->ApplyVMT();
    clientModeVMT->HookVM((void*)Hooks::CreateMove, (int)Indexes::CreateMove);
    clientModeVMT->ApplyVMT();

	panelVMT->HookVM((void*)Hooks::PaintTraverse, (int)Indexes::PanelTraverse);
    panelVMT->ApplyVMT();
    //launcherMgrVMT->ApplyVMT();

	//materialVMT->HookVM( Hooks::BeginFrame, 42 );
	//materialVMT->ApplyVMT();

	//surfaceVMT->HookVM( Hooks::LockCursor, 67 );
	//surfaceVMT->ApplyVMT();

	//NetVarManager::DumpNetvars();
	Offsets::GetOffsets();

	//Settings::LoadSettings();

	srand(time(NULL)); // Seed random # Generator so we can call rand() later

    cvar->ConsoleColorPrintf( ColorRGBA(0, 225, 0), "\nskeletux Successfully loaded.\n" );

}
#ifndef WIN32
/* Entrypoint to the Library. Called when loading */
int __attribute__((constructor)) Startup()
{
	std::thread mainThread(MainThread);
	// The root of all suffering is attachment
	// Therefore our little buddy must detach from this realm.
	// Farewell my thread, may we join again some day..
	mainThread.detach();

	return 0;
}
/* Called when un-injecting the library */
void __attribute__((destructor)) Shutdown()
{
#ifdef HUJ
	cvar->FindVar("cl_mouseenable")->SetValue(1);

	clientVMT->ReleaseVMT();
    clientModeVMT->ReleaseVMT();
    engineVGuiVMT->ReleaseVMT();
    inputInternalVMT->ReleaseVMT();
    launcherMgrVMT->ReleaseVMT();
    materialVMT->ReleaseVMT();
    surfaceVMT->ReleaseVMT();

	*s_bOverridePostProcessingDisable = false;

	/* Cleanup ConVars we have made */
	for( ConVar* var : Util::createdConvars ){
		cvar->UnregisterConCommand(var);
	}
    if( panoramaEngine->AccessUIEngine()->IsValidPanelPointer( GUI::skeleMain ) ){
        GUI::skeleMain->RemoveAndDeleteChildren();
        panorama::IUIPanel* parent = GUI::skeleMain->GetParent();
        if( panoramaEngine->AccessUIEngine()->IsValidPanelPointer( parent ) ) {
            parent->RemoveChild( GUI::skeleMain );
        } else {
            cvar->ConsoleDPrintf("ERROR unloading, panel parent is invalid!\n");
        }
    }

    cvar->ConsoleColorPrintf(ColorRGBA(255, 0, 0), "skeletux unloaded successfully.\n");
#endif
}
#else
//dllmain
BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDll,
    _In_ DWORD fdwReason,
    _In_opt_ LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
	DisableThreadLibraryCalls(hinstDll);
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hinstDll, 0, nullptr);
	return TRUE;
    case DLL_PROCESS_DETACH:
	//if (lpvReserved == nullptr)
	//   return OnDllDetach();
	return TRUE;
    default:
	return TRUE;
    }
}

#endif
