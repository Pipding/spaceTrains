#include "GameStateManager.h"

GameState GameStateManager::togglePaused() {
    if (this->gameState == GameState::Gameplay) {
        this->gameState = GameState::Paused;
    } else if (this->gameState == GameState::Paused) {
        this->gameState = GameState::Gameplay;
    }

    return this->gameState;
}

GameState GameStateManager::getState() {
    return this->gameState;
}

void GameStateManager::onKeyPressed(int key) {
    if (key == KEY_P) {
        this->togglePaused();
    }
}

GameState GameStateManager::setState(GameState newState) {
    this->gameState = newState;
    return this->gameState;
}