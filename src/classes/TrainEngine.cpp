#include "TrainEngine.h"

/**
 * Default constructor for the TrainEngine
*/
TrainEngine::TrainEngine() { }

/**
 * The front engine of a train. Driven by the player
 * @param actor             Pointer to an Actor which will act as the visual representation of the train
 * @param accelerationRate  The rate at which the train accelerates
 * @param develerationRate  The rate at which the train decelerates
 * @param topSpeed          The max velocity of the train
 * @param rotationRate      How quickly can the train yaw
*/
TrainEngine::TrainEngine(Model model, Texture2D texture, float accelerationRate, float decelerationRate, float topSpeed, float rotationRate) {
    this->model = model;
    this->setTexture(texture);
    this->accelerationRate = accelerationRate;
    this->decelerationRate = decelerationRate;
    this->topSpeed = topSpeed;
    this->rotationRate = rotationRate;
}