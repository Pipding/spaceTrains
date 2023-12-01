#pragma once
#include "raylib.h"
#include "raymath.h"
#include "src/globals/SpaceTrainDebug.h"

/**
* Similar to the Actor class in Unreal Engine. Something which can be placed or spawned in the world
*/
class Actor {
private:
    Texture2D texture;
    BoundingBox boundsOrigin;
    BoundingBox bounds;
    Matrix transform = MatrixIdentity();
    
public:
    Vector3 position = {0.f, 0.f, 0.f};
    Vector3 rotation = {0.f, 0.f, 0.f};
    Model model;
    float scale = 0.1f;
    Color color = WHITE;

    /**
     * Default Actor constructor
    */
    Actor();

    /**
     * An object which can be spawned or placed in the world, similar to the class
     * of the same name in Unreal Engine
     * @param model     The 3D model representing the Actor in the world
     * @param texture   The texture to be applied to model
    */
    Actor(Model model, Texture2D texture);

    /**
     * An object which can be spawned or placed in the world, similar to the class
     * of the same name in Unreal Engine
     * @param position  Initial location of the Actor in-world
     * @param model     The 3D model representing the Actor in the world
     * @param texture   The texture to be applied to model
    */
    Actor(Vector3 position, Model model, Texture2D texture);


    /**
     * Update the texture applied to the model of the Actor
     * @param texture   The texture to be applied to the model
     */
    void setTexture(Texture2D texture);

    // TODO: Comments
    Texture2D* getTexture();

    // TODO: Comments
    Model* getModel();

    /**
    * Set the rotation of the Actor
    * @param rotation   Vector3 of angles in radians in x, y and z
    */
    void setRotation(Vector3 rotation);

    /**
    * Rotate the Actor by the given rotation vector
    * @param rotation   Vector3 of angles in radians in x, y and z
    */
    void rotateBy(Vector3 rotation);

    /**
     * Get the collision bounds of the Actor
    */
    BoundingBox getBounds();

    /**
     * Get the forward vector of this Actor
    */
    Vector3 getForwardVector();

    /**
     * Get the right vector of this Actor
    */
    Vector3 getRightVector();

    /**
     * Calculates a Vector3 which points from this Actor to the target Vector3. Normalized by default but normalization can be skipped by using the normalized parameter
     * @param target        Vector3 which the generated Vector3 should point toward
     * @param normalized    Optional, defaults to true. If true the returned vector will be normalized
    */
    Vector3 getVectorTowardTarget(Vector3 target, bool normalized = true);

    /**
     * Calculates the angle of rotation between this Actor's position and the given target vector
     * @param target    The target vector which will be used to calculate rotation
    */
    float angleToVector(Vector3 target);

    /**
     * Update the Actor based on current state
    */
    void update();

    /**
     * Draw the Actor by calling the raylib DrawModel function using the member variables of this Actor
     */
    void draw();

    /**
     * Unloads the texture and model used by this Actor from memory
    */
    void unload();
};
