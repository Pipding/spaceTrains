#pragma once
#include "TrainComponent.h"
#include "src/enums/Direction.h"
#include "src/enums/Direction.h"
#include "src/interfaces/IKeyboardListener.h"

/**
 * The front engine of a train. Driven by the player
*/
class TrainEngine: public TrainComponent, public IKeyboardListener {
public:
    Vector3 velocity = {0.f, 0.f, 0.f};
    float accelerationRate = .2f;
    float decelerationRate = 20.f; // This needs to be greater than 1. Otherwise deceleration will cause acceleration
    float topSpeed = 20.f;
    float rotationRate = 0.05f;
    Direction accelerationDirection = Direction::None;
    Direction rotationDirection = Direction::None;

    TrainEngine();
    TrainEngine(Model model, Texture2D texture, float accelerationRate, float decelerationRate, float topSpeed, float rotationRate);

    void update(float deltatime);
    void accelerate(float deltatime, bool forward = true);
    void decayAcceleration(float deltaTime);

    void onKeyPressed(int);
    void onKeyReleased(int);
};