#include "TrainCar.h"

TrainCar::TrainCar(): TrainComponent() { }


TrainCar::TrainCar(Model model, Texture2D texture, TrainComponent* engine, Vector3 position, int power, float reloadTime)
: TrainComponent(position, model, texture) {
    this->engine = engine;
    this->rotation = {0.f, 0.f, 0.f};
    this->power = power;
    this->reloadTime = reloadTime;
}

void TrainCar::update() {
    // Update the train car to be pointing towards and following the TrainComponent assigned to the engine parameter
    Vector3 pulledDirection = this->getVectorTowardTarget(this->engine->position);
    Vector3 invertedPulledDirection = Vector3Negate(pulledDirection);
    Vector3 scaledInvertedPulledDirection = Vector3Scale(invertedPulledDirection, 50.f);
    this->position = Vector3Add(this->engine->position, scaledInvertedPulledDirection);
    this->setRotation({0, this->angleToVector(this->engine->position), 0});

    Actor::update();
}
