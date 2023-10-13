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

    // Camera & model loading borrowed from https://www.youtube.com/watch?v=TTa75ocharg
    // TODO: Remove the ducky (both code and assets)
    Model model = LoadModel("assets/models/RubberDuck_LOD0.obj"); // This model & texture come from https://www.cgtrader.com/items/2033848/download-page
    Texture2D tex = LoadTexture("assets/textures/RubberDuck_AlbedoTransparency.png");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tex;

    Camera cam = {0};
    cam.position = (Vector3){50.0f, 50.0f, 50.0f};
    cam.target = (Vector3){0.0f, 0.0f, 0.0f};
    cam.up = (Vector3){0.0f, 1.0f, 0.0f};
    cam.fovy = 90.f;
    cam.projection = CAMERA_PERSPECTIVE;

    Vector3 pos = {0.f, 0.f, 0.f};
    Vector3 velocity = (Vector3){0.f, 0.f, 0.f};
    BoundingBox bounds = GetMeshBoundingBox(model.meshes[0]);

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
                velocity.x = 1;
            } else if (IsKeyDown(KEY_S)) {
                velocity.x = -1;
            } else {
                velocity.x = 0;
            }

            if (IsKeyDown(KEY_A)) {
                velocity.z = -1;
            } else if (IsKeyDown(KEY_D)) {
                velocity.z = 1;
            } else {
                velocity.z = 0;
            }

            // Update
            pos = Vector3Add(pos, velocity);
            cam.position = Vector3Add(pos, (Vector3){-50.0f, 50.0f, 0.0f});
            cam.target = pos;

            UpdateCamera(&cam, CAMERA_THIRD_PERSON);
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(cam);
        DrawModel(model, pos, 1.f, WHITE);
        DrawGrid(20, 20.f);
        DrawBoundingBox(bounds, GREEN);
        EndMode3D();


        if (g_paused) {
            DrawText("Paused", 600, 340, 40, GREEN);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    UnloadModel(model);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
