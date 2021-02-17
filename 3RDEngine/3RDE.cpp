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
#ifdef _WIN32
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

_3RDE_API_ WP3RDENGINE gwpEngine;

SP3RDENGINE I3RDEngine::get()
{
    return Get3RDEngine();
}

SP3RDENGINE Get3RDEngine()
{
    SP3RDENGINE spEngine;
    if (gwpEngine.expired())
    {
        spEngine = std::make_shared<g3RDEngine>();
        gwpEngine = spEngine;
    }
    return gwpEngine.lock();
}