#include "TrainCar.h"

/**
 * Default TrainCar constructor
*/
TrainCar::TrainCar()
: TrainComponent() { }

/**
 * A train car. Follows another TrainComponent and can be followed by another TrainCar
 * @param model             The 3D model representing the TrainEngine in the world
 * @param texture           The texture to be applied to model
 * @param engine            Pointer to a TrainComponent which this car will follow
 * @param position          Initial position of the train car
 * @param rotation          Initial rotation of the train car
*/
TrainCar::TrainCar(Model model, Texture2D texture, TrainComponent* engine, Vector3 position, Vector3 rotation)
: TrainComponent(position, model, texture) {
    this->engine = engine;
    this->rotation = rotation;
}

/**
 * Updates the train car to be pointing towards and following the TrainComponent assigned to the engine parameter
*/
void TrainCar::update() {

    // Figure out the rotation of this thing
    Vector3 pulledDirection = Vector3Normalize(Vector3Subtract(this->engine->position, this->position));
    Vector3 invertedPulledDirection = Vector3Negate(pulledDirection);
    Vector3 scaledInvertedPulledDirection = Vector3Scale(invertedPulledDirection, 50.f);
    this->position = Vector3Add(this->engine->position, scaledInvertedPulledDirection);

    // Angle (in rads) between 2 vectors is given by atan2
    float angleBetweenDucks = atan2(pulledDirection.x, pulledDirection.z);
    // For whatever reason the angle is offset by 90 degrees so we need to subtract that. 1.5708rad = 90deg
    // TODO: Probably should figure out why the angle is offset from what you expected
    this->setRotation({0, angleBetweenDucks- 1.5708f, 0});

    Actor::update();
}
