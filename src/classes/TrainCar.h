#pragma once
#include "TrainEngine.h"
#include "raylib.h"
#include "raymath.h"
#include "Actor.h"

class TrainCar : public TrainComponent {
public:
    TrainComponent* engine;

    TrainCar();
    TrainCar(Model, Texture2D, TrainComponent*, Vector3 = {0.f, 0.f, 0.f}, Vector3 = {0.f, 0.f, 0.f});

    void update();
};