#include "raylib.h"
#include "raymath.h"

#include <string>
#include <sstream>

class Actor {
  public:
    Vector3 position = {0.f, 0.f, 0.f};
    Vector3 rotation = {0.f, 0.f, 0.f};
    Model model;
    Texture2D texture;
    float scale = 1.f;
    Color color = WHITE;

    void draw() {
      DrawModel(this->model, this->position, this->scale, this->color);
    }
};

bool g_paused = false;

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
    Model duckModel = LoadModel("assets/models/RubberDuck_LOD0.obj"); // This model & texture come from https://www.cgtrader.com/items/2033848/download-page
    Texture2D tex = LoadTexture("assets/textures/RubberDuck_AlbedoTransparency.png");
    duckModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tex;

    Vector3 duckPos = {0.f, 0.f, 0.f};
    Vector3 duckRotation = {0.f, 0.f, 0.f};
    float duckRotationRate = 0.05f;
    Vector3 duckVelocity = {0.f, 0.f, 0.f};
    float duckAccelerationRate = .2f;
    float duckDecelerationRate = 20.f; // This needs to be greater than 1. Otherwise deceleration will cause acceleration
    float duckTopSpeed = 20.f;
    BoundingBox originalDuckBounds = GetMeshBoundingBox(duckModel.meshes[0]);
    BoundingBox currentDuckBounds = originalDuckBounds;
    Color duckColor = WHITE;


    // This borrowed from the models_box_collisions example: https://github.com/raysan5/raylib/blob/master/examples/models/models_box_collisions.c
    Vector3 upgradeTowerPos = { 120.0f, 0.f, 120.f };
    Vector3 upgradeTowerSize = { 50.0f, 100.0f, 50.0f };

    bool collision = false;

    //==================================================
    // Moving target stuff
    //==================================================
    Actor target;
    target.position = { 300.f, 0.f, 0.f };
    target.rotation = {0.f, -1.5f, 0.f};
    target.model = LoadModel("assets/models/archery_target.obj"); // This model & texture come from https://www.cgtrader.com/items/642062/download-page
    target.texture = LoadTexture("assets/textures/archery_target_albedo.png");
    target.model.transform = MatrixRotateXYZ(target.rotation);
    target.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = target.texture;


    //==================================================
    // Camera stuff
    //==================================================
    Camera cam = {0};
    cam.position = (Vector3){50.0f, 50.0f, 50.0f};
    cam.target = (Vector3){0.0f, 0.0f, 0.0f};
    cam.up = (Vector3){0.0f, 1.0f, 0.0f};
    cam.fovy = 90.f;
    cam.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        if (IsKeyPressed(KEY_P)) {
            g_paused = !g_paused;
        }

        if (!g_paused) {
            // Input
            if (IsKeyDown(KEY_W)) {
                duckVelocity = Vector3Add(duckVelocity, Vector3Scale(Vector3Transform({1.f, 0.f, 0.f}, duckModel.transform), duckAccelerationRate));
            } else if (IsKeyDown(KEY_S)) {
                duckVelocity = Vector3Subtract(duckVelocity, Vector3Scale(Vector3Transform({1.f, 0.f, 0.f}, duckModel.transform), duckAccelerationRate));
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
                duckRotation.y += duckRotationRate;
                duckModel.transform = MatrixRotateXYZ(duckRotation);
            } else if (IsKeyDown(KEY_D)) {
                duckRotation.y -= duckRotationRate;
                duckModel.transform = MatrixRotateXYZ(duckRotation);
            }

            // Box collision check based on the models_box_collisions example: https://github.com/raysan5/raylib/blob/master/examples/models/models_box_collisions.c
            collision = CheckCollisionBoxes(
                currentDuckBounds,
                (BoundingBox) {
                    (Vector3){ upgradeTowerPos.x - upgradeTowerSize.x/2, upgradeTowerPos.y - upgradeTowerSize.y/2, upgradeTowerPos.z - upgradeTowerSize.z/2 },
                    (Vector3){ upgradeTowerPos.x + upgradeTowerSize.x/2, upgradeTowerPos.y + upgradeTowerSize.y/2, upgradeTowerPos.z + upgradeTowerSize.z/2 }
                }
            );

            if (collision) {
                duckColor = RED;
            } else {
                duckColor = WHITE;
            }

            // Update
            duckPos = Vector3Add(duckPos, duckVelocity);
            // TODO: Bounding box is axis-aligned, so it doesn't rotate with the model. Unsure what if anything to do about this atm
            currentDuckBounds.min = Vector3Add(duckPos, originalDuckBounds.min);
            currentDuckBounds.max = Vector3Add(duckPos, originalDuckBounds.max);

            cam.position = Vector3Add(duckPos, (Vector3){-50.0f, 50.0f, 0.0f});
            cam.target = duckPos;

            UpdateCamera(&cam, CAMERA_THIRD_PERSON);
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(cam);
        DrawCube(upgradeTowerPos, upgradeTowerSize.x, upgradeTowerSize.y, upgradeTowerSize.z, GRAY);
        DrawModel(duckModel, duckPos, 1.f, duckColor);
        target.draw();
        DrawGrid(2000, 20.f);
        DrawBoundingBox(currentDuckBounds, GREEN);
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

    UnloadModel(duckModel);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
