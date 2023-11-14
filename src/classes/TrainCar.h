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

    // How long between shots (milliseconds)
    int reloadTime;

    // Use of chrono for time measurement found on StackOverflow here: https://stackoverflow.com/a/27739925
    // Timestamp of the last time this TrainCar fired its weapon
    std::chrono::steady_clock::time_point lastShot = std::chrono::steady_clock::now();

    /**
     * Default constructor
    */
    TrainCar();

    /**
     * A train car. Follows another TrainComponent and can be followed by another TrainCar
     * @param model         The 3D model representing the TrainCar in the world
     * @param texture       The texture to be applied to model
     * @param engine        Pointer to a TrainComponent which this car will follow
     * @param position      Initial position of the train car
     * @param power         How much damage the TrainComponent deals in combat
     * @param reloadTime    Time in milliseconds between shots in combat
    */
    TrainCar(Model model, Texture2D texture, TrainComponent* engine, Vector3 position, int power, int reloadTime);

    /**
     * Update function
    */
    void update();
};