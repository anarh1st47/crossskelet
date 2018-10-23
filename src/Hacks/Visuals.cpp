#include "Visuals.h"

void Visuals::PaintTraverse() {
    if (!engine->IsInGame()) return;
    for (auto i = 0; i < engine->GetMaxClients(); i++) {
		auto ent = entityList->GetClientEntity(i);
		if (!ent)
			continue;

		ClientClass* client = ent->GetClientClass();

		if (client->m_ClassID == EClassIds::CCSPlayer)
		{
			C_BasePlayer* player = (C_BasePlayer*)ent;

			if (player->GetDormant() || !player->GetAlive())
				continue;
			auto orig = player->GetVecOrigin();
			Vector out;
			debugOverlay->ScreenPosition(orig, out);
			Draw::Rectangle(out.x, out.y, out.x + 10, out.y + 20, Color(255, 0,0,255));
		}
	}
}