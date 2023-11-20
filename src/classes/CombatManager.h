#pragma once

#include "raylib.h"
#include "raymath.h"
#include "src/classes/FollowCam.h"
#include "src/classes/Hostile.h"
#include "src/interfaces/ICombatant.h"
#include "src/classes/TrainComponent.h"
#include "src/classes/TrainCar.h"
#include <vector>
#include <chrono>

/**
 * Manager class for combat. Takes care of targeting and acts as an intermediary between objects dealing damage to one another
*/
class CombatManager: public IUpdatable, public IKeyboardListener {
private:
    // The active target of combat
    Hostile* activeTarget = nullptr;

    bool targetLocked = false;

    std::vector<Hostile*> hostiles;

    // A reference to the camera, as the camera is used for targeting
    FollowCam* camera;

    // The actor controlled by the player
    // TODO: This won't always be an actor. In fact it should be an ICombatant
    Actor* player;

    // The player-controlled train
    std::vector<TrainComponent*> train;

    // Which train component does the player currently have selected?
    TrainComponent* activeTrainComponent;
    
    // The index of the active train component in the train vector
    int activeTrainComponentIndex;

    int playerHealth;

public:
    /**
     * Constructor
     * @param camera    A pointer to the main camera
    */
    CombatManager(FollowCam* camera, Actor* playerActor);

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
    */
    void update();
    
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

    /**
     * Initializes the player-controlled train
     * TODO: Not sure if this belongs here. Probably not
     * @param train     A vector of pointers to TrainComponents. The first item in the vector will be considered the default component
    */
    void initializeTrain(std::vector<TrainComponent*> train);

    /**
     * Get the size of the player-controlled train
     * @return  Returns the number of components in the Train (including the Engine)
    */
    int getTrainSize();

    /**
     * Gets the currently active train component
     * @return  The currently selected train component in the train
    */
    TrainComponent* getActiveTrainComponent();

    /**
     * Gets the index of the currently active train component
     * @return  The index of the currently selected train component in the train
    */
    int getActiveTrainComponentIndex();

    /**
     * Returns true if the currently-active train component can fire
     * @param return Returns true if pressing the shoot button should fire a shot
    */
    bool canShoot();

    /**
     * Returns the TrainComponent at the given index of the train
     * @param index     The index of the train form which to fetch a TrainComponent
     * @return Returns a pointer to a TrainComponent
    */
    TrainComponent* getTrainComponent(int index);
};