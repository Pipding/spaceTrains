#include "SpaceTrainDebug.h"

bool SpaceTrainDebug::toggleDrawBoundingBoxes() {
    this->drawBoundingBoxes = !this->drawBoundingBoxes;
    return this->getDrawBoundingBoxes();
}

/*
*
*/
bool SpaceTrainDebug::getDrawBoundingBoxes() {
    return this->drawBoundingBoxes;
}