#include <stdlib.h>
#include <cstdio>
#include "game.h"
#include "game_math.h"
#include "d3d.h"

static float gameTimeSecs = 0.0f;

struct game_input {
    bool isAnalog;
    float analogLtThumbX;
    float analogLtThumbY;
    button panLeft;
    button panRight;
    button panUp;
    button panDown;
    button jump;
    button cycleA;
    button cycleB;
    button abilityA;
    button abilityB;
    button zoom;
    button menu;
    button altKey;
    button keyM;
    button volumeUp;
    button volumeDown;
    button undo;

    button select;
    button scroll;
    button context;
    vec2 pixCoords;
    vec2 viewCoords;

    button mouseRight;
    button mouseMiddle;
    vec2 rClickPos;
    vec2 mClickPos;
};

static vec4 GetMovementInput(game_input* gameInput) {
    vec4 move = {0};
    if (gameInput->isAnalog) {
        move.x = gameInput->analogLtThumbX;
        move.y = gameInput->analogLtThumbY;
    } else {
        if (gameInput->panUp.isPressed) {
            move.y = 0.5f;
        } else if (gameInput->panDown.isPressed) {
            move.y = -0.5f;
        }

        if (gameInput->panRight.isPressed) {
            move.x = 0.5f;
        } else if (gameInput->panLeft.isPressed) {
            move.x = -0.5f;
        }
    }

    if (BeenPressed(gameInput->abilityA)) {
        move.z = -0.1f;
    } else if (BeenPressed(gameInput->abilityB)) {
        move.z = 0.1f;
    }

    if (gmath::Approx0(move.x)) {
        move.x = 0.0f;
    }

    if (gmath::Approx0(move.y)) {
        move.y = 0.0f;
    }

    return move;
}

static void ReadUserInput(game_input *gameInput, platform_input *platInput, int32_t w, int32_t h) {
    gameInput->isAnalog = platInput->isAnalog;
    gameInput->analogLtThumbX = platInput->analogLtThumbX;
    gameInput->analogLtThumbY = platInput->analogLtThumbY;

    gameInput->panLeft = platInput->keyA || platInput->dpadLeft;
    gameInput->panRight = platInput->keyD || platInput->dpadRight;
    gameInput->panUp = platInput->keyW || platInput->dpadUp;
    gameInput->panDown = platInput->keyS || platInput->dpadDown;

    gameInput->jump = platInput->spacebar || platInput->buttonA;
    gameInput->cycleA = platInput->keyLeft || platInput->buttonLtShldr;
    gameInput->cycleB = platInput->keyRight || platInput->buttonRtShldr;
    gameInput->abilityA = platInput->keyUp || platInput->buttonX;
    gameInput->abilityB = platInput->keyDown || platInput->buttonB;
    gameInput->menu = platInput->escKey || platInput->buttonStart;
    gameInput->altKey = platInput->altKey;
    gameInput->keyM = platInput->keyM;
    gameInput->mouseRight = platInput->mouseRight;
    gameInput->mouseMiddle = platInput->mouseMiddle;
    gameInput->volumeUp = platInput->keyPgUp;
    gameInput->volumeDown = platInput->keyPgDown;
    button undo = button{};
    undo.isPressed = (platInput->ctrlKey.wasPressed
            && (platInput->keyZ.isPressed && !platInput->keyZ.wasPressed));
    gameInput->undo = undo;

    gameInput->select = platInput->mouseLeft;
    gameInput->scroll = platInput->mouseMiddle;
    gameInput->context = platInput->mouseRight;

    gameInput->viewCoords.x = (platInput->mouseX - 0.5f) * w;
    gameInput->viewCoords.y = (platInput->mouseY - 0.5f) * -h;

    if (JustPressed(gameInput->mouseRight)) {
        gameInput->rClickPos = gameInput->viewCoords;
    }

    if (JustPressed(gameInput->mouseMiddle)) {
        gameInput->mClickPos = gameInput->viewCoords;
    }

    gameInput->pixCoords.x = platInput->mouseX * w;
    gameInput->pixCoords.y = platInput->mouseY * h;
}

static float CosWave(float freq) {
    float radians = freq * gameTimeSecs * 2.0f * PI;
    float cosWave = gmath::Cos(radians);
    return cosWave;
}

