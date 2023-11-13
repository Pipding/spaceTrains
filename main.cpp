#include "raylib.h"
#include "raymath.h"
#include "src/classes/Actor.h"
#include "src/classes/CombatManager.h"
#include "src/classes/Hostile.h"
#include "src/classes/FollowCam.h"
#include "src/classes/TrainEngine.h"
#include "src/classes/TrainCar.h"
#include "src/globals/AssetManager.h"
#include "src/globals/GameStateManager.h"
#include "src/globals/InputManager.h"
#include "src/globals/SpaceTrainDebug.h"

// Setting up singletons
static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();
static AssetManager& _assets = AssetManager::getInstance();
static InputManager& _inputManager = InputManager::getInstance();
static GameStateManager& _gameStateManager = GameStateManager::getInstance();

/**
 * TODO
 * - Update CombatManager to have an activeTrainComponent member
 * - Update how the shoot button works so the CombatManager handles damage
 * - Allow activeTrainComponent to change with key presses
 * - Have the camera follow activeTrainComponent
 * - Add train models
 * - Add turret models
 * - Make turret models rotate to face the direction the camera faces
 * - Create IMouseListener so things can take mouse input properly
*/

int main(void)
{
    //==================================================
    // Initialization
    //==================================================
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "SPACE TRAIN");
    SetTargetFPS(60);
    DisableCursor();

    // TODO: Remove the ducky (both code and assets)
    // Model & texture come from https://www.cgtrader.com/items/2033848/download-page
    _assets.loadModel("assets/models/ducky.obj", "duck");
    _assets.loadTexture("assets/textures/ducky_albedo.png", "duck");

    TrainEngine engine(_assets.getModel("duck"), _assets.getTexture("duck"), 10.f, 50000.f, 20.f, 0.05f);
    TrainCar carriage(_assets.getModel("duck"), _assets.getTexture("duck"), &engine, {-50.f, 0.f, 0.f});
    TrainCar carriage2(_assets.getModel("duck"), _assets.getTexture("duck"), &carriage, {-100.f, 0.f, 0.f});

    // Camera which follows the player-controlled TrainEngine
    FollowCam followCam(&engine, {-150.0f, 100.f, 0.0f});

    // This borrowed from the models_box_collisions example: https://github.com/raysan5/raylib/blob/master/examples/models/models_box_collisions.c
    Vector3 upgradeTowerPos = { 120.0f, 0.f, 120.f };
    Vector3 upgradeTowerSize = { 50.0f, 100.0f, 50.0f };

    //==================================================
    // Managers
    //==================================================
    CombatManager combatManager = CombatManager(&followCam, &engine);

    Hostile hostile({1000.f, 0.f, 1000.f}, _assets.getModel("duck"), _assets.getTexture("duck"), &engine.position);
    combatManager.addHostile(&hostile);

    bool collision = false;

    // ==================================================
    // Register input listeners
    // ==================================================
    _inputManager.addListener(&_debug, KEY_M, GameState::Stateless);
    _inputManager.addListeners(&engine, {KEY_W, KEY_S, KEY_A, KEY_D}, GameState::Gameplay);
    _inputManager.addListener(&followCam, KEY_R, GameState::Gameplay);
    _inputManager.addListener(&_gameStateManager, KEY_P, GameState::Stateless);
    _inputManager.addListeners(&combatManager, {KEY_LEFT_SHIFT, KEY_SPACE}, GameState::Gameplay);

    // Set the game state
    _gameStateManager.setState(GameState::Gameplay);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        const float deltaTime = GetFrameTime();

        // Take user input
        _inputManager.update();

        if (_gameStateManager.getState() == GameState::Gameplay) {
            // Box collision check based on the models_box_collisions example: https://github.com/raysan5/raylib/blob/master/examples/models/models_box_collisions.c
            collision = CheckCollisionBoxes(
                engine.getBounds(),
                (BoundingBox) {
                    (Vector3){ upgradeTowerPos.x - upgradeTowerSize.x/2, upgradeTowerPos.y - upgradeTowerSize.y/2, upgradeTowerPos.z - upgradeTowerSize.z/2 },
                    (Vector3){ upgradeTowerPos.x + upgradeTowerSize.x/2, upgradeTowerPos.y + upgradeTowerSize.y/2, upgradeTowerPos.z + upgradeTowerSize.z/2 }
                }
            );

            if (collision) {
                engine.color = RED;
            } else {
                engine.color = WHITE;
            }

            // TODO: Bounding box is axis-aligned, so it doesn't rotate with the model. Unsure what if anything to do about this atm
            engine.update(deltaTime);
            carriage.update();
            carriage2.update();
            followCam.update();
            hostile.update();

            combatManager.update();
        }

        if (_gameStateManager.getState() == GameState::Gameplay || _gameStateManager.getState() == GameState::Paused) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            
            BeginMode3D(followCam.camera);
            DrawCube(upgradeTowerPos, upgradeTowerSize.x, upgradeTowerSize.y, upgradeTowerSize.z, GRAY);
            engine.draw();
            carriage.draw();
            carriage2.draw();
            hostile.draw();
            followCam.draw();
            combatManager.draw();
            DrawGrid(2000, 20.f);
            EndMode3D();

            DrawText(TextFormat("Velocity: %f, %f", engine.velocity.x, engine.velocity.z), 20, 20, 40, GREEN);

            if (_gameStateManager.getState() == GameState::Paused) {
                DrawText("Paused", 600, 340, 40, GREEN);
            }

            EndDrawing();
        }
    }

    //==================================================
    // De-initialization
    //==================================================
    _assets.unloadAll();
    CloseWindow();

    return 0;
}
