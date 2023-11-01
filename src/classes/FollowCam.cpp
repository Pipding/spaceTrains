#include "FollowCam.h"

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

FollowCam::FollowCam(Actor* parent, Vector3 offset) {
    // Camera initialization borrowed from https://www.youtube.com/watch?v=TTa75ocharg
    this->parent = parent;
    this->parentOffset = offset;
    this->camera.up = {0.0f, 1.0f, 0.0f};
    this->camera.fovy = 90.f;
    this->camera.projection = CAMERA_PERSPECTIVE;

    this->camera.position = Vector3Add(this->parent->position, this->parentOffset);
    this->camera.target = this->parent->position;
}


void FollowCam::update() {

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
        this->defaultCameraRotation = this->parent->rotation;
    }

    this->appliedOffset = calculateAppliedOffset();

    this->camera.position = Vector3Add(this->parent->position, this->appliedOffset);
    this->camera.target = this->parent->position;
}


void FollowCam::resetmouseRotationAdjustment() {
    this->userCameraRotationAdjustment = 0.f;
}


Vector3 FollowCam::calculateAppliedOffset() {
    this->appliedRotation = MatrixRotateXYZ({0.f, userCameraRotationAdjustment + this->defaultCameraRotation.y, 0.f});
    return Vector3Transform(this->parentOffset, this->appliedRotation);
}

void FollowCam::setTarget(Actor* target) {
    this->target = target;
    this->hasTarget = true;
}

void FollowCam::unsetTarget() {
    this->target = NULL;
    this->hasTarget = false;
}

bool FollowCam::getHasTarget() {
    return this->hasTarget;
}