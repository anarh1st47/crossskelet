#pragma once

#include <fstream>
#include <set>

#include "SDK/SDK.h"
#include "Utils/util.h"
#include "Utils/vmt.h"

inline IBaseClientDLL* client;
inline ISurface* surface;
inline IVPanel* panel;
inline IEngineClient* engine;
inline IClientEntityList* entityList;
inline IVDebugOverlay* debugOverlay;
inline IVModelInfo* modelInfo;
inline IVModelRender* modelRender;
inline IClientMode* clientMode;
inline IEngineTrace* trace;
inline IInputSystem* inputSystem;
inline IInputInternal* inputInternal;
inline IMaterialSystem* material;
inline ICvar* cvar;
inline CGlobalVars* globalVars;
inline CEffects* effects;
inline IGameEventManager2* gameEvents;
inline IPhysicsSurfaceProps* physics;
inline CViewRender* viewRender;
inline IPrediction* prediction;
inline IGameMovement* gameMovement;
inline IMoveHelper* moveHelper;
inline ILauncherMgr* launcherMgr;
inline C_CSPlayerResource** csPlayerResource;
inline C_CSGameRules** csGameRules;
inline IEngineVGui* engineVGui;
inline IEngineSound* sound;
inline ILocalize* localize;
inline ICommandLine* commandline;
inline CInput* input;
inline IPanoramaUIEngine* panoramaEngine;

inline VMT* panelVMT;
inline VMT* clientVMT;
inline VMT* clientModeVMT;
inline VMT* inputInternalVMT;
inline VMT* materialVMT;
inline VMT* surfaceVMT;
inline VMT* launcherMgrVMT;
inline VMT* engineVGuiVMT;

inline InitKeyValuesFn InitKeyValues;


inline int* nPredictionRandomSeed;
inline CMoveData* g_MoveData;
inline bool* s_bOverridePostProcessingDisable;


inline char
#ifdef WIN32
           *CLIENT_FILE = "client_panorama.dll", *ENGINE_FILE = "engine.dll", *VSTD_FILE = "vstdlib.dll",
	       *VGUI_FILE = "vguimatsurface.dll", *VGUI2_FILE = "vgui2.dll", *MATSYS_FILE = "materialsystem.dll",
	       *DATACACHE_FILE = "datacache.dll", *VPHYS_FILE = "vphysics.dll", *INSYS_FILE = "inputsystem.dll",
	       *LOCALIZE_FILE = "localize.dll", *PANORAMA_FILE = "panorama.dll";
#else
          *CLIENT_FILE = "./csgo/bin/linux64/client_panorama_client.so",
          *ENGINE_FILE = "./bin/linux64/engine_client.so",
          *VSTD_FILE = "vstdlib.dll",
          *VGUI_FILE = "./bin/linux64/vguimatsurface_client.so",
          *VGUI2_FILE = "./bin/linux64/vgui2_client.so",
          *MATSYS_FILE = "./bin/linux64/materialsystem_client.so",
          *DATACACHE_FILE = "datacache.dll",
          *VPHYS_FILE = "./bin/linux64/vphysics_client.so",
          *INSYS_FILE = "./bin/linux64/inputsystem_client.so",
          *LOCALIZE_FILE = "./bin/linux64/localize_client.so",
          *PANORAMA_FILE = "./bin/linux64/panorama_client.so";
#endif

namespace Interfaces {
	void FindInterfaces();
	void DumpInterfaces();
}