#include "TargetCam.h"

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

/**
 * A camera which follows an actor at a distance
 * @param target    Pointer to the Actor to follow
 * @param offset    Offset from the Actor's location where the camera should be by default
*/
TargetCam::TargetCam(Vector3* target, Vector3 offset) {
    // Camera initialization borrowed from https://www.youtube.com/watch?v=TTa75ocharg
    this->target = target;
    this->targetOffset = offset;
    this->camera.up = {0.0f, 1.0f, 0.0f};
    this->camera.fovy = 90.f;
    this->camera.projection = CAMERA_PERSPECTIVE;

    this->camera.position = Vector3Add(*this->target, this->targetOffset);
    this->camera.target = *this->target;
}

/**
 * Update camera position & target vectors
*/
void TargetCam::update(Vector3 rotation) {

    //==================================================
    // User control
    // TODO: Not a fan of user control being inside here
    //==================================================
    Vector2 mouseDelta = GetMouseDelta();

    if (mouseDelta.x < 0) {
        this->userCameraRotationAdjustment += this->mouseAdjustmentFactor;
    } else if (mouseDelta.x > 0) {
        this->userCameraRotationAdjustment -= this->mouseAdjustmentFactor;
    }

    //==================================================
    // Camera position calculation
    //==================================================
    if (userCameraRotationAdjustment == 0.f) {
        this->defaultCameraRotation = rotation;
    }

    this->appliedOffset = calculateAppliedOffset();

    this->camera.position = Vector3Add(*this->target, this->appliedOffset);
    this->camera.target = *this->target;
}

/**
 * Resets any adjustments which have been made to the camera rotation by the user
*/
void TargetCam::resetmouseRotationAdjustment() {
    this->userCameraRotationAdjustment = 0.f;
}

/**
 * Calculates the camera's offset from the target's position, accounting for
 * user adjustment
 * Note: This function will recalculate and update appliedRotation
*/
Vector3 TargetCam::calculateAppliedOffset() {
    this->appliedRotation = MatrixRotateXYZ({0.f, userCameraRotationAdjustment + this->defaultCameraRotation.y, 0.f});
    return Vector3Transform(this->targetOffset, this->appliedRotation);
}