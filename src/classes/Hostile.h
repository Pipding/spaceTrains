#pragma once

#include "Actor.h"

class Hostile : public Actor {
public:
    Vector3* target;
    float speed;
    float targetDistance;
    int maxHitpoints;
    int currentHitpoints;

    Hostile(Vector3 position, Model model, Texture texture, Vector3* target);

    void update();

    /**
     * Removes the given damageReceived from the Hostile's currentHitpoints. Will not reduce currentHitpoints below 0
     * @param damageReceived    The amount of damage to subtract from currentHitpoints
     * @return  Returns currentHitpoints after damage is applied
    */
    int receiveDamage(int damageReceived);
};