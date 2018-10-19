#pragma once

#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "../hooker.h"
#include "../GUI/gui.h"
#include "../Hacks/NoFog.h"


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


namespace Hooks
{
	void __stdcall FrameStageNotify(ClientFrameStage_t stage);
	void SetKeyCodeState(void* thisptr, ButtonCode_t code, bool bPressed);
	void BeginFrame(void* thisptr, float frameTime);
	bool ShouldDrawFog(void* thisptr);

	void LockCursor(void* thisptr);

	void Paint(void* thisptr, PaintMode_t mode);
}

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
