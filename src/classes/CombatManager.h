#pragma once

#include "raylib.h"
#include "raymath.h"
#include "src/classes/FollowCam.h"
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

    // A reference to the camera, as the camera is used for targeting
    FollowCam* camera;

    // The actor controlled by the player
    // TODO: This won't always be an actor. In fact it should be an ICombatant
    Actor* player;

public:
    /**
     * Constructor
     * @param camera    A pointer to the main camera
    */
    CombatManager(FollowCam* camera, Actor* playerActor);

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

    Vector3 calculateNormalizedTargetLocationVector();
};