static QuadVertex MakeQuadVertex(vec3 pos, float w, float h, float d, vec4 col) {
    float z = pos.z + d;
    if (gmath::Approx0(pos.z)) {
        z = gmath::epsilon;
    }

    QuadVertex qv = {
        pos.x + w, pos.y + h, z,
        0.0f, 0.0f, -1.0f, // TODO: fix the normal
        0.0f, 0.0f,
        col.r, col.g, col.b,
    };

    return qv;
}

static Cube MakeCube(vec3 pos, float w, float h, float d, vec4 col) {
    // NOTE: quad triangles are drawn 0, 1, 2, 2, 3, 0
    // float3 pos, float3 norm, float2 texcoord, float3 color
    Cube c = {0};

    vec4 red = {1.0f, 0.0f, 0.0f, 1.0f};
    vec4 green = {0.0f, 1.0f, 0.0f, 1.0f};
    vec4 blue = {0.0f, 0.0f, 1.0f, 1.0f};
    vec4 yellow = {1.0f, 1.0f, 0.0f, 1.0f};
    vec4 teal = {0.0f, 1.0f, 1.0f, 1.0f};

    // front face
    c.qs[0].vs[0] = MakeQuadVertex(pos, 0, 0, d, col);
    c.qs[0].vs[1] = MakeQuadVertex(pos, 0, h, d, col);
    c.qs[0].vs[2] = MakeQuadVertex(pos, w, h, d, col);
    c.qs[0].vs[3] = MakeQuadVertex(pos, w, 0, d, col);

    // right face
    c.qs[1].vs[0] = MakeQuadVertex(pos, w, 0, d, red);
    c.qs[1].vs[1] = MakeQuadVertex(pos, w, h, d, red);
    c.qs[1].vs[2] = MakeQuadVertex(pos, w, h, 0, red);
    c.qs[1].vs[3] = MakeQuadVertex(pos, w, 0, 0, red);

    // back face
    c.qs[2].vs[0] = MakeQuadVertex(pos, w, 0, 0, green);
    c.qs[2].vs[1] = MakeQuadVertex(pos, w, h, 0, green);
    c.qs[2].vs[2] = MakeQuadVertex(pos, 0, h, 0, green);
    c.qs[2].vs[3] = MakeQuadVertex(pos, 0, 0, 0, green);

    // left face
    c.qs[3].vs[0] = MakeQuadVertex(pos, 0, 0, 0, blue);
    c.qs[3].vs[1] = MakeQuadVertex(pos, 0, h, 0, blue);
    c.qs[3].vs[2] = MakeQuadVertex(pos, 0, h, d, blue);
    c.qs[3].vs[3] = MakeQuadVertex(pos, 0, 0, d, blue);

    // top face
    c.qs[4].vs[0] = MakeQuadVertex(pos, 0, h, d, yellow);
    c.qs[4].vs[1] = MakeQuadVertex(pos, 0, h, 0, yellow);
    c.qs[4].vs[2] = MakeQuadVertex(pos, w, h, 0, yellow);
    c.qs[4].vs[3] = MakeQuadVertex(pos, w, h, d, yellow);

    // bottom face
    c.qs[5].vs[0] = MakeQuadVertex(pos, 0, 0, 0, teal);
    c.qs[5].vs[1] = MakeQuadVertex(pos, 0, 0, d, teal);
    c.qs[5].vs[2] = MakeQuadVertex(pos, w, 0, d, teal);
    c.qs[5].vs[3] = MakeQuadVertex(pos, w, 0, 0, teal);

    return c;
}

