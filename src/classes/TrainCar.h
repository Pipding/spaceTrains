#pragma once
#include "TrainComponent.h"

class TrainCar : public TrainComponent {
public:
    TrainComponent* engine;

    TrainCar();
    TrainCar(Model model, Texture2D texture, TrainComponent* engine, Vector3 position = {0.f, 0.f, 0.f}, Vector3 rotation = {0.f, 0.f, 0.f});

    void update();
};