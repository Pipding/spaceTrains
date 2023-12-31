#pragma once
#include "TrainComponent.h"
#include <chrono>

class TrainCar : public TrainComponent {
public:
    // The TrainComponent which this car follows
    // TODO: Rename
    TrainComponent* engine;

    // The distance at which this TrainCar follows the TrainComponent ahead of it
    float followDistance;

    // How much damage does this TrainCar deal when it shoots?
    int power;

    const char* projectileLaunchSFX;
    const char* projectileDestroySFX;

    bool canShoot;

    // How long between shots (milliseconds)
    int reloadTime;

    // Use of chrono for time measurement found on StackOverflow here: https://stackoverflow.com/a/27739925
    // Timestamp of the last time this TrainCar fired its weapon
    std::chrono::steady_clock::time_point lastShot = std::chrono::steady_clock::now();

    // The number of milliseconds until this car can fire another shot
    int timeUntilReloaded;

    /**
     * Default constructor
    */
    TrainCar();

    /**
     * A train car. Follows another TrainComponent and can be followed by another TrainCar
     * @param model                 The 3D model representing the TrainCar in the world
     * @param texture               The texture to be applied to model
     * @param engine                Pointer to a TrainComponent which this car will follow
     * @param followDist            Distance at which this TrainCar follows the TrainComponent given as the engine param
     * @param power                 How much damage the TrainComponent deals in combat
     * @param reloadTime            Time in milliseconds between shots in combat
     * @param projectileModel       3D model representing the projectiles fired by this TrainComponent
     * @param projectileTexture     Texture to apply to the projectiles
     * @param projectileLaunchSFX   Name of the Sound to play when a projectile is fired
     * @param projectileDestroySFX  Name of the Sound to play when a projectile is destroyed
    */
    TrainCar(Model model, Texture2D texture, TrainComponent* engine, float followDist, int power, int reloadTime, Model projectileModel, Texture2D projectiletexture, const char* projectileLaunchSFX, const char* projectileDestroySFX); // TODO: This should accept pointers to model/texture

    /**
     * Update function
     * @param deltaTime Time in seconds for last frame drawn
    */
    void update(float delatTime);

    /**
     * Handles the logic associated with shooting
     * @return Returns the outgoing damage of this TrainCar
    */
    int shoot();

    /**
     * Returns true if this TrainCar can currently fire a shot
     * @return True if the this TrainCar can currently fire a shot
    */
    bool getCanShoot();

    /**
     * Gets amount of time until this TrainCar can shoot again
     * @return int milliseconds
    */
   int getTimeUntilReloaded();
};