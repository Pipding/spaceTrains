#include "UIManager.h"

static GameStateManager& _gameStateManager = GameStateManager::getInstance();

UIManager::UIManager(CombatManager* combatMan) {
    this->combatManager = combatMan;
}

void UIManager::update(float deltaTime) {
    
}


void UIManager::draw() {

}