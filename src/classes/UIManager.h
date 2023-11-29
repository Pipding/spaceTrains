#pragma once

#include "CombatManager.h"
#include "ScoreManager.h"
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
   ScoreManager* scoreManager;
   Font font;

public:
    /**
     * Constructor
    */
    UIManager(CombatManager* combatManager, ScoreManager* scoreManager, Font font);

    void update(float deltaTime);

    void draw(int screenWidth, int screenHeight);
};