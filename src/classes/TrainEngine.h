#pragma once
#include "TrainComponent.h"
#include "src/enums/Direction.h"
#include "src/interfaces/IKeyboardListener.h"

/**
 * The front engine of a train. Driven by the player
*/
class TrainEngine: public TrainComponent, public IKeyboardListener {
private:
    Music engineSound;
    float engineVolume = 0.f;
    float currentSpeed = 0.f;

public:
    Vector3 velocity = {0.f, 0.f, 0.f};
    float accelerationRate = .2f;
    float decelerationRate = 20.f; // This needs to be greater than 1. Otherwise deceleration will cause acceleration
    float topSpeed = 2.f;
    float rotationRate;
    Direction accelerationDirection = Direction::None;
    Direction rotationDirection = Direction::None;

    TrainEngine();
    TrainEngine(Model model, Texture2D texture, float accelerationRate, float decelerationRate, float topSpeed, float rotationRate);

    void update(float deltatime);
    void accelerate(float deltatime, bool forward = true);
    void decayAcceleration(float deltaTime);

    void onKeyPressed(int);
    void onKeyReleased(int);

    // ==================================================
    // Getters
    // ==================================================

    /**
     * The current speed of the train
     * @return float value representing the curernt speed of the Train. Calculated
     * by getting the length of its velocity vector
    */
    float getSpeed();
};