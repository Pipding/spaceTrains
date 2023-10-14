#pragma once
#include "raylib.h"
#include "raymath.h"

/**
* Similar to the Actor class in Unreal Engine. Something which can be placed or spawned in the world
*/
class Actor {
private:
    Texture2D texture;
    
public:
    Vector3 position = {0.f, 0.f, 0.f};
    Vector3 rotation = {0.f, 0.f, 0.f};
    Model model;
    float scale = 1.f;
    Color color = WHITE;

    Actor(Model, Texture2D);
    Actor(Vector3, Model, Texture2D);

    void setTexture(Texture2D);
    void draw();
    void unload();
};
