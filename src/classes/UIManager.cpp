#include "UIManager.h"

static GameStateManager& _gameStateManager = GameStateManager::getInstance();
static AssetManager& _assets = AssetManager::getInstance();

UIManager::UIManager(int screenWidth, int screenHeight, CombatManager* combatMan, ScoreManager* scoreMan, Font font)
    : screenWidth(screenWidth), screenHeight(screenHeight), combatManager(combatMan), scoreManager(scoreMan), font(font) {
        this->screenWidthCentre = screenWidth / 2;
        this->screenHeightCentre = screenHeight / 2;

        this->gameOverRect = {
                                this->screenWidthCentre - (this->gameOverRectWidth / 2),
                                this->screenHeightCentre - (this->gameOverRectHeight / 2),
                                (float)this->gameOverRectWidth,
                                (float)this->gameOverRectHeight
                            };
}

void UIManager::update(float deltaTime) { }

void UIManager::draw(int screenWidth, int screenHeight) {

    if (_gameStateManager.getState() == GameState::GameOver) {
        // Draw the outline of the Game Over box
        DrawRectangleRounded(this->gameOverRect, 0.15f, 3, WHITE);
        DrawRectangleRoundedLines(this->gameOverRect, 0.15f, 3, 4.f, BLACK);

        // To keep text centered, check the dimensions of the rendered text and use that to inform
        // where it will be drawn when calling DrawTextEx
        Vector2 gameOverTextSize = MeasureTextEx(this->font, this->gameOverText, this->font.baseSize, 0.f);

        DrawTextEx(
            this->font,
            "GAME OVER",
            {this->screenWidthCentre - (gameOverTextSize.x / 2), this->screenHeightCentre - (this->gameOverRectHeight / 3)}, // position
            this->font.baseSize,
            0,
            GREEN
        );
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