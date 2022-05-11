#include <stdint.h>
#include <stdio.h>

#include "d3d.cpp"
#include "game.cpp"

#include <Windows.h>
#include <Xinput.h>

// global game stuff
static bool isRunning = false;

// global input stuff
typedef DWORD x_input_get_state(DWORD dwUserIndex, XINPUT_STATE* pState);
typedef DWORD x_input_set_state(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);

DWORD XInputGetStateStub(DWORD dwUserIndex, XINPUT_STATE* pState) {
    return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD XInputSetStateStub(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration) {
    return ERROR_DEVICE_NOT_CONNECTED;
}

x_input_get_state* XInputGetState_ = XInputGetStateStub;
x_input_set_state* XInputSetState_ = XInputSetStateStub;

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

struct mouse_input {
    float rawCoordX; // NOTE: value between 0 and 1
    float rawCoordY; // NOTE: value between 0 and 1
    button buttonLeft;
    button buttonMiddle;
    button buttonRight;
};

struct gamepad_input {
    bool isAnalog;
    float analogLtThumbX;
    float analogLtThumbY;
    uint8_t triggerLeft;
    uint8_t triggerRight;
    button dpadLeft;
    button dpadRight;
    button dpadUp;
    button dpadDown;
    button buttonLtShldr;
    button buttonRtShldr;
    button buttonA;
    button buttonB;
    button buttonX;
    button buttonY;
    button buttonLtThumb;
    button buttonRtThumb;
    button buttonStart;
};

static void InitXInput() {
    HMODULE xInputLib = LoadLibraryA("xinput1_4.dll");

    if (!xInputLib) {
        xInputLib = LoadLibraryA("xinput1_3.dll");
    }

    if (!xInputLib) {
        xInputLib = LoadLibraryA("xinput9_1_0.dll");
    }

    if (xInputLib) {
        XInputGetState = (x_input_get_state*)GetProcAddress(xInputLib, "XInputGetState");
        XInputSetState = (x_input_set_state*)GetProcAddress(xInputLib, "XInputSetState");
    }
}

static void HandleKeyboardInput(platform_input *input, MSG msg) {
    bool isPressed = ((1 << 31) & msg.lParam) == 0;
    bool wasPressed = ((1 << 30) & msg.lParam) != 0;
    bool altPressed = ((1 << 29) & msg.lParam) != 0;

    if (wasPressed == isPressed) {
        // intentionally ignore repeats, only capture transitions
        // because keyboard repeat signals are slow
        return;
    }

    input->altKey.isPressed = altPressed;

    switch (msg.wParam) {
        
        case 'A' : {
            input->keyA.wasPressed = wasPressed;
            input->keyA.isPressed = isPressed;
        } break;
        
        case 'C' : {
            input->keyC.wasPressed = wasPressed;
            input->keyC.isPressed = isPressed;
        } break;
        
        case 'D' : {
            input->keyD.wasPressed = wasPressed;
            input->keyD.isPressed = isPressed;
        } break;
        
        case 'E' : {
            input->keyE.wasPressed = wasPressed;
            input->keyE.isPressed = isPressed;
        } break;
        
        case 'L' : {
            input->keyL.wasPressed = wasPressed;
            input->keyL.isPressed = isPressed;
        } break;
        
        case 'M' : {
            input->keyM.wasPressed = wasPressed;
            input->keyM.isPressed = isPressed;
        } break;
        
        case 'R' : {
            input->keyR.wasPressed = wasPressed;
            input->keyR.isPressed = isPressed;
        } break;
        
        case 'S' : {
            input->keyS.wasPressed = wasPressed;
            input->keyS.isPressed = isPressed;
        } break;

        case 'W' : {
            input->keyW.wasPressed = wasPressed;
            input->keyW.isPressed = isPressed;
        } break;

        case 'X' : {
            input->keyX.wasPressed = wasPressed;
            input->keyX.isPressed = isPressed;
        } break;

        case 'Z' : {
            input->keyZ.wasPressed = wasPressed;
            input->keyZ.isPressed = isPressed;
        } break;
        
        case VK_NUMPAD0 :
        case '0' : {
            input->key0.wasPressed = wasPressed;
            input->key0.isPressed = isPressed;
        } break;
                     
        case VK_RETURN : {
            input->enter.isPressed = isPressed;
            input->enter.wasPressed = wasPressed;
        } break;
        
        case VK_SPACE : {
            input->spacebar.wasPressed = wasPressed;
            input->spacebar.isPressed = isPressed;
        } break;
        
        case VK_UP : {
            input->keyUp.isPressed = isPressed;
            input->keyUp.wasPressed = wasPressed;
        } break;
        
        case VK_LEFT : {
            input->keyLeft.isPressed = isPressed;
            input->keyLeft.wasPressed = wasPressed;
        } break;
        
        case VK_DOWN : {
            input->keyDown.isPressed = isPressed;
            input->keyDown.wasPressed = wasPressed;
        } break;
        
        case VK_RIGHT : {
            input->keyRight.isPressed = isPressed;
            input->keyRight.wasPressed = wasPressed;
        } break;
        
        case VK_OEM_MINUS :
        case VK_SUBTRACT : {
            input->keyMinus.isPressed = isPressed;
            input->keyMinus.wasPressed = wasPressed;
        } break;
        
        case VK_OEM_PLUS :
        case VK_ADD : {
            input->keyPlus.isPressed = isPressed;
            input->keyPlus.wasPressed = wasPressed;
        } break;
        
        case VK_ESCAPE : {
            input->escKey.isPressed = isPressed;
            input->escKey.wasPressed = wasPressed;
        } break;

        case VK_CONTROL: {
            input->ctrlKey.isPressed = isPressed;
            input->ctrlKey.wasPressed = wasPressed;
        } break;

        case VK_SHIFT: {
            input->shiftKey.isPressed = isPressed;
            input->shiftKey.wasPressed = wasPressed;
        } break;

        case VK_F1 : {
             input->keyF1.isPressed = isPressed;
             input->keyF1.wasPressed = wasPressed;
        } break;

        case VK_F2 : {
             input->keyF2.isPressed = isPressed;
             input->keyF2.wasPressed = wasPressed;
        } break;

        case VK_F3 : {
             input->keyF3.isPressed = isPressed;
             input->keyF3.wasPressed = wasPressed;
        } break;

        case VK_F4 : {
            if (isPressed && altPressed) {
                isRunning = false;
            }
            input->keyF4.isPressed = isPressed;
            input->keyF4.wasPressed = wasPressed;
        } break;
                     
        case VK_F5 : {
            input->keyF5.isPressed = isPressed;
            input->keyF5.wasPressed = wasPressed;
        } break;
                     
        case VK_F6 : {
            input->keyF6.isPressed = isPressed;
            input->keyF6.wasPressed = wasPressed;
        } break;
                     
        case VK_F7 : {
            input->keyF7.isPressed = isPressed;
            input->keyF7.wasPressed = wasPressed;
        } break;
                     
        case VK_F8 : {
            input->keyF8.isPressed = isPressed;
            input->keyF8.wasPressed = wasPressed;
        } break;
                     
        case VK_F9 : {
            input->keyF9.isPressed = isPressed;
            input->keyF9.wasPressed = wasPressed;
        } break;
                     
        case VK_F10 : {
            input->keyF10.isPressed = isPressed;
            input->keyF10.wasPressed = wasPressed;
        } break;
                     
        case VK_F11 : {
            input->keyF11.isPressed = isPressed;
            input->keyF11.wasPressed = wasPressed;
        } break;
                     
        case VK_F12 : {
            input->keyF12.isPressed = isPressed;
            input->keyF12.wasPressed = wasPressed;
        } break;
                     
        case VK_PRIOR : {
            input->keyPgUp.isPressed = isPressed;
            input->keyPgUp.wasPressed = wasPressed;
        } break;
                     
        case VK_NEXT : {
            input->keyPgDown.isPressed = isPressed;
            input->keyPgDown.wasPressed = wasPressed;
        } break;

       default: { 
            // ??? what do we do with other keys? Nothing. Maybe. Sure.
        } break;
    }
}

static void HandleMouseInput(HWND window, mouse_input* mouse) {
    POINT mousePointer;
    bool gotCoords = GetCursorPos(&mousePointer);
    if (gotCoords) {
        gotCoords = ScreenToClient(window, &mousePointer);
    }

    if (gotCoords) {
        RECT rect = {0};
        GetClientRect(window, &rect);

        if (rect.right > 0) {
            int32_t clampedX = mousePointer.x < 0 ? 0 : mousePointer.x;
            if (clampedX > rect.right) {
                clampedX = rect.right;
            }
            mouse->rawCoordX = static_cast<float>(clampedX)/static_cast<float>(rect.right);
        }

        if (rect.bottom > 0) {
            int32_t clampedY = mousePointer.y < 0 ? 0 : mousePointer.y;
            if (clampedY > rect.bottom) {
                clampedY = rect.bottom;
            }
            mouse->rawCoordY = static_cast<float>(clampedY)/static_cast<float>(rect.bottom);
        }
    }

    mouse->buttonLeft.wasPressed = mouse->buttonLeft.isPressed;
    mouse->buttonLeft.isPressed = (GetKeyState(VK_LBUTTON) & (1 << 16)) != 0;
    mouse->buttonMiddle.wasPressed = mouse->buttonMiddle.isPressed;
    mouse->buttonMiddle.isPressed = (GetKeyState(VK_MBUTTON) & (1 << 16)) != 0;
    mouse->buttonRight.wasPressed = mouse->buttonRight.isPressed;
    mouse->buttonRight.isPressed = (GetKeyState(VK_RBUTTON) & (1 << 16)) != 0;
}

static void HandleGamepadInput(gamepad_input* gamepadNew, XINPUT_STATE state, gamepad_input gamepadOld) {
    XINPUT_GAMEPAD pad = state.Gamepad;
    if (pad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        gamepadNew->analogLtThumbX = ((float)(pad.sThumbLX + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))/((float)(32768 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
        gamepadNew->isAnalog = true;
    } else if (pad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        gamepadNew->analogLtThumbX = ((float)(pad.sThumbLX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))/((float)(32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
        gamepadNew->isAnalog = true;
    }

    if (pad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        gamepadNew->analogLtThumbY = ((float)(pad.sThumbLY + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))/((float)(32768 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
        gamepadNew->isAnalog = true;
    } else if (pad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        gamepadNew->analogLtThumbY = ((float)(pad.sThumbLY - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))/((float)(32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
        gamepadNew->isAnalog = true;
    }
    
    WORD buttons = pad.wButtons;

    if (!gamepadNew->isAnalog) {
        gamepadNew->dpadUp.wasPressed = gamepadOld.dpadUp.isPressed;
        gamepadNew->dpadUp.isPressed = (XINPUT_GAMEPAD_DPAD_UP & buttons) != 0;

        gamepadNew->dpadDown.wasPressed = gamepadOld.dpadDown.isPressed;
        gamepadNew->dpadDown.isPressed = (XINPUT_GAMEPAD_DPAD_DOWN & buttons) != 0;

        gamepadNew->dpadLeft.wasPressed = gamepadOld.dpadLeft.isPressed;
        gamepadNew->dpadLeft.isPressed = (XINPUT_GAMEPAD_DPAD_LEFT & buttons) != 0;

        gamepadNew->dpadRight.wasPressed = gamepadOld.dpadRight.isPressed;
        gamepadNew->dpadRight.isPressed = (XINPUT_GAMEPAD_DPAD_RIGHT & buttons) != 0;
    }

    uint8_t maxPress = 255;
    float maxWithThreshold = (float)maxPress - (float)XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    if (pad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
        float percentPressed = ((float)pad.bRightTrigger - (float)XINPUT_GAMEPAD_TRIGGER_THRESHOLD)/maxWithThreshold;
        gamepadNew->triggerRight = (uint8_t)(maxPress * percentPressed);
    } else {
        gamepadNew->triggerRight = 0; 
    }

    if (pad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
        float percentPressed = ((float)pad.bLeftTrigger - (float)XINPUT_GAMEPAD_TRIGGER_THRESHOLD)/maxWithThreshold;
        gamepadNew->triggerLeft = (uint8_t)(maxPress * percentPressed);
    } else {
        gamepadNew->triggerLeft = 0; 
    }

    gamepadNew->buttonA.wasPressed = gamepadOld.buttonA.isPressed;
    gamepadNew->buttonA.isPressed = (XINPUT_GAMEPAD_A & buttons) != 0;
    
    gamepadNew->buttonB.wasPressed = gamepadOld.buttonB.isPressed;
    gamepadNew->buttonB.isPressed = (XINPUT_GAMEPAD_B & buttons) != 0;
    
    gamepadNew->buttonX.wasPressed = gamepadOld.buttonX.isPressed;
    gamepadNew->buttonX.isPressed = (XINPUT_GAMEPAD_X & buttons) != 0;
    
    gamepadNew->buttonY.wasPressed = gamepadOld.buttonY.isPressed;
    gamepadNew->buttonY.isPressed = (XINPUT_GAMEPAD_Y & buttons) != 0;
    
    gamepadNew->buttonLtShldr.wasPressed = gamepadOld.buttonLtShldr.isPressed;
    gamepadNew->buttonLtShldr.isPressed = (XINPUT_GAMEPAD_LEFT_SHOULDER & buttons) != 0;
    
    gamepadNew->buttonRtShldr.wasPressed = gamepadOld.buttonRtShldr.isPressed;
    gamepadNew->buttonRtShldr.isPressed = (XINPUT_GAMEPAD_RIGHT_SHOULDER & buttons) != 0;
    
    gamepadNew->buttonLtThumb.wasPressed = gamepadOld.buttonLtThumb.isPressed;
    gamepadNew->buttonLtThumb.isPressed = (XINPUT_GAMEPAD_LEFT_THUMB & buttons) != 0;
    
    gamepadNew->buttonRtThumb.wasPressed = gamepadOld.buttonRtThumb.isPressed;
    gamepadNew->buttonRtThumb.isPressed = (XINPUT_GAMEPAD_RIGHT_THUMB & buttons) != 0;
    
    gamepadNew->buttonStart.wasPressed = gamepadOld.buttonStart.isPressed;
    gamepadNew->buttonStart.isPressed = (XINPUT_GAMEPAD_START & buttons) != 0;
}

static void AggregateInputs(platform_input* platformInput, gamepad_input gamepad, mouse_input mouse) {
    platformInput->isAnalog = gamepad.isAnalog;
    platformInput->analogLtThumbX = gamepad.analogLtThumbX;
    platformInput->analogLtThumbY = gamepad.analogLtThumbY;

    platformInput->dpadLeft = gamepad.dpadLeft;
    platformInput->dpadRight = gamepad.dpadRight;
    platformInput->dpadUp = gamepad.dpadUp;
    platformInput->dpadDown = gamepad.dpadDown;

    platformInput->buttonA = gamepad.buttonA;
    platformInput->buttonB = gamepad.buttonB;
    platformInput->buttonX = gamepad.buttonX;
    platformInput->buttonY = gamepad.buttonY;
    platformInput->buttonLtShldr = gamepad.buttonLtShldr;
    platformInput->buttonRtShldr = gamepad.buttonRtShldr;

    platformInput->buttonLtThumb = gamepad.buttonLtThumb;
    platformInput->buttonRtThumb = gamepad.buttonRtThumb;
    platformInput->buttonStart = gamepad.buttonStart;

    platformInput->mouseX = mouse.rawCoordX;
    platformInput->mouseY = mouse.rawCoordY;
    platformInput->mouseLeft = mouse.buttonLeft;
    platformInput->mouseMiddle = mouse.buttonMiddle;
    platformInput->mouseRight = mouse.buttonRight;
}

static void SetKeyHistory(platform_input *input) {
    input->keyA.wasPressed     = input->keyA.isPressed;
    input->keyB.wasPressed     = input->keyB.isPressed;
    input->keyC.wasPressed     = input->keyC.isPressed;
    input->keyD.wasPressed     = input->keyD.isPressed;
    input->keyE.wasPressed     = input->keyE.isPressed;
    input->keyF.wasPressed     = input->keyF.isPressed;
    input->keyG.wasPressed     = input->keyG.isPressed;
    input->keyH.wasPressed     = input->keyH.isPressed;
    input->keyI.wasPressed     = input->keyI.isPressed;
    input->keyJ.wasPressed     = input->keyJ.isPressed;
    input->keyK.wasPressed     = input->keyK.isPressed;
    input->keyL.wasPressed     = input->keyL.isPressed;
    input->keyM.wasPressed     = input->keyM.isPressed;
    input->keyN.wasPressed     = input->keyN.isPressed;
    input->keyO.wasPressed     = input->keyO.isPressed;
    input->keyP.wasPressed     = input->keyP.isPressed;
    input->keyQ.wasPressed     = input->keyQ.isPressed;
    input->keyR.wasPressed     = input->keyR.isPressed;
    input->keyS.wasPressed     = input->keyS.isPressed;
    input->keyT.wasPressed     = input->keyT.isPressed;
    input->keyU.wasPressed     = input->keyU.isPressed;
    input->keyV.wasPressed     = input->keyV.isPressed;
    input->keyW.wasPressed     = input->keyW.isPressed;
    input->keyX.wasPressed     = input->keyX.isPressed;
    input->keyY.wasPressed     = input->keyY.isPressed;
    input->keyZ.wasPressed     = input->keyZ.isPressed;

    input->key0.wasPressed     = input->key0.isPressed;
    input->key1.wasPressed     = input->key0.isPressed;
    input->key2.wasPressed     = input->key0.isPressed;
    input->key3.wasPressed     = input->key0.isPressed;
    input->key4.wasPressed     = input->key0.isPressed;
    input->key5.wasPressed     = input->key0.isPressed;
    input->key6.wasPressed     = input->key0.isPressed;
    input->key7.wasPressed     = input->key0.isPressed;
    input->key8.wasPressed     = input->key0.isPressed;
    input->key9.wasPressed     = input->key0.isPressed;
    
    input->enter.wasPressed    = input->enter.isPressed;
    input->spacebar.wasPressed = input->spacebar.isPressed;
    input->keyLeft.wasPressed  = input->keyLeft.isPressed;
    input->keyUp.wasPressed    = input->keyUp.isPressed;
    input->keyRight.wasPressed = input->keyRight.isPressed;
    input->keyDown.wasPressed  = input->keyDown.isPressed;
    input->keyPlus.wasPressed  = input->keyPlus.isPressed;
    input->keyMinus.wasPressed = input->keyMinus.isPressed;
    input->keyPgUp.wasPressed  = input->keyPgUp.isPressed;
    input->keyPgDown.wasPressed = input->keyPgDown.isPressed;
    input->escKey.wasPressed   = input->escKey.isPressed;
    input->ctrlKey.wasPressed  = input->ctrlKey.isPressed;
    input->shiftKey.wasPressed   = input->shiftKey.isPressed;
    input->altKey.wasPressed   = input->altKey.isPressed;

    input->keyF1.wasPressed    = input->keyF1.isPressed;
    input->keyF2.wasPressed    = input->keyF2.isPressed;
    input->keyF3.wasPressed    = input->keyF3.isPressed;
    input->keyF4.wasPressed    = input->keyF4.isPressed;
    input->keyF5.wasPressed    = input->keyF5.isPressed;
    input->keyF6.wasPressed    = input->keyF6.isPressed;
    input->keyF7.wasPressed    = input->keyF7.isPressed;
    input->keyF8.wasPressed    = input->keyF8.isPressed;
    input->keyF9.wasPressed    = input->keyF9.isPressed;
    input->keyF10.wasPressed   = input->keyF10.isPressed;
    input->keyF11.wasPressed   = input->keyF11.isPressed;
    input->keyF12.wasPressed   = input->keyF12.isPressed;
}

static LRESULT MainProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (uMsg) {

        case WM_ACTIVATEAPP: {
        } break;
    
        case WM_CLOSE: {
            isRunning = false;
        } break;
    
        case WM_DESTROY: {
            isRunning = false;
        } break;
    
        case WM_PAINT: {
            PAINTSTRUCT p;
            BeginPaint(hwnd, &p);
            EndPaint(hwnd, &p);
        } break;
    
        case WM_SIZE: {
        } break;

        default: {
            result = DefWindowProcA(hwnd, uMsg, wParam, lParam);
        } break;

    }

    return result;
}

static void ReadCompiledShader(const char *path, shader_stage *shdrStage) {
    HANDLE shaderFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (shaderFile == INVALID_HANDLE_VALUE) {
        OutputDebugString("error reading shader file\n");

        DWORD code = GetLastError();
        if (code == ERROR_PATH_NOT_FOUND) {
            OutputDebugString("path not found\n");
        }

        uint32_t *nope = 0;
        *nope = 1;
    }

    HRESULT success = ReadFile(shaderFile, &shdrStage->buffer, shaderBufferSize, &shdrStage->bytesRead, 0);
    if (!success) {
        // TODO: GetLastError();
    }
    CloseHandle(shaderFile);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    InitXInput(); 

    WNDCLASSEXA windClass = {};

    windClass.cbSize = sizeof(WNDCLASSEXA);
    windClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windClass.lpfnWndProc = MainProc;
    windClass.hInstance = hInstance;
    windClass.hCursor = LoadCursor(0, IDC_ARROW);
    windClass.lpszMenuName = "D3D Example\n";
    windClass.lpszClassName = "D3D Example Class\n";

    if (RegisterClassExA(&windClass)) {
        HWND window = CreateWindowA(
            windClass.lpszClassName,
            "D3D Example\n",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            hInstance,
            0
        );

        DWORD invalidControllerIndex = XUSER_MAX_COUNT + 1;
        DWORD connectedGamepadIndex = invalidControllerIndex;

        uint32_t w = 1440;
        uint32_t h = 740;

        resources resValue = {0};
        resources *res = &resValue;

        shader_stage vertex = {0};
        ReadCompiledShader("..\\d3d-example\\assets\\shaders\\QuadVertex.cso", &vertex);

        shader_stage pixel = {0};
        ReadCompiledShader("..\\d3d-example\\assets\\shaders\\QuadPixel.cso", &pixel);

        InitD3D(window, w, h, &vertex, &pixel, res);

        // NOTE: For fullscreen support:
        WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };

        gamepad_input gamepadOld = {0};
        gamepad_input gamepadNew = {0};
        mouse_input mouse = {0};
        platform_input platformInput = {0};

        isRunning = true;
        while(isRunning) {
            gamepadNew = {0};

            SetKeyHistory(&platformInput);

            MSG msg;
            
            while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_KEYDOWN || 
                    msg.message == WM_KEYUP || 
                    msg.message == WM_SYSKEYDOWN || 
                    msg.message == WM_SYSKEYUP) {
                        bool breakHere = false;
                        if (platformInput.keyA.wasPressed) {
                            breakHere = true;
                        }
                        HandleKeyboardInput(&platformInput, msg);
                } else {
                    TranslateMessage(&msg);
                    DispatchMessageA(&msg);
                }
            }

            HandleMouseInput(window, &mouse);

            XINPUT_STATE state;
            DWORD dwResult;
            if (connectedGamepadIndex == invalidControllerIndex) {
                for (DWORD gamepadIndex=0; gamepadIndex< XUSER_MAX_COUNT; gamepadIndex++ ) {
                    ZeroMemory(&state, sizeof(XINPUT_STATE));

                    dwResult = XInputGetState(gamepadIndex, &state);

                    if(dwResult == ERROR_SUCCESS) {
                        connectedGamepadIndex = gamepadIndex;
                        break;
                    } else {
                        // Controller is not connected
                    }
                }
            } else {
                dwResult = XInputGetState(connectedGamepadIndex, &state);
                if(dwResult != ERROR_SUCCESS) {
                    // Controller is not connected
                    connectedGamepadIndex = invalidControllerIndex;
                }
            }
        
            if (dwResult == ERROR_SUCCESS) {
                HandleGamepadInput(&gamepadNew, state, gamepadOld);
            }

            AggregateInputs(&platformInput, gamepadNew, mouse);

            bool toggleFullscreen = (platformInput.keyF11.isPressed && !platformInput.keyF11.wasPressed)
                || (platformInput.altKey.isPressed && platformInput.enter.isPressed && !platformInput.enter.wasPressed);
            if (toggleFullscreen) {
                // NOTE: influenced by Raymond Chen blog:
                //       https://devblogs.microsoft.com/oldnewthing/20100412-00/?p=14353 
                //       accessed May 22, 2021
                DWORD dwStyle = GetWindowLong(window, GWL_STYLE);
                if (dwStyle & WS_OVERLAPPEDWINDOW) {
                    MONITORINFO mi = { sizeof(mi) };
                    HMONITOR hmon = MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY);
                    if (GetWindowPlacement(window, &g_wpPrev) && GetMonitorInfo(hmon, &mi)) {
                        SetWindowLong(window, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
                        SetWindowPos(window, HWND_TOP, 
                                     mi.rcMonitor.left, mi.rcMonitor.top,
                                     mi.rcMonitor.right - mi.rcMonitor.left,
                                     mi.rcMonitor.bottom - mi.rcMonitor.top,
                                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
                        ShowCursor(false);
                    }
                } else {
                    SetWindowLong(window, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
                    SetWindowPlacement(window, &g_wpPrev);
                    SetWindowPos(window, 0, 0, 0, 0, 0,
                            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
                    ShowCursor(true);
                }
            }

            gamepadOld = gamepadNew;
            UpdateAndRender(res, w, h, &platformInput);
            res->swapChain->Present(1, 0);
        }

        timeEndPeriod(1);
        ReleaseAll(res);
    }

    return 0;
}
