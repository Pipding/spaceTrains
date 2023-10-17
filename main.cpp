#include "raylib.h"
#include "raymath.h"
#include "src/classes/Actor.h"
#include "src/classes/TargetCam.h"
#include "src/globals/SpaceTrainDebug.h"

bool g_paused = false;

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

// TODO: Extract this to its own file
class TrainEngine {
public:
    Vector3 velocity = {0.f, 0.f, 0.f};
    Actor actor;
    float accelerationRate = .2f;
    float decelerationRate = 20.f; // This needs to be greater than 1. Otherwise deceleration will cause acceleration
    float topSpeed = 20.f;
    float rotationRate = 0.05f;
    // TODO: Add a rearAnchorPoint and/or rearAnchorPointOffset which will be the point the next train car "follows"

    TrainEngine(Actor actor, float accelerationRate, float decelerationRate, float topSpeed, float rotationRate) {
        this->actor = actor;
        this->accelerationRate = accelerationRate;
        this->decelerationRate = decelerationRate;
        this->topSpeed = topSpeed;
        this->rotationRate = rotationRate;
    }

    void rotateBy(Vector3 rotation) {
        this->actor.rotateBy(rotation);
    }

    void draw() {
        this->actor.draw();
    }
};

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
    Actor duck(LoadModel("assets/models/ducky.obj"), LoadTexture("assets/textures/ducky_albedo.png")); // Model & texture come from https://www.cgtrader.com/items/2033848/download-page

    TrainEngine engine(duck, 0.2f, 20.f, 20.f, 0.05f);

    Actor duck2({-50.f, 0.f, 0.f}, LoadModel("assets/models/ducky.obj"), LoadTexture("assets/textures/ducky_albedo.png"));

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
    TargetCam targetCam(&engine.actor, {-150.0f, 100.f, 0.0f});

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

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
                engine.velocity = Vector3Add(engine.velocity, Vector3Scale(Vector3Transform({1.f, 0.f, 0.f}, engine.actor.model.transform), engine.accelerationRate));
            } else if (IsKeyDown(KEY_S)) {
                engine.velocity = Vector3Subtract(engine.velocity, Vector3Scale(Vector3Transform({1.f, 0.f, 0.f}, engine.actor.model.transform), engine.accelerationRate));
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
                engine.actor.getBounds(),
                (BoundingBox) {
                    (Vector3){ upgradeTowerPos.x - upgradeTowerSize.x/2, upgradeTowerPos.y - upgradeTowerSize.y/2, upgradeTowerPos.z - upgradeTowerSize.z/2 },
                    (Vector3){ upgradeTowerPos.x + upgradeTowerSize.x/2, upgradeTowerPos.y + upgradeTowerSize.y/2, upgradeTowerPos.z + upgradeTowerSize.z/2 }
                }
            );

            if (collision) {
                engine.actor.color = RED;
            } else {
                engine.actor.color = WHITE;
            }
            engine.actor.position = Vector3Add(engine.actor.position, engine.velocity);

            // Updating duck2
            Vector3 duck2pulledDirection = Vector3Normalize(Vector3Subtract(engine.actor.position, duck2.position));
            Vector3 invertedPulledDirection = Vector3Negate(duck2pulledDirection);
            Vector3 scaledInvertedPulledDirection = Vector3Scale(invertedPulledDirection, 50.f);
            duck2.position = Vector3Add(engine.actor.position, scaledInvertedPulledDirection);

            // Angle (in rads) between 2 vectors is given by atan2
            float angleBetweenDucks = atan2(duck2pulledDirection.x, duck2pulledDirection.z);
            // For whatever reason the angle is offset by 90 degrees so we need to subtract that. 1.5708rad = 90deg
            // TODO: Probably should figure out why the angle is offset from what you expected
            duck2.setRotation({0, angleBetweenDucks- 1.5708f, 0});

            // TODO: Bounding box is axis-aligned, so it doesn't rotate with the model. Unsure what if anything to do about this atm
            engine.actor.update();
            targetCam.update();
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(targetCam.camera);
        DrawCube(upgradeTowerPos, upgradeTowerSize.x, upgradeTowerSize.y, upgradeTowerSize.z, GRAY);
        // duck.draw();
        duck2.draw();
        engine.draw();
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
    duck.unload();
    target.unload();
    CloseWindow();

    return 0;
}
