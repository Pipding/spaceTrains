#include "raylib.h"
#include "raymath.h"
#include "src/classes/CombatManager.h"
#include "src/classes/FollowCam.h"
#include "src/classes/ParallaxBackground.h"
#include "src/classes/UIManager.h"
#include "src/globals/AssetManager.h"
#include "src/globals/GameStateManager.h"
#include "src/globals/InputManager.h"
#include "src/globals/SpaceTrainDebug.h"

// Setting up singletons
static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();
static AssetManager& _assets = AssetManager::getInstance();
static InputManager& _input = InputManager::getInstance();
static GameStateManager& _gameState = GameStateManager::getInstance();

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
    SetRandomSeed((unsigned)time(NULL)); // Method for generating a random seed taken from StackOverflow: https://stackoverflow.com/a/11765384

    // Assets are loaded through a custom AssetManager singleton class. There is only one instance 
    // of this class across the entire application and it can be referenced wherever needed. It has
    // a utility function which loads all the assets required for the game
    _assets.loadAssets();

    /**
     * The Train is the player character. The Train consists of a collection of
     * TrainComponents (1 TrainEngine and n TrainCars)
     * Here we instantiate a TrainEngine and some TrainCars and use them to
     * construct a train
    */
    TrainEngine engine(
        _assets.getModel("train_engine"),       // Model
        _assets.getTexture("train_engine"),     // Texture
        0.2f,                                   // Acceleration
        16.f,                                   // Deceleration
        2.f,                                    // Top speed
        2.5f                                    // Rotation rate
    );

    TrainCar carriage1(
        _assets.getModel("train_wagon"),
        _assets.getTexture("wagon"),
        &engine,
        6.f,
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

    // Train train({&engine, &carriage1, &carriage2}, 100); // The player character
    Train train({&engine, &carriage1, &carriage2}, 100); // The player character

    // Create a parallax background
    ParallaxBackground background(&train.head()->position, 2, _assets.getTextureRef("galaxy"));
    background.setLayerOffset(0, {1.f, 1.f});
    background.setLayerOffset(1, {5.f, 5.f});

    // Camera which follows the player character
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
    _input.addListener(&_debug, KEY_M, GameState::Stateless);
    _input.addListeners(&engine, {KEY_W, KEY_S, KEY_A, KEY_D}, GameState::Gameplay);
    _input.addListener(&followCam, KEY_R, GameState::Gameplay);
    _input.addListener(&_gameState, KEY_P, GameState::Stateless);
    _input.addListeners(&combatManager, {KEY_LEFT_SHIFT, KEY_SPACE, KEY_UP, KEY_DOWN}, GameState::Gameplay);

    // Set the initial game state. This acts as a loading screen of sorts. Game logic will not
    // be updated until GameState has been set here
    _gameState.setState(GameState::Gameplay);

    // ==================================================
    // Main game loop
    // ==================================================
    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        // Take user input and update the UI
        _input.update(deltaTime);
        uiManager.update(deltaTime);

        // Only update gameplay objects during gameplay
        if (_gameState.getState() == GameState::Gameplay) {
            train.update(deltaTime);
            followCam.update(deltaTime);
            combatManager.update(deltaTime);
        }

        // Start drawing
        BeginDrawing();
            ClearBackground(BLACK);

            // Draw the game world only when the game is in a state where it should be visible, e.g. not at the main menu
            // In practice this means all game states because main menu is not implemented
            if (_gameState.getState() == GameState::Gameplay || _gameState.getState() == GameState::Paused || _gameState.getState() == GameState::GameOver) {
                BeginMode3D(followCam.camera);
                    train.draw();
                    followCam.draw();
                    combatManager.draw();
                    background.draw();

                    if (_debug.getDrawBoundingBoxes()) {
                        DrawGrid(2000, 2.f);
                    }

                EndMode3D();
            }

            // UI needs to be drawn outside of 3D mode and regardless of game state
            // because UIManager does its own internal checks for game state
            uiManager.draw(screenWidth, screenHeight);

        EndDrawing();
    }

    _assets.unloadAll();
    CloseWindow();
    return 0;
}
