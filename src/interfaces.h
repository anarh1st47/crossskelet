#pragma once

#include <set>
#include <fstream>

#include "SDK/SDK.h"
#include "Utils/vmt.h"
#include "Utils/util.h"

#ifdef WIN32
#include <Windows.h>
#endif



extern IBaseClientDLL* client;
extern ISurface* surface;
extern IVPanel* panel;
extern IEngineClient* engine;
extern IClientEntityList* entityList;
extern IVDebugOverlay* debugOverlay;
extern IVModelInfo* modelInfo;
extern IVModelRender* modelRender;
extern IClientMode* clientMode;
extern IEngineTrace* trace;
extern IInputSystem* inputSystem;
extern IInputInternal* inputInternal;
extern IMaterialSystem* material;
extern ICvar* cvar;
extern CGlobalVars* globalVars;
extern CEffects* effects;
extern IGameEventManager2* gameEvents;
extern IPhysicsSurfaceProps* physics;
extern CViewRender* viewRender;
extern IPrediction* prediction;
extern IGameMovement* gameMovement;
extern IMoveHelper* moveHelper;
extern ILauncherMgr* launcherMgr;
extern C_CSPlayerResource** csPlayerResource;
extern C_CSGameRules** csGameRules;
extern IEngineVGui* engineVGui;
extern IEngineSound* sound;
extern ILocalize* localize;
extern ICommandLine* commandline;
extern CInput* input;
extern IPanoramaUIEngine* panoramaEngine;

extern VMT* panelVMT;
extern VMT* clientVMT;
extern VMT* clientModeVMT;
extern VMT* inputInternalVMT;
extern VMT* materialVMT;
extern VMT* surfaceVMT;
extern VMT* launcherMgrVMT;
extern VMT* engineVGuiVMT;

extern InitKeyValuesFn InitKeyValues;
extern LoadFromBufferFn LoadFromBuffer;

extern SetNamedSkyBoxFn SetNamedSkyBox;

extern int* nPredictionRandomSeed;
extern CMoveData* g_MoveData;
extern bool* s_bOverridePostProcessingDisable;

namespace Interfaces
{
	void FindInterfaces();
	void DumpInterfaces();
}
