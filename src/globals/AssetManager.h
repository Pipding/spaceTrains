#pragma once
#include "raylib.h"
#include <map>
#include <string>


/**
 * 
*/
class AssetManager {
private:
    AssetManager() {}

    std::map<std::string, Font> fonts;
    std::map<std::string, Model> models;
    std::map<std::string, Texture2D> textures;
    std::map<std::string, Sound> sounds;

public:

    // ==================================================
    // Singleton stuff
    // ==================================================
    static AssetManager& getInstance()
    {
        static AssetManager instance;
        return instance;
    }

    AssetManager(AssetManager const&) = delete;
    void operator=(AssetManager const&)  = delete;

    // ==================================================
    // Getters
    // ==================================================

    /**
     * Gets a Font from the fonts map
    */
    Font getFont(const char* assetName);

    /**
     * Gets a Model from the models map
    */
    Model getModel(const char* assetName);

    /**
     * Gets a Texture2D from the textures map
    */
    Texture2D getTexture(const char* assetName);

    /**
     * Gets a pointer to a Texture2D from the textures map
    */
    Texture2D* getTextureRef(const char* assetName);

    /**
     * Gets a Sound from the sounds map
    */
    Sound getSound(const char* assetName);

    // ==================================================
    // Loading & unloading
    // ==================================================

    /**
     * Loads a Font and stores it in the fonts map
     * @param filename      Filepath for the asset to load
     * @param assetName     A name for the asset, used as a key in the models map
    */
    void loadFont(const char* filename, const char* assetName);

    /**
     * Unloads the Font for the given key if it exists in the fonts map
     * @param assetName     The name of the asset to be unloaded
    */
    void unloadFont(const char* assetName);

    /**
     * Loads a Model and stores it in the models map
     * @param filename      Filepath for the asset to load
     * @param assetName     A name for the asset, used as a key in the models map
    */
    void loadModel(const char* filename, const char* assetName);

    /**
     * Unloads the Model for the given key if it exists in the models map
     * @param assetName     The name of the asset to be unloaded
    */
    void unloadModel(const char* assetName);

    /**
     * Loads a Texture2D and stores it in the textures map
     * @param filename      Filepath for the asset to load
     * @param assetName     A name for the asset, used as a key in the textures map
    */
    void loadTexture(const char* filename, const char* assetName);

    /**
     * Unloads the Texture2D for the given key if it exists in the textures map
     * @param assetName     The name of the asset to be unloaded
    */
    void unloadTexture(const char* assetName);

    /**
     * Loads a Sound and stores it in the sounds map
     * @param filename      Filepath for the asset to load
     * @param assetName     A name for the asset, used as a key in the sounds map
    */
    void loadSound(const char* filename, const char* assetName);

    /**
     * Unloads the Sound for the given key if it exists in the sounds map
     * @param assetName     The name of the asset to be unloaded
    */
    void unloadSound(const char* assetName);

    /**
     * Unloads all assets from all maps
    */
    void unloadAll();
};