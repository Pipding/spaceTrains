#pragma once

/**
 * Abstract class to provide an interface for anything with an update method
*/
class IUpdatable {
public:
    virtual void update(float deltaTime) = 0;

    /**
     * If this virtual destructor isn't declared, you'll get a warning whenever
     * you try to delete an instance of a class which inherits from IUpdatable.
     * 
     * Here's the explanation copy/pasted from Reddit:
     *      If you create a class with virtual methods, you almost always want to also 
     *      have a virtual destructor. The reason is you're likely going to use this class 
     *      polymorphically (via pointer or reference to the base class) and you want to make 
     *      sure the destructor of the derived class is called.
     * 
     * Link: https://www.reddit.com/r/cpp_questions/comments/b3hbcg/comment/eizkvl4/
    */
    virtual ~IUpdatable() = default;
};