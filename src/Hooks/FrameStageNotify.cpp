#include "hooks.h"
#include "../Hacks/SkyBox.h"

void __stdcall Hooks::FrameStageNotify(ClientFrameStage_t stage)
{
	SkyBox::FrameStageNotify( stage );

	static auto ofunc = clientVMT->GetOriginalMethod<FrameStageNotifyFn>(37);
	ofunc(client, stage);
}
