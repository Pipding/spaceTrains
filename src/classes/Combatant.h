#pragma once

/**
 * Any object which needs to use the system must inherit from the Combatant class
*/
class Combatant {
private:
    bool isAlive = true;
    int maxHitpoints = 1;
    int currentHitpoints = 1;
};