#include "CombatManager.h"

CombatManager::CombatManager(FollowCam* camera) {
    this->camera = camera;
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