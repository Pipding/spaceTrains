#include "raylib.h"
#include "src/globals/GameStateManager.h"
#include "src/interfaces/IUpdatable.h"

/**
 * A singleton class for playing audio
 * 
 * The implementation of singletone I'm using comes from Martin York on StackOverflow: https://stackoverflow.com/a/1008289
 * and is designed to be an implementation of the Singleton design pattern that is lazy-evaluated,
 * correctly-destroyed, and thread-safe
 * 
*/
class AudioManager {
private:

    AudioManager() {}

public:

    // ==================================================
    // Singleton stuff
    // ==================================================
    static AudioManager& getInstance()
    {
        static AudioManager instance;
        return instance;
    }

    AudioManager(AudioManager const&) = delete;
    void operator=(AudioManager const&)  = delete;
};