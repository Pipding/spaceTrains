#pragma once
#include "raylib.h"
#include "raymath.h"
#include "src/interfaces/IUpdatable.h"
#include "src/globals/SpaceTrainDebug.h"

/**
* Similar to the Actor class in Unreal Engine. Something which can be placed or spawned in the world
*/
class Actor : public IUpdatable {
private:
    Texture2D texture;
    BoundingBox boundsOrigin;
    BoundingBox bounds;
    Matrix transform = MatrixIdentity();
    
public:
    Vector3 position = {0.f, 0.f, 0.f};
    Vector3 rotation = {0.f, 0.f, 0.f};
    Model model;
    float scale = 1.f;
    Color color = WHITE;

    Actor();
    Actor(Model model, Texture2D texture);
    Actor(Vector3 position, Model model, Texture2D texture);

    void setTexture(Texture2D texture);
    void setRotation(Vector3 rotation);
    void rotateBy(Vector3 rotation);
    BoundingBox getBounds();
    Vector3 getForwardVector();

    void update();
    void draw();
    void unload();
};
