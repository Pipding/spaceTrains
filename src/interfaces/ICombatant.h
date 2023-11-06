#pragma once

/**
 * Any object which needs to use the system must inherit from the Combatant class
*/
class ICombatant {
protected:
    // Is the combatant still alive? (Can it move, deal dadmage, be damaged etc.)
    bool isAlive = true;
    int maxHitpoints = 1;
    int currentHitpoints = 1;
};