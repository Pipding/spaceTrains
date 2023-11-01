#pragma once

#include "Actor.h"

class Hostile : public Actor {
public:
    Vector3* target;
    float speed;
    float targetDistance;
    int hitpoints;

    Hostile(Vector3 position, Model model, Texture texture, Vector3* target);

    void update();
};