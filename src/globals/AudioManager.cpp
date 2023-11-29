#include "src/globals/AudioManager.h"

static AssetManager& _assets = AssetManager::getInstance();

void AudioManager::play(const char* assetName) {
    PlaySound(_assets.getSound(assetName));
}

void AudioManager::play(Sound* sound) {
    PlaySound(*sound);
}