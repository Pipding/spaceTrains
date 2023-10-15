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

    // The degree by which mouse movements can rotate the camera on each update cycle
    float mouseAdjustmentFactor = 0.05f;

    // targetOffset represents the offset from the target's position 
    // where the camera should start its life, and the point to which
    // it should return to when reset
    Vector3 targetOffset;

    TargetCam(Actor*, Vector3);

    void update();
    void resetmouseRotationAdjustment();

private:
    // mouseAdjustedOffset is applied by the user. Side-to-side
    // movement is translated into a rotation around the target
    float mouseRotationAdjustment = 0.f;

    // appliedOffset is the offset from the target's position where
    // the camera actually ends up after all transformations are applied
    // appliedOffset is basically: (target.position + targetOffset) * (target.rotation + mouseRotationAdjustment)
    Vector3 appliedOffset;

    // appliedRotation is used to calculate appliedOffset
    Matrix appliedRotation;
};
