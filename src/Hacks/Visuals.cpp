#include "../Hooks/hooks.h"
#include "Visuals.h"

void Visuals::PaintTraverse() {
    if (!engine->IsInGame()) 
        return;
    for (auto i = 0; i < entityList->GetHighestEntityIndex(); i++) {
        auto ent = entityList->GetClientEntity(i);
        if (!ent)
            continue;

        auto client = ent->GetClientClass();

        if (client->m_ClassID != EClassIds::CChicken)
            continue;

        Vector out;
        debugOverlay->ScreenPosition(ent->GetVecOrigin(), out);
        Draw::Text(out.x, out.y, "chicken", Settings::Visuals::pFont, Color(255, 0,0,255));
    }
}
