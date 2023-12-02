#include "ParallaxBackground.h"

static AssetManager& _assets = AssetManager::getInstance();

ParallaxBackground::ParallaxBackground(Vector3* parent, int layers, Model* model, Texture2D* texture)
    : layerCount(layers), parent(parent) {

        for (int i = 0; i < layers; i++) {
            this->layerModels.push_back(model);
            this->layerTextures.push_back(texture);
        }
}