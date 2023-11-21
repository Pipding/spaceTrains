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

void TrainEngine::update(float deltaTime) {
    // TODO: Implement deltaTime

    if (this->accelerationDirection == Direction::None) {
        // Not accelerating forward or back, so velocity should decay
        this->decayAcceleration(deltaTime);
    } else {
        this->accelerate(deltaTime, this->accelerationDirection == Direction::Forward);
    }

    if (this->rotationDirection != Direction::None) {
        if (this->rotationDirection == Direction::Left) {
            this->rotateBy({0.f, this->rotationRate, 0.f}); //TODO: Deltatime
        } else if (this->rotationDirection == Direction::Right) {
            this->rotateBy({0.f, -this->rotationRate, 0.f}); //TODO: Deltatime
        }
    }

    this->position = Vector3Add(this->position, this->velocity);

    // Actor::update();
}

/**
 * Applies the engine's acceleration rate to its velocity.
 * @param forward   Optional, defaults to true. If true, acceleration is applied in the direction of the engine's forward vector.
 *                  Otherwise acceleration is applied in the opposite direction
*/
void TrainEngine::accelerate(float deltaTime, bool forward) {
    if (forward) {
        this->velocity = Vector3Add(this->velocity, Vector3Scale(this->getForwardVector(), this->accelerationRate * deltaTime));
    } else {
        this->velocity = Vector3Subtract(this->velocity, Vector3Scale(this->getForwardVector(), this->accelerationRate * deltaTime));
    }

    this->velocity = Vector3Clamp(this->velocity, (Vector3){-this->topSpeed, 0.f, -this->topSpeed}, (Vector3){this->topSpeed, 0.f, this->topSpeed});
}

void TrainEngine::decayAcceleration(float deltaTime) {
    if (this->velocity.x != 0.f || this->velocity.z != 0.f) {

        // If the velocity in either X or Z axis is close to 0, just set it to 0
        if (this->velocity.x != 0.f && abs(this->velocity.x) < 0.1f) {
            this->velocity.x = 0.f;
        }

        if (this->velocity.z != 0.f && abs(this->velocity.z) < 0.1f) {
            this->velocity.z = 0.f;
        }

        // If the velocity in either X or Z isn't close to zero, decay it by decelerationRate
        if (abs(this->velocity.x) > 0) {
            this->velocity.x *= (1 - (this->decelerationRate * deltaTime / this->decelerationRate * deltaTime * 100.f)); // TODO: Deceleration doesn't work right. decelerationRate is effectively pointless. Fix.
        }

        if (abs(this->velocity.z) > 0) {
            this->velocity.z *= (1 - (this->decelerationRate * deltaTime / this->decelerationRate * deltaTime * 100.f));
        }
    }
}

void TrainEngine::onKeyPressed(int key) {
    switch (key) {

        case KEY_W:
            if (this->accelerationDirection == Direction::Back) {
                this->accelerationDirection = Direction::None;
            } else {
                this->accelerationDirection = Direction::Forward;
            }
            break;
        case KEY_S:
            if (this->accelerationDirection == Direction::Forward) {
                this->accelerationDirection = Direction::None;
            } else {
                this->accelerationDirection = Direction::Back;
            }
            break;
        case KEY_A:
            if (this->rotationDirection == Direction::Right) {
                this->rotationDirection = Direction::None;
            } else {
                this->rotationDirection = Direction::Left;
            }
            break;
        case KEY_D:
            if (this->rotationDirection == Direction::Left) {
                this->rotationDirection = Direction::None;
            } else {
                this->rotationDirection = Direction::Right;
            }
            break;
        default:
            break;
    }
}

void TrainEngine::onKeyReleased(int key) {
    switch (key) {
        case KEY_W:
            if (this->accelerationDirection == Direction::None) {
                this->accelerationDirection = Direction::Back;
            } else {
                this->accelerationDirection = Direction::None;
            }
            break;
        case KEY_S:
            if (this->accelerationDirection == Direction::None) {
                this->accelerationDirection = Direction::Forward;
            } else {
                this->accelerationDirection = Direction::None;
            }
            break;
        case KEY_A:
            if (this->rotationDirection == Direction::None) {
                this->rotationDirection = Direction::Right;
            } else {
                this->rotationDirection = Direction::None;
            }
            break;
        case KEY_D:
            if (this->rotationDirection == Direction::None) {
                this->rotationDirection = Direction::Left;
            } else {
                this->rotationDirection = Direction::None;
            }
            break;
        default:
            break;
    }
}