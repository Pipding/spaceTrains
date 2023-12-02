#pragma once

#include "src/interfaces/IUpdatable.h"
#include "src/globals/AssetManager.h"
#include <vector>

// Used to render two or more layers of background and move them relative to 
// some in-game object (usually a camera or character) to give the illusion of depth
class ParallaxBackground: public IUpdatable {
private:
    int layerCount;
    std::vector<Model> layerModels;
    std::vector<Texture2D*> layerTextures;
    std::vector<Vector2> layerOffsets;

    // The position on which the parallax effect will be based
    Vector3* parent;

public:
    /**
     * Constructs a ParallaxBackround with a number of layers, each one using the same
     * Texture2D and with a  default offset. To change the texture for individual layers
     * use ParallaxBackground::setLayer(). To change offsets, use ParallaxBackground::setLayerOffset()
    */
    ParallaxBackground(Vector3* parent, int layers, Texture2D* texture);

    /**
     * Change the Texture2D used for a given layer
    */
    void setLayer(int layer, Texture2D* texture);

    /**
     * Change the offset of the given layer
    */
    void setLayerOffset(int layer, Vector2 offset);

    // Update the ParallaxBackground
    void update(float deltaTime);
    
    // Draw the ParallaxBackground
    void draw();
};