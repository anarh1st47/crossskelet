#pragma once

#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "../hooker.h"
#include "../GUI/gui.h"

enum class Indexes : int // probably we should add +1(2nd destructor) for linux but im not sure
{
#ifdef WIN32
    SetKeyCodeState = 91,
	CreateMove = 24,
	PanelTraverse = 41
#else
    SetKeyCodeState = 92,
    CreateMove = 25,
    PanelTraverse = 42
#endif

};


using FrameStageNotifyFn = void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t);
using SetKeyCodeStateFn = void(__thiscall*)(void*, ButtonCode_t, bool);
using BeginFrameFn = void(__thiscall*)(void*, float);
using PaintFn =  void(__thiscall*)(void*, PaintMode_t);
using LockCursorFn = void(__thiscall*)(void*);

struct Hooks // with namespace we cannot use thiscall
{
	static void __stdcall FrameStageNotify(ClientFrameStage_t stage);
	static void __thiscall SetKeyCodeState(void* thisptr, ButtonCode_t code, bool bPressed);
	static void __fastcall BeginFrame(void* thisptr, int, float frameTime);
	static bool __fastcall ShouldDrawFog(void* thisptr);

	static void __fastcall LockCursor(void* thisptr);

	static void __fastcall Paint(void* thisptr, int, PaintMode_t mode);


	static bool __thiscall CreateMove(void* thisptr, float flInputSampleTime, CUserCmd* cmd);
	static void __thiscall PaintTraverse(void*, VPANEL p, bool forceRepaint, bool allowForce);
};

namespace CreateMove
{
	inline bool sendPacket; // probably i'll grabb it
}

namespace OverrideView
{
	inline float currentFOV;
}

namespace SetKeyCodeState
{
	inline bool shouldListen;
	inline ButtonCode_t* keyOutput;
}
