#pragma once
#include "TrainComponent.h"
#include <chrono>

class TrainCar : public TrainComponent {
public:
    // The TrainComponent which this car follows
    // TODO: Rename
    TrainComponent* engine;

    // How much damage does this TrainCar deal when it shoots?
    int power;

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
     * @param position              Initial position of the train car
     * @param power                 How much damage the TrainComponent deals in combat
     * @param reloadTime            Time in milliseconds between shots in combat
     * @param projectileModel       3D model representing the projectiles fired by this TrainComponent
     * @param projectileTexture     Texture to apply to the projectiles
    */
    TrainCar(Model model, Texture2D texture, TrainComponent* engine, Vector3 position, int power, int reloadTime, Model projectileModel, Texture2D projectiletexture); // TODO: This should accept pointers to model/texture

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