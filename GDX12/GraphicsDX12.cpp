#ifdef _WIN32
#include <Windows.h>
#include "GraphicsDX12.h"

extern lpfnThrowException fnThrowException = 0;

// ------------------------------------
//
//		*** class gGraphicsDX12::gRenderQueueDX12 ***
//
// ------------------------------------

gGraphicsDX12::gRenderQueueDX12::gRenderQueueDX12( gGraphicsDX12* graphics ) 
	: m_pGraphics(graphics)
{
}

gGraphicsDX12::gRenderQueueDX12::~gRenderQueueDX12()
{

}

void gGraphicsDX12::gRenderQueueDX12::add(gRenderElement re)
{

}

void gGraphicsDX12::gRenderQueueDX12::execute()
{
	MessageBox(0, "Test from dynamic lib", "Boooo", MB_OK);
}

// ------------------------------------
//
//		*** class gGraphicsDX12 ***
//
// ------------------------------------

gGraphicsDX12::gGraphicsDX12(SPPLATFORM platform, SPCONFIGURATION configuration) :
    m_useWARPDevice( false ), 
    m_rtvDescriptorSize(0),
    m_dsvDescriptorSize(0),
    m_srvDescriptorSize(0)
{
    auto wParams = platform->getWindow()->getWindowParameters();

	m_renderQueue = IGraphics::SPRENDERQUEUE(new gRenderQueueDX12(this));
    m_viewport.TopLeftX = m_viewport.TopLeftY = 0;
    m_viewport.Width = wParams.width;
    m_viewport.Height = wParams.height;

    m_scissorRect.left = m_scissorRect.top = 0;
    m_scissorRect.bottom = wParams.width;
    m_scissorRect.right = wParams.height;

    m_wHandle = static_cast<HWND>(platform->getWindow()->getWindowHanlde());
}

gGraphicsDX12::~gGraphicsDX12()
{
	MessageBox(0, "gGraphicsDX12", "Destructor called!", MB_OK);
	finalize();
}

bool gGraphicsDX12::initialize()
{
    
    //-------------------------------------------------------------
    // Use debug layer if needeed
    //-------------------------------------------------------------
    UINT dxgiFactoryFlags = createDebugLayerIfNeeded();

    //-------------------------------------------------------------
    // Create DXGI factory
    //-------------------------------------------------------------
    ComPtr < IDXGIFactory4 > factory;
    ECHECKHR(CreateDXGIFactory1(IID_PPV_ARGS(&factory)), // удалить если все скомпилится
        "Cannot create DXGI Factory1!");
    ECHECKHR(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)),
        "Cannot create DXGI Factory2!");
    
    //-------------------------------------------------------------
    // Create d3d12 device
    //-------------------------------------------------------------
    ECHECK(createDevice(factory), "Cannot create D3D12 device!");

    //-------------------------------------------------------------
    // Create command queues
    //-------------------------------------------------------------
    ECHECK(createCommandQueues(), "Cannot create D3D12 command queues!");

    //-------------------------------------------------------------
    // Create default swap chain
    //-------------------------------------------------------------
    ECHECK(createSwapChain(factory), "Cannot create D3D12 swap chain!");

    //-------------------------------------------------------------
    // Create descriptor heaps
    //-------------------------------------------------------------
    ECHECK(createDescriptorHeaps(), "Cannot create descriptor heaps!");
    
	return true;
}

IGraphics::SPRENDERQUEUE gGraphicsDX12::getRenderQueue()
{
	return m_renderQueue;
}

UINT gGraphicsDX12::createDebugLayerIfNeeded()
{
#if defined(_DEBUG)
    // Enable the D3D12 debug layer.
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }

    return DXGI_CREATE_FACTORY_DEBUG;
#endif
    return 0;
}

bool gGraphicsDX12::createCommandQueues()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    return FAILED( m_cpD3DDev->CreateCommandQueue( &queueDesc, IID_PPV_ARGS(&m_cpCommQueue) ) );
}

bool gGraphicsDX12::createDevice(ComPtr<IDXGIFactory4> factory)
{
    // Select Hardware or WARP adapter type
    if (m_useWARPDevice)
    {
        ComPtr<IDXGIAdapter> warpAdapter;
        if(FAILED(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter))))
            return false;

        if( FAILED( D3D12CreateDevice( warpAdapter.Get(), 
                D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_cpD3DDev) ) ) )
            return false;
    }
    else
    {
        ComPtr<IDXGIAdapter1> hardwareAdapter;
        GetHardwareAdapter(factory.Get(), &hardwareAdapter, true);

        if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(),
                D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_cpD3DDev))))
            return false;
    }
    return true;
}

