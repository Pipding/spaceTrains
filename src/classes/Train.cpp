#include "Train.h"

// The idea to use an initializer_list here comes from https://stackoverflow.com/a/13979720
Train::Train(std::initializer_list<TrainComponent*> cars, int health)
    :train(cars) {
        this->health = health;
        this->activeComponentIndex = 0;
    }

TrainEngine* Train::head() {
    return dynamic_cast<TrainEngine*>(this->getComponent(0));
}

TrainComponent* Train::getComponent(int index) {
    return this->train[index];
}

int Train::size() {
    return this->train.size();
}

int Train::getHealth() {
    return this->health;
}

TrainComponent* Train::getActiveComponent() {
    return this->train[this->activeComponentIndex];
}

TrainEngine* Train::resetActiveComponent() {
    this->activeComponentIndex = 0;
    return this->head();
}

int Train::getActiveComponentIndex() {
    return this->activeComponentIndex;
}

TrainComponent* Train::incrementActiveComponent() {
    this->activeComponentIndex = (this->activeComponentIndex + 1) % this->train.size();
    return this->getActiveComponent();
}

TrainComponent* Train::decrementActiveComponent() {
    this->activeComponentIndex = this->activeComponentIndex == 0 ? this->train.size() - 1 : this->activeComponentIndex - 1;
    return this->getActiveComponent();
}

bool Train::canShoot() {
    if (this->activeComponentIndex == 0) {
        return false;
    }
    
    // TODO: This is kind of flaky. It'll crash if the active component
    // cannot be cast to a TrainCar. There's at least one safeguard above
    return dynamic_cast<TrainCar*>(this->getActiveComponent())->getCanShoot();
}

int Train::shoot() {
     if (!this->canShoot()) return 0;

     // Cast source from StackOverflow: https://stackoverflow.com/a/307801
     return dynamic_cast<TrainCar*>(this->getActiveComponent())->shoot();
}