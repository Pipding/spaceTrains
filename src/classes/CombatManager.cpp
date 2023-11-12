#include "CombatManager.h"

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

CombatManager::CombatManager(FollowCam* camera, Actor* player) {
    this->camera = camera;
    this->player = player;
}

void CombatManager::setTarget(ICombatant* newTarget) {
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

ICombatant* CombatManager::getActiveTarget() {
    return this->activeTarget;
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

    for (Hostile* hostile : this->hostiles ) {
        RayCollision coll = GetRayCollisionBox(targetRay, hostile->getBounds());

        if (coll.hit) {
            this->setTarget(hostile);
            this->camera->setTarget(hostile);
        }
    }
}

void CombatManager::draw() {
    // Only draw CombatManager stuff if debug is enabled
    if (!_debug.getDrawBoundingBoxes()) {
        return;
    }

    DrawLine3D(this->getTargetingRay().position, Vector3Add(this->getTargetingRay().position, Vector3Scale(this->getTargetingRay().direction, 300.f)), this->hasTarget() ? RED : BLUE);
}