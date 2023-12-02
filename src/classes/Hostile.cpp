#include "Hostile.h"

Hostile::Hostile(Vector3 position, Model model, Texture texture, Vector3* target, float minEngagementDistance, float maxEngagementDistance, float maxSpeed, Model projectileModel, Texture2D projectileTexture)
:Actor(position, model, texture) {
    this->maxSpeed = maxSpeed;
    this->currentSpeed = maxSpeed; //TODO: Need to implement acceleration and deceleration for hostiles
    this->target = target;
    this->minEngagementDistance = minEngagementDistance;
    this->maxEngagementDistance = maxEngagementDistance;
    this->maxHitpoints = 100;
    this->currentHitpoints = 100;
    this->projectileModel = projectileModel;
    this->projectileTexture = projectileTexture;
    this->reloadTime = 2000;
    this->power = 10;
    this->setScale(0.2f);
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

Texture2D Hostile::getProjectileTexture() {
    return this->projectileTexture;
}

Model Hostile::getProjectileModel() {
    return this->projectileModel;
}

void Hostile::update(float deltaTime) {
    // Calculate where your target is
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

    // If you're reloading or in the "fleeing" state, try to get away from the player
    if (this->isFleeing || this->reloading) {

        // Calculate a vector which is in the exact opposite direction of the player
        Vector3 fleeVector = Vector3Add(this->position, Vector3Scale(Vector3Negate(this->directionToTarget), 100.f));

        // Turn away from the player and run
        this->setRotation({0, this->angleToVector(fleeVector), 0});
        this->position = Vector3Add(this->position, Vector3Scale(this->getVectorTowardTarget(fleeVector), this->currentSpeed * deltaTime));

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
    if (this->reloading || this->isFleeing) return false;

    if ((this->minEngagementDistance < this->distanceToTarget) && (this->distanceToTarget < this->maxEngagementDistance)) {
        return true;
    } else {
        return false;
    }
}

Projectile* Hostile::shoot(Vector3* targetPos) {
    if (!this->canShoot()) return nullptr;

    Projectile* p = new Projectile(this->position, 1000.f, this->power, targetPos, this->projectileModel, this->projectileTexture, "missile_fire", "explosion");

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

        // If hitpoints have dropped below 20%, start fleeing
        // TODO: Give hostiles a random chance to start fleeing instead of a definite threshold
        if ((this->currentHitpoints / this->maxHitpoints) <= 0.2f) {
            this->isFleeing = true;
        }
    }

    return this->currentHitpoints;
}

bool Hostile::isAlive() {
    return this->alive;
}