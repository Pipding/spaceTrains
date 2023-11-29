#include "Powerup.h"

Powerup::Powerup(Vector3 position, Model model, Texture2D texture, PowerupType type, int magnitude)
: Actor(position, model, texture) {
    this->type = type;
    this->magnitude = magnitude;
}

PowerupType Powerup::getType() {
    return this->type;
}

int Powerup::getMagnitude() {
    return this->magnitude;
}

bool Powerup::getIsAlive() {
    return this->isAlive;
}

void Powerup::setIsAlive(bool value) {
    this->isAlive = value;
}

void Powerup::update(float deltaTime) {

    this->rotateBy( Vector3Scale({0.f, this->rotationDirection == Direction::Right ? -1.5f : 1.5f, 0.f}, deltaTime));

    Actor::update();    
}

void Powerup::draw() {
    Actor::draw();
}