#pragma once

#ifndef WIN32
#define __thiscall
#endif

#define FCVAR_NONE 0

#define FCVAR_UNREGISTERED (1 << 0)
#define FCVAR_DEVELOPMENTONLY (1 << 1)
#define FCVAR_GAMEDLL (1 << 2)
#define FCVAR_CLIENTDLL (1 << 3)
#define FCVAR_HIDDEN (1 << 4)

#define FCVAR_PROTECTED (1 << 5)
#define FCVAR_SPONLY (1 << 6)
#define FCVAR_ARCHIVE (1 << 7)
#define FCVAR_NOTIFY (1 << 8)
#define FCVAR_USERINFO (1 << 9)
#define FCVAR_CHEAT (1 << 14)

#define FCVAR_PRINTABLEONLY (1 << 10)
#define FCVAR_UNLOGGED (1 << 11)
#define FCVAR_NEVER_AS_STRING (1 << 12)
#define FCVAR_RELEASE (1 << 19)

#define FCVAR_REPLICATED (1 << 13)
#define FCVAR_DEMO (1 << 16)
#define FCVAR_DONTRECORD (1 << 17)

#define FCVAR_NOT_CONNECTED (1 << 22)
#define FCVAR_ARCHIVE_XBOX (1 << 24)
#define FCVAR_SERVER_CAN_EXECUTE (1 << 28)
#define FCVAR_SERVER_CANNOT_QUERY (1 << 29)
#define FCVAR_CLIENTCMD_CAN_EXECUTE (1 << 30)

// Not really ideal but there's already a Color struct used by the GUI.
struct ColorRGBA
{
    ColorRGBA(unsigned char Red, unsigned char Green, unsigned char Blue, unsigned char Alpha = 255)
    {
	RGBA[0] = Red;
	RGBA[1] = Green;
	RGBA[2] = Blue;
	RGBA[3] = Alpha;
    }

    unsigned char RGBA[4];
};

enum ConVarType_t : int
{
    INTEGER = 2,
    FLOAT_e = 4,
    STRING = 15,
};

enum class CvarIndexes : char
{
    GetFloat =
#ifdef WIN32
	12
#else
	15
#endif
	,
	GetInt,
    SetValueStr,
    SetValueFloat,
    SetValueInt,
    SetValueColor
};
class ConVar
{
public:
    float GetFloat()
    {
	return fValue;
    }

    int GetInt()
    {
	return iValue;
    }

    void SetValue(const char* value)
    {
	typedef void (__thiscall*oSetValue)(void*, const char*);
	return getvfunc<oSetValue>(this, (int)CvarIndexes::SetValueStr)(this, value);
    }

    void SetValue(float value)
    {
	typedef void(__thiscall * oSetValue)(void*, float);
	return getvfunc<oSetValue>(this, (int)CvarIndexes::SetValueFloat)(this, value);
    }

    void SetValue(int value)
    {
	typedef void(__thiscall * oSetValue)(void*, int);
	return getvfunc<oSetValue>(this, (int)CvarIndexes::SetValueInt)(this, value);
    }

    void SetValue(Color value)
    {
	typedef void(__thiscall * oSetValue)(void*, Color);
	return getvfunc<oSetValue>(this, (int)CvarIndexes::SetValueColor)(this, value);
    }

    void* vtable;
    ConVar* next; //0x0008
    bool isRegistered; //0x0010
    char pad_0011[7]; //0x0011
    char* name; //0x0018
    char* description; //0x0020
    int32_t flags; //0x0028
    char pad_0x0018[0x4]; //0x0018
    void* s_pAccessor;
    ConVar* pParent;
    char* strDefault;
    char* strValue;
    ConVarType_t type;
    float fValue;
    int32_t iValue;
    bool hasMin;
    char _padmin[3];
    float minVal;
    bool hasMax;
    char _padmax[3];
    float maxVal;
    void* fnChangeCallback;
};

class ConCommandBase;

class ICvar : public IAppSystem
{
public:
    virtual void* AllocateDLLIdentifier() = 0; // 9
    virtual void RegisterConCommand(ConCommandBase* pCommandBase) = 0; //10
    virtual void UnregisterConCommand(ConCommandBase* pCommandBase) = 0;
    virtual void UnregisterConCommands(int id) = 0;
    virtual const char* GetCommandLineValue(const char* pVariableName) = 0;
    virtual ConCommandBase* FindCommandBase(const char* name) = 0;
    virtual const ConCommandBase* FindCommandBase(const char* name) const = 0;
    virtual ConVar* FindVar(const char* var_name) = 0; //16
    virtual const ConVar* FindVar(const char* var_name) const = 0;
    virtual void* FindCommand(const char* name) = 0;
    virtual const void* FindCommand(const char* name) const = 0;
    virtual void InstallGlobalChangeCallback(void* callback) = 0;
    virtual void RemoveGlobalChangeCallback(void* callback) = 0;
    virtual void CallGlobalChangeCallbacks(ConVar* var, const char* pOldString, float flOldValue) = 0;
    virtual void InstallConsoleDisplayFunc(void* pDisplayFunc) = 0;
    virtual void RemoveConsoleDisplayFunc(void* pDisplayFunc) = 0;
    virtual void ConsoleColorPrintf(ColorRGBA clr, const char* pFormat, ...) const = 0;
    virtual void ConsolePrintf(const char* pFormat, ...) const = 0;
    virtual void ConsoleDPrintf(const char* pFormat, ...) const = 0;
    virtual void RevertFlaggedConVars(int nFlag) = 0;
};
