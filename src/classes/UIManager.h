#pragma once

#include "CombatManager.h"
#include "ScoreManager.h"
#include "src/globals/GameStateManager.h"

class UIManager: public IUpdatable {

private:
   CombatManager* combatManager;
   ScoreManager* scoreManager;
   Font font;

    /**
     * Draws the UI for the targeting system
    */
   void drawTargetLockUI();

public:
    /**
     * Constructor
    */
    UIManager(CombatManager* combatManager, ScoreManager* scoreManager, Font font);

    void update(float deltaTime);

    void draw(int screenWidth, int screenHeight);
};