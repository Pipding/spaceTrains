#include "raylib.h"
#include "raymath.h"
#include "src/classes/Actor.h"
#include "src/classes/CombatManager.h"
#include "src/classes/Hostile.h"
#include "src/classes/FollowCam.h"
#include "src/classes/PowerUp.h"
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

/**
 * TODO
 *  MUST
 *      - Add "Game Over" screen
 *      - Implement more than one powerup type and have them randomly spawn
 *      - Add sound effect when a projectile hits
 *      - Add engine SFX
 *      - Improve lock-on UI
 *      - Make train component selection UI vertical
 *      - Add train models
 *      - Add turret models
 *      - Make turret models rotate to face the direction the camera faces
 *      - Improve enemy AI and shooting behaviour
 *      - Comments in all .h files
 *      - Organise code
 * 
 *  SHOULD
 *      - Limit rotation of the engine based on the car behind it
 *      - Limit lock-on by distance
 *      - ICombatant should be virtual/abstract in the same way IUpdatable is
 *      - Add some more rays for target detection so it's less finicky
 *      - Add UI to show when a target can be locked
 *      - Make repeat fire work if the fire button is held down
 *      - Update selected component UI so it's less confusing
 *      - Create IMouseListener so things can take mouse input properly
 *          - Update FollowCam to implement IMouseListener
 * 
 *  COULD
 *      - Add UI to show damage being dealt to enemies
 *      - Add UI to show damage being dealt to player
 *      - Add a health bar
 *      - Add a menu system
 *      - Implement an UpdateManager
 * 
 *  WON'T
 *      - Update target display UI to show a visual representation of the current target
 *      - Add DR and DT to Hostiles
 *      - Add DR and DT to the train
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

    //==================================================
    // Load assets
    //==================================================
    // Duck model & texture come from https://www.cgtrader.com/items/2033848/download-page
    _assets.loadModel("assets/models/ducky.obj", "duck");
    _assets.loadModel("assets/models/missile1.obj", "missile1");
    _assets.loadModel("assets/models/health_powerup.obj", "health_powerup");
    _assets.loadModel("assets/models/speed_powerup.obj", "speed_powerup");

    _assets.loadTexture("assets/textures/ducky_albedo.png", "duck");
    _assets.loadTexture("assets/textures/missile1_albedo.png", "missile1");
    _assets.loadTexture("assets/textures/health_powerup_albedo.png", "health_powerup");
    _assets.loadTexture("assets/textures/speed_powerup_albedo.png", "speed_powerup");

    _assets.loadSound("assets/sounds/missile_fire.wav", "missile_fire");

    _assets.loadFont("assets/fonts/space-wham.ttf", "space_wham");

    TrainEngine engine(_assets.getModel("duck"), _assets.getTexture("duck"), 10.f, 80.f, 20.f, 2.5f);

    TrainCar carriage(
        _assets.getModel("duck"),
        _assets.getTexture("duck"),
        &engine,
        {-50.f, 0.f, 0.f},
        20,
        3000,
        _assets.getModel("missile1"),
        _assets.getTexture("missile1")
    );

    TrainCar carriage2(
        _assets.getModel("duck"),
        _assets.getTexture("duck"),
        &carriage, {-100.f, 0.f, 0.f},
        5,
        200,
        _assets.getModel("missile1"),
        _assets.getTexture("missile1")
    );

    Train train({&engine, &carriage, &carriage2}, 100);

    // Camera which follows the player-controlled TrainEngine
    FollowCam followCam(&engine, {-150.0f, 100.f, 0.0f});

    //==================================================
    // Managers
    //==================================================
    ScoreManager scoreManager = ScoreManager();
    CombatManager combatManager = CombatManager(&followCam, &train, &scoreManager);
    UIManager uiManager = UIManager(&combatManager, &scoreManager, _assets.getFont("space_wham"));

    // ==================================================
    // Register input listeners
    // ==================================================
    _inputManager.addListener(&_debug, KEY_M, GameState::Stateless);
    _inputManager.addListeners(&engine, {KEY_W, KEY_S, KEY_A, KEY_D}, GameState::Gameplay);
    _inputManager.addListener(&followCam, KEY_R, GameState::Gameplay);
    _inputManager.addListener(&_gameStateManager, KEY_P, GameState::Stateless);
    _inputManager.addListeners(&combatManager, {KEY_LEFT_SHIFT, KEY_SPACE, KEY_UP, KEY_DOWN}, GameState::Gameplay);

    // Set the game state
    _gameStateManager.setState(GameState::Gameplay);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        const float deltaTime = GetFrameTime();

        // Take user input
        _inputManager.update(deltaTime);

        // Only update gameplay objects during gameplay
        if (_gameStateManager.getState() == GameState::Gameplay) {
            train.update(deltaTime);
            followCam.update(deltaTime);
            combatManager.update(deltaTime);
        }

        if (_gameStateManager.getState() == GameState::Gameplay || _gameStateManager.getState() == GameState::Paused || _gameStateManager.getState() == GameState::GameOver) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            
            BeginMode3D(followCam.camera);
            train.draw();
            followCam.draw();
            combatManager.draw();
            DrawGrid(2000, 20.f);
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
