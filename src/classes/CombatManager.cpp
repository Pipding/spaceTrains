#include "CombatManager.h"

CombatManager::CombatManager() { }

void CombatManager::setTarget(ICombatant* newTarget) {
    this->activeTarget = newTarget;
}