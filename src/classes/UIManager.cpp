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

void UIManager::update(float deltaTime) {
    this->scoreText = TextFormat("SCORE: %i", this->scoreManager->getScore());
    this->speedText = TextFormat("VELOCITY: %.2f KM/S", this->combatManager->getTrain()->head()->getSpeed());
    this->healthText = TextFormat("HEALTH: %i/%i ", this->combatManager->getTrain()->getHealth(), this->combatManager->getTrain()->getMaxHealth());
}

void UIManager::draw(int screenWidth, int screenHeight) {

    if (_gameStateManager.getState() == GameState::GameOver) {
        this->drawGameOverBox();
        return;
    }

    if (_gameStateManager.getState() == GameState::Paused) {
        DrawText("Paused", 600, 340, 40, GREEN);
        return;
    }

    // Draw score, speed and health
    DrawTextEx(this->font, this->scoreText, {20, 20}, this->font.baseSize, 2, GREEN );
    DrawTextEx(this->font, this->speedText, {20, (float)this->screenHeight - 60}, 40, 2, GREEN );
    DrawTextEx(this->font, this->healthText, {20, (float)this->screenHeight - 120}, 40, 2, RED );

    this->drawTargetLockUI();
    this->drawTrainComponentSelectionUI();
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

void UIManager::drawGameOverBox() {
    // Draw the outline of the Game Over box
    DrawRectangleRounded(this->gameOverRect, 0.15f, 3, LIGHTGRAY);
    DrawRectangleRoundedLines(this->gameOverRect, 0.15f, 3, 4.f, BLACK);

    // To keep text centered, check the dimensions of the rendered text and use that to inform
    // where it will be drawn when calling DrawTextEx
    Vector2 gameOverTextSize = MeasureTextEx(this->font, this->gameOverText, this->font.baseSize, 0.f);
    Vector2 scoreTextSize = MeasureTextEx(this->font, this->scoreText, this->font.baseSize, 0.f);

    DrawTextEx(
        this->font,
        this->gameOverText,
        {this->screenWidthCentre - (gameOverTextSize.x / 2), this->screenHeightCentre - (this->gameOverRectHeight / 3)}, // position
        this->font.baseSize,
        0,
        DARKGREEN
    );

    DrawTextEx(
        this->font,
        this->scoreText,
        {this->screenWidthCentre - (scoreTextSize.x / 2), this->screenHeightCentre}, // position
        this->font.baseSize,
        0,
        DARKGREEN
    );
}

void UIManager::drawTrainComponentSelectionUI(bool vertical) {
    // How many boxes to draw
    int trainBoxesCount = this->combatManager->getTrain()->size();
    float trainBoxesWidth = 80.f;
    float gapBetweenBoxes = 20.f;
    float trainBoxesHeight = 40.f;
    float totalTrainUIWidth = (trainBoxesWidth * trainBoxesCount) + (gapBetweenBoxes * (trainBoxesCount - 1));
    float totalTrainUIHeight = (trainBoxesHeight * trainBoxesCount) + (gapBetweenBoxes * (trainBoxesCount - 1));
    float trainUIStartX = (screenWidth - totalTrainUIWidth) / 2;
    float trainUIStartY = (screenHeight - totalTrainUIHeight) / 2;

    for (int i = 0; i < trainBoxesCount; i++) {

        if (vertical) {
            DrawRectangle(screenWidth - (trainBoxesWidth * 2), trainUIStartY, trainBoxesWidth, trainBoxesHeight, this->combatManager->getTrain()->getActiveComponentIndex() == i ? GREEN : WHITE);
        } else {
            DrawRectangle(trainUIStartX, screenHeight - trainBoxesWidth, trainBoxesWidth, trainBoxesHeight, this->combatManager->getTrain()->getActiveComponentIndex() == i ? GREEN : WHITE);
        }

        if (i != 0) {
            TrainCar* car = dynamic_cast<TrainCar*>(this->combatManager->getTrain()->getComponent(i));
            if (!car->getCanShoot()) {
                if (vertical) {
                    DrawText(TextFormat("%i", (car->getTimeUntilReloaded() / 100)), trainUIStartX, screenHeight - trainBoxesWidth, 30, BLACK);
                } else {
                    DrawText(TextFormat("%i", (car->getTimeUntilReloaded() / 100)), screenWidth - (trainBoxesWidth * 2), trainUIStartY, 30, BLACK);
                }
            }
        }

        if (vertical) {
            trainUIStartY += (trainBoxesHeight + gapBetweenBoxes);
        } else {
            trainUIStartX += (trainBoxesWidth + gapBetweenBoxes);
        }
    }
}