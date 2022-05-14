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

static float NormCosWave(float freq) {
    float radians = freq * gameTimeSecs * 2.0f * PI;
    float cosWave = gmath::Cos(radians);
    cosWave += 1.0f;
    cosWave /= 2.0f;
    return cosWave;
}

static QuadVertex MakeVertex(float x, float y, float z, vec4 col) {
    // float3 pos, float3 norm, float2 texcoord, float3 color
    QuadVertex qv = {
        x,    y,     z,
        0.0f, 0.0f, -1.0f, // TODO: fix the normal
        0.0f, 0.0f,
        col.r, col.g, col.b,
    };

    return qv;
}

static Cube MakeCube(vec4 col) {
    Cube c = {0};

    // colors
    vec4 red = {1.0f, 0.0f, 0.0f, 1.0f};
    vec4 green = {0.0f, 1.0f, 0.0f, 1.0f};
    vec4 blue = {0.0f, 0.0f, 1.0f, 1.0f};
    vec4 yellow = {1.0f, 1.0f, 0.0f, 1.0f};

    float x = 0.5f;
    float y = 0.5f;
    float z = 0.5f;

    c.vs[0] = MakeVertex(-x, -y,  z, col);
    c.vs[1] = MakeVertex(-x,  y,  z, col);
    c.vs[2] = MakeVertex( x,  y,  z, col);
    c.vs[3] = MakeVertex( x, -y,  z, col);
    c.vs[4] = MakeVertex(-x, -y, -z, red);
    c.vs[5] = MakeVertex(-x,  y, -z, green);
    c.vs[6] = MakeVertex( x,  y, -z, blue);
    c.vs[7] = MakeVertex( x, -y, -z, yellow);

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

    gmath::m4x4 camRotate = gmath::Identity4x4();
    if (!gmath::Approx0(c.pitch) || !gmath::Approx0(0.0f) ||  !gmath::Approx0(c.yaw)) {
        vec3 rotRads = gmath::DegsToRadsV(vec3{c.pitch, 0.0f, c.yaw});

        gmath::m4x4 rotAxisX = gmath::RotMatX(rotRads.x);
        gmath::m4x4 rotAxisY = gmath::RotMatY(rotRads.y);
        gmath::m4x4 rotAxisZ = gmath::RotMatZ(rotRads.z);

        camRotate = rotAxisX * rotAxisY * rotAxisZ;
    }

    vec3 translation = vec3{c.pos.x + c.pan.x, c.pos.y + c.pan.y, c.pos.z + c.dolly};
    gmath::m4x4 camTranslate = gmath::Translate(gmath::Identity4x4(), translation);

    // (TR)v
    gmath::m4x4 camTrans = camTranslate * camRotate;
    vec4 camPos = gmath::ColW(camTrans);
    gmath::m4x4 view = gmath::CameraTransform(camTrans, camPos.xyz);

    // D3D is LH, so negate coords influencing z pos 
    view.elems[2][0] *= -1.0f;
    view.elems[2][1] *= -1.0f;
    view.elems[2][2] *= -1.0f;
    view.elems[2][3] *= -1.0f;
    return view;
}

static gmath::m4x4 PerspectiveProjection(float fovyRads, float wOverH, float n = 1.0f, float f = 1000.0f) {
    float sinFov = gmath::Sin(fovyRads * 0.5f);
    float cosFov = gmath::Cos(fovyRads * 0.5f);
    float tanHalf = sinFov/cosFov;
    float halfH = n * tanHalf;
    float halfW = halfH * wOverH;

    // monkeying with numbers
    float zRange = (f-n);
    float depth = f/zRange;
    float zOffset = -n * depth;
    float zCoeff = 1.0f;

    // TODO: how to avoid potential divide by zeroes during perspective divide?
    gmath::m4x4 perspM = {
        n/halfW, 0.0f,    0.0f,   0.0f, 
        0.0f,    n/halfH, 0.0f,   0.0f, 
        0.0f,    0.0f,    depth,  zOffset, 
        0.0f,    0.0f,    zCoeff, 0.0f 
    };

    return perspM;
}

static gmath::m4x4 CalcWVP(camera c, vec3 cubePos, float cubeRotY, vec3 cubeScale, float fovy, float wOverH, float n = 1.0f, float f = 1000.0f) {

    gmath::m4x4 worldRotate = gmath::Identity4x4();
    gmath::m4x4 worldScale = gmath::Identity4x4();
    gmath::m4x4 worldTranslate = gmath::Translate(gmath::Identity4x4(), cubePos);

    float rotx = 0.0f;
    float roty = cubeRotY;
    float rotz = 0.0f;

    if (!gmath::Approx0(rotx) || !gmath::Approx0(roty) ||  !gmath::Approx0(rotz)) {
        vec3 rotRads = gmath::DegsToRadsV(vec3{rotx, roty, rotz});

        gmath::m4x4 rotAxisX = gmath::RotMatX(rotRads.x);
        gmath::m4x4 rotAxisY = gmath::RotMatY(rotRads.y);
        gmath::m4x4 rotAxisZ = gmath::RotMatZ(rotRads.z);

        worldRotate = rotAxisX * rotAxisY * rotAxisZ;
    }

    worldScale.elems[0][0] = cubeScale.x;
    worldScale.elems[1][1] = cubeScale.y;
    worldScale.elems[2][2] = cubeScale.z;

    // (TSR)v
    gmath::m4x4 modelToWorld = worldTranslate * worldScale * worldRotate;

    gmath::m4x4 worldToView = CalcViewMatrix(c); 

    gmath::m4x4 projectToClip = PerspectiveProjection(fovy, wOverH);

    // (PVW)v
    gmath::m4x4 wvp = projectToClip * worldToView * modelToWorld;

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
    static vec3 cubePos = vec3{0.0f, 0.0f, -5.0f};
    static float cubeRotY = 0.0f;
    cubeRotY = 360.0f * NormCosWave(0.2f);
    vec3 cubeScale = vec3{1.0f, 1.0f, 1.0f};
    cubeScale *= (1.0f + NormCosWave(0.2f));
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

    gmath::m4x4 wvp = CalcWVP(cam, cubePos, cubeRotY, cubeScale, fovy, wOverH, n, f);

    CopyDynamic(devCtx1, res->constantBuffers[CB_TYPE], sizeof(wvp), &wvp);

    Quad quadBatch[4096] = {0};
    uint32_t quadCount = 0;

    Cube c = MakeCube(cubeCol);
    // face
    quadBatch[quadCount++] = Quad{c.vs[0], c.vs[1], c.vs[2], c.vs[3]};
    // right
    quadBatch[quadCount++] = Quad{c.vs[3], c.vs[2], c.vs[6], c.vs[7]};
    // back
    quadBatch[quadCount++] = Quad{c.vs[7], c.vs[6], c.vs[5], c.vs[4]};
    // left
    quadBatch[quadCount++] = Quad{c.vs[4], c.vs[5], c.vs[1], c.vs[0]};
    // top
    quadBatch[quadCount++] = Quad{c.vs[1], c.vs[5], c.vs[6], c.vs[2]};
    // bottom
    quadBatch[quadCount++] = Quad{c.vs[4], c.vs[0], c.vs[3], c.vs[7]};

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

