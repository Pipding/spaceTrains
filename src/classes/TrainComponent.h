#pragma once
#include "Actor.h"
#include "src/interfaces/IUpdatable.h"

class TrainComponent : public Actor, public IUpdatable {
public:
    /**
     * Default constructor
    */
    TrainComponent();

    /**
     * A component of a Train
     * @param position      The starting position of the TrainComponent
     * @param model         Pointer to a 3D model representing the TrainComponent
     * @param texture       Texture to apply to the model
    */
    TrainComponent(Vector3 position, Model model, Texture2D texture);
 };