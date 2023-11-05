#pragma once

class IKeyboardListener {
public:
    /**
     * Callback which will be invoked whenever a key pressed event this class listens to is triggered
     * @param key   The key which was pressed
    */
    virtual void onKeyPressed(int key) = 0;

    /**
     * Callback which will be invoked whenever a key released event this class listens to is triggered
     * @param key   The key which was released
    */
    virtual void onKeyReleased(int key) = 0;
};