#include "Projectile.h"

Projectile::Projectile(Vector3 position, float speed, int damage, Vector3* destination, Model model, Texture2D texture)
    :Actor(position, model, texture), destination(destination) {
        this->speed = speed;
        this->damage = damage;
        this->alive = true;

        // TODO: This should really be a parameter but in the interest of time it's hard-coded for now
        this->scale = 8.f;
    }

void Projectile::update(float deltaTime) {

    Vector3 vectorToTarget = this->getVectorTowardTarget(*this->destination, false);

    // If the projectile has reached (or is close enough) to its target, destroy projectile
    if (Vector3Length(vectorToTarget) <= 10.f) {
        this->alive = false;
    } else {
        this->setRotation({0, this->angleToVector(*this->destination), 0});

        // Move the projectile towards its target at speed
        this->position = Vector3Add(this->position, Vector3Scale(this->getVectorTowardTarget(*this->destination), this->speed * deltaTime));

        Actor::update();
    }
}

bool Projectile::isAlive() {
    return this->alive;
}

int Projectile::getDamage() {
    return this->damage;
}