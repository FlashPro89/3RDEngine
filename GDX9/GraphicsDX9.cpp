#ifdef _WIN32
#include <Windows.h>
#include "GraphicsDX9.h"

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
	MessageBox(0, "Test from dynamic lib", "Boooo", MB_OK);
}

// ------------------------------------
//
//		*** class gGraphicsDX9 ***
//
// ------------------------------------

gGraphicsDX9::gGraphicsDX9(SPPLATFORM platform, SPCONFIGURATION configuration) :
	m_lpD3D9( NULL),
	m_lpDev( NULL)
{
	m_renderQueue = IGraphics::SPRENDERQUEUE(new gRenderQueueDX9(this));

}

gGraphicsDX9::~gGraphicsDX9()
{
	MessageBox(0, "gGraphicsDX9", "Destructor called!", MB_OK);
	finalize();
}

bool gGraphicsDX9::initialize()
{
	return true;
}

IGraphics::SPRENDERQUEUE gGraphicsDX9::getRenderQueue()
{
	return m_renderQueue;
}

bool gGraphicsDX9::finalize()
{
	if (m_lpDev)
		m_lpDev->Release();
	if (m_lpD3D9)
		m_lpD3D9->Release();
	return true;
}

#endif
