#pragma once
#ifndef _3RDE_GRAPHICS_DX9_
#define _3RDE_GRAPHICS_DX9_

#ifdef _WIN32

#include <d3d9.h>
#include "../3RDEngine/Interfaces.h"

class gGraphicsDX9;
typedef std::shared_ptr<gGraphicsDX9> SPGRAPHICSDX9;
typedef std::weak_ptr<gGraphicsDX9> WPGRAPHICSDX9;

class gGraphicsDX9 : public IGraphics
{
public:

	class gRenderQueueDX9 : public IRenderQueue
	{
	public:
		gRenderQueueDX9( gGraphicsDX9* graphics );
		~gRenderQueueDX9();

		void add( gRenderElement re );
		void execute();

	protected:
		gRenderQueueDX9() {}
		gRenderQueueDX9(const gRenderQueueDX9&) {}
		gGraphicsDX9* m_pGraphics;
	};


	gGraphicsDX9( SPPLATFORM platform, SPCONFIGURATION configuration );
	~gGraphicsDX9();

	bool initialize();
	SPRENDERQUEUE getRenderQueue();

protected:
	bool finalize();
	SPRENDERQUEUE m_renderQueue;
	LPDIRECT3D9 m_lpD3D9;
	LPDIRECT3DDEVICE9 m_lpDev;
};

#endif

#endif
