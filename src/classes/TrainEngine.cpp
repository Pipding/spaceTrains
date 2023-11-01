#include "TrainEngine.h"

/**
 * Default TrainEngine constructor
*/
TrainEngine::TrainEngine()
: TrainComponent() { }

/**
 * The front engine of a train. Driven by the player
 * @param model             The 3D model representing the TrainEngine in the world
 * @param texture           The texture to be applied to model
 * @param accelerationRate  The rate at which the train accelerates
 * @param decelerationRate  The rate at which the train decelerates
 * @param topSpeed          The max velocity of the train
 * @param rotationRate      How quickly can the train yaw
*/
TrainEngine::TrainEngine(Model model, Texture2D texture, float accelerationRate, float decelerationRate, float topSpeed, float rotationRate)
: TrainComponent({0.f, 0.f, 0.f}, model, texture) {
    this->accelerationRate = accelerationRate;
    this->decelerationRate = decelerationRate;
    this->topSpeed = topSpeed;
    this->rotationRate = rotationRate;
}

void TrainEngine::update() {

    if (this->accelerationDirection != Direction::None) {
        this->accelerate(this->accelerationDirection == Direction::Forward);
    }

    Actor::update();
}

/**
 * Applies the engine's acceleration rate to its velocity.
 * TODO: Deltatime
 * @param forward   Optional, defaults to true. If true, acceleration is applied in the direction of the engine's forward vector.
 *                  Otherwise acceleration is applied in the opposite direction
*/
void TrainEngine::accelerate(bool forward) {
    if (forward) {
        this->velocity = Vector3Add(this->velocity, Vector3Scale(this->getForwardVector(), this->accelerationRate));
    } else {
        this->velocity = Vector3Subtract(this->velocity, Vector3Scale(this->getForwardVector(), this->accelerationRate));
    }

    this->velocity = Vector3Clamp(this->velocity, (Vector3){-this->topSpeed, 0.f, -this->topSpeed}, (Vector3){this->topSpeed, 0.f, this->topSpeed});
}
