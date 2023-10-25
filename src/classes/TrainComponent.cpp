#include "TrainComponent.h"

/**
 * Default TrainComponent constructor
*/
TrainComponent::TrainComponent()
:Actor(){ }


/**
 * A train component - either an engine or a car
 * @param position          Initial position of the train component
 * @param model             The 3D model representing the TrainEngine in the world
 * @param texture           The texture to be applied to model
*/
TrainComponent::TrainComponent(Vector3 position, Model model, Texture2D texture)
:Actor(position, model, texture){ }