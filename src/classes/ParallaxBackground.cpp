#include "ParallaxBackground.h"

static AssetManager& _assets = AssetManager::getInstance();

ParallaxBackground::ParallaxBackground(Vector3* parent, int layers, Texture2D* texture)
    : layerCount(layers), parent(parent) {

    for (int i = 0; i < layers; i++) {
        this->layerModels.push_back(_assets.getModel("plane"));
        this->layerTextures.push_back(texture);
        this->layerOffsets.push_back((Vector2){i * 25.f, i * 25.f});

        this->layerModels[i].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *texture;
    }
}

void ParallaxBackground::setLayer(int layer, Texture2D* texture) {
    this->layerTextures[layer] = texture;
    this->layerModels[layer].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *texture;
}

void ParallaxBackground::setLayerOffset(int layer, Vector2 offset) {
    this->layerOffsets[layer] = offset;
}

void ParallaxBackground::update(float deltaTime) { }

void ParallaxBackground::draw() {

    for(int i = 0; i < this->layerCount; i++) {
        DrawModel(
            this->layerModels[i],
            { this->parent->x + (this->parent->x * -1 / this->layerOffsets[i].x) , -200.f, this->parent->z + this->parent->z * -1 / this->layerOffsets[i].y},
            1.f,
            WHITE
        );
    }
}