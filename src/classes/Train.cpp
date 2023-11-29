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
}

void Train::draw() {

    // Draw train components
    for (std::vector<TrainComponent*>::iterator it = this->train.begin(); it != this->train.end(); ++it) {
        (*it)->draw();
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

Projectile* Train::shoot(Vector3* targetPos) {
    if (!this->canShoot()) return nullptr;

    int outgoingDamage = dynamic_cast<TrainCar*>(this->getActiveComponent())->shoot(); // Learned about dynamic_cast from StackOverflow: https://stackoverflow.com/a/307801

    // When a projectile is fired, create a new projectile on the heap using "new"
    Projectile* p = new Projectile(this->getActiveComponent()->position, 1000.f, outgoingDamage, targetPos, this->getActiveComponent()->projectileModel, this->getActiveComponent()->projectileTexture);
    
    return p;
}

int Train::receiveDamage(int damageReceived) {
    this->currentHitpoints -= damageReceived;

    if (currentHitpoints < 0) {
        currentHitpoints = 0;
        this->alive = false;
    }

    return currentHitpoints;
}

void Train::receivePowerUp(PowerUp* pup) {
    switch (pup->getType()) {
        case PowerUpType::HealthPack:
            if (this->currentHitpoints + pup->getMagnitude() >= this->maxHitpoints) {
                this->currentHitpoints = this->maxHitpoints;
            } else {
                this->currentHitpoints += pup->getMagnitude();
            }
            break;
        case PowerUpType::SpeedBoost:
            this->head()->topSpeed += pup->getMagnitude();
            break;
        default:
            break;
    }
}