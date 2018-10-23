#pragma once
const int GETNAMEVPANELINDEX = 
#ifdef WIN32
36
#else
37
#endif
;

#ifndef WIN32
#define __thiscall
#endif

class IVPanel
{
public:
	const char *GetName(VPANEL vguiPanel)
	{
	    typedef const char*(__thiscall * oGetName)(void*, VPANEL);
	    return getvfunc<oGetName>(this, GETNAMEVPANELINDEX)(this, vguiPanel);
	}
};
