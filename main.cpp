#include "raylib.h"
#include "raymath.h"
#include "src/classes/Actor.h"
#include "src/classes/Hostile.h"
#include "src/classes/FollowCam.h"
#include "src/classes/TrainEngine.h"
#include "src/classes/TrainCar.h"
#include "src/globals/GameStateManager.h"
#include "src/globals/InputManager.h"
#include "src/globals/SpaceTrainDebug.h"

// Setting up singletons
static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();
static InputManager& _inputManager = InputManager::getInstance();
static GameStateManager& _gameStateManager = GameStateManager::getInstance();

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
    Model duckModel = LoadModel("assets/models/ducky.obj");
    Texture2D duckTexture = LoadTexture("assets/textures/ducky_albedo.png");

    TrainEngine engine(duckModel, duckTexture, 10.f, 50000.f, 20.f, 0.05f);

    TrainCar carriage(duckModel, duckTexture, &engine, {-50.f, 0.f, 0.f});
    TrainCar carriage2(duckModel, duckTexture, &carriage, {-100.f, 0.f, 0.f});

    // This borrowed from the models_box_collisions example: https://github.com/raysan5/raylib/blob/master/examples/models/models_box_collisions.c
    Vector3 upgradeTowerPos = { 120.0f, 0.f, 120.f };
    Vector3 upgradeTowerSize = { 50.0f, 100.0f, 50.0f };

    Hostile hostile({1000.f, 0.f, 1000.f}, duckModel, duckTexture, &engine.position);

    bool collision = false;

    // Camera which follows the player-controlled TrainEngine
    FollowCam followCam(&engine, {-150.0f, 100.f, 0.0f});

    // ==================================================
    // Register input listeners
    // ==================================================
    _inputManager.addListener(&_debug, KEY_M, GameState::Stateless);
    _inputManager.addListeners(&engine, {KEY_W, KEY_S, KEY_A, KEY_D}, GameState::Gameplay);
    _inputManager.addListener(&followCam, KEY_R, GameState::Gameplay);
    _inputManager.addListener(&_gameStateManager, KEY_P, GameState::Stateless);

    // Set the game state
    _gameStateManager.setState(GameState::Gameplay);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        const float deltaTime = GetFrameTime();

        // Take user input
        _inputManager.update();

        if (_gameStateManager.getState() == GameState::Gameplay) {

            // Lock on to an enemy with right click
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                followCam.setTarget(&hostile);
            } else {
                followCam.unsetTarget();
            }

            if (IsKeyDown(KEY_SPACE) && followCam.getHasTarget()) {
                hostile.receiveDamage(2);
            }

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
    // duck.unload();
    engine.unload(); // TODO: If multiple Actors use the same model/texture then attempting to unload something after it has already been unloaded will cause a segfault
    CloseWindow();

    return 0;
}
