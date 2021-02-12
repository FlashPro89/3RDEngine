#include "../include/3RDE.h"
#include "3RDEngine.h"

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

using namespace std;

_3RDE_API_ SP3RDENGINE Create3RDEngine() // run once per application
{	
    SP3RDENGINE spEngine = shared_ptr<g3RDEngine>(new g3RDEngine());

	auto use = spEngine.use_count(); // test
	auto unique = spEngine.unique(); // test

	return spEngine;
}

_3RDE_API_ SP3RDENGINE Get3RDEngine()
{
    SP3RDENGINE p;
    return p;
    //return gspEngine;
}