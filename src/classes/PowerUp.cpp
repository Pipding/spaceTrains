#include "PowerUp.h"

PowerUp::PowerUp(Vector3 position, Model model, Texture2D texture, PowerUpType type, int magnitude)
: Actor(position, model, texture) {
    this->type = type;
    this->magnitude = magnitude;
}

PowerUpType PowerUp::getType() {
    return this->type;
}

int PowerUp::getMagnitude() {
    return this->magnitude;
}

void PowerUp::update(float deltaTime) {

    this->rotateBy( Vector3Scale({0.f, this->rotationDirection == Direction::Right ? -1.5f : 1.5f, 0.f}, deltaTime));

    Actor::update();    
}

void PowerUp::draw() {
    Actor::draw();
}