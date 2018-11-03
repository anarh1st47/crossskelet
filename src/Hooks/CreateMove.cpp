#include "hooks.h"
#include "../Hacks/bhop.h"

using CreatemoveFn = bool(__thiscall*)(void*, float, CUserCmd*);

bool __fastcall Hooks::CreateMove(void* thisptr, int, float flInputSampleTime, CUserCmd* cmd){
    //if (cvar->FindVar("skele_enable_bhop")->GetBool())
		BunnyHop::OnCreateMove(cmd);
	cvar->ConsolePrintf("hooked cm\n");
    return clientModeVMT->GetOriginalMethod<CreatemoveFn>((size_t)Indexes::CreateMove)(thisptr, flInputSampleTime, cmd);
}
