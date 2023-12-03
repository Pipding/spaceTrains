#include "TrainCar.h"

TrainCar::TrainCar(): TrainComponent() { }


TrainCar::TrainCar(Model model, Texture2D texture, TrainComponent* engine, float followDist, int power, int reloadTime, Model projectileModel, Texture2D projectileTexture, const char* projectileLaunchSFX, const char* projectileDestroySFX)
: TrainComponent({0.f, 0.f, 0.f}, model, texture, projectileModel, projectileTexture) {
    this->engine = engine;
    this->rotation = {0.f, 0.f, 0.f};
    this->power = power;
    this->reloadTime = reloadTime;
    this->canShoot = true;
    this->projectileLaunchSFX = projectileLaunchSFX;
    this->projectileDestroySFX = projectileDestroySFX;
    this->followDistance = followDist;

    // Initialize the position of the TrainCar based on its "engine"
    Vector3 engineBack = Vector3Negate(this->engine->getForwardVector());
    // TODO: The distance the car initially spawns behind its parent is multiplied by 5 to prevent them from intersecting on spawn
    // Come up with a cleaner way to handle this
    this->position = Vector3Add(*this->engine->getRearAttachmentPoint(), Vector3Scale(engineBack, this->followDistance * 5));
}

/**
 * Update method
 * @param deltaTime Time in seconds for last frame drawn
*/
void TrainCar::update(float deltaTime) {
    this->rearAttachmentPoint = Vector3Subtract(this->position, Vector3Scale(this->getForwardVector(), 2.f));

   // Get rear attachment point of parent
   Vector3* parentAnchor = this->engine->getRearAttachmentPoint();

    // Update the train car to be pointing towards and following the TrainComponent assigned to the engine parameter
    Vector3 directionToParent = this->getVectorTowardTarget(*parentAnchor);
    Vector3 followPosition = Vector3Scale(Vector3Negate(directionToParent), this->followDistance); // The position the TrainComponent needs to be at to follow its parent at followDistance
    this->position = Vector3Add(*parentAnchor, followPosition);
    this->setRotation({0, this->angleToVector(*parentAnchor), 0});

    // Calculate how long is left until this car can shoot again
    if (!this->canShoot) {
        int64_t timeSinceLastShot = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - this->lastShot).count();

        if (timeSinceLastShot > this->reloadTime) {
            this->canShoot = true;
        } else {
            this->timeUntilReloaded = this->reloadTime - timeSinceLastShot;
        }
    }

    Actor::update();
}



int TrainCar::shoot() {
    this->canShoot = false;
    this->lastShot = std::chrono::steady_clock::now();
    return this->power;
}

bool TrainCar::getCanShoot() {
    return this->canShoot;
}

int TrainCar::getTimeUntilReloaded() {
    return this->timeUntilReloaded;
}