#pragma once
#ifndef _3RDE_GRAPHICS_DX9_
#define _3RDE_GRAPHICS_DX9_

#ifdef _WIN32

#define MAXFRAMESNUM 2
#define MAXTEXTURESNUM 4096
#define MAXTEXTUREUPLOADSNUM 4096
#define MAXVERTEXBUFFERSNUM 256
#define MAXINDEXBUFFERSNUM 2048
#define MAXCONSTANTBUFFERSNUM 2048
#define MAXPIPELINESTATESNUM 256
#define MAXROOTSIGNATURESNUM 256

#include "../3RDEngine/Interfaces.h"

#include <wrl.h>
//#include <d3d12.h>
#include "d3dx12.h"
#include <d3d12sdklayers.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;
using Microsoft::WRL::WeakRef;

class gGraphicsDX12;
typedef std::shared_ptr<gGraphicsDX12> SPGRAPHICSDX12;
typedef std::weak_ptr<gGraphicsDX12> WPGRAPHICSDX12;

//пока такой костыль =)
extern lpfnThrowException fnThrowException; //for exception export to engine
#define ETHROW(msg) fnThrowException( __FUNCSIG__, __FILE__, __LINE__, (msg) )
#define ECHECK(e,msg) if(!e)fnThrowException( __FUNCSIG__, __FILE__, __LINE__, (msg) )
#define ECHECKHR(hr,msg) if(FAILED(hr))fnThrowException( __FUNCSIG__, __FILE__, __LINE__, (msg) )

class gGraphicsDX12 : public IGraphics
{
public:

	class gRenderQueueDX12 : public IRenderQueue
	{
	public:
		gRenderQueueDX12(gGraphicsDX12* graphics);
		~gRenderQueueDX12();

		void add(gRenderElement re);
		void execute();

	protected:
		gRenderQueueDX12() {}
		gRenderQueueDX12(const gRenderQueueDX12&) {}
		gGraphicsDX12* m_pGraphics;
	};


	gGraphicsDX12(SPPLATFORM platform, SPCONFIGURATION configuration);
	~gGraphicsDX12();

	bool initialize();
	SPRENDERQUEUE getRenderQueue();

protected:
	void GetHardwareAdapter(IDXGIFactory1* pFactory, 
		IDXGIAdapter1** ppAdapter,
		bool requestHighPerformanceAdapter);
	UINT createDebugLayerIfNeeded();
	bool createCommandQueue();
	bool createDevice(ComPtr<IDXGIFactory4> factory);
	bool createSwapChain(ComPtr<IDXGIFactory4> factory);
	bool createDescriptorHeaps();
	bool createDepthStensil();
	bool createFrameResources();
	bool createDefaultRootSignature();
	bool createDefaultPipelineState();
	bool createCommandLists();
	bool createFence();

	void WaitForPreviousFrame();
	void PopulateCommandList();

	bool finalize();
	SPRENDERQUEUE m_renderQueue;
	WPPLATFORM m_wpPlatform;
	WPCONFIGURATION m_wpConfiguration;


	//settings
	bool m_useWARPDevice; //default : false

	D3D_DRIVER_TYPE                     m_driverType = D3D_DRIVER_TYPE_NULL;
	ComPtr< ID3D12Device >              m_cpD3DDev;
	ComPtr< ID3D12CommandQueue >        m_cpCommQueue;
	ComPtr< ID3D12CommandAllocator >    m_cpCommAllocator;
	ComPtr< ID3D12GraphicsCommandList > m_cpCommList;
	ComPtr< IDXGISwapChain3 >           m_cpSwapChain;
	ComPtr< ID3D12DescriptorHeap >      m_cpRTVHeap;
	ComPtr< ID3D12DescriptorHeap >      m_cpCBVSrvHeap;
	ComPtr< ID3D12DescriptorHeap >      m_cpDSVHeap;
	ComPtr< ID3D12DescriptorHeap >      m_cpSRVHeap;
	ComPtr< ID3D12RootSignature >       m_cpRootSignature[MAXROOTSIGNATURESNUM];
	ComPtr< ID3D12PipelineState >       m_cpPipelineState[MAXPIPELINESTATESNUM];
	ComPtr< ID3D12Resource >            m_cpRenderTargets[MAXFRAMESNUM];
	ComPtr< ID3D12Resource >            m_cpDepthStencil;
	ComPtr< ID3D12Resource >            m_textures[MAXTEXTURESNUM];
	ComPtr< ID3D12Resource >            m_textureUploads[MAXTEXTURESNUM];

	// App resources
	ComPtr<ID3D12Resource> m_cpVertexBuffers[MAXVERTEXBUFFERSNUM];
	D3D12_VERTEX_BUFFER_VIEW m_cpVertexBufferViews;
	ComPtr<ID3D12Resource> m_cpIndexBuffers[MAXINDEXBUFFERSNUM];
	D3D12_VERTEX_BUFFER_VIEW m_cpIndexBufferViews[MAXINDEXBUFFERSNUM];

	// Syncronization objects
	UINT m_frameIndex = 0;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_cpFence;
	UINT64 m_fenceValue;

	// Rendering region
	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_scissorRect;

	// Descriptor heaps
	size_t m_rtvDescriptorSize;
	size_t m_dsvDescriptorSize;
	size_t m_srvDescriptorSize;
};

#endif

#endif
