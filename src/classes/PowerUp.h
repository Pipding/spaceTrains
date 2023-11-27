#pragma once

#include "Actor.h"
#include "src/enums/Direction.h"
#include "src/enums/PowerUpType.h"

class PowerUp: public Actor {
private:

    // The type of powerup
    PowerUpType type;

    // The magnitude of the bonus granted. e.g. A powerup of type HealthPack with magnitude 50 will restore 50 health
    int magnitude;

    Direction rotationDirection = Direction::Right;

public:

    PowerUp(PowerUpType type, int magnitude);

   void update();

   void draw(float deltaTime);

};