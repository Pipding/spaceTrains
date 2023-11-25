#pragma once

#include "Actor.h"
#include "src/interfaces/ICombatant.h"
#include "src/interfaces/IUpdatable.h"

// TODO:
// What does a hostile want to do?
// Get within weapons range of the player
// If they're within shooting distance, try to orbit the player
// Shoot at the player
// If their health gets low, run away




class Hostile : public Actor, public ICombatant, public IUpdatable {
private:
    // Hostile needs to get within this range before it can fire its weapon
    float maxEngagementDistance;

    // Hostile can't fire weapons if it's too close to target.
    // If hostile is too close to target, it should try to get away
    float minEngagementDistance;

    // How far away the Hostile is from its target
    float distanceToTarget;

    // Normalized Vector3 pointing to the target
    Vector3 directionToTarget;

    // Is the hostile trying to escape?
    bool isFleeing = false;

    float maxSpeed;

    float currentSpeed;

public:
    Vector3* target;

    Hostile(Vector3 position, Model model, Texture texture, Vector3* target, float minEngagementDistance, float maxEngagementDistance, float maxSpeed);

    /**
     * Update method
     * @param deltaTime Time in seconds for last frame drawn
    */
    void update(float deltaTime);

    // ==================================================
    // Getters & setters
    // ==================================================

    /**
     * Sets isfleeing, which determines whether the Hostile is trying to get away from its target
     * @param value     Value to set isFleeing to
    */
    void setIsFleeing(bool value);

    bool getIsFleeing();

    float getMinEngagementDistance();

    float getMaxEngagementDistance();

    float getMaxSpeed();

    // ==================================================
    // Combat-related functions
    // ==================================================

    /**
     * Checks if the Hostile can shoot
     * @return Returns true if the Hostile can shoot, else false
    */
    bool canShoot();

    /**
     * Fire!
     * Note: If the Hostile cannot shoot, will return 0
     * @param targetPos     Pointer to a vector representing the target being shot
     * @return Returns the outgoing damage
    */
    int shoot(Vector3* targetPos);

    /**
     * Removes the given damageReceived from the Hostile's currentHitpoints. Will not reduce currentHitpoints below 0
     * @param damageReceived    The amount of damage to subtract from currentHitpoints
     * @return  Returns currentHitpoints after damage is applied
    */
    int receiveDamage(int damageReceived);

    /**
     * Returns true if the combatant is alive
    */
    bool isAlive();
};