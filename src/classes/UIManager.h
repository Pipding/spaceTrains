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
    const char* healthText = "HEALTH";
    const char* scoreText = "SCORE";
    const char* speedText = "VELOCITY";

    /**
     * Draws the UI for the targeting system
    */
    void drawTargetLockUI();

    /**
     * Draws the game over screen with the player's score
    */
    void drawGameOverBox();

public:
    /**
    * Constructor
    */
    UIManager(int screenWidth, int screenHeight, CombatManager* combatManager, ScoreManager* scoreManager, Font font);

    void update(float deltaTime);

    void draw(int screenWidth, int screenHeight);
};