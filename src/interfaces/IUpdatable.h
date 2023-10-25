#pragma once

/**
 * Abstract class to provide an interface for anything with an update method
*/
class IUpdatable {
public:
    virtual void update() = 0;
};