#include "Actor.h"

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

/**
 * Default Actor constructor
*/
Actor::Actor() { }

/**
 * An object which can be spawned or placed in the world, similar to the class
 * of the same name in Unreal Engine
 * @param model     The 3D model representing the Actor in the world
 * @param texture   The texture to be applied to model
*/
Actor::Actor(Model model, Texture2D texture) : Actor({0, 0, 0}, model, texture) { } // This is a delegated constructor https://en.wikipedia.org/wiki/C++11#Object_construction_improvement

/**
 * An object which can be spawned or placed in the world, similar to the class
 * of the same name in Unreal Engine
 * @param position  Initial location of the Actor in-world
 * @param model     The 3D model representing the Actor in the world
 * @param texture   The texture to be applied to model
*/
Actor::Actor(Vector3 position, Model model, Texture2D texture) {
    this->position = position;
    this->model = model;
    this->setTexture(texture);
    this->boundsOrigin = GetMeshBoundingBox(this->model.meshes[0]);
}

/**
 * Update the texture applied to the model of the Actor
 * @param texture   The texture to be applied to the model
 */
void Actor::setTexture(Texture2D texture) {
    this->texture = texture;
    this->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = this->texture;
}

/**
* Set the rotation of the Actor
* @param rotation   Vector3 of angles in radians in x, y and z
*/
void Actor::setRotation(Vector3 rotation) {
    this->rotation = rotation;
    this->transform = MatrixRotateXYZ(this->rotation);
    this->model.transform = this->transform;
}

/**
* Rotate the Actor by the given rotation vector
* @param rotation   Vector3 of angles in radians in x, y and z
*/
void Actor::rotateBy(Vector3 rotation) {
    this->setRotation(Vector3Add(this->rotation, rotation));
}

/**
 * Get the collision bounds of the Actor
*/
BoundingBox Actor::getBounds() {
    return this->bounds;
}

/**
 * Get the forward vector of this Actor
*/
Vector3 Actor::getForwardVector() {
    return Vector3Transform({1.f, 0.f, 0.f}, this->transform);
}

/**
 * Calculates a Vector3 which points from this Actor to the target Vector3. Normalized by default but normalization can be skipped by using the normalized parameter
 * @param target        Vector3 which the generated Vector3 should point toward
 * @param normalized    Optional, defaults to true. If true the returned vector will be normalized
*/
Vector3 Actor::getVectorTowardTarget(Vector3 target, bool normalized) {
    Vector3 vectorToTarget = Vector3Subtract(target, this->position);

    if (normalized) {
        return Vector3Normalize(vectorToTarget);
    }

    return vectorToTarget;
}

/**
 * Calculates the angle of rotation between this Actor's position and the given target vector
 * TODO: This may or may not rely on the two vectors being coplanar. Honestly not sure
 * @param target    The target vector which will be used to calculate rotation
*/
float Actor::angleToVector(Vector3 target) {
    Vector3 vectorToTarget = this->getVectorTowardTarget(target);

    // Angle (in rads) between 2 vectors is given by atan2
    // but the angle is offset by 90 degrees so we need to subtract that. 1.5708rad = 90deg
    // TODO: Figure out why the angle is offset from what you expected
    return atan2(vectorToTarget.x, vectorToTarget.z) - 1.5708f;
}


/**
 * Update the Actor based on current state
*/
void Actor::update() {
    this->bounds.min = Vector3Add(this->position, this->boundsOrigin.min);
    this->bounds.max = Vector3Add(this->position, this->boundsOrigin.max);
}

/**
 * Draw the actor by calling the raylib DrawModel function using the member variables of this Actor
 */
void Actor::draw() {
    DrawModel(this->model, this->position, this->scale, this->color);

    if (_debug.getDrawBoundingBoxes()) {
        DrawBoundingBox(this->getBounds(), GREEN);
    }
}

/**
 * Unloads the texture and model used by this Actor from memory
*/
void Actor::unload() {
    UnloadTexture(this->texture);
    UnloadModel(this->model);
}