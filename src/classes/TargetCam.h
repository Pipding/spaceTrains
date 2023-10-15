#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Actor.h"
#include "src/globals/SpaceTrainDebug.h"

/**
* A camera which follows a given Actor with a given offset
*
* This camera is somewhat user adjustable. It can orbit the target from
* side-to-side at an angle determined by the player. When an adjustment of
* this kind has been made, the camera becomes unlocked from the rotation
* of its target, allowing the target to rotate without the camera following 
* its orientation.Resetting the user adjustment re-locks the camera's rotation 
* to its target
*/
class TargetCam {    
public:
    Camera camera = {0};
    Actor* target;

    // The degree by which user adjustment rotates the camera on each update cycle
    float mouseAdjustmentFactor = 0.05f;

    // targetOffset is the position relatigve to the target where the camera should 
    // start its life, and the point to which it should return to when reset
    Vector3 targetOffset;

    TargetCam(Actor*, Vector3);

    void update();
    void resetmouseRotationAdjustment();
    Vector3 calculateAppliedOffset();

private:
    // Side-to-side orbital movement applied by the user
    float userCameraRotationAdjustment = 0.f;

    // The offset from the target's position where the camera actually ends up after all transformations are applied
    Vector3 appliedOffset; // appliedOffset is basically: (target.position + targetOffset) * (defaultCameraRotation + userCameraRotationAdjustment)

    // Used to calculate appliedOffset
    Matrix appliedRotation;

    // The rotation the camera would have without user adjustments
    Vector3 defaultCameraRotation;
};
