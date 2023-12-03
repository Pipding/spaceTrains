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
    this->position = Vector3Add(*this->engine->getRearAttachmentPoint(), Vector3Scale(engineBack, this->followDistance));
}

// https://gamedev.stackexchange.com/a/18998
bool isInFront(Vector3* point, Vector3* planeOrigin, Vector3 planeForward)
{
    double product = (point->x - planeOrigin->x) * planeForward.x
                     + (point->y - planeOrigin->y) * planeForward.y
                     + (point->z - planeOrigin->z) * planeForward.z;
    return (product > 0.f);
}

/**
 * Update method
 * @param deltaTime Time in seconds for last frame drawn
*/
void TrainCar::update(float deltaTime) {
    this->rearAttachmentPoint = Vector3Subtract(this->position, Vector3Scale(this->getForwardVector(), 2.f));
    // Thinking up a new way of handling the rotation/position update
    // Current solution works but allows TrainEngine to rotate freely without impacting cars behind it

    /**
     * Proposed new solution:
     * 1. Calculate the rear anchor point of parent
     * 2. Rotate self to face that rear anchor point
     * 3. New position should be calculated based on new rotation + followDistance
     * 4. Define a plane based on rear anchor point + parent forward vector
     * 5. If my position is positive on that plane;
     *  5a. Push me in the opposite direction of my parent's forward vector
     *  5b. Repeat steps 2-5 until 5 is false
     * 6. New position found
    */ 

   // Get rear attachment point of parent
   Vector3* parentAnchor = this->engine->getRearAttachmentPoint();

   // Calculate what my rotation would need to be to face that
//    float potentialRotation = this->angleToVector(this->engine->position);

   // While I am in the positive of a plane described by my parent's forward vector and anchor point, push me back
//    while (isInFront(&this->position, parentAnchor, this->engine->getForwardVector())) {
//     this->position = Vector3Subtract(this->position, this->engine->getForwardVector());
//    }

//     // When we're no longer in front of the parent, update the rotation
//    float potentialRotation = this->angleToVector(this->engine->position);
//    this->setRotation({0, potentialRotation, 0});

//    // Now make sure we're close enough
//    while ( Vector3Length(this->getVectorTowardTarget(*this->engine->getRearAttachmentPoint(), false)) > this->followDistance ) {
//     this->position = Vector3Add(this->position, this->getForwardVector());
//    }


   



    // Update the train car to be pointing towards and following the TrainComponent assigned to the engine parameter
    // Vector3 directionToParent = this->getVectorTowardTarget(this->engine->position);
    Vector3 directionToParent = this->getVectorTowardTarget(*parentAnchor);
    Vector3 scaledInvertedPulledDirection = Vector3Scale(Vector3Negate(directionToParent), this->followDistance);
    this->position = Vector3Add(*parentAnchor, scaledInvertedPulledDirection);
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