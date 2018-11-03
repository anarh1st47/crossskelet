#include "bhop.h"
#include "../Hooks/hooks.h"

//thanks MarkHC
void BunnyHop::OnCreateMove(CUserCmd* cmd)
{
	auto localplayer = (C_BasePlayer*)entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!localplayer)
		return;
    static auto jumped_last_tick = false;
    static auto should_fake_jump = false;

    if (!jumped_last_tick && should_fake_jump)
    {
		should_fake_jump = false;
		cmd->buttons |= IN_JUMP;
    }
    else if (cmd->buttons & IN_JUMP)
    {
		if (localplayer->GetFlags() & FL_ONGROUND)
		{
			jumped_last_tick = true;
			should_fake_jump = true;
		}
		else
		{
			cmd->buttons &= ~IN_JUMP;
			jumped_last_tick = false;
		}
    }
    else
    {
		jumped_last_tick = false;
		should_fake_jump = false;
    }
}
