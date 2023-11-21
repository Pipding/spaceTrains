#pragma once

#include "Actor.h"
#include "src/interfaces/ICombatant.h"
#include "src/interfaces/IUpdatable.h"

class Hostile : public Actor, public ICombatant, public IUpdatable {
public:
    Vector3* target;
    float speed;
    float targetDistance;

    Hostile(Vector3 position, Model model, Texture texture, Vector3* target);

    /**
     * Update method
     * @param deltaTime Time in seconds for last frame drawn
    */
    void update(float deltaTime);

    /**
     * Removes the given damageReceived from the Hostile's currentHitpoints. Will not reduce currentHitpoints below 0
     * @param damageReceived    The amount of damage to subtract from currentHitpoints
     * @return  Returns currentHitpoints after damage is applied
    */
    int receiveDamage(int damageReceived);
};