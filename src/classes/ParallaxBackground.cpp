#include "ParallaxBackground.h"

static AssetManager& _assets = AssetManager::getInstance();

ParallaxBackground::ParallaxBackground(Vector3* parent, int layers, Texture2D* texture)
    : layerCount(layers), parent(parent) {

        for (int i = 0; i < layers; i++) {
            this->layerModels.push_back(_assets.getModel("plane"));
            this->layerTextures.push_back(texture);
            this->layerOffsets.push_back((Vector2){i * 25, i * 25});
        }
}

void ParallaxBackground::setLayer(int layer, Texture2D* texture) {
    this->layerTextures[layer] = texture;
}

void ParallaxBackground::setLayerOffset(int layer, Vector2 offset) {
    this->layerOffsets[layer] = offset;
}

void ParallaxBackground::update(float deltaTime) { }

void ParallaxBackground::draw() {
    
}