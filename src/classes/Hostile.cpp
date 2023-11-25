#include "Hostile.h"

Hostile::Hostile(Vector3 position, Model model, Texture texture, Vector3* target, float minEngagementDistance, float maxEngagementDistance, float maxSpeed)
:Actor(position, model, texture) {
    this->maxSpeed = maxSpeed;
    this->currentSpeed = maxSpeed; //TODO: Need to implement acceleration and deceleration for hostiles
    this->target = target;
    this->maxHitpoints = 100;
    this->currentHitpoints = 100;
}

void Hostile::update(float deltaTime) {
    if (this->isAlive) {
        // Move self towards target
        Vector3 vectorToTarget = this->getVectorTowardTarget(*this->target, false);

        // Don't get too close to the target
        if (Vector3Length(vectorToTarget) >= this->minEngagementDistance) {
            this->position = Vector3Add(this->position, Vector3Scale(Vector3Normalize(vectorToTarget), this->currentSpeed * deltaTime));
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