static void CopyDynamic(ID3D11DeviceContext1 *devCtx1, ID3D11Buffer *dest, size_t srcSize, void *src) {
    D3D11_MAPPED_SUBRESOURCE mappedData = {0};
    devCtx1->Map(dest, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
    memcpy(mappedData.pData, src, srcSize);
    devCtx1->Unmap(dest, 0);
}

struct camera {
    vec3 pos;
    vec2 pan;
    float dolly;
    float pitch;
    float yaw;
};

static gmath::m4x4 CalcViewMatrix(camera c) {
    // view calculations
    vec3 camPos = c.pos;
    camPos.xy = c.pan;
    camPos.z += c.dolly;

    if (gmath::Approx0(camPos.z)) {
        camPos.z = gmath::epsilon;
    }

    float rotRadX = gmath::DegsToRads(c.pitch);
    gmath::m4x4 axisX = gmath::RotMatX(rotRadX);

    gmath::m4x4 axisY = gmath::Identity4x4();

    float rotRadZ = gmath::DegsToRads(c.yaw);
    gmath::m4x4 axisZ = gmath::RotMatZ(rotRadZ);

    gmath::m4x4 axisM = axisX * axisY * axisZ;

    // D3D is LH, so negate z axis 
    axisM.elems[(4 * 2) + 0] *= -1.0f;
    axisM.elems[(4 * 2) + 1] *= -1.0f;
    axisM.elems[(4 * 2) + 2] *= -1.0f;
    /* axisM.elems[(4 * 2) + 3] *= -1.0f; */

    camPos = axisM * camPos;

    gmath::m4x4 camMat = gmath::CameraTransform(axisM, camPos);
    return camMat;
}

static gmath::m4x4 PerspectiveProjection(float fovyRads, float wOverH, float n = 1.0f, float f = 1000.0f) {
    float sinFov = gmath::Sin(fovyRads * 0.5f);
    float cosFov = gmath::Cos(fovyRads * 0.5f);
    float tanHalf = sinFov/cosFov;
    float halfH = n * tanHalf;
    float halfW = halfH * wOverH;

    // monkeying with numbers
    float zRange = (f-n);
    float depth = -f/zRange;
    float zOffset = n * depth;
    float zCoeff = -1.0f;

    gmath::m4x4 perspM = {
        1.0f/halfW, 0.0f,       0.0f,  0.0f, 
        0.0f,       1.0f/halfH, 0.0f,  0.0f, 
        0.0f,       0.0f,       depth, zOffset, 
        0.0f,       0.0f,       zCoeff,  0.0f 
    };

    return perspM;
}

static gmath::m4x4 CalcWVP(camera c, float cubeRotY, float fovy, float wOverH, float n = 1.0f, float f = 1000.0f) {
    // model to world matrix 
    gmath::m4x4 wm = gmath::Identity4x4();

    float rotx = 0.0f;
    float roty = cubeRotY;
    float rotz = 0.0f;

    if (!gmath::Approx0(rotx) || !gmath::Approx0(roty) ||  !gmath::Approx0(rotz)) {
        vec3 rotRads = gmath::DegsToRadsV(vec3{rotx, roty, rotz});

        gmath::m4x4 rxm = gmath::RotMatX(rotRads.x);
        gmath::m4x4 rym = gmath::RotMatY(rotRads.y);
        gmath::m4x4 rzm = gmath::RotMatZ(rotRads.z);

        wm = rzm * (rym * rxm);
    }

    // view matrix
    gmath::m4x4 vm = CalcViewMatrix(c); 

    // projection matrix
    gmath::m4x4 pm = PerspectiveProjection(fovy, wOverH);

    // row major (pre mult): vWVP
    /* gmath::m4x4 wvp = wm * (vm * pm); */
    // col major (post mult): PVWv 
    gmath::m4x4 wvp = pm * (vm * wm);

    return wvp;
}

static gmath::m4x4 CalcDXWVP(camera c, vec3 cubePos, float cubeRotY, float fovy, float wOverH, float n = 1.0f, float f = 1000.0f) {

    // model to world matrix
    DirectX::XMMATRIX dxw = DirectX::XMMatrixIdentity();

    if (cubeRotY) {
        DirectX::XMVECTOR yaxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        float rads = DirectX::XMConvertToRadians(cubeRotY);
        dxw = DirectX::XMMatrixRotationNormal(yaxis, rads);
    }

    // world to view matrix
    DirectX::FXMVECTOR eye   = DirectX::XMVectorSet(0.0f, 0.0f, c.pos.z + c.dolly, 1.0f);
    DirectX::FXMVECTOR focus = DirectX::XMVectorSet(0.0f, 0.0f, cubePos.z, 1.0f);
    DirectX::FXMVECTOR up    = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
    DirectX::XMMATRIX dxv = DirectX::XMMatrixLookAtRH(eye, focus, up);

    // view to clip space projection matrix
    DirectX::XMMATRIX dxp = DirectX::XMMatrixPerspectiveFovRH(fovy, wOverH, n, f);

    DirectX::XMMATRIX dxwvp = dxw * (dxv * dxp);

    // hlsl expects column major so transpose
    dxwvp = DirectX::XMMatrixTranspose(dxwvp);

    // copy over to game native matrix struct
    DirectX::XMFLOAT4X4 t;
    DirectX::XMStoreFloat4x4(&t, dxwvp);
    gmath::m4x4 wvp = {
        t.m[0][0], t.m[0][1], t.m[0][2], t.m[0][3],
        t.m[1][0], t.m[1][1], t.m[1][2], t.m[1][3], 
        t.m[2][0], t.m[2][1], t.m[2][2], t.m[2][3], 
        t.m[3][0], t.m[3][1], t.m[3][2], t.m[3][3] 
    };

    return wvp;
}

void BeginTriangles(ID3D11DeviceContext1 *devCtx1, resources *res) {
    devCtx1->OMSetRenderTargets(1, &res->frameBufferView, res->depthBufferView);

    vec4 clearColor = colors::GRAYISH;
    FLOAT bgColor[4] = {clearColor.r, clearColor.g, clearColor.b, clearColor.a};
    devCtx1->ClearRenderTargetView(res->frameBufferView, bgColor);
    devCtx1->ClearDepthStencilView(res->depthBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    devCtx1->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    devCtx1->IASetInputLayout(res->inputLayout);

    UINT stride = 11 * 4;
    UINT offset = 0;
    devCtx1->IASetVertexBuffers(0, 1, &res->quadVertexBuffer, &stride, &offset);
    devCtx1->IASetIndexBuffer(res->quadIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    devCtx1->VSSetShader(res->vShader, 0, 0);
    devCtx1->VSSetConstantBuffers(0, 1, res->constantBuffers);
    devCtx1->PSSetShader(res->pShader, 0, 0);
}

void UpdateAndRender(resources *res, int32_t w, int32_t h, platform_input* in) {

    // camera config
    camera cam = {0};
    static float dolly = 0.0f;
    float fovDegrees = 60.0f;
    float fovy = gmath::DegsToRads(fovDegrees);
    float wOverH = (float)w/(float)h;
    float n = 1.0f;
    float f = 1000.0f;

    // cube config
    float cubeW = 10.0f;
    float cubeH = 10.0f;
    float cubeD = 10.0f;
    static vec3 cubePos = vec3{-16.0f, -10.0f, -50.0f};
    static float cubeRotY = 0.0f;
    /* cubeRotY = 180.0f * CosWave(0.2f); */
    vec4 cubeCol = colors::BLACK;

    // player input
    game_input input = {0};
    ReadUserInput(&input, in, w, h);

    // NOTE: x/y is pan, z is dolly 
    vec4 move = GetMovementInput(&input);

    cubePos.xy += move.xyz.xy;
    dolly += move.z;
    cam.dolly = dolly;

    // rendering
    ID3D11DeviceContext1 *devCtx1;
    res->deviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void **>(&devCtx1));

    BeginTriangles(devCtx1, res);

#if 0
    gmath::m4x4 wvp = CalcWVP(cam, cubeRotY, fovy, wOverH, n, f);
#else
    gmath::m4x4 wvp = CalcDXWVP(cam, cubePos, cubeRotY, fovy, wOverH, n, f);
#endif

    CopyDynamic(devCtx1, res->constantBuffers[CB_TYPE], sizeof(wvp), &wvp);

    Quad quadBatch[4096] = {0};
    uint32_t quadCount = 0;

    Cube c = MakeCube(cubePos, cubeW, cubeH, cubeD, cubeCol);
    for (uint32_t face = 0; face < 6; face++) {
        quadBatch[quadCount++] = c.qs[face];
    }

    size_t batchSize = sizeof(Quad) * quadCount;
    CopyDynamic(devCtx1, res->quadVertexBuffer, batchSize, quadBatch);
    uint32_t indicesCount = 6 * quadCount;
    devCtx1->DrawIndexed(indicesCount, 0, 0);

    // done drawing, release context
    devCtx1->Release();
    
    gameTimeSecs += 0.016667f;
    if (gameTimeSecs >= 1000.0f) {
        float remainder = gameTimeSecs - 1000.0f;
        gameTimeSecs = remainder;
    }
}

