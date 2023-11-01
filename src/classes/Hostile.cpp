#include "Hostile.h"

Hostile::Hostile(Vector3 position, Model model, Texture texture, Vector3* target)
:Actor(position, model, texture) {
    this->speed = 5.f;
    this->target = target;
    this->targetDistance = 200.f;
    this->maxHitpoints = 100;
    this->currentHitpoints = 100;
}

void Hostile::update() {
    if (this->isAlive) {
        // Move self towards target
        Vector3 vectorToTarget = this->getVectorTowardTarget(*this->target, false);

        // Don't get too close to the target
        if (Vector3Length(vectorToTarget) >= targetDistance) {
            this->position = Vector3Add(this->position, Vector3Scale(Vector3Normalize(vectorToTarget), this->speed)); // TODO: deltatime
        }
        
        this->setRotation({0, this->angleToVector(*this->target), 0});

        Actor::update();
    }
}



int Hostile::receiveDamage(int damageReceived) {

    if (this->currentHitpoints <= damageReceived) {
        this->currentHitpoints = 0;
        this->isAlive = false;
        this->color = RED;
    } else {
        this->currentHitpoints -= damageReceived;
    }

    return this->currentHitpoints;
}