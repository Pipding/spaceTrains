#pragma once

#include "src/interfaces/ICombatant.h"
#include <vector>

/**
 * Manager class for combat. Takes care of targeting and acts as an intermediary between objects dealing damage to one another
*/
class CombatManager {
private:
    // The active target of combat
    ICombatant* activeTarget = nullptr;

    // All extant objects which might engage in combat
    std::vector<ICombatant*> combatants;

    // Constructor
    CombatManager();

    /**
     * Sets the active target
     * @param newTarget     The new target
    */
    void setTarget(ICombatant* newTarget);
};