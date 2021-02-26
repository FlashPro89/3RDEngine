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
    m_rtvDescriptorSize(NULL),
    m_dsvDescriptorSize(NULL),
    m_srvDescriptorSize(NULL)
{
    auto wParams = platform->getWindow()->getWindowParameters();

	m_renderQueue = IGraphics::SPRENDERQUEUE(new gRenderQueueDX12(this));
    m_viewport.TopLeftX = m_viewport.TopLeftY = 0;
    m_viewport.Width = wParams.width;
    m_viewport.Height = wParams.height;

    m_scissorRect.left = m_scissorRect.top = 0;
    m_scissorRect.bottom = wParams.width;
    m_scissorRect.right = wParams.height;

    m_wpPlatform = platform;
    m_wpConfiguration = configuration;
}

gGraphicsDX12::~gGraphicsDX12()
{
	MessageBox(0, "gGraphicsDX12", "Destructor called!", MB_OK);
	finalize();
}

bool gGraphicsDX12::initialize()
{
    //-------------------------------------------------------------
    // Check first window in platform system is initialized
    //-------------------------------------------------------------
    ECHECK(!m_wpPlatform.expired(), "Initialize platform before graphics!");

    //-------------------------------------------------------------
    // Use debug layer if needeed
    //-------------------------------------------------------------
    UINT dxgiFactoryFlags = createDebugLayerIfNeeded();

    //-------------------------------------------------------------
    // Create DXGI factory
    //-------------------------------------------------------------
    ComPtr < IDXGIFactory4 > factory;
    ECHECKHR(CreateDXGIFactory1(IID_PPV_ARGS(&factory)), 
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
    ECHECK(createCommandQueue(), "Cannot create D3D12 command queues!");

    //-------------------------------------------------------------
    // Create default swap chain
    //-------------------------------------------------------------
    ECHECK(createSwapChain(factory), "Cannot create D3D12 swap chain!");

    //-------------------------------------------------------------
    // Create descriptor heaps
    //-------------------------------------------------------------
    ECHECK(createDescriptorHeaps(), "Cannot create D3D12 descriptor heaps!");
    
    //-------------------------------------------------------------
    // Create frame resources
    //-------------------------------------------------------------
    ECHECK(createFrameResources(), "Cannot create D3D12 frame resources!");

    //-------------------------------------------------------------
    // Create default assets
    //-------------------------------------------------------------
    ECHECK(createDefaultRootSignature(), "Cannot create D3D12 default root signature!");
    ECHECK(createDefaultPipelineState(), "Cannot create D3D12 default pipeline state!");

    //-------------------------------------------------------------
    // Create command lists
    //-------------------------------------------------------------
    ECHECK(createCommandLists(), "Cannot create D3D12 command lists!");

    //-------------------------------------------------------------
    // Create fence
    //-------------------------------------------------------------
    ECHECK(createFence(), "Cannot create D3D12 fence!");

    // Close and execute command list
    ECHECKHR(m_cpCommList->Close(), "Cannot close D3D12 Command list!");
    PopulateCommandList();
    
    // Execute the command list.
    ID3D12CommandList* ppCommandLists[] = { m_cpCommList.Get() };
    m_cpCommQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Present the frame.
    ECHECKHR(m_cpSwapChain->Present(1, 0),
        "SwapChain->Present() failed!");
    WaitForPreviousFrame();

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

bool gGraphicsDX12::createCommandQueue()
{
    if (FAILED(m_cpD3DDev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cpCommAllocator))))
        return false;

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    return SUCCEEDED( m_cpD3DDev->CreateCommandQueue( &queueDesc, IID_PPV_ARGS(&m_cpCommQueue) ) );
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
    // Get window parameters
    auto wParams = m_wpPlatform.lock()->getWindow()->getWindowParameters();
    HWND hWnd = static_cast<HWND>(wParams.handle);

    m_renderQueue = IGraphics::SPRENDERQUEUE(new gRenderQueueDX12(this));
    m_viewport.TopLeftX = m_viewport.TopLeftY = 0;
    m_viewport.Width = wParams.width;
    m_viewport.Height = wParams.height;

    m_scissorRect.left = m_scissorRect.top = 0;
    m_scissorRect.bottom = wParams.width;
    m_scissorRect.right = wParams.height;

    ComPtr<IDXGISwapChain> swapChain;
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = MAXFRAMESNUM;
    swapChainDesc.BufferDesc.Width = static_cast<UINT>(m_viewport.Width);
    swapChainDesc.BufferDesc.Height = static_cast<UINT>(m_viewport.Height);
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    
    if (FAILED(factory->CreateSwapChain(m_cpCommQueue.Get(), &swapChainDesc, &swapChain)))
        return false;

    // This realization does not support fullscreen transitions.
    if (FAILED(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER)))
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

