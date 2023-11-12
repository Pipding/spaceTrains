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

    std::map<std::string, Model> models;
    std::map<std::string, Texture2D> textures;

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
    // Methods
    // ==================================================

    /**
     * Gets a Model from the models map
    */
    Model getModel(const char* assetName);

    /**
     * Gets a Texture2D from the textures map
    */
    Texture2D getTexture(const char* assetName);

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
     * Unloads all assets from all maps
    */
    void unloadAll();
};