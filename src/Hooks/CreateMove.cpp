#include "hooks.h"
#include "../Hacks/bhop.h"

using CreatemoveFn = bool(__thiscall*)(void*, float, CUserCmd*);

bool __fastcall Hooks::CreateMove(void* thisptr, int, float flInputSampleTime, CUserCmd* cmd){
    clientModeVMT->GetOriginalMethod<CreatemoveFn>((size_t)Indexes::CreateMove)(thisptr, flInputSampleTime, cmd);
    if(cmd&&cmd->command_number){
        if (cvar->FindVar("skele_enable_bhop")->GetBool())
            BunnyHop::OnCreateMove(cmd);
    }
    return false;
}
