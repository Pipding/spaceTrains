#pragma once

class IKeyboardListener {
public:
    virtual void onKeyPressed(int key) = 0;
    virtual void onKeyReleased(int key) = 0;
};