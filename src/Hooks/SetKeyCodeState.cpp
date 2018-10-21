#include "hooks.h"

void __fastcall Hooks::SetKeyCodeState(void* thisptr, int, ButtonCode_t code, bool bPressed)
{
	if( !bPressed )
	return inputInternalVMT->GetOriginalMethod<SetKeyCodeStateFn>((int)Indexes::SetKeyCodeState)(thisptr, code, bPressed);

	switch( code ){
		case ButtonCode_t::KEY_INSERT:
            GUI::ToggleUI();
            break;
		case ButtonCode_t::KEY_END:

			break;
        default:
            break;
	}
}
