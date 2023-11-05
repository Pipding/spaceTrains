#pragma once
#include "raylib.h"
#include "src/interfaces/IKeyboardListener.h"

/*
* A singleton class for storing debug flags so they're accessible anywhere in the application
* 
* This class comes from Martin York on StackOverflow: https://stackoverflow.com/a/1008289
* and is designed to be an implementation of the Singleton design pattern that is lazy-evaluated,
* correctly-destroyed, and thread-safe
*/
class SpaceTrainDebug: public IKeyboardListener {
public:

    /**
     * Returns the SpaceTrainDebug singleton
    */
    static SpaceTrainDebug& getInstance()
    {
        static SpaceTrainDebug instance;
        return instance;
    }

private:
    // Stubbed constructor - not to be used outside this class
    SpaceTrainDebug() {}

    // Flag indicating whether bounding boxes should be visible
    bool drawBoundingBoxes = false;
        
    public:
        
        /**
         * Constructor
        */
        SpaceTrainDebug(SpaceTrainDebug const&) = delete;

        /**
         * Override the equals operator
        */
        void operator=(SpaceTrainDebug const&)  = delete;

        /**
         * Toggles whether bounding boxes should be drawn
        */
        bool toggleDrawBoundingBoxes();

        /**
         * Returns true if bounding boxes should be drawn
        */
        bool getDrawBoundingBoxes();

        /**
         * Sets the flag which determines whether or not to draw bounding boxes
         * @param value   bool indicating whether or not bounding boxes should be drawn
        */
        void setDrawBoundingBoxes(bool value);

        /**
         * Allows this object to receive key pressed events
         * @param key   The RayLib keycode of the key which was pressed
        */
        void onKeyPressed(int key);

        /**
         * Allows this object to receive key released events
         * @param key   The RayLib keycode of the key which was released
        */
        void onKeyReleased(int key);
};