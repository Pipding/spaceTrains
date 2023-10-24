#include "TrainCar.h"

TrainCar::TrainCar() { }

TrainCar::TrainCar(Model model, Texture2D texture, TrainComponent* engine, Vector3 position, Vector3 rotation) {
    this->model = model;
    this->setTexture(texture);
    this->engine = engine;
    this->position = position;
    this->rotation = rotation;
}

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

