#include "InputManager.h"

void InputManager::addListener(IKeyboardListener* listener, int key, GameState state) {
    this->listeners[key] = listener;
    this->listenedKeysByGameState[state].push_back(key);
}

void InputManager::addListeners(IKeyboardListener* listener, std::vector<int> keys, GameState state) {
    for (std::vector<int>::iterator it = keys.begin(); it != keys.end(); ++it) {
        this->addListener(listener, *it, state);
    }
}

void InputManager::update(float deltaTime) {
    // TODO: Implement deltaTime

    std::vector<int> keysValidInThisGameState = this->listenedKeysByGameState[GameStateManager::getInstance().getState()];

    // This iterates through all keys with a registered listener & if the key has been pressed, it triggers the onKeyPressed method ot the listener
    // The same is done for released keys via the onKeyReleased method
    for (std::vector<int>::iterator it = keysValidInThisGameState.begin(); it != keysValidInThisGameState.end(); ++it) {
        if (IsKeyPressed(*it)) {
            this->listeners[*it]->onKeyPressed(*it);
        } else if (IsKeyReleased(*it)) {
            this->listeners[*it]->onKeyReleased(*it);
        }
    }

    //Stateless keys are valid in all game states
    // TODO: This is a near-exact copy of the loop above
    for (std::vector<int>::iterator it = this->listenedKeysByGameState[GameState::Stateless].begin(); it != this->listenedKeysByGameState[GameState::Stateless].end(); ++it) {
        if (IsKeyPressed(*it)) {
            this->listeners[*it]->onKeyPressed(*it);
        } else if (IsKeyReleased(*it)) {
            this->listeners[*it]->onKeyReleased(*it);
        }
    }
}