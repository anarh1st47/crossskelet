#include "Visuals.h"

void Visuals::PaintTraverse() {
    if (!engine->IsInGame()) 
		return;
    for (auto i = 0; i < engine->GetMaxClients(); i++) {
		auto ent = entityList->GetClientEntity(i);
		if (!ent)
			continue;

		auto client = ent->GetClientClass();

		if (client->m_ClassID != EClassIds::CChicken)
		    continue;
		
		auto player = reinterpret_cast<C_BasePlayer*>(ent);

			
		if (player->GetDormant() || !player->GetAlive())
			continue;
		auto orig = player->GetVecOrigin();
		Vector out;
		debugOverlay->ScreenPosition(orig, out);
		Draw::Text(out.x, out.y, "nigga", pFont, Color(255, 0,0,255));
	}
}