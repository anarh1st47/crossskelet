#include "hooks.h"

#include "../Hacks/Visuals.h"

using PaintTraverseFn = void(__thiscall*)(IVPanel*, VPANEL, bool, bool);

void __stdcall Hooks::PaintTraverse(VPANEL p, bool forceRepaint, bool allowForce)
{
    static auto panelId = VPANEL{ 0 };
    static auto oPaintTraverse = panelVMT->GetOriginalMethod<PaintTraverseFn>((int)Indexes::PanelTraverse);
    oPaintTraverse(panel, p, forceRepaint, allowForce);
    if (!panelId)
    {
		const auto panelName = panel->GetName(p);
		if (!strcmp(panelName, "FocusOverlayPanel"))
			panelId = p;
	}
	else if (panelId == p) {
		if (!Visuals::pFont)
			Visuals::pFont = Draw::CreateFontt("tahoma", 15);
		Visuals::PaintTraverse();
		Draw::Text(100, 100, "CrossSkelet", Visuals::pFont, Color(255, 0, 0, 255));
	}
}