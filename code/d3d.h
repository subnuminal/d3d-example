#pragma once

#include <d3dcommon.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include "game_math.h"

// vert values: (3 + 3 + 2 + 3 ) = 11
// vert count : * 4 = 44 
const uint32_t QuadVertexCount = 44;

struct QuadVertex {
    float elems[11];
};

struct Quad {
    QuadVertex vs[4];
};

struct Cube {
    QuadVertex vs[8];
};

const uint32_t shaderBufferSize = 4096;

enum ConstantBufferType {
    CB_TYPE, // todo: rename to CB_WVP
    CB_COUNT
};

struct shader_stage {
    uint8_t buffer[shaderBufferSize];
    DWORD bytesRead;
};

struct il_desc_data {
    uint32_t size;
    D3D11_INPUT_ELEMENT_DESC *desc;
};

struct resources {
    ID3D11Device *device;
    ID3D11DeviceContext *deviceContext;
    IDXGISwapChain1 *swapChain;
    ID3D11RenderTargetView *frameBufferView;
    ID3D11DepthStencilView *depthBufferView;
    D3D11_VIEWPORT viewport;
    ID3D11InputLayout *inputLayout;
    ID3D11VertexShader *vShader;
    ID3D11PixelShader *pShader;
    ID3D11Buffer *constantBuffers[CB_COUNT];
    ID3D11Buffer *quadVertexBuffer;
    ID3D11Buffer *quadIndexBuffer;
    ID3D11RasterizerState1 *rasterizerState;
    ID3D11DepthStencilState *depthStencilState;
};
