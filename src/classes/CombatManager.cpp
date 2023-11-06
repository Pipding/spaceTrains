#include "CombatManager.h"

CombatManager::CombatManager() { }

void CombatManager::setTarget(Combatant* newTarget) {
    this->activeTarget = newTarget;
}