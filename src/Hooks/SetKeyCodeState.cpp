#include "hooks.h"

void __thiscall Hooks::SetKeyCodeState(void* thisptr, ButtonCode_t code, bool bPressed)
{
	if( !bPressed )
		return inputInternalVMT->GetOriginalMethod<SetKeyCodeStateFn>((int)Indexes::SetKeyCodeState)(thisptr, code, bPressed);

	switch( code ){
		case ButtonCode_t::KEY_INSERT:
            GUI::ToggleUI();
            break;
		case ButtonCode_t::KEY_END:
			for( int i = 0; i < 1024; i++ ){
				panorama::IUIPanel *panel = panorama::panelArray->slots[i].panel;
				if( panoramaEngine->AccessUIEngine()->IsValidPanelPointer(panel) ){
					cvar->ConsoleDPrintf("Panel %d: (%s)\n", i, panel->GetID());
				}
			}
			break;
        default:
            break;
	}
}
