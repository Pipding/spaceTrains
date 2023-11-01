#pragma once
#include "Actor.h"

class TrainComponent : public Actor {
public:
    int hitpoints;

    TrainComponent();
    TrainComponent(Vector3 position, Model model, Texture2D texture);
 };