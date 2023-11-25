#include "Hostile.h"

Hostile::Hostile(Vector3 position, Model model, Texture texture, Vector3* target, float minEngagementDistance, float maxEngagementDistance, float maxSpeed)
:Actor(position, model, texture) {
    this->maxSpeed = maxSpeed;
    this->currentSpeed = maxSpeed; //TODO: Need to implement acceleration and deceleration for hostiles
    this->target = target;
    this->minEngagementDistance = minEngagementDistance;
    this->maxEngagementDistance = maxEngagementDistance;
    this->maxHitpoints = 100;
    this->currentHitpoints = 100;
}

void Hostile::update(float deltaTime) {
    if (this->isAlive) { // TODO: If this thing isn't alive it should be deleted. That's a job for the CombatManager

        Vector3 vectorToTarget = this->getVectorTowardTarget(*this->target, false);
        this->distanceToTarget = Vector3Length(vectorToTarget);
        this->directionToTarget = Vector3Normalize(vectorToTarget);

        if (this->isFleeing) {
            // Try to get away from the target
            this->isAlive = false;
            // TODO: This is a stub
        } else {
            // Try to get closer to the target
            if (this->distanceToTarget > this->minEngagementDistance) {
                this->position = Vector3Add(this->position, Vector3Scale(this->directionToTarget, this->currentSpeed * deltaTime));
            }

            this->setRotation({0, this->angleToVector(*this->target), 0});
        }

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