bool gGraphicsDX12::createSwapChain(ComPtr<IDXGIFactory4> factory)
{
    ComPtr<IDXGISwapChain> swapChain;
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = MAXFRAMESNUM;
    swapChainDesc.BufferDesc.Width = static_cast<UINT>(m_viewport.Width);
    swapChainDesc.BufferDesc.Height = static_cast<UINT>(m_viewport.Height);
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = m_wHandle;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    
    if (FAILED(factory->CreateSwapChain(m_cpCommQueue.Get(), &swapChainDesc, &swapChain)))
        return false;

    // This realization does not support fullscreen transitions.
    if (FAILED(factory->MakeWindowAssociation(m_wHandle, DXGI_MWA_NO_ALT_ENTER)))
        return false;

    swapChain.As(&m_cpSwapChain);
    m_frameIndex = m_cpSwapChain->GetCurrentBackBufferIndex();

    return true;
}

bool gGraphicsDX12::createDescriptorHeaps()
{
    //-------------------------------------------------------------
    // Describe and create a render target view (RTV) descriptor heap.
    //-------------------------------------------------------------
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = MAXFRAMESNUM;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (FAILED(m_cpD3DDev->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_cpRTVHeap))))
        return false;
    m_rtvDescriptorSize = m_cpD3DDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    //-------------------------------------------------------------
    // Describe and create a depth stencil view (DSV) descriptor heap.
    //-------------------------------------------------------------
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if(FAILED(m_cpD3DDev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_cpDSVHeap))))
        return false;
    m_dsvDescriptorSize = m_cpD3DDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

    //-------------------------------------------------------------
    // Describe and create a shader resource view (SRV) heap for the texture.
    //-------------------------------------------------------------
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    UINT maxDescriptorsNum = MAXTEXTURESNUM + MAXCONSTANTBUFFERSNUM +
        MAXVERTEXBUFFERSNUM + MAXINDEXBUFFERSNUM;
    srvHeapDesc.NumDescriptors = maxDescriptorsNum;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (FAILED(m_cpD3DDev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_cpSRVHeap))))
        return false;
    m_srvDescriptorSize = m_cpD3DDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    return true;
}

bool gGraphicsDX12::createDepthStensil()
{
    return true;
}

bool gGraphicsDX12::createFrameResource()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_cpRTVHeap->GetCPUDescriptorHandleForHeapStart());
    
    // Create a RTV for each frame.
    for (UINT n = 0; n < MAXFRAMESNUM; n++)
    {
        if (FAILED(m_cpSwapChain->GetBuffer(n, IID_PPV_ARGS(&m_cpRenderTargets[n]))))
            return false;

        m_cpD3DDev->CreateRenderTargetView(m_cpRenderTargets[n].Get(), nullptr, rtvHandle);
        rtvHandle.Offset( 1, m_rtvDescriptorSize );

        std::wstring name = L"m_cpRenderTargets[" + std::to_wstring(n) + L"]";
        m_cpRenderTargets[n].Get()->SetName( (name.c_str()) );
    }

    // Create a DSV
    {
        D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
        depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

        D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
        depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
        depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
        depthOptimizedClearValue.DepthStencil.Stencil = 0;

        D3D12_HEAP_PROPERTIES props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, 
            static_cast<UINT>(m_viewport.Width), static_cast<UINT>(m_viewport.Height),
            1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

        CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_cpDSVHeap->GetCPUDescriptorHandleForHeapStart());

        if(FAILED(m_cpD3DDev->CreateCommittedResource( &props, D3D12_HEAP_FLAG_NONE, &desc,
                D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthOptimizedClearValue,
                IID_PPV_ARGS(&m_cpDepthStencil))))
            return false;

        //NAME_D3D12_OBJECT(pDepthStencil);
        m_cpDepthStencil.Get()->SetName(L"m_cpDepthStencil");
        m_cpD3DDev->CreateDepthStencilView(m_cpDepthStencil.Get(), 
            &depthStencilDesc, m_cpDSVHeap->GetCPUDescriptorHandleForHeapStart());
    }
    return true;
}

bool gGraphicsDX12::finalize()
{

	return true;
}

void gGraphicsDX12::GetHardwareAdapter(
    IDXGIFactory1* pFactory,
    IDXGIAdapter1** ppAdapter,
    bool requestHighPerformanceAdapter)
{
    *ppAdapter = nullptr;

    ComPtr<IDXGIAdapter1> adapter;

    ComPtr<IDXGIFactory6> factory6;
    if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        for (
            UINT adapterIndex = 0;
            DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
                adapterIndex,
                requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
                IID_PPV_ARGS(&adapter));
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            //continue;

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }
    else
    {
        for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }

    *ppAdapter = adapter.Detach();
}

#endif
