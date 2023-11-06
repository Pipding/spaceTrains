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

public:
    // Constructor
    CombatManager();

    /**
     * Sets the active target
     * @param newTarget     The new target
    */
    void setTarget(ICombatant* newTarget);

    /**
     * Removes the active target
    */
    void unsetTarget();

    /**
     * Checks whether there's an active target
     * @return Returns true if there's an active target, otherwise false
    */
    bool hasTarget();

    /**
     * Adds the given combatant to the list of combatants managed by the CombatManager
     * @param combatant     Pointer to a combatant
    */
    void addCombatant(ICombatant* combatant);

    /**
     * Returns a pointer to the active target
     * @return  The active target of the CombatManager
    */
    ICombatant* getActiveTarget();
};