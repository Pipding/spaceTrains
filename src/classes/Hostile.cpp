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

bool Hostile::getIsFleeing() {
    return this->isFleeing;
}

float Hostile::getMinEngagementDistance() {
    return this->minEngagementDistance;
}

float Hostile::getMaxEngagementDistance() {
    return this->maxEngagementDistance;
}

float Hostile::getMaxSpeed() {
    return this->maxSpeed;
}

void Hostile::update(float deltaTime) {
    Vector3 vectorToTarget = this->getVectorTowardTarget(*this->target, false);
    this->distanceToTarget = Vector3Length(vectorToTarget);
    this->directionToTarget = Vector3Normalize(vectorToTarget);

    if (this->reloading) {
        int64_t timeSinceLastShot = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - this->lastShot).count();

        if (timeSinceLastShot > this->reloadTime) {
            this->reloading = false;
        } else {
            this->timeUntilReloaded = this->reloadTime - timeSinceLastShot;
        }
    }

    if (this->isFleeing) {
        // Try to get away from the target
        this->alive = false;
        // TODO: This is a stub
    } else {
        // Try to get closer to the target
        if (this->distanceToTarget > this->minEngagementDistance) {
            this->position = Vector3Add(this->position, Vector3Scale(this->directionToTarget, this->currentSpeed * deltaTime));
            this->setRotation({0, this->angleToVector(*this->target), 0});
        } else if (this->distanceToTarget < this->minEngagementDistance) {
            // try to get away
        }
    }

    Actor::update();
}

bool Hostile::canShoot() {
    if (this->reloading) return false;

    if ((this->minEngagementDistance < this->distanceToTarget) && (this->distanceToTarget < this->maxEngagementDistance)) {
        return true;
    } else {
        return false;
    }
}

Projectile* Hostile::shoot(Vector3* targetPos) {
    if (!this->canShoot()) return nullptr;

    Projectile* p = new Projectile(this->position, 1000.f, this->power, targetPos, this->projectileModel, this->projectileTexture);

    this->lastShot = std::chrono::steady_clock::now();
    this->reloading = true;
    
    return p;
}

int Hostile::receiveDamage(int damageReceived) {

    if (this->currentHitpoints <= damageReceived) {
        this->currentHitpoints = 0;
        this->alive = false;
        this->color = RED;
    } else {
        this->currentHitpoints -= damageReceived;
    }

    return this->currentHitpoints;
}

bool Hostile::isAlive() {
    return this->alive;
}