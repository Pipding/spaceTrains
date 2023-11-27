#pragma once

#include "CombatManager.h"
#include "src/globals/GameStateManager.h"

class UIManager: public IUpdatable {

    /**
     * What does a UI manager do?
     * He draws;
     * - Player health
     * - Player speed
     * - Player score
     * - Target box
     * - The train indicator
    */
private:
   CombatManager* combatManager;

public:
    /**
     * Constructor
    */
    UIManager(CombatManager* combatManager);

    void update(float deltaTime);

    void draw();
};