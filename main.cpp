#include "raylib.h"
#include "raymath.h"
#include "src/classes/Actor.h"
#include "src/classes/CombatManager.h"
#include "src/classes/Hostile.h"
#include "src/classes/FollowCam.h"
#include "src/classes/Powerup.h"
#include "src/classes/TrainEngine.h"
#include "src/classes/TrainCar.h"
#include "src/classes/UIManager.h"
#include "src/globals/AssetManager.h"
#include "src/globals/GameStateManager.h"
#include "src/globals/InputManager.h"
#include "src/globals/SpaceTrainDebug.h"

// Setting up singletons
static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();
static AssetManager& _assets = AssetManager::getInstance();
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
    InitAudioDevice();
    SetTargetFPS(60);
    DisableCursor();

    // Method for generating a random seed taken from StackOverflow: https://stackoverflow.com/a/11765384
    SetRandomSeed((unsigned)time(NULL));

    //==================================================
    // Load assets
    //==================================================
    _assets.loadAssets();

    Model galaxyBg = _assets.getModel("plane");
    galaxyBg.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *_assets.getTextureRef("galaxy");

    //==================================================
    // Create the Train (player character) by creating a 
    // TrainEngine and some TrainCars. Then use them to 
    // create a Train
    //==================================================
    TrainEngine engine(_assets.getModel("train_engine"), _assets.getTexture("train_engine"), 0.1f, 16.f, 2.f, 2.5f);

    TrainCar carriage1(
        _assets.getModel("train_wagon"),
        _assets.getTexture("wagon"),
        &engine,
        12.f,
        20,
        3000,
        _assets.getModel("missile"),
        _assets.getTexture("missile"),
        "missile_fire",
        "explosion"
    );

    TrainCar carriage2(
        _assets.getModel("train_wagon"),
        _assets.getTexture("wagon"),
        &carriage1,
        10.5f,
        5,
        200,
        _assets.getModel("bullet"),
        _assets.getTexture("bullet"),
        "bullet_fire",
        "laser_hit"
    );

    Train train({&engine, &carriage1, &carriage2}, 100);

    //==================================================
    // Camera which follows the player character
    //==================================================
    FollowCam followCam(&engine, {-17.5f, 12.5f, 0.0f});

    //==================================================
    // Managers
    //==================================================
    ScoreManager scoreManager = ScoreManager();
    CombatManager combatManager = CombatManager(&followCam, &train, &scoreManager);
    UIManager uiManager = UIManager(screenWidth, screenHeight, &combatManager, &scoreManager, _assets.getFont("space_wham"));

    // ==================================================
    // Register input listeners. These are game objects
    // which need to react to user input
    // ==================================================
    // TODO: Bit redundant to have both addListener and addListeners. Maybe overload addListeners
    _inputManager.addListener(&_debug, KEY_M, GameState::Stateless);
    _inputManager.addListeners(&engine, {KEY_W, KEY_S, KEY_A, KEY_D}, GameState::Gameplay);
    _inputManager.addListener(&followCam, KEY_R, GameState::Gameplay);
    _inputManager.addListener(&_gameStateManager, KEY_P, GameState::Stateless);
    _inputManager.addListeners(&combatManager, {KEY_LEFT_SHIFT, KEY_SPACE, KEY_UP, KEY_DOWN}, GameState::Gameplay);

    // ==================================================
    // Set the initial game state
    // ==================================================
    _gameStateManager.setState(GameState::Gameplay);

    // ==================================================
    // Main game loop
    // ==================================================
    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        // Take user input
        _inputManager.update(deltaTime);

        // Only update gameplay objects during gameplay
        if (_gameStateManager.getState() == GameState::Gameplay) {
            train.update(deltaTime);
            followCam.update(deltaTime);
            combatManager.update(deltaTime);
            uiManager.update(deltaTime);
        }

        if (_gameStateManager.getState() == GameState::Gameplay || _gameStateManager.getState() == GameState::Paused || _gameStateManager.getState() == GameState::GameOver) {
            BeginDrawing();

            ClearBackground(BLACK);
            
            BeginMode3D(followCam.camera);
                train.draw();
                followCam.draw();
                combatManager.draw();

                if (_debug.getDrawBoundingBoxes()) {
                    DrawGrid(2000, 2.f);
                }

                DrawModel(_assets.getModel("plane"), {engine.position.x + (engine.position.x * -1 / 50) , -200.f, engine.position.z + engine.position.z * -1 / 50}, 1.f, WHITE);
                DrawModel(_assets.getModel("plane"), {engine.position.x + (engine.position.x * -1 / 25) , -100.f, engine.position.z + engine.position.z * -1 / 25}, 1.f, WHITE);
            EndMode3D();

            // UI needs to be drawn outside of 3D mode
            uiManager.draw(screenWidth, screenHeight);

            EndDrawing();
        }
    }

    _assets.unloadAll();
    CloseWindow();
    return 0;
}
