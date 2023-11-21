#pragma once

#include "raylib.h"
#include "raymath.h"
#include "src/classes/FollowCam.h"
#include "src/classes/Hostile.h"
#include "src/interfaces/ICombatant.h"
#include "src/interfaces/IUpdatable.h"
#include "src/classes/Train.h"
#include <vector>
#include <chrono>

/**
 * Manager class for combat. Takes care of targeting and acts as an intermediary between objects dealing damage to one another
*/
class CombatManager: public IUpdatable, public IKeyboardListener {
private:

    // ==================================================
    // Targeting member variables
    // ==================================================
    // The Hostile the player is currently looking at (if any)
    Hostile* activeTarget = nullptr;

    // Is the player locked onto activeTarget
    bool targetLocked = false;

    // A reference to the camera - used for targeting
    FollowCam* camera;

    // ==================================================
    // Combatants (Hostiles & the player)
    // ==================================================
    std::vector<Hostile*> hostiles;
    Train* train;

public:
    /**
     * Constructor
     * @param camera    A pointer to the main camera
    */
    CombatManager(FollowCam* camera, Train* train);

    /**
     * Sets the active target
     * @param newTarget     The new target
    */
    void setTarget(Hostile* newTarget);

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
    Hostile* getActiveTarget();

    /**
     * Returns true if the activeTarget is locked
    */
    bool getTargetLocked();

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
     * Returns the current health of the player
    */
    int getPlayerHealth();
    
    /**
     * Update function. Currently only checks if the player is facing a Hostile and,
     * if so, allows the player to lock on to that Hostile with right click
     * @param deltaTime Time in seconds for last frame drawn
    */
    void update(float deltaTime);
    
    /**
     * Draw function
    */
    void draw();

    /**
     * Callback which will be invoked whenever a key pressed event this class listens to is triggered
     * @param key   The key which was pressed
    */
    void onKeyPressed(int key);

    /**
     * Callback which will be invoked whenever a key released event this class listens to is triggered
     * @param key   The key which was released
    */
    void onKeyReleased(int key);
};