#include "Train.h"

// The idea to use an initializer_list here comes from https://stackoverflow.com/a/13979720
Train::Train(std::initializer_list<TrainComponent*> cars, int health)
:train(cars) {
    this->maxHitpoints = health;
    this->currentHitpoints = health;
    this->activeComponentIndex = 0;
}

void Train::update(float deltaTime) {

    // Update each train component
    for (std::vector<TrainComponent*>::iterator it = this->train.begin(); it != this->train.end(); ++it) {
        dynamic_cast<IUpdatable*>(*it)->update(deltaTime);
    }

    // Update projectiles. This loop needs to be different because projectiles are stored on the heap
    // and therefore we're responsible for deleting them. If we delete them without removing them
    // from the projectiles vector, we'll crash. If we remove something from the projectiles vector
    // in a for loop, we'll skip an element
    // This loop sourced from StackOverflow: https://stackoverflow.com/a/13102374

    std::vector<Projectile*>::iterator it = this->projectiles.begin();

    while (it != this->projectiles.end()) {
        // Update the projectile
        dynamic_cast<IUpdatable*>(*it)->update(deltaTime);

        // If the projectile is not alive after updating, delete it from heap and vector
        if (!(*it)->isAlive()) {
            delete (*it);
            it = this->projectiles.erase(it);
        }
        else {
            ++it;
        }
    }
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
    return this->currentHitpoints;
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

int Train::shoot(Vector3* targetPos) {
     if (!this->canShoot()) return 0;

    // When a projectile is fired, create a new projectile on the heap using "new"
    Projectile* p = new Projectile(this->head()->position, 10.f, targetPos, this->getActiveComponent()->projectileModel, this->getActiveComponent()->projectileTexture);

    // The address of the new projectile is stored in the projectiles vector
    this->projectiles.push_back(p);

    // Tell the active train component to shoot
    return dynamic_cast<TrainCar*>(this->getActiveComponent())->shoot(); // Learned about dynamic_cast from StackOverflow: https://stackoverflow.com/a/307801
}

int Train::receiveDamage(int damageReceived) {
    this->currentHitpoints -= damageReceived;

    if (currentHitpoints < 0) {
        currentHitpoints = 0;
    }

    return currentHitpoints;
}