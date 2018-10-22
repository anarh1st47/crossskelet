#include "hooks.h"

using CreatemoveFn = bool(__thiscall*)(void*, float, CUserCmd*);

bool __fastcall Hooks::CreateMove(void* thisptr, int, float flInputSampleTime, CUserCmd* cmd){
    return clientModeVMT->GetOriginalMethod<CreatemoveFn>((size_t)Indexes::CreateMove)(thisptr, flInputSampleTime, cmd);
}