bool gGraphicsDX12::createFrameResources()
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


bool gGraphicsDX12::createDefaultRootSignature()
{
    D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

    // This is the highest version the sample supports. If CheckFeatureSupport succeeds, 
    // the HighestVersion returned will not be greater than this.
    featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
    if (FAILED(m_cpD3DDev->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
    //printf("WARNING: force use ROOT_SIGNATURE_VERSION_1_0!\n");
    

    CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
    ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
    /*
    ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    ranges[0].NumDescriptors = 1;
    ranges[0].BaseShaderRegister = 0;
    ranges[0].RegisterSpace = 0;
    ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    ranges[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
    */

    CD3DX12_ROOT_PARAMETER1 rootParameters[2];
    rootParameters[1].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
    rootParameters[0].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_VERTEX);

    D3D12_STATIC_SAMPLER_DESC sampler[1] = {};
    sampler[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    sampler[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler[0].MipLODBias = 0;
    sampler[0].MaxAnisotropy = 0;
    sampler[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    sampler[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    sampler[0].MinLOD = 0.0f;
    sampler[0].MaxLOD = D3D12_FLOAT32_MAX;
    sampler[0].ShaderRegister = 0;
    sampler[0].RegisterSpace = 0;
    sampler[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    if (FAILED(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error)))
        return false;
    //ECHECKHR(hr, "Cannot serialize versioned root signature!");

    if (FAILED(m_cpD3DDev->CreateRootSignature(0, signature->GetBufferPointer(),
        signature->GetBufferSize(), IID_PPV_ARGS(&m_cpRootSignature[0]))))
        return false;
    //ECHECKHR(hr, "Cannot create root signature!");
    
    return true;
}

bool gGraphicsDX12::createDefaultPipelineState()
{
    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
    // Enable better shader debugging with the graphics debugging tools.
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif

    const char vs_source[] =
        "struct VS_INPUT                    "
        "{                                   "
        "    float4 pos : POSITION;          "
        "    float2 texCoord : TEXCOORD;     "
        "    float4 color : COLOR;          "
        "};   "

        "struct VS_OUTPUT                       "
        "{                                      "
        "   float4 pos : SV_POSITION;           "
        "    float2 texCoord : TEXCOORD;        "
        "    float4 color : COLOR;              "
        "};                                     "


        "VS_OUTPUT main(VS_INPUT input)         "
        "{                                      "
        "   VS_OUTPUT output;                   "
        "   output.pos = input.pos;             "
        "   output.texCoord = input.texCoord;  "
        "   output.color = input.color;        "
        "   return output;                     "
        "}";

    const char ps_source[] =
        "struct PSInput                         "
        "{                                      "
        "    float4 position : SV_POSITION;     "
        "    float2 uv : TEXCOORD;              "
        "    float4 color : COLOR;              "
        "};                                     "

        "Texture2D g_texture : register(t0);       "
        "SamplerState g_sampler : register(s0);     "

        "float4 main(PSInput input) : SV_TARGET     "
        "{                                          "
        "    return g_texture.Sample(g_sampler, input.uv); "
        "} ";
   
    
    ID3DBlob* errorBlob;

    if( FAILED( D3DCompile( vs_source, sizeof(vs_source), "vs_default", nullptr, 
                nullptr, "main", "vs_5_0", compileFlags, 0, &vertexShader, &errorBlob)))
        return false;

    if (FAILED(D3DCompile(ps_source, sizeof(ps_source), "ps_default", nullptr,
        nullptr, "main", "ps_5_0", compileFlags, 0, &pixelShader, &errorBlob)))
        return false;

    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Describe and create the graphics pipeline state object (PSO).
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = m_cpRootSignature[0].Get();
    psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
    psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    if (FAILED(m_cpD3DDev->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_cpPipelineState[0]))))
        return false;

    return true;
}

bool gGraphicsDX12::createCommandLists()
{
    return (SUCCEEDED(m_cpD3DDev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
        m_cpCommAllocator.Get(), m_cpPipelineState[0].Get(), IID_PPV_ARGS(&m_cpCommList))));
}

bool gGraphicsDX12::createFence()
{
    if (FAILED(m_cpD3DDev->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_cpFence))))
        return false;
        
    m_fenceValue = 1;

    // Create an event handle to use for frame synchronization.
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr)
    {
        if (FAILED(HRESULT_FROM_WIN32(GetLastError())))
            return false;
    }
    return true;
}

