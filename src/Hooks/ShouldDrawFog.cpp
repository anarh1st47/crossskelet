#include "hooks.h"

bool __fastcall Hooks::ShouldDrawFog( void* thisptr ) {
    return NoFog::ShouldDrawFog( thisptr );
}