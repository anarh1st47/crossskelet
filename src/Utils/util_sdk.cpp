#include "util_sdk.h"

bool Util::RegisterConVar(const char* name, const char* value, uint32_t flags, const char* helpString, bool bMin, float fMin, bool bMax, float fMax) {
    auto command = new ConVar(name, value, FCVAR_CLIENTDLL, helpString);
    cvar->RegisterConCommand(command);
    Util::createdConvars.push_back(command);
    cvar->ConsoleDPrintf("Registered convar %s @ %p\n", command->m_pszName, (void*)command);
    return true;
}