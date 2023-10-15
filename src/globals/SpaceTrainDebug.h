#pragma once

/*
* A singleton class for storing debug flags so they're accessible anywhere in the application
* 
* This class comes from Martin York on StackOverflow: https://stackoverflow.com/a/1008289
* and is designed to be an implementation of the Singleton design pattern that is lazy-evaluated,
* correctly-destroyed, and thread-safe
*/
class SpaceTrainDebug
{
    public:
        static SpaceTrainDebug& getInstance()
        {
            static SpaceTrainDebug instance;
            return instance;
        }
    private:
        SpaceTrainDebug() {}
        bool drawBoundingBoxes = false;
        
    public:
        SpaceTrainDebug(SpaceTrainDebug const&) = delete;
        void operator=(SpaceTrainDebug const&)  = delete;
        bool toggleDrawBoundingBoxes();
        bool getDrawBoundingBoxes();
};