#include "src/globals/AssetManager.h"

// ==================================================
// Getters
// ==================================================

// TODO: This could error out if a bad assetName is given
Font AssetManager::getFont(const char* assetName) {
    return this->fonts[assetName];
}

// TODO: This could error out if a bad assetName is given
Model AssetManager::getModel(const char* assetName) {
    return this->models[assetName];
}

// TODO: This could error out if a bad assetName is given
Texture2D AssetManager::getTexture(const char* assetName) {
    return this->textures[assetName];
}

// TODO: This could error out if a bad assetName is given
Texture2D* AssetManager::getTextureRef(const char* assetName) {
    return &this->textures[assetName];
}

// TODO: This could error out if a bad assetName is given
Sound AssetManager::getSound(const char* assetName) {
    return this->sounds[assetName];
}


// ==================================================
// Loading & unloading
// ==================================================

void AssetManager::loadFont(const char* filename, const char* assetName){
    this->fonts[assetName] = LoadFont(filename);
}

void AssetManager::unloadFont(const char* assetName) {
    // Elegant way to check if a key exists, found here: https://stackoverflow.com/a/11765524
    if (this->fonts.count(assetName)) {
        UnloadFont(fonts[assetName]);
    }
}

void AssetManager::loadModel(const char* filename, const char* assetName){
    this->models[assetName] = LoadModel(filename);
}

void AssetManager::unloadModel(const char* assetName) {
    // Elegant way to check if a key exists, found here: https://stackoverflow.com/a/11765524
    if (this->models.count(assetName)) {
        UnloadModel(models[assetName]);
    }
}

void AssetManager::loadTexture(const char* filename, const char* assetName){
    this->textures[assetName] = LoadTexture(filename);
}

void AssetManager::unloadTexture(const char* assetName) {
    // Elegant way to check if a key exists, found here: https://stackoverflow.com/a/11765524
    if (this->textures.count(assetName)) {
        UnloadTexture(textures[assetName]);
    }
}

void AssetManager::loadSound(const char* filename, const char* assetName){
    this->sounds[assetName] = LoadSound(filename);
}

void AssetManager::unloadSound(const char* assetName) {
    // Elegant way to check if a key exists, found here: https://stackoverflow.com/a/11765524
    if (this->sounds.count(assetName)) {
        UnloadSound(sounds[assetName]);
    }
}

void AssetManager::unloadAll() {
    // Found this loop structure here: https://favtutor.com/blogs/iterate-through-map-cpp
    for (auto keyValuePair : this->models) {
        UnloadModel(keyValuePair.second);
    }

    for (auto keyValuePair : this->textures) {
        UnloadTexture(keyValuePair.second);
    }

    for (auto keyValuePair : this->sounds) {
        UnloadSound(keyValuePair.second);
    }
}