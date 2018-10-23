#include "hooks.h"

using PaintTraverseFn = void(__thiscall*)(IVPanel*, VPANEL, bool, bool);

void __stdcall Hooks::PaintTraverse(VPANEL p, bool forceRepaint, bool allowForce)
{
    static auto pFont = Draw::CreateFontA("tahoma", 15);
    auto oPaintTraverse = panelVMT->GetOriginalMethod<PaintTraverseFn>((int)Indexes::PanelTraverse);
    oPaintTraverse(panel, p, forceRepaint, allowForce);
    //Draw::Text(100, 100, "CrossSkelet", pFont, Color(255, 0, 0));
    //Draw::FilledRectangle(100, 100, 200, 200, Color(0, 255, 0));
}