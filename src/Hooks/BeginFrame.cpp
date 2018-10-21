#include "hooks.h"
#include "../Hacks/DisablePostProcessing.h"

void __fastcall Hooks::BeginFrame(void* thisptr, int, float frameTime)
{
	DisablePostProcessing::BeginFrame();

	return materialVMT->GetOriginalMethod<BeginFrameFn>(42)(thisptr, frameTime);
}
