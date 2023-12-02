#pragma once

#include "src/interfaces/IUpdatable.h"
#include "src/globals/AssetManager.h"
#include <vector>

// Used to render two or more layers of background and move them relative to 
// some in-game object (usually a camera or character) to give the illusion of depth
class ParallaxBackground: public IUpdatable {
private:
    int layerCount;
    std::vector<Model*> layerModels;
    std::vector<Texture2D*> layerTextures;
    std::vector<Vector2> layerOffsets;

    // The position on which the parallax effect will be based
    Vector3* parent;

public:
    /**
     * Constructs a ParallaxBackround with a number of layers, each one using the same
     * Model and Texture2D and witha  default offset. To change the model and texture for individual layers
     * use ParallaxBackground::setLayer(). To change offsets, use ParallaxBackground::setLayerOffset()
    */
    ParallaxBackground(Vector3* parent, int layers, Model* model, Texture2D* texture);

    /**
     * Change the Model and/or Texture2D used for a given layer
    */
    void setLayer(int layer, Model* model, Texture2D* texture);

    /**
     * Change the offset of the given layer
    */
    void setLayerOffset(int layer, Model* model, Texture2D* texture);

    // Update the ParallaxBackground
    void update(float deltaTime);
    
    // Draw the ParallaxBackground
    void draw();
};