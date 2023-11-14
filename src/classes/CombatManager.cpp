#include "CombatManager.h"

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

CombatManager::CombatManager(FollowCam* camera, Actor* player) {
    this->camera = camera;
    this->player = player;
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

    Vector3 normalizedTargetingVector = Vector3Normalize(Vector3Subtract(this->player->position, groundedCameraPos));

    return normalizedTargetingVector;
}

Ray CombatManager::getTargetingRay() {
    // The ray needs to be a little bit off the ground. If the Y position
    // is 0, the ray doesn't intersect with bounding boxes which are 
    // on the ground. Hence 0.5f
    return {{this->player->position.x, 0.5f, this->player->position.z}, this->calculateNormalizedTargetLocationVector()};
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
                this->activeTrainComponentIndex = 0;
                this->activeTrainComponent = this->train[0];
                this->camera->parent = this->activeTrainComponent;
            }
        }
    } else if (key == KEY_SPACE) {
        if (this->hasTarget() && this->targetLocked) {
            // If the activeTrainComponentIndex is 0 then the player has the engine selected. The engine can't fire, so return
            if (this->activeTrainComponentIndex == 0) return;

            TrainCar* shooter = dynamic_cast<TrainCar*>(this->activeTrainComponent);

            if (!shooter->getCanShoot()) return;

            // Cast source from StackOverflow: https://stackoverflow.com/a/307801
            this->getActiveTarget()->receiveDamage(shooter->shoot());
        }
    } else if (key == KEY_UP) {
        if (this->targetLocked) {
            this->activeTrainComponentIndex = (this->activeTrainComponentIndex + 1) % this->train.size();
            this->activeTrainComponent = this->train[this->activeTrainComponentIndex];
            this->camera->parent = this->activeTrainComponent;
        }
    } else if (key == KEY_DOWN) {
        if (this->targetLocked) {
            this->activeTrainComponentIndex = this->activeTrainComponentIndex == 0 ? this->train.size() - 1 : this->activeTrainComponentIndex - 1;
            this->activeTrainComponent = this->train[this->activeTrainComponentIndex];
            this->camera->parent = this->activeTrainComponent;
        }
    }
}

void CombatManager::onKeyReleased(int key) { }

void CombatManager::initializeTrain(std::vector<TrainComponent*> train) {
    this->train = train;
    this->activeTrainComponentIndex = 0;
    this->activeTrainComponent = this->train[0];
    this->camera->parent = this->activeTrainComponent;
}

int CombatManager::getTrainSize() {
    return this->train.size();
}

TrainComponent* CombatManager::getActiveTrainComponent() {
    return this->activeTrainComponent;
}

int CombatManager::getActiveTrainComponentIndex() {
    return this->activeTrainComponentIndex;
}

bool CombatManager::canShoot() {
    if (this->getActiveTrainComponentIndex() == 0) {
        return false;
    }

    // TODO: This is kind of flaky. It'll crash if the active component
    // cannot be cast to a TrainCar. There's at least one safeguard above
    return dynamic_cast<TrainCar*>(this->activeTrainComponent)->getCanShoot();
}
