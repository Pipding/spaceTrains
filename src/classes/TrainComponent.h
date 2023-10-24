#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Actor.h"

class TrainComponent {
public:
    Vector3 position;
    Vector3 rotation;
    Actor* actor;
};