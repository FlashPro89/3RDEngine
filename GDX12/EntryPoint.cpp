#ifdef _WIN32

#pragma comment( lib , "d3d12.lib" )
#pragma comment( lib , "d3dcompiler.lib" )
#pragma comment( lib , "dxguid.lib" )
#pragma comment( lib , "dxgi.lib" )

#include <Windows.h>
#include "GraphicsDX12.h"
#include "gdx12.h"

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

GDX12_API_C IGraphics* createGraphicsDX12(SPPLATFORM platform, SPCONFIGURATION configuration)
{
    auto ret = new gGraphicsDX12(platform, configuration);
    return ret;
}

GDX12_API_C void destroyGraphicsDX12(IGraphics* p)
{
    if (p) delete p;
    p = 0;
}

GDX12_API_C void setThrowExceptionFunction(void* fn)
{
    fnThrowException = static_cast<lpfnThrowException>(fn);
}


#endif