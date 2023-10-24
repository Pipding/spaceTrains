#pragma once
#include "TrainEngine.h"
#include "raylib.h"
#include "raymath.h"
#include "Actor.h"

class TrainCar {
public:
    TrainEngine* engine; // TODO: This needs to be a generic TrainComponent class, but that class doesn't exist yet
    Actor actor;
    Vector3 position = {0.f, 0.f, 0.f};
    Vector3 rotation = {0.f, 0.f, 0.f};

    TrainCar();
    TrainCar(TrainEngine*, Actor, Vector3 = {0.f, 0.f, 0.f}, Vector3 = {0.f, 0.f, 0.f});

    void draw();
    void update();
};