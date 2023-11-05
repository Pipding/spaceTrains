#include "raylib.h"
#include "src/interfaces/IKeyboardListener.h"

/**
 * A singleton class for managing game state (paused, playing, etc.)
 * 
 * The implementation of singletone I'm using comes from Martin York on StackOverflow: https://stackoverflow.com/a/1008289
 * and is designed to be an implementation of the Singleton design pattern that is lazy-evaluated,
 * correctly-destroyed, and thread-safe
*/
class GameStateManager {
private:
    GameStateManager() {}

public:

    // ==================================================
    // Singleton stuff
    // ==================================================
    static GameStateManager& getInstance()
    {
        static GameStateManager instance;
        return instance;
    }

    GameStateManager(GameStateManager const&) = delete;
    void operator=(GameStateManager const&)  = delete;
};