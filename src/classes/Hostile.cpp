#include "Hostile.h"

Hostile::Hostile(Vector3 position, Model model, Texture texture, Vector3* target)
:Actor(position, model, texture) {
    this->speed = 5.f;
    this->target = target;
    this->targetDistance = 200.f;
    this->hitpoints = 100;
}

void Hostile::update() {
    // Move self towards target

    // TODO: This code is stolen from the TrainCar
    // Figure out the rotation of this thing
    Vector3 vectorBetweenMeAndTarget = Vector3Subtract(*this->target, this->position);
    Vector3 pulledDirection = Vector3Normalize(vectorBetweenMeAndTarget);

    // Don't get too close to the target
    if (Vector3Length(vectorBetweenMeAndTarget) >= targetDistance) {
        this->position = Vector3Add(this->position, Vector3Scale(pulledDirection, this->speed));
    }

    // Angle (in rads) between 2 vectors is given by atan2
    float angleBetweenDucks = atan2(pulledDirection.x, pulledDirection.z);
    // For whatever reason the angle is offset by 90 degrees so we need to subtract that. 1.5708rad = 90deg
    // TODO: Probably should figure out why the angle is offset from what you expected
    this->setRotation({0, angleBetweenDucks- 1.5708f, 0});

    Actor::update();
}