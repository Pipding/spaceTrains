#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Actor.h"

/**
 * The front engine of a train. Driven by the player
*/
class TrainEngine {
public:
    Vector3 velocity = {0.f, 0.f, 0.f};
    Actor actor;
    float accelerationRate = .2f;
    float decelerationRate = 20.f; // This needs to be greater than 1. Otherwise deceleration will cause acceleration
    float topSpeed = 20.f;
    float rotationRate = 0.05f;

    TrainEngine(Actor, float, float, float, float);

    void rotateBy(Vector3);
    void draw();
};