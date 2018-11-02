#include "hooks.h"

void __fastcall Hooks::BeginFrame(void* thisptr, int, float frameTime)
{
	return materialVMT->GetOriginalMethod<BeginFrameFn>(42)(thisptr, frameTime);
}
