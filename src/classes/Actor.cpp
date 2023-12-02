#include "Actor.h"

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

// ==================================================
// Constructors
// ==================================================
Actor::Actor() { }

Actor::Actor(Model model, Texture2D texture) : Actor({0, 0, 0}, model, texture) { } // This is a delegated constructor https://en.wikipedia.org/wiki/C++11#Object_construction_improvement

Actor::Actor(Vector3 position, Model model, Texture2D texture)
    : position(position), model(model) {

    this->setTexture(texture);
    this->setScale(scale);
}


// ==================================================
// Setters & getters
// ==================================================
Texture2D* Actor::getTexture() {
    return &this->texture;
}

Model* Actor::getModel() {
    return &this->model;
}

BoundingBox Actor::getBounds() {
    return this->bounds;
}

void Actor::setTexture(Texture2D texture) {
    this->texture = texture;
    this->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = this->texture;
}

void Actor::setScale(float scale) {
    this->scale = scale;

    this->bounds.min = Vector3Scale(this->bounds.min, this->scale);
    this->bounds.max = Vector3Scale(this->bounds.max, this->scale);

    this->boundsOrigin = GetMeshBoundingBox(this->model.meshes[0]);
    this->boundsOrigin.min = Vector3Scale(this->boundsOrigin.min, this->scale);
    this->boundsOrigin.max = Vector3Scale(this->boundsOrigin.max, this->scale);
}

void Actor::setRotation(Vector3 rotation) {
    this->rotation = rotation;
    this->transform = MatrixRotateXYZ(this->rotation);
    this->model.transform = this->transform;
}

// ==================================================
// Methods for calculating & updating things
// ==================================================

void Actor::rotateBy(Vector3 rotation) {
    this->setRotation(Vector3Add(this->rotation, rotation));
}

Vector3 Actor::getForwardVector() {
    return Vector3Transform({1.f, 0.f, 0.f}, this->transform);
}

Vector3 Actor::getRightVector() {
    return Vector3CrossProduct(this->getForwardVector(), {0, 1, 0});
}

Vector3 Actor::getVectorTowardTarget(Vector3 target, bool normalized) {
    Vector3 vectorToTarget = Vector3Subtract(target, this->position);

    if (normalized) {
        return Vector3Normalize(vectorToTarget);
    }

    return vectorToTarget;
}

// TODO: This may or may not rely on the two vectors being coplanar. Honestly not sure
float Actor::angleToVector(Vector3 target) {
    Vector3 vectorToTarget = this->getVectorTowardTarget(target);

    // Angle (in rads) between 2 vectors is given by atan2
    // but the angle is offset by 90 degrees so we need to subtract that. 1.5708rad = 90deg
    // TODO: Figure out why the angle is offset from what you expected
    return atan2(vectorToTarget.x, vectorToTarget.z) - 1.5708f;
}

// ==================================================
// Game engine functions
// ==================================================
void Actor::update() {
    this->bounds.min = Vector3Add(this->position, this->boundsOrigin.min);
    this->bounds.max = Vector3Add(this->position, this->boundsOrigin.max);
}

void Actor::draw() {
    DrawModel(this->model, this->position, this->scale, this->color);

    if (_debug.getDrawBoundingBoxes()) {
        DrawBoundingBox(this->getBounds(), GREEN);
    }
}
