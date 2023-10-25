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