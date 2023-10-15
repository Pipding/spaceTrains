#include "TargetCam.h"

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

/**
 * A camera which follows an actor at a distance
 * @param target    Pointer to the Actor to follow
 * @param offset    Offset from the Actor's location where the camera should be by default
*/
TargetCam::TargetCam(Actor* target, Vector3 offset) {
    this->target = target;
    this->targetOffset = offset;
    this->camera.up = {0.0f, 1.0f, 0.0f};
    this->camera.fovy = 90.f;
    this->camera.projection = CAMERA_PERSPECTIVE;

    this->camera.position = Vector3Add(this->target->position, this->targetOffset);
    this->camera.target = this->target->position;
}

/**
 * Update camera position & target vectors
*/
void TargetCam::update() {

    Vector2 mouseDelta = GetMouseDelta();

    if (mouseDelta.x < 0) {
        this->mouseRotationAdjustment += this->mouseAdjustmentFactor;
    } else if (mouseDelta.x > 0) {
        this->mouseRotationAdjustment -= this->mouseAdjustmentFactor;
    }

    this->appliedRotation = MatrixRotateXYZ({0.f, mouseRotationAdjustment + this->target->rotation.y, 0.f});
    this->appliedOffset = Vector3Transform(this->targetOffset, this->appliedRotation);
    this->camera.position = Vector3Add(this->target->position, this->appliedOffset);

    this->camera.target = this->target->position;
}

/**
 * Resets any adjustments which have been made to the camera rotation by the user
*/
void TargetCam::resetmouseRotationAdjustment() {
    this->mouseRotationAdjustment = 0.f;
}