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

void AssetManager::loadAssets() {
    this->loadModel("assets/models/bullet.obj", "bullet");
    this->loadModel("assets/models/ducky.obj", "duck");
    this->loadModel("assets/models/health_powerup.obj", "health_powerup");
    this->loadModel("assets/models/missile.obj", "missile");
    this->loadModel("assets/models/plane.obj", "plane");
    this->loadModel("assets/models/speed_powerup.obj", "speed_powerup");
    this->loadModel("assets/models/train_engine.obj", "train_engine");
    this->loadModel("assets/models/ufo.obj", "ufo");
    this->loadModel("assets/models/wagon.obj", "train_wagon");

    this->loadTexture("assets/textures/bullet_albedo.png", "bullet");
    this->loadTexture("assets/textures/ducky_albedo.png", "duck");
    this->loadTexture("assets/textures/health_powerup_albedo.png", "health_powerup");
    this->loadTexture("assets/textures/missile_albedo.png", "missile");
    this->loadTexture("assets/textures/speed_powerup_albedo.png", "speed_powerup");
    this->loadTexture("assets/textures/train_engine_specular.png", "train_engine");
    this->loadTexture("assets/textures/ufo_albedo.png", "ufo");
    this->loadTexture("assets/textures/wagon_albedo.png", "wagon");

    this->loadTexture("assets/textures/galaxy_background_transparent.png", "galaxy");

    this->loadTexture("assets/ui/no_target_ui.png", "no_target_ui");
    this->loadTexture("assets/ui/target_available_ui.png", "target_available_ui");
    this->loadTexture("assets/ui/target_locked_ui.png", "target_locked_ui");

    this->loadSound("assets/sounds/8_bit_explosion.wav", "explosion");
    this->loadSound("assets/sounds/bullet_fire.wav", "bullet_fire");
    this->loadSound("assets/sounds/engine.wav", "engine");
    this->loadSound("assets/sounds/laser_hit.wav", "laser_hit");
    this->loadSound("assets/sounds/missile_fire.wav", "missile_fire");

    this->loadFont("assets/fonts/ds-digi.ttf", "ds_digi");
    this->loadFont("assets/fonts/space-wham.ttf", "space_wham");
}

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