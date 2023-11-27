#include "UIManager.h"

static GameStateManager& _gameStateManager = GameStateManager::getInstance();

UIManager::UIManager(CombatManager* combatMan) {
    this->combatManager = combatMan;
}

void UIManager::update(float deltaTime) {
    
}

void UIManager::draw(int screenWidth, int screenHeight) {
    DrawText(TextFormat("Velocity: %f", Vector3Length(this->combatManager->getTrain()->head()->velocity)), 20, 20, 40, GREEN);

    DrawText(TextFormat("Health: %i", this->combatManager->getPlayerHealth()), 20, screenHeight - 60, 40, RED);
}