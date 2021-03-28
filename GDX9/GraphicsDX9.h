#pragma once
#ifndef _3RDE_GRAPHICS_DX9_
#define _3RDE_GRAPHICS_DX9_

#ifdef _WIN32

#include <d3d9.h>
#include <wrl.h>
#include "../3RDEngine/Interfaces.h"

using Microsoft::WRL::ComPtr;
using Microsoft::WRL::WeakRef;

class gGraphicsDX9;
typedef std::shared_ptr<gGraphicsDX9> SPGRAPHICSDX9;
typedef std::weak_ptr<gGraphicsDX9> WPGRAPHICSDX9;

//пока такой костыль =)
extern lpfnThrowException fnThrowException; //for exception export to engine
#define ETHROW(msg) fnThrowException( __FUNCSIG__, __FILE__, __LINE__, (msg) )
#define ECHECK(e,msg) if(!e)fnThrowException( __FUNCSIG__, __FILE__, __LINE__, (msg) )
#define ECHECKHR(hr,msg) if(FAILED(hr))fnThrowException( __FUNCSIG__, __FILE__, __LINE__, (msg) )

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
	WPPLATFORM m_wpPlatform;
	WPCONFIGURATION m_wpConfiguration;

	ComPtr< IDirect3D9 > m_cpD3D9;
	ComPtr< IDirect3DDevice9 >  m_cpD3DDev;
};

#endif

#endif
