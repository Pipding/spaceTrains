#pragma once

#include "raylib.h"
#include "raymath.h"
#include "ScoreManager.h"
#include "src/classes/FollowCam.h"
#include "src/classes/Hostile.h"
#include "src/classes/Powerup.h"
#include "src/interfaces/ICombatant.h"
#include "src/interfaces/IUpdatable.h"
#include "src/classes/Train.h"
#include "src/globals/AssetManager.h"
#include "src/globals/AudioManager.h"
#include "src/globals/GameStateManager.h"
#include <chrono>
#include <map>
#include <vector>

/**
 * Manager class for combat. Takes care of targeting and acts as an intermediary between objects dealing damage to one another
*/
class CombatManager: public IUpdatable, public IKeyboardListener {
private:

    // Reference to the ScoreManager so the CombatManager can update the score
    ScoreManager* scoreManager;

    // Types of Hostiles the CombatManager can spawn
    std::vector<Hostile> hostileTypes;

    // Types of Powerup the CombatManager can spawn
    std::vector<Powerup> powerupTypes;

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

    std::map<ICombatant*, std::vector<Projectile*>> projectiles;
    Train* train;

    std::vector<Powerup*> powerups;

    void spawnHostile();

    void spawnPowerup(Vector3 pos);

public:
    /**
     * Constructor
     * @param camera    A pointer to the main camera
     * TODO: Update comments
    */
    CombatManager(FollowCam* camera, Train* train, ScoreManager* scoreMan);

    Train* getTrain();

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
     * Gets an array of targeting rays which can be used to scan for targets
     * @param count     The number of rays which should be in the array
     * @param spacing   The amount of space between adjacent rays
     * @return Returns a vector of Rays
    */
    std::vector<Ray> getTargetingRays(int count, float spacing);

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

    // =======================================================
    // Convenience functions. Really just exctracting these
    // for code readability
    // =======================================================
    
    /**
     * Defines all the hostiles the CombatManager can spawn
     * and adds them to the hostileTypes map
    */
    void populateHostileTypes();

    /**
     * Defines all the powerups the CombatManager can spawn
     * and adds them to the powerupTypes map
    */
    void populatePowerupTypes();
};