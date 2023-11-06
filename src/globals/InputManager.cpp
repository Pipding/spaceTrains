#include "InputManager.h"

void InputManager::addListener(IKeyboardListener* listener, int key, GameState state) {
    this->listeners[key] = listener;
    this->listenedKeys.push_back(key);
}

void InputManager::addListeners(IKeyboardListener* listener, std::vector<int> keys, GameState state) {
    for (std::vector<int>::iterator it = keys.begin(); it != keys.end(); ++it) {
        this->listeners[*it] = listener;
        this->listenedKeys.push_back(*it);
    }
}

void InputManager::update() {
    // This iterates through all keys with a registered listener & if the key has been pressed, it triggers the onKeyPressed method ot the listener
    // The same is done for released keys via the onKeyReleased method
    for (std::vector<int>::iterator it = this->listenedKeys.begin(); it != listenedKeys.end(); ++it) {
        if (IsKeyPressed(*it)) {
            this->listeners[*it]->onKeyPressed(*it);
        } else if (IsKeyReleased(*it)) {
            this->listeners[*it]->onKeyReleased(*it);
        }
    }
}