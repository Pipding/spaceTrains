#pragma once
#include "Actor.h"
#include "Projectile.h"
#include "src/interfaces/IUpdatable.h"

class TrainComponent : public Actor, public IUpdatable {
public:
    /**
     * Default constructor
    */
    TrainComponent();

    // TODO: Make these private
    Model projectileModel;
    Texture2D projectileTexture;
    Sound projectileSFX;

    /**
     * A component of a Train
     * @param position              The starting position of the TrainComponent
     * @param model                 3D model representing the TrainComponent
     * @param texture               Texture to apply to the model
     * @param projectileModel       3D model representing the projectiles fired by this TrainComponent
     * @param projectileTexture     Texture to apply to the projectiles
    */
    TrainComponent(Vector3 position, Model model, Texture2D texture, Model projectileModel, Texture2D projectiletexture); // TODO: This should accept pointers to assets
 };