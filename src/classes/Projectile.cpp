#include "Projectile.h"

Projectile::Projectile(Vector3 position, float speed, Vector3* destination, Model* model, Texture2D* texture)
    :Actor(position, *model, *texture), destination(destination) {
        this->speed = speed;
        this->alive = true;

        // TODO: This should really be a parameter but in the interest of time it's hard-coded for now
        this->scale = 8.f;
    }

void Projectile::update(float deltaTime) {

    Actor::update();
}

bool Projectile::isAlive() {
    return this->alive;
}