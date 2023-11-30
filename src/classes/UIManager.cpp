#include "UIManager.h"

static GameStateManager& _gameStateManager = GameStateManager::getInstance();
static AssetManager& _assets = AssetManager::getInstance();

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

    DrawText(TextFormat("Speed: %f", this->combatManager->getTrain()->head()->getSpeed()), 20, screenHeight - 60, 40, GREEN);
    DrawText(TextFormat("Health: %i", this->combatManager->getPlayerHealth()), 20, screenHeight - 120, 40, RED);

    this->drawTargetLockUI();

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

void UIManager::drawTargetLockUI() {

    if (!this->combatManager->hasTarget()) {
        DrawTexture(*_assets.getTextureRef("no_target_ui"), 0.f, 200.f, WHITE);
        return;
    }

    if(this->combatManager->getTargetLocked()) {
        DrawTexture(*_assets.getTextureRef("target_locked_ui"), 0.f, 200.f, WHITE);
    } else {
        DrawTexture(*_assets.getTextureRef("target_available_ui"), 0.f, 200.f, WHITE);
    }
}