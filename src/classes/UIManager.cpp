#include "UIManager.h"

static GameStateManager& _gameStateManager = GameStateManager::getInstance();

UIManager::UIManager(CombatManager* combatMan, ScoreManager* scoreMan, Font font) {
    this->combatManager = combatMan;
    this->scoreManager = scoreMan;
    this->font = font;
}

void UIManager::update(float deltaTime) { }

void UIManager::draw(int screenWidth, int screenHeight) {

    if (_gameStateManager.getState() == GameState::GameOver) {
        DrawText("GameOver", 600, 340, 40, GREEN);
        return;
    }

    if (_gameStateManager.getState() == GameState::Paused) {
        DrawText("Paused", 600, 340, 40, GREEN);
        return;
    }

    DrawTextEx(this->font, TextFormat("Score: %i", this->scoreManager->getScore()), {20, 20}, this->font.baseSize, 2, GREEN );

    // DrawText(TextFormat("Score: %i", this->scoreManager->getScore()), 20, 20, 40, GREEN);

    DrawText(TextFormat("Speed: %f", Vector3Length(this->combatManager->getTrain()->head()->velocity)), 20, screenHeight - 60, 40, GREEN);
    DrawText(TextFormat("Health: %i", this->combatManager->getPlayerHealth()), 20, screenHeight - 120, 40, RED);

    // ==================================================
    // Target box
    // ==================================================
    DrawRectangle(0.f, 200.f, 200.f, 200.f, WHITE);
    DrawText(TextFormat("Target"), 0.f, 200.f, 30, BLUE);
    if (this->combatManager->hasTarget()) {
        DrawText(TextFormat("%i", this->combatManager->getActiveTarget()), 0.f, 300.f, 30, this->combatManager->getTargetLocked() ? RED : GREEN);
    }

    // ==================================================
    // Train UI
    // ==================================================
    // How many boxes to draw
    int trainBoxesCount = this->combatManager->getTrain()->size();
    float trainBoxesWidth = 80.f;
    float gapBetweenBoxes = 20.f;
    float trainBoxesHeight = 40.f;
    float totalTrainUIWidth = (trainBoxesWidth * trainBoxesCount) + (gapBetweenBoxes * (trainBoxesCount - 1));
    float trainUIStartX = (screenWidth - totalTrainUIWidth) / 2;

    for (int i = 0; i < trainBoxesCount; i++) {
        DrawRectangle(trainUIStartX, screenHeight - trainBoxesWidth, trainBoxesWidth, trainBoxesHeight, this->combatManager->getTrain()->getActiveComponentIndex() == i ? GREEN : WHITE);

        if (i != 0) {
            TrainCar* car = dynamic_cast<TrainCar*>(this->combatManager->getTrain()->getComponent(i));
            if (!car->getCanShoot()) {
                DrawText(TextFormat("%i", (car->getTimeUntilReloaded() / 100)), trainUIStartX, screenHeight - trainBoxesWidth, 30, BLACK);
            }
        }

        trainUIStartX += (trainBoxesWidth + gapBetweenBoxes);
    }
}