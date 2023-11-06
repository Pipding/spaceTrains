#include "CombatManager.h"

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

void CombatManager::addCombatant(ICombatant* combatant) {
    this->combatants.push_back(combatant);
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