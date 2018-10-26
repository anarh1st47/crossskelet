#pragma once

#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "../hooker.h"
#include "../GUI/gui.h"
#include "../Hacks/NoFog.h"

enum class Indexes : int
{
#ifdef WIN32
    SetKeyCodeState = 91, // linux 92
	CreateMove = 24, // linux 25
	PanelTraverse = 41 // linux 42
#else
    SetKeyCodeState = 92,
    CreateMove = 25,
    PanelTraverse = 42
#endif

};


//typedef void (*FrameStageNotifyFn) (void*, ClientFrameStage_t);
using FrameStageNotifyFn = void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t);
//typedef void (*SetKeyCodeStateFn) (void*, ButtonCode_t, bool);
using SetKeyCodeStateFn = void(__thiscall*)(void*, ButtonCode_t, bool);
//typedef void (*BeginFrameFn) (void*, float);
using BeginFrameFn = void(__thiscall*)(void*, float);
//typedef void (*PaintFn) (void*, PaintMode_t);
using PaintFn =  void(__thiscall*)(void*, PaintMode_t);
//typedef void (*LockCursorFn) (void*);
using LockCursorFn = void(__thiscall*)(void*);

struct Hooks // with namespace we cannot use thiscall
{
	static void __stdcall FrameStageNotify(ClientFrameStage_t stage);
	static void __thiscall SetKeyCodeState(void* thisptr, ButtonCode_t code, bool bPressed);
	static void __fastcall BeginFrame(void* thisptr, int, float frameTime);
	static bool __fastcall ShouldDrawFog(void* thisptr);

	static void __fastcall LockCursor(void* thisptr);

	static void __fastcall Paint(void* thisptr, int, PaintMode_t mode);


	static bool __fastcall CreateMove(void* thisptr, int, float flInputSampleTime, CUserCmd* cmd);
	static void __thiscall PaintTraverse(void*, VPANEL p, bool forceRepaint, bool allowForce);
};

namespace CreateMove
{
	inline bool sendPacket;
	inline QAngle lastTickViewAngles;
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
