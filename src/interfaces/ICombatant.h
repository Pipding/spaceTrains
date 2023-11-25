#pragma once

/**
 * Any object which needs to use the system must inherit from the Combatant class
*/
class ICombatant {
protected:
    // Is the combatant still alive? (Can it move, deal dadmage, be damaged etc.)
    bool alive = true;
    int maxHitpoints = 1;
    int currentHitpoints = 1;

    /**
     * Removes the given damageReceived from the combatant's currentHitpoints. Will not reduce currentHitpoints below 0
     * @param damageReceived    The amount of damage to subtract from currentHitpoints
     * @return  Returns currentHitpoints after damage is applied
    */
    int receiveDamage(int damageReceived);

public:
    /**
     * Returns true if the combatant is alive
    */
    bool isAlive();
};