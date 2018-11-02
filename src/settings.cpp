#include "settings.h"

bool Settings::RegisterConVars()
{
    bool convarsOK = true;
    convarsOK &= Util::RegisterConVar("skele_enable_bhop", "1");
    convarsOK &= Util::RegisterConVar("skele_enable_esp", "1");

    return convarsOK;
}