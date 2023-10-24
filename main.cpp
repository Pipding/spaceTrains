#include "raylib.h"
#include "raymath.h"
#include "src/classes/Actor.h"
#include "src/classes/TargetCam.h"
#include "src/classes/TrainEngine.h"
#include "src/classes/TrainCar.h"
#include "src/globals/SpaceTrainDebug.h"

bool g_paused = false;

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

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

    TrainEngine engine(duckModel, duckTexture, 0.2f, 20.f, 20.f, 0.05f);
    TrainCar carriage(duckModel, duckTexture, &engine, {-50.f, 0.f, 0.f});
    TrainCar carriage2(duckModel, duckTexture, &carriage, {-100.f, 0.f, 0.f});

    // This borrowed from the models_box_collisions example: https://github.com/raysan5/raylib/blob/master/examples/models/models_box_collisions.c
    Vector3 upgradeTowerPos = { 120.0f, 0.f, 120.f };
    Vector3 upgradeTowerSize = { 50.0f, 100.0f, 50.0f };

    bool collision = false;

    //==================================================
    // Moving target stuff
    //==================================================
    Actor target({ 300.f, 0.f, 0.f }, LoadModel("assets/models/archery_target.obj"), LoadTexture("assets/textures/archery_target_albedo.png"));
    target.setRotation({0.f, -1.5f, 0.f});

    //==================================================
    // Camera stuff
    //==================================================
    TargetCam targetCam(&engine, {-150.0f, 100.f, 0.0f});

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // const float deltaTime = GetFrameTime();

        if (IsKeyPressed(KEY_P)) {
            g_paused = !g_paused;
        }

        if (IsKeyPressed(KEY_M)) {
            _debug.toggleDrawBoundingBoxes();
        }

        if (IsKeyPressed(KEY_R)) {
            targetCam.resetmouseRotationAdjustment();
        }

        if (!g_paused) {
            // Input
            if (IsKeyDown(KEY_W)) {
                engine.velocity = Vector3Add(engine.velocity, Vector3Scale(Vector3Transform({1.f, 0.f, 0.f}, engine.model.transform), engine.accelerationRate));
            } else if (IsKeyDown(KEY_S)) {
                engine.velocity = Vector3Subtract(engine.velocity, Vector3Scale(Vector3Transform({1.f, 0.f, 0.f}, engine.model.transform), engine.accelerationRate));
            } else if (engine.velocity.x != 0.f || engine.velocity.z != 0.f) {
                // The user is not pressing any buttons. Velocity should be decaying
                if (engine.velocity.x != 0.f && abs(engine.velocity.x) < 0.1f) {
                    engine.velocity.x = 0.f;
                }

                if (engine.velocity.z != 0.f && abs(engine.velocity.z) < 0.1f) {
                    engine.velocity.z = 0.f;
                }

                if (abs(engine.velocity.x) > 0) {
                    engine.velocity.x *= (1 - (1 / engine.decelerationRate));
                }

                if (abs(engine.velocity.z) > 0) {
                    engine.velocity.z *= (1 - (1 / engine.decelerationRate));
                }
            }

            engine.velocity = Vector3Clamp(engine.velocity, (Vector3){-engine.topSpeed, 0.f, -engine.topSpeed}, (Vector3){engine.topSpeed, 0.f, engine.topSpeed});

            if (IsKeyDown(KEY_A)) {
                engine.rotateBy({0.f, engine.rotationRate, 0.f});
            } else if (IsKeyDown(KEY_D)) {
                engine.rotateBy({0.f, -engine.rotationRate, 0.f});
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
            engine.position = Vector3Add(engine.position, engine.velocity);

            // TODO: Bounding box is axis-aligned, so it doesn't rotate with the model. Unsure what if anything to do about this atm
            engine.update();
            carriage.update();
            carriage2.update();
            targetCam.update();
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(targetCam.camera);
        DrawCube(upgradeTowerPos, upgradeTowerSize.x, upgradeTowerSize.y, upgradeTowerSize.z, GRAY);
        engine.draw();
        carriage.draw();
        carriage2.draw();
        target.draw();
        DrawGrid(2000, 20.f);
        EndMode3D();

        DrawText(TextFormat("Velocity: %f, %f", engine.velocity.x, engine.velocity.z), 20, 20, 40, GREEN);

        if (g_paused) {
            DrawText("Paused", 600, 340, 40, GREEN);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    //==================================================
    // De-initialization
    //==================================================
    // duck.unload();
    engine.unload(); // TODO: If multiple Actors use the same model/texture then attempting to unload something after it has already been unloaded will cause a segfault
    target.unload();
    CloseWindow();

    return 0;
}
