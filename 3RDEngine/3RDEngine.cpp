#include "../include/3RDEngine.h"
#include "3RDEngineImpl.h"

/*

#ifdef _WIN32
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

#endif
*/

using namespace std;

_3RDE_API_ SP3RDENGINE Create3RDEngine()
{	
	SP3RDENGINE spEngine = shared_ptr<g3RDEngineImpl>(new g3RDEngineImpl);

	auto use = spEngine.use_count();
	auto unicue = spEngine.unique();

	return spEngine;
}