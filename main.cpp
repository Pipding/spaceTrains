#include "raylib.h"
#include "raymath.h"

bool g_paused = false;

int main(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

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
    Vector3 duckVelocity = (Vector3){0.f, 0.f, 0.f};
    BoundingBox duckBounds = GetMeshBoundingBox(duckModel.meshes[0]);


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
                duckVelocity.x = 1;
            } else if (IsKeyDown(KEY_S)) {
                duckVelocity.x = -1;
            } else {
                duckVelocity.x = 0;
            }

            if (IsKeyDown(KEY_A)) {
                duckVelocity.z = -1;
            } else if (IsKeyDown(KEY_D)) {
                duckVelocity.z = 1;
            } else {
                duckVelocity.z = 0;
            }

            // Update
            duckPos = Vector3Add(duckPos, duckVelocity);
            cam.position = Vector3Add(duckPos, (Vector3){-50.0f, 50.0f, 0.0f});
            cam.target = duckPos;

            UpdateCamera(&cam, CAMERA_THIRD_PERSON);
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(cam);
        DrawModel(duckModel, duckPos, 1.f, WHITE);
        DrawGrid(20, 20.f);
        DrawBoundingBox(duckBounds, GREEN);
        EndMode3D();


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
