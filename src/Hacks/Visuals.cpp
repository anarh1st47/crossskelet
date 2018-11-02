#include "../Hooks/hooks.h"
#include "../settings.h"

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

		auto orig = ent->GetVecOrigin();
		Vector out;
		debugOverlay->ScreenPosition(orig, out);
		Draw::Text(out.x, out.y, "chicken", Settings::Visuals::pFont, Color(255, 0,0,255));
	}
}
