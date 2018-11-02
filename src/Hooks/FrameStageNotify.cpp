#include "hooks.h"

void __stdcall Hooks::FrameStageNotify(ClientFrameStage_t stage)
{
	static auto ofunc = clientVMT->GetOriginalMethod<FrameStageNotifyFn>(37);
	ofunc(client, stage);
}
