#include "raylib.h"
#include "raymath.h"
#include "src/classes/Actor.h"
#include "src/classes/TargetCam.h"
#include "src/globals/SpaceTrainDebug.h"
#include <string>
#include <sstream>

bool g_paused = false;

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    std::ostringstream oss;
    std::string velocityOutput;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    //==================================================
    // Duck stuff
    //==================================================

    // Camera & model loading borrowed from https://www.youtube.com/watch?v=TTa75ocharg
    // TODO: Remove the ducky (both code and assets)
    Actor duck(LoadModel("assets/models/ducky.obj"), LoadTexture("assets/textures/ducky_albedo.png")); // Model & texture come from https://www.cgtrader.com/items/2033848/download-page
    float duckRotationRate = 0.05f;
    Vector3 duckVelocity = {0.f, 0.f, 0.f};
    float duckAccelerationRate = .2f;
    float duckDecelerationRate = 20.f; // This needs to be greater than 1. Otherwise deceleration will cause acceleration
    float duckTopSpeed = 20.f;

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
    TargetCam targetCam(&duck, {-50.0f, 50.0f, 0.0f});

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        if (IsKeyPressed(KEY_P)) {
            g_paused = !g_paused;
        }

        if (IsKeyPressed(KEY_M)) {
            _debug.toggleDrawBoundingBoxes();
        }

        if (!g_paused) {
            // Input
            if (IsKeyDown(KEY_W)) {
                duckVelocity = Vector3Add(duckVelocity, Vector3Scale(Vector3Transform({1.f, 0.f, 0.f}, duck.model.transform), duckAccelerationRate));
            } else if (IsKeyDown(KEY_S)) {
                duckVelocity = Vector3Subtract(duckVelocity, Vector3Scale(Vector3Transform({1.f, 0.f, 0.f}, duck.model.transform), duckAccelerationRate));
            } else if (duckVelocity.x != 0.f || duckVelocity.z != 0.f) {
                // The user is not pressing any buttons. Velocity should be decaying
                if (duckVelocity.x != 0.f && abs(duckVelocity.x) < 0.1f) {
                    duckVelocity.x = 0.f;
                }

                if (duckVelocity.z != 0.f && abs(duckVelocity.z) < 0.1f) {
                    duckVelocity.z = 0.f;
                }

                if (abs(duckVelocity.x) > 0) {
                    duckVelocity.x *= (1 - (1 / duckDecelerationRate));
                }

                if (abs(duckVelocity.z) > 0) {
                    duckVelocity.z *= (1 - (1 / duckDecelerationRate));
                }
            }

            duckVelocity = Vector3Clamp(duckVelocity, (Vector3){-duckTopSpeed, 0.f, -duckTopSpeed}, (Vector3){duckTopSpeed, 0.f, duckTopSpeed});

            if (IsKeyDown(KEY_A)) {
                duck.rotateBy({0.f, duckRotationRate, 0.f});
            } else if (IsKeyDown(KEY_D)) {
                duck.rotateBy({0.f, -duckRotationRate, 0.f});
            }

            // Box collision check based on the models_box_collisions example: https://github.com/raysan5/raylib/blob/master/examples/models/models_box_collisions.c
            collision = CheckCollisionBoxes(
                duck.getBounds(),
                (BoundingBox) {
                    (Vector3){ upgradeTowerPos.x - upgradeTowerSize.x/2, upgradeTowerPos.y - upgradeTowerSize.y/2, upgradeTowerPos.z - upgradeTowerSize.z/2 },
                    (Vector3){ upgradeTowerPos.x + upgradeTowerSize.x/2, upgradeTowerPos.y + upgradeTowerSize.y/2, upgradeTowerPos.z + upgradeTowerSize.z/2 }
                }
            );

            if (collision) {
                duck.color = RED;
            } else {
                duck.color = WHITE;
            }

            // Update
            duck.position = Vector3Add(duck.position, duckVelocity);
            // TODO: Bounding box is axis-aligned, so it doesn't rotate with the model. Unsure what if anything to do about this atm
            duck.update();
            targetCam.update();
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(targetCam.camera);
        DrawCube(upgradeTowerPos, upgradeTowerSize.x, upgradeTowerSize.y, upgradeTowerSize.z, GRAY);
        duck.draw();
        target.draw();
        DrawGrid(2000, 20.f);
        EndMode3D();

        oss.str("");
        oss.clear();
        oss << "Velocity: " << duckVelocity.x << ", " << duckVelocity.z;
        velocityOutput = oss.str();

        DrawText(velocityOutput.c_str(), 20, 20, 40, GREEN);

        if (g_paused) {
            DrawText("Paused", 600, 340, 40, GREEN);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    duck.unload();
    target.unload();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
