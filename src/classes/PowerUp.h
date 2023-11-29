#pragma once

#include "Actor.h"
#include "src/enums/Direction.h"
#include "src/enums/PowerupType.h"

class Powerup: public Actor {
private:

    // The type of powerup
    PowerupType type;

    // The magnitude of the bonus granted. e.g. A powerup of type HealthPack with magnitude 50 will restore 50 health
    int magnitude;

    bool isAlive = true;

    Direction rotationDirection = Direction::Right;

public:

    Powerup(Vector3 position, Model model, Texture2D texture, PowerupType type, int magnitude);

    PowerupType getType();
    int getMagnitude();
    bool getIsAlive();

    void setIsAlive(bool value);
    
    void update(float deltaTime);

    void draw();

};