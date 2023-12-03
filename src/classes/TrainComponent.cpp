#include "TrainComponent.h"

TrainComponent::TrainComponent() : Actor() {}

TrainComponent::TrainComponent(Vector3 position, Model model, Texture2D texture, Model projectileModel, Texture2D projectileTexture)
    : Actor(position, model, texture), projectileModel(projectileModel), projectileTexture(projectileTexture) {
        // TODO: This is WIP
        this->rearAttachmentPoint = Vector3Subtract(this->position, Vector3Scale(this->getForwardVector(), 2.f));
    }

Vector3* TrainComponent::getRearAttachmentPoint() {
    return &this->rearAttachmentPoint;
}