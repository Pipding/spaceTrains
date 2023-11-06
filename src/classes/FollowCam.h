#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Actor.h"
#include "src/globals/SpaceTrainDebug.h"
#include "src/interfaces/IKeyboardListener.h"

/**
* A camera which follows a given Actor with a given offset
*
* This camera is somewhat user adjustable. It can orbit its parent from
* side-to-side at an angle determined by the player. When an adjustment of
* this kind has been made, the camera becomes unlocked from the rotation
* of its parent, allowing the parent to rotate without the camera following 
* its orientation. Resetting the user adjustment re-locks the camera's rotation 
* to its parent
*/
class FollowCam: public IKeyboardListener {
public:
    Camera camera = {0};

    // The Actor which this camera will follow
    Actor* parent;

    // The degree by which user adjustment rotates the camera on each update cycle
    float mouseAdjustmentFactor = 0.05f;

    // The offset from the position of the parent to which the camera should default
    Vector3 parentOffset;

    /**
     * A camera which follows an Actor at a distance
    * @param parent    Pointer to the Actor to follow
    * @param offset    Offset from the Actor's location where the camera should be by default
    */
    FollowCam(Actor* parent, Vector3 offset);

    /**
     * Update camera position & parent vectors
    */
    void update();

    /**
     * Draws certain camera data. Useful for debugging
    */
    void draw();

    /**
     * Resets userCameraRotationAdjustment to 0
    */
    void resetmouseRotationAdjustment();

    /**
     * Calculates the camera's offset from the target's position, accounting for user adjustment
     * Note: This function will recalculate and update appliedRotation
    */
    Vector3 calculateAppliedOffset();

    /**
     * Sets the target of the FollowCam
    */
    void setTarget(Actor* target);

    /**
     * Sets the target of the FollowCam
    */
    void unsetTarget();

    /**
     * Returns true if the FollowCam has a target
    */
    bool getHasTarget();

private:
    // Side-to-side orbital movement applied by the user
    float userCameraRotationAdjustment = 0.f;

    // The offset from the parent's position where the camera actually ends up after all transformations are applied
    Vector3 appliedOffset; // appliedOffset is basically: (parent.position + parentOffset) * (defaultCameraRotation + userCameraRotationAdjustment)

    // Used to calculate appliedOffset
    Matrix appliedRotation;

    // The rotation the camera would have without user adjustments
    Vector3 defaultCameraRotation;

    // A secondary Actor which the camera will try to point toward
    Actor* target = NULL;

    // Indicates whether the FollowCam has a target
    bool hasTarget = false; //TODO: Can this be replaced with a function which checks whether target == nullptr?

    /**
     * Callback which will be invoked whenever a key pressed event this class listens to is triggered
     * @param key   The key which was pressed
    */
    void onKeyPressed(int key);

    /**
     * Callback which will be invoked whenever a key released event this class listens to is triggered
     * @param key   The key which was released
    */
    void onKeyReleased(int key);
};
