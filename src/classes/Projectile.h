#pragma once

#include "raylib.h"
#include "raymath.h"

#include "Actor.h"

class Projectile: public Actor {
private:
    Vector3 velocity;
    Vector3* destination;

public:

    /**
     * Constructor
     * @param position      Initial position of the projectile
     * @param velocity      Velocity at which the projectile will approach its target
     * @param destination   Where the projectile should end up
     * @param model         The 3D model which should represent the projectile
     * @param texture       The texture to be applied to the model
    */
    Projectile(Vector3 position, Vector3 velocity, Vector3* destination, Model model, Texture2D texture);

    /**
     * Updates the projectile, moving it closer to its destination
    */
    void update();
};