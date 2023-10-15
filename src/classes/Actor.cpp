#include "Actor.h"
#include "src/globals/SpaceTrainDebug.h"

static SpaceTrainDebug& _debug = SpaceTrainDebug::getInstance();

Actor::Actor(Model model, Texture2D texture) : Actor({0, 0, 0}, model, texture) { } // This is a delegated constructor https://en.wikipedia.org/wiki/C++11#Object_construction_improvement

Actor::Actor(Vector3 position, Model model, Texture2D texture) {
    this->position = position;
    this->model = model;
    this->setTexture(texture);
    this->boundsOrigin = GetMeshBoundingBox(this->model.meshes[0]);
    this->update();
}

/**
 * Update the texture applied to the model of the Actor
 * @param texture   The texture to be applied to the model
 */
void Actor::setTexture(Texture2D texture) {
    this->texture = texture;
    this->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = this->texture;
}

BoundingBox Actor::getBounds() {
    return this->bounds;
}

void Actor::update() {
    this->bounds.min = Vector3Add(this->position, this->boundsOrigin.min);
    this->bounds.max = Vector3Add(this->position, this->boundsOrigin.max);
}

/**
 * Calls the raylib DrawModel function using the member variables of this Actor
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