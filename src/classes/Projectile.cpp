#include "Projectile.h"

Projectile::Projectile(Vector3 position, Vector3 velocity, Vector3* destination, Model model, Texture2D texture)
    :Actor(position, model, texture), velocity(velocity), destination(destination) {}

void Projectile::update() {

}