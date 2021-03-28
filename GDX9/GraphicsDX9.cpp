#ifdef _WIN32
#include <Windows.h>
#include "GraphicsDX9.h"

extern lpfnThrowException fnThrowException = 0;

// ------------------------------------
//
//		*** class gGraphicsDX9::gRenderQueueDX9 ***
//
// ------------------------------------

gGraphicsDX9::gRenderQueueDX9::gRenderQueueDX9( gGraphicsDX9* graphics ) 
	: m_pGraphics(graphics)
{
}

gGraphicsDX9::gRenderQueueDX9::~gRenderQueueDX9()
{

}

void gGraphicsDX9::gRenderQueueDX9::add(gRenderElement re)
{

}

void gGraphicsDX9::gRenderQueueDX9::execute()
{
	//MessageBox(0, "Test from dynamic lib", "Boooo", MB_OK);

    auto cpDevice = this->m_pGraphics->getDevice();
    HRESULT hr;
    hr = cpDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0x7F7F7F7F, 1.0f, 0);
    hr = cpDevice->BeginScene();
    hr = cpDevice->EndScene();
    hr = cpDevice->Present(0, 0, 0, 0);
}

// ------------------------------------
//
//		*** class gGraphicsDX9 ***
//
// ------------------------------------

gGraphicsDX9::gGraphicsDX9(SPPLATFORM platform, SPCONFIGURATION configuration)
{
	m_renderQueue = IGraphics::SPRENDERQUEUE(new gRenderQueueDX9(this));
    m_wpPlatform = platform;
    m_wpConfiguration = configuration;
}

gGraphicsDX9::~gGraphicsDX9()
{
	MessageBox(0, "gGraphicsDX9", "Destructor called!", MB_OK);
	finalize();
}

ComPtr<IDirect3DDevice9> gGraphicsDX9::getDevice()
{
    return m_cpD3DDev;
}

bool gGraphicsDX9::initialize()
{
    //-------------------------------------------------------------
    // Check first window in platform system is initialized
    //-------------------------------------------------------------
    ECHECK(!m_wpPlatform.expired(), "Initialize platform before graphics!");

    //-------------------------------------------------------------
    // Create D3D9 Main Object
    //-------------------------------------------------------------
    m_cpD3D9 = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface
    if (m_cpD3D9 == nullptr)
        return false;

    //-------------------------------------------------------------
    // Create Device
    //-------------------------------------------------------------
    D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information
    HWND hWnd = static_cast<HWND>( m_wpPlatform.lock()->getWindow()->getWindowParameters().handle );
    LPDIRECT3DDEVICE9 lpDevice = nullptr;

    ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
    d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
    d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    // create a device class using this information and information from the d3dpp stuct
    HRESULT hr = m_cpD3D9->CreateDevice( D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &lpDevice);

    if (FAILED(hr))
        return false;

    m_cpD3DDev = lpDevice;
    return true;
}

IGraphics::SPRENDERQUEUE gGraphicsDX9::getRenderQueue()
{
	return m_renderQueue;
}

bool gGraphicsDX9::finalize()
{
	return true;
}

#endif