void gGraphicsDX12::WaitForPreviousFrame()
{
    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
    // This is code implemented as such for simplicity. More advanced samples 
    // illustrate how to use fences for efficient resource usage.

    // Signal and increment the fence value.
    const UINT64 fence = m_fenceValue;
    m_cpCommQueue->Signal(m_cpFence.Get(), fence);
    m_fenceValue++;

    // Wait until the previous frame is finished.
    if (m_cpFence->GetCompletedValue() < fence)
    {
        ECHECKHR(m_cpFence->SetEventOnCompletion(fence, m_fenceEvent),
            "D3D12 Fence->SetEventOnCompletion() failed!");
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }

    m_frameIndex = m_cpSwapChain->GetCurrentBackBufferIndex();
}

void gGraphicsDX12::PopulateCommandList()
{
    // Command list allocators can only be reset when the associated 
    // command lists have finished execution on the GPU; apps should use 
    // fences to determine GPU execution progress.
    ECHECKHR(m_cpCommAllocator->Reset(), 
        "D3D12 CommandAllocator->Reset() failed!");

    // However, when ExecuteCommandList() is called on a particular command 
    // list, that command list can then be reset at any time and must be before 
    // re-recording.
    ECHECKHR(m_cpCommList->Reset(m_cpCommAllocator.Get(), m_cpPipelineState[0].Get()),
        "D3D12 CommandList->Reset() failed!");

    // Set necessary state.
    m_cpCommList->SetGraphicsRootSignature(m_cpRootSignature[0].Get());

    ID3D12DescriptorHeap* ppHeaps[] = { m_cpSRVHeap.Get() };
    m_cpCommList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    CD3DX12_GPU_DESCRIPTOR_HANDLE h(m_cpSRVHeap->GetGPUDescriptorHandleForHeapStart());

    m_cpCommList->RSSetViewports(1, &m_viewport);
    m_cpCommList->RSSetScissorRects(1, &m_scissorRect);

    // Indicate that the back buffer will be used as a render target.
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_cpRenderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_cpCommList->ResourceBarrier(1, &barrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_cpRTVHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_cpDSVHeap->GetCPUDescriptorHandleForHeapStart());

    m_cpCommList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[] = { 0.0f, 0.4f, 0.2f, 1.0f };
    m_cpCommList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    m_cpCommList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, 0);

    // Indicate that the back buffer will now be used to present.
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_cpRenderTargets[m_frameIndex].Get(), 
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_cpCommList->ResourceBarrier(1, &barrier);

    ECHECKHR(m_cpCommList->Close(),
        "D3D12 CommandList->Close() failed!");
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
