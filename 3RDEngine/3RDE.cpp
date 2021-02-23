#include "../include/3RDE.h"
#include "Engine.h"

//#include "../lua/lua.hpp"

#ifdef _DEBUG
#ifdef _WIN64
#pragma comment ( lib, "../lib/x64/lua_d.lib" )
#else
#pragma comment ( lib, "../lib/x86/lua_d.lib" )
#endif
#else
#ifdef _WIN64
#pragma comment ( lib, "../lib/x64/lua.lib" )
#else
#pragma comment ( lib, "../lib/x86/lua.lib" )
#endif
#endif

/*
#include <Windows.h>

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
*/

using namespace std;

SP3RDENGINE I3RDEngine::get()
{  
    static WP3RDENGINE s_wpEngine;

    SP3RDENGINE spEngine;
    if ( s_wpEngine.expired() )
    {
        spEngine = std::make_shared<g3RDEngine>();
        s_wpEngine = spEngine;
    }
    return 	s_wpEngine.lock();
}

_3RDE_API_ SP3RDENGINE Get3RDEngine()
{
    return I3RDEngine::get();
}