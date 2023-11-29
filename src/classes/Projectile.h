#pragma once

#include "raylib.h"
#include "raymath.h"

#include "Actor.h"
#include "src/interfaces/IUpdatable.h"
class Projectile: public Actor, public IUpdatable {
private:
    float speed;
    Vector3* destination;
    bool alive;
    int damage;
    const char* launchSFX;
    const char* destroySFX;

public:

    /**
     * Constructor
     * @param position      Initial position of the projectile
     * @param speed         Speed at which the projectile will approach its target
     * @param damage        Amount of damage the projectile deals on impact
     * @param destination   Where the projectile should end up
     * @param model         The 3D model which should represent the projectile
     * @param texture       The texture to be applied to the model
     * @param launchSFX     The name of the sound effect to play when this projectile is launched
     * @param destroySFX    The name of the sound effect to play when this projectile is destroyed
    */
    Projectile(Vector3 position, float speed, int damage, Vector3* destination, Model model, Texture2D texture, const char* launchSFX, const char* destroySFX); // TODO: This should accept pointers to model/textures

    /**
     * Updates the projectile, moving it closer to its destination
     * @param deltaTime Time in seconds for last frame drawn
    */
    void update(float deltaTime);

    // TODO: Comments
    bool isAlive();

    int getDamage();
};