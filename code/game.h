#pragma once

struct button {
    bool wasPressed;
    bool isPressed;
};

bool JustPressed(button btn) {
    return btn.isPressed && !btn.wasPressed;
}

bool BeenPressed(button btn) {
    return btn.isPressed && btn.wasPressed;
}

bool WasPressed(button btn) {
    return !btn.isPressed && btn.wasPressed;
}

button operator ||(button b1, button b2) {
    button result = {};
    result.isPressed = b1.isPressed || b2.isPressed;
    result.wasPressed = b1.wasPressed || b2.wasPressed;
    return result;
}

struct platform_input {
        // keyboard input
        button keyA;
        button keyB;
        button keyC;
        button keyD;
        button keyE;
        button keyF;
        button keyG;
        button keyH;
        button keyI;
        button keyJ;
        button keyK;
        button keyL;
        button keyM;
        button keyN;
        button keyO;
        button keyP;
        button keyQ;
        button keyR;
        button keyS;
        button keyT;
        button keyU;
        button keyV;
        button keyW;
        button keyX;
        button keyY;
        button keyZ;

        button key0;
        button key1;
        button key2;
        button key3;
        button key4;
        button key5;
        button key6;
        button key7;
        button key8;
        button key9;

        button enter;
        button spacebar;
        button keyLeft;
        button keyRight;
        button keyUp;
        button keyDown;
        button keyPgUp;
        button keyPgDown;
        button keyPlus;
        button keyMinus;
        button escKey;
        button ctrlKey;
        button shiftKey;
        button altKey;

        button keyF1;
        button keyF2;
        button keyF3;
        button keyF4;
        button keyF5;
        button keyF6;
        button keyF7;
        button keyF8;
        button keyF9;
        button keyF10;
        button keyF11;
        button keyF12;

        // mouse input
        float mouseX;
        float mouseY;
        button mouseLeft;
        button mouseMiddle;
        button mouseRight;

        // gamepad input
        bool isAnalog;
        float analogLtThumbX;
        float analogLtThumbY;
        button dpadUp;
        button dpadDown;
        button dpadLeft;
        button dpadRight;
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

