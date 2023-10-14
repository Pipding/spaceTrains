#include "Actor.h"

Actor::Actor(Model model, Texture2D texture) : Actor({0, 0, 0}, model, texture) { } // This is a delegated constructor https://en.wikipedia.org/wiki/C++11#Object_construction_improvement

Actor::Actor(Vector3 position, Model model, Texture2D texture) {
    this->position = position;
    this->model = model;
    this->setTexture(texture);
}

// TODO: Actors should manage their own bounds

/**
 * Update the texture applied to the model of the Actor
 * @param texture   The texture to be applied to the model
 */
void Actor::setTexture(Texture2D texture) {
    this->texture = texture;
    this->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = this->texture;
}

/**
 * Calls the raylib DrawModel function using the member variables of this Actor
 */
void Actor::draw() {
DrawModel(this->model, this->position, this->scale, this->color);
// TODO: When Actors manage their own bounds, this function should optionally draw bounding boxes
}

/**
 * Unloads the texture and model used by this Actor from memory
*/
void Actor::unload() {
    UnloadTexture(this->texture);
    UnloadModel(this->model);
}