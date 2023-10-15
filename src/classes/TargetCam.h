#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Actor.h"
#include "src/globals/SpaceTrainDebug.h"

/**
* A camera which follows a given Actor with a given offset
*/
class TargetCam {    
public:
    Camera camera = {0};
    Actor* target;
    Vector3 targetOffset;

    TargetCam(Actor*, Vector3);

    void update();
};
