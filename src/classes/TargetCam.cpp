#include "TargetCam.h"
#include "src/globals/SpaceTrainDebug.h"

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

    this->update();
}

/**
 * Update camera position & target vectors
*/
void TargetCam::update() {
    this->camera.position = Vector3Add(this->target->position, this->targetOffset);
    this->camera.target = this->target->position;
    UpdateCamera(&this->camera, CAMERA_THIRD_PERSON);
}