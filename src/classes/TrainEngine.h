#pragma once
#include "raylib.h"
#include "raymath.h"
#include "TrainComponent.h"
#include "src/enums/Direction.h"

/**
 * The front engine of a train. Driven by the player
*/
class TrainEngine: public TrainComponent {
public:
    Vector3 velocity = {0.f, 0.f, 0.f};
    float accelerationRate = .2f;
    float decelerationRate = 20.f; // This needs to be greater than 1. Otherwise deceleration will cause acceleration
    float topSpeed = 20.f;
    float rotationRate = 0.05f;
    Direction accelerationDirection = Direction::None;

    TrainEngine();
    TrainEngine(Model, Texture2D, float, float, float, float);

    void update();
    void accelerate(bool = true);
};