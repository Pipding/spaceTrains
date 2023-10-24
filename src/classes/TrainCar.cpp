#include "TrainCar.h"

TrainCar::TrainCar() { }

TrainCar::TrainCar(TrainEngine* engine, Actor actor, Vector3 position, Vector3 rotation) {
    this->engine = engine;
    this->actor = actor;
    this->position = position;
    this->rotation = rotation;
}

void TrainCar::draw() {
    this->actor.draw();
}

void TrainCar::update() {
    this->actor.position = this->position;

    // Figure out the rotation of this thing
    Vector3 pulledDirection = Vector3Normalize(Vector3Subtract(this->engine->actor.position, this->actor.position));
    Vector3 invertedPulledDirection = Vector3Negate(pulledDirection);
    Vector3 scaledInvertedPulledDirection = Vector3Scale(invertedPulledDirection, 50.f);
    this->position = Vector3Add(this->engine->actor.position, scaledInvertedPulledDirection);

    // Angle (in rads) between 2 vectors is given by atan2
    float angleBetweenDucks = atan2(pulledDirection.x, pulledDirection.z);
    // For whatever reason the angle is offset by 90 degrees so we need to subtract that. 1.5708rad = 90deg
    // TODO: Probably should figure out why the angle is offset from what you expected
    this->actor.setRotation({0, angleBetweenDucks- 1.5708f, 0});
}

