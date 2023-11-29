#include "Projectile.h"
#include "src/globals/AssetManager.h"

// TODO: This is a bit of a kludge. I don't think this class should necessarily be directly using the AssetManager
// but until assets are properly passed around as pointers, this is more performant
static AssetManager& _assets = AssetManager::getInstance();

Projectile::Projectile(Vector3 position, float speed, int damage, Vector3* destination, Model model, Texture2D texture, const char* launchSFX, const char* destroySFX)
    :Actor(position, model, texture), speed(speed), destination(destination), alive(true), damage(damage), launchSFX(launchSFX), destroySFX(destroySFX) {

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

const char* Projectile::getlaunchSFX() {
    return this->launchSFX;
}

const char*  Projectile::getdestroySFX() {
    return this->destroySFX;
}