#include "FollowCam.h"
#include "raylib.h"

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

    // If there's no target, the player has control over the camera
    if (!this->hasTarget) {
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
    }

    this->appliedOffset = calculateAppliedOffset();

    this->camera.position = Vector3Add(this->parent->position, this->appliedOffset);
    this->camera.target = this->parent->position;
}

void FollowCam::draw() {
    // Only draw camera stuff if debug is enabled
    if (!_debug.getDrawBoundingBoxes()) {
        return;
    }

    if (this->hasTarget) {
        Vector3 vectorToTarget = this->parent->getVectorTowardTarget(this->target->position, false);
        DrawLine3D(this->parent->position, Vector3Add(this->parent->position, vectorToTarget), RED);
    }
}


void FollowCam::resetmouseRotationAdjustment() {
    this->userCameraRotationAdjustment = 0.f;
}


Vector3 FollowCam::calculateAppliedOffset() {

    // To calculate the rotation applied to the camera
    // We generate a Matrix called appliedRotation
    // This applied rotation is then applied to parentOffset, which is the offset
    // from the parent object where the camera normally lives.

    // If we have a target, appliedRotation is calculated by looking at the angle between
    // the parent of the FollowCam and the target of the FollowCam
    // If we don't have a target, appliedRotation is calcualted using a
    // variable called userCameraRotationAdjustment

    if (this->hasTarget) {
        this->appliedRotation = MatrixRotateXYZ({0.f, this->parent->angleToVector(this->target->position), 0.f});
    } else {
        this->appliedRotation = MatrixRotateXYZ({0.f, userCameraRotationAdjustment + this->defaultCameraRotation.y, 0.f});
    }
    
    return Vector3Transform(this->parentOffset, this->appliedRotation);
}

// ==================================================
// Target-related functions
// ==================================================
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

void FollowCam::onKeyPressed(int key) {
    if (key == KEY_R) {
        this->resetmouseRotationAdjustment();
    }
}

void FollowCam::onKeyReleased(int key) { }