#pragma once

#include "CombatManager.h"
#include "ScoreManager.h"
#include "src/globals/GameStateManager.h"
#include "src/globals/AssetManager.h"

class UIManager: public IUpdatable {

private:
    int screenWidth;
    int screenHeight;
    float screenWidthCentre;
    float screenHeightCentre;
    CombatManager* combatManager;
    ScoreManager* scoreManager;
    Font font;

    int gameOverRectWidth = 640;
    int gameOverRectHeight = 360;

    Rectangle gameOverRect;

    const char* gameOverText = "GAME OVER";
    const char* scoreText;

    /**
     * Draws the UI for the targeting system
    */
    void drawTargetLockUI();

public:
    /**
    * Constructor
    */
    UIManager(int screenWidth, int screenHeight, CombatManager* combatManager, ScoreManager* scoreManager, Font font);

    void update(float deltaTime);

    void draw(int screenWidth, int screenHeight);
};