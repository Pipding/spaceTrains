#pragma once
#include "raylib.h"
#include "raymath.h"
#include "src/globals/SpaceTrainDebug.h"

/**
* Similar to the Actor class in Unreal Engine. Something which can be placed or spawned in the world
*/
class Actor {
private:
    Texture2D texture;
    BoundingBox boundsOrigin;
    BoundingBox bounds;
    Vector3 position = {0.f, 0.f, 0.f};
    
public:
    Vector3 rotation = {0.f, 0.f, 0.f};
    Model model;
    float scale = 1.f;
    Color color = WHITE;

    Actor();
    Actor(Model, Texture2D);
    Actor(Vector3, Model, Texture2D);

    void setTexture(Texture2D);
    void setRotation(Vector3);
    void rotateBy(Vector3);
    BoundingBox getBounds();

    void update();
    void draw(Vector3 position);
    void unload();
};
