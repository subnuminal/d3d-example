#include "d3d.h"

const uint32_t MAX_QUADS_IN_BATCH = 4096;

// input element desc: float3 position, float3 normal, float2 texcoord, float3 color
D3D11_INPUT_ELEMENT_DESC quadInputDesc[] = {
    { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// input element desc: float3 position, float3 color
D3D11_INPUT_ELEMENT_DESC lineInputDesc[] = {
    {"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    {"COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static void SafeRelease(IUnknown *resource) {
    if (resource != 0) {
        resource->Release();
    }
    resource = 0;
}

static void InitD3D(HWND window, int32_t w, int32_t h, shader_stage *vertex, shader_stage *pixel, resources *res) {
    D3D_FEATURE_LEVEL flvls[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };

    UINT createFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

    // create directx resources
    D3D11CreateDevice(
            0,
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            createFlags,
            flvls,
            ARRAYSIZE(flvls),
            D3D11_SDK_VERSION,
            &res->device,
            0,
            &res->deviceContext);

    ID3D11Device1 *device1;
    res->device->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void **>(&device1));
    IDXGIDevice1 *dxgiDevice;
    device1->QueryInterface(__uuidof(IDXGIDevice1), reinterpret_cast<void **>(&dxgiDevice));
    IDXGIAdapter* dxgiAdapter;
    dxgiDevice->GetAdapter(&dxgiAdapter);
    SafeRelease(dxgiDevice);
    IDXGIFactory2* dxgiFactory;
    dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void **>(&dxgiFactory));
    SafeRelease(dxgiAdapter);

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
    swapChainDesc.Width              = w;
    swapChainDesc.Height             = h;
    swapChainDesc.Format             = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.Stereo             = FALSE;
    swapChainDesc.SampleDesc.Count   = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount        = 2;
    swapChainDesc.Scaling            = DXGI_SCALING_NONE;
    swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags              = 0;

    dxgiFactory->CreateSwapChainForHwnd(device1, window, &swapChainDesc, 0, 0, &res->swapChain);
    DXGI_RGBA bkgrnd = {0.0f, 0.0f, 0.0f, 1.0f};
    res->swapChain->SetBackgroundColor(&bkgrnd);
    dxgiFactory->MakeWindowAssociation(window, DXGI_MWA_NO_ALT_ENTER);

    il_desc_data ilData = il_desc_data{ARRAYSIZE(quadInputDesc), quadInputDesc};
    device1->CreateInputLayout(ilData.desc, ilData.size, &vertex->buffer, vertex->bytesRead, &res->inputLayout);
        
    device1->CreateVertexShader(&vertex->buffer, vertex->bytesRead, 0, &res->vShader);
    device1->CreatePixelShader(&pixel->buffer, pixel->bytesRead, 0, &res->pShader);

    SafeRelease(dxgiFactory);

    ID3D11Texture2D *frameBuffer = 0; // just needed for a render target view and a desc
    res->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&frameBuffer));

    res->device->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void **>(&device1));

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    device1->CreateRenderTargetView(frameBuffer, &rtvDesc, &res->frameBufferView);

    D3D11_TEXTURE2D_DESC depthBufferDesc = {0};
    frameBuffer->GetDesc(&depthBufferDesc);

    SafeRelease(frameBuffer);

    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    ID3D11Texture2D *depthBuffer;  // just needed for a depth stencil view
    device1->CreateTexture2D(&depthBufferDesc, 0, &depthBuffer);

    device1->CreateDepthStencilView(depthBuffer, 0, &res->depthBufferView);
    SafeRelease(depthBuffer);

    D3D11_RASTERIZER_DESC1 rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID; 
    rasterizerDesc.FrontCounterClockwise = false; // default

#if 1
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
#elif 0
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
#else
    rasterizerDesc.CullMode = D3D11_CULL_FRONT;
#endif
    
    device1->CreateRasterizerState1(&rasterizerDesc, &res->rasterizerState);

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    /* depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER; */
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    device1->CreateDepthStencilState(&depthStencilDesc, &res->depthStencilState);

    // create buffers
    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.ByteWidth = sizeof(gmath::m4x4); // NOTE: make sure it conforms to 16 byte boundary
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    for (uint32_t ci = 0; ci < CB_COUNT; ci++) {
        device1->CreateBuffer(&constantBufferDesc, 0, &res->constantBuffers[ci]);
    }

    size_t quadVertCount = 4;
    size_t quadVertSize = 11 * sizeof(float);
    size_t quadBufferSize = quadVertCount * quadVertSize * MAX_QUADS_IN_BATCH;

    D3D11_BUFFER_DESC bufferDesc = {0};
    bufferDesc.ByteWidth = static_cast<uint32_t>(quadBufferSize);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    device1->CreateBuffer(&bufferDesc, 0, &res->quadVertexBuffer);

    size_t quadIndexCount = 6;
    size_t quadIndexSize = quadIndexCount * sizeof(uint32_t) * MAX_QUADS_IN_BATCH;

    // 6 * 4096 = 24576 
    const uint32_t maxIndices = 24576;
    uint32_t quadIndexData[maxIndices] = {0};
    uint32_t indexOffset = 0;

    for (uint32_t qdex = 0; qdex < maxIndices; qdex += 6) {
        quadIndexData[qdex + 0] = 0 + indexOffset;
        quadIndexData[qdex + 1] = 1 + indexOffset;
        quadIndexData[qdex + 2] = 2 + indexOffset;
        quadIndexData[qdex + 3] = 2 + indexOffset;
        quadIndexData[qdex + 4] = 3 + indexOffset;
        quadIndexData[qdex + 5] = 0 + indexOffset;

        indexOffset += 4;
    }

    bufferDesc = {0};
    bufferDesc.ByteWidth = static_cast<uint32_t>(quadIndexSize);
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags= D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA indexData = {quadIndexData};

    device1->CreateBuffer(&bufferDesc, &indexData, &res->quadIndexBuffer);

    res->viewport = {
        0.0f, 0.0f,
        static_cast<float>(depthBufferDesc.Width), static_cast<float>(depthBufferDesc.Height),
        0.0f, 1.0f
    };

    ID3D11DeviceContext1 *devCtx1;
    res->deviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void **>(&devCtx1));

    devCtx1->RSSetViewports(1, &res->viewport);
    devCtx1->RSSetState(res->rasterizerState);

    D3D11_BLEND_DESC1 blendDesc = {0};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE; // only RenderTarget[0] is used
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ID3D11BlendState1 *blendState = 0;
    device1->CreateBlendState1(&blendDesc, &blendState);

    devCtx1->OMSetDepthStencilState(res->depthStencilState, 0);
    uint32_t sampleMask = 0xffffffff;
    devCtx1->OMSetBlendState(blendState, 0, sampleMask);

    SafeRelease(device1);
    SafeRelease(devCtx1);
}

static void ReleaseAll(resources *res) {
    SafeRelease(res->device);
    SafeRelease(res->deviceContext);
    SafeRelease(res->swapChain);
    SafeRelease(res->frameBufferView);
    SafeRelease(res->depthBufferView);

    SafeRelease(res->inputLayout);
    SafeRelease(res->vShader);
    SafeRelease(res->pShader);

    for (uint32_t ci = 0; ci < CB_COUNT; ci++) {
        SafeRelease(res->constantBuffers[ci]);
    } 

    SafeRelease(res->quadVertexBuffer);
    SafeRelease(res->quadIndexBuffer);

    SafeRelease(res->rasterizerState);
    SafeRelease(res->depthStencilState);
}
