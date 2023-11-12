#pragma once

#include "raylib.h"
#include "raymath.h"
#include "src/classes/FollowCam.h"
#include "src/classes/Hostile.h"
#include "src/interfaces/ICombatant.h"
#include <vector>

/**
 * Manager class for combat. Takes care of targeting and acts as an intermediary between objects dealing damage to one another
*/
class CombatManager: public IUpdatable {
private:
    // The active target of combat
    ICombatant* activeTarget = nullptr;

    std::vector<Hostile*> hostiles;

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
     * Adds the given Hostile to the list of Hostiles managed by the CombatManager
     * @param hostile     Pointer to a Hostile to be added to the CombatManager
    */
    void addHostile(Hostile* hostile);

    /**
     * Returns a pointer to the active target
     * @return  The active target of the CombatManager
    */
    ICombatant* getActiveTarget();

    /**
     * Calculates a normalized vector which is projected from the player Actor in the direction the camera faces
    */
    Vector3 calculateNormalizedTargetLocationVector();

    /**
     * Gets a Ray which is used for detecting which, if any, Hostiles the camera is facing towards
     * @return  A Ray with origin at the player's position pointing 
     *          in the same direction as the camera
    */
    Ray getTargetingRay();
    
    /**
     * Update function. Currently only checks if the player is facing a Hostile and,
     * if so, allows the player to lock on to that Hostile with right click
    */
    void update();
    
    /**
     * Draw function
    */
    void draw();
};