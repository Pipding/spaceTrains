#include "TrainComponent.h"

TrainComponent::TrainComponent() : Actor() {}

TrainComponent::TrainComponent(Vector3 position, Model model, Texture2D texture, Model projectileModel, Texture2D projectileTexture)
    : Actor(position, model, texture), projectileModel(projectileModel), projectileTexture(projectileTexture) {}