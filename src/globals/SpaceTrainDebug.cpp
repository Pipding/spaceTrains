#include "SpaceTrainDebug.h"

/*
* Toggles whether bounding boxes should be drawn
*/
bool SpaceTrainDebug::toggleDrawBoundingBoxes() {
    this->setDrawBoundingBoxes(!this->drawBoundingBoxes);
    return this->getDrawBoundingBoxes();
}

/*
* Returns true if bounding boxes should be drawn
*/
bool SpaceTrainDebug::getDrawBoundingBoxes() {
    return this->drawBoundingBoxes;
}

/*
* Sets the value for whether bounding boxes should be drawn
*/
void SpaceTrainDebug::setDrawBoundingBoxes(bool value) {
    this->drawBoundingBoxes = value;
}

/*
* Handles any input this object needs to look out for
*/
void SpaceTrainDebug::onKeyPressed(int key) {
    if (key == KEY_M) {
        this->toggleDrawBoundingBoxes();
    }
}

// Stub, unused
void SpaceTrainDebug::onKeyReleased(int key) { }