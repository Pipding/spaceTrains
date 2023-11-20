#include "CombatManager.h"

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

CombatManager::CombatManager(FollowCam* camera, Train* train) {
    this->camera = camera;
    this->train = train;
    this->camera->parent = this->train->head();
}

void CombatManager::setTarget(Hostile* newTarget) {
    this->activeTarget = newTarget;
}

void CombatManager::unsetTarget() {
    this->activeTarget = nullptr;
}

// TODO: Not sure what an inline function is but this seems like a candidate...?
bool CombatManager::hasTarget() {
    return this->activeTarget != nullptr;
}

void CombatManager::addHostile(Hostile* hostile) {
    this->hostiles.push_back(hostile);
}

Hostile* CombatManager::getActiveTarget() {
    return this->activeTarget;
}

bool CombatManager::getTargetLocked() {
    return this->targetLocked;
}

Vector3 CombatManager::calculateNormalizedTargetLocationVector() {
    // TODO: Clean up this function

    // How to work out the vector for detecting targets;
    // Grounded position of camera
    // Subtract position of player from above
    // Normalize

    // Get the vector from the camera to its target
    Vector3 groundedCameraPos = {this->camera->camera.position.x, 0.f, this->camera->camera.position.z};

    Vector3 normalizedTargetingVector = Vector3Normalize(Vector3Subtract(this->train->head()->position, groundedCameraPos));

    return normalizedTargetingVector;
}

Ray CombatManager::getTargetingRay() {
    // The ray needs to be a little bit off the ground. If the Y position
    // is 0, the ray doesn't intersect with bounding boxes which are 
    // on the ground. Hence 0.5f
    return {{this->train->head()->position.x, 0.5f, this->train->head()->position.z}, this->calculateNormalizedTargetLocationVector()};
}

int CombatManager::getPlayerHealth() {
    return this->train->getHealth();
}

void CombatManager::update() {
    Ray targetRay = this->getTargetingRay();

    // Don't bother scanning for new targets if there's already a locked target
    if (!targetLocked) {
        for (Hostile* hostile : this->hostiles ) {
            RayCollision coll = GetRayCollisionBox(targetRay, hostile->getBounds());

            if (coll.hit) {
                this->setTarget(hostile);
            } else {
                this->unsetTarget();
            }
        }
    }
}

void CombatManager::draw() {
    // Only draw CombatManager stuff if debug is enabled
    if (!_debug.getDrawBoundingBoxes()) {
        return;
    }

    DrawLine3D(this->getTargetingRay().position, Vector3Add(this->getTargetingRay().position, Vector3Scale(this->getTargetingRay().direction, 300.f)), this->hasTarget() ? RED : GREEN);
}

void CombatManager::onKeyPressed(int key) {
    if (key == KEY_LEFT_SHIFT) {
        if (this->hasTarget()) {
            this->targetLocked = !this->targetLocked;

            if (this->targetLocked) {
                this->camera->setTarget(this->getActiveTarget());
            } else {
                this->camera->unsetTarget();
                this->camera->parent = this->train->resetActiveComponent();
            }
        }
    } else if (key == KEY_SPACE) {
        if (this->hasTarget() && this->targetLocked) {
            if (!this->train->canShoot()) return;
            
            this->getActiveTarget()->receiveDamage(this->train->shoot());
        }
    } else if (key == KEY_UP) {
        if (this->targetLocked) {
            this->camera->parent = this->train->incrementActiveComponent();
        }
    } else if (key == KEY_DOWN) {
        if (this->targetLocked) {
            this->camera->parent = this->train->decrementActiveComponent();
        }
    }
}

void CombatManager::onKeyReleased(int key) { }