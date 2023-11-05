#include "SpaceTrainDebug.h"

bool SpaceTrainDebug::toggleDrawBoundingBoxes() {
    this->setDrawBoundingBoxes(!this->drawBoundingBoxes);
    return this->getDrawBoundingBoxes();
}

bool SpaceTrainDebug::getDrawBoundingBoxes() {
    return this->drawBoundingBoxes;
}

void SpaceTrainDebug::setDrawBoundingBoxes(bool value) {
    this->drawBoundingBoxes = value;
}

void SpaceTrainDebug::onKeyPressed(int key) {
    if (key == KEY_M) {
        this->toggleDrawBoundingBoxes();
    }
}

void SpaceTrainDebug::onKeyReleased(int key) { }