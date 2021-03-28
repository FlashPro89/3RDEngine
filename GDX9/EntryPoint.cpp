#ifdef _WIN32

#pragma comment( lib , "d3d9.lib" )
#pragma comment( lib , "d3dx9.lib" )

#include <Windows.h>
#include "GraphicsDX9.h"
#include "gdx9.h"

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

GDX9_API_C IGraphics* createGraphicsDX9(SPPLATFORM platform, SPCONFIGURATION configuration)
{
    auto ret = new gGraphicsDX9(platform, configuration);
    return ret;
}

GDX9_API_C void destroyGraphicsDX9(IGraphics* p)
{
    if (p) delete p;
    p = 0;
}

GDX9_API_C void setThrowExceptionFunction(void* fn)
{
    fnThrowException = static_cast<lpfnThrowException>(fn);
}

#endif