#include "GameStateManager.h"

GameState GameStateManager::togglePaused() {
    if (this->gameState == GameState::Gameplay) {
        this->gameState = GameState::Paused;
    } else if (this->gameState == GameState::Gameplay) {
        this->gameState = GameState::Gameplay;
    }

    return this->gameState;
}