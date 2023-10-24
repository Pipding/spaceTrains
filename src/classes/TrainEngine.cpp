#include "TrainEngine.h"

/**
 * Default constructor for the TrainEngine
*/
TrainEngine::TrainEngine() { }

/**
 * The front engine of a train. Driven by the player
 * @param actor             The visual representation of the train
 * @param accelerationRate  The rate at which the train accelerates
 * @param develerationRate  The rate at which the train decelerates
 * @param topSpeed          The max velocity of the train
 * @param rotationRate      How quickly can the train yaw
*/
TrainEngine::TrainEngine(Actor actor, float accelerationRate, float decelerationRate, float topSpeed, float rotationRate) {
    this->actor = actor;
    this->accelerationRate = accelerationRate;
    this->decelerationRate = decelerationRate;
    this->topSpeed = topSpeed;
    this->rotationRate = rotationRate;
}

/**
 * Rotate the train by a given rotation vector
 * @param rotation   Vector3 of angles in radians in x, y and z
*/
void TrainEngine::rotateBy(Vector3 rotation) {
    this->actor.rotateBy(rotation);
}

/**
 * Draw the train by calling the draw function of its Actor
*/
void TrainEngine::draw() {
    this->actor.draw();
}