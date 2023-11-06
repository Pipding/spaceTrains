#pragma once

#include "src/classes/Combatant.h"
#include <vector>

/**
 * Manager class for combat. Takes care of targeting and acts as an intermediary between objects dealing damage to one another
*/
class CombatManager {
private:
    // The active target of combat
    Combatant* activeTarget = nullptr;

    // All extant objects which might engage in combat
    std::vector<Combatant*> combatants;

    // Constructor
    CombatManager();

    /**
     * Sets the active target
     * @param newTarget     The new target
    */
    void setTarget(Combatant* newTarget);
};