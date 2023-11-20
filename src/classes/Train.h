#pragma once
#include "TrainComponent.h"
#include "TrainEngine.h"
#include "TrainCar.h"
#include <initializer_list>
#include <vector>

class Train {
private:
    std::vector<TrainComponent*> train;
    int activeComponentIndex;
    int health;

public:
    /**
     * Constructor
    */
    Train(std::initializer_list<TrainComponent*> cars, int health);

    // ==================================================
    // convenience methods
    // ==================================================
    
    /**
     * Get the TrainEngine at the front of the Train
     * @return Pointer to a TrainEngine
    */
    TrainEngine* head();

    /**
     * Returns the size of the train
     * @return Returns the number of TrainComponents in this Train
    */
    int size();

    // ==================================================
    // Getters
    // ==================================================
    
    /**
     * Gets the TrainComponent at the given index. Can throw an index out-of-bounds-exception
     * @return Returns a pointer to a TrainComponent
    */
    TrainComponent* getComponent(int index);

    /**
     * Gets activeComponentIndex, i.e. the index of the active TrainComponent in the train vector
     * @return int index of the active TrainComponent in the train vector
    */
    int getActiveComponentIndex();

    /**
     * Returns the health of the Train
     * @return int Train health
    */
    int getHealth();

    // ==================================================
    // Functions for handling the active Train component
    // ==================================================

    /**
     * Get the active TrainComponent
     * @return Pointer to the active TrainComponent of the Train
    */
    TrainComponent* getActiveComponent();

    /**
     * Sets the active TrainComponent back to the TrainEngine
     * @return A pointer to the TrainEngine at the front of the Train
    */
    TrainEngine* resetActiveComponent();

    /**
     * Increments the active TrainComponent. "Increments" here means moving toward the
     * back of the train. Wraps around to the front if at the end of the train
     * @return Pointer to the new active TrainComponent
    */
    TrainComponent* incrementActiveComponent();

    /**
     * Decrements the active TrainComponent. "Decrements" here means moving toward the 
     * front of the train. Wraps around to the back if called while at the front of the train
     * @return Pointer to the new active TrainComponent
    */
    TrainComponent* decrementActiveComponent();

    // ==================================================
    // Functions related to shooting
    // ==================================================

    /**
     * Checks if the currently active TrainComponent can shoot
     * @return Returns true if the currently active TrainComponent can shoot, else false
    */
    bool canShoot();

    /**
     * Fire!
     * Note: If the currently active TrainComponent cannot shoot, will return 0
     * @return Returns the outgoing damage
    */
    int shoot();
};