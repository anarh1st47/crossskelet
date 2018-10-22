#include "hooks.h"

using CreatemoveFn = void(__stdcall*)(int, float, bool);

void __stdcall Hooks::CreateMove(int sequence_number, float input_sample_frametime, bool active) {
    clientVMT->GetOriginalMethod<CreatemoveFn>(22)(sequence_number, input_sample_frametime, active);
}