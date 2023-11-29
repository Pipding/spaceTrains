#include "raylib.h"
#include "src/globals/AssetManager.h"

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
        InitAudioDevice();
        static AudioManager instance;
        return instance;
    }

    AudioManager(AudioManager const&) = delete;
    void operator=(AudioManager const&)  = delete;

    /**
     * Plays a sound
     * @param assetName The name of the sound to play. Note: if assetName isn't the name of a sound
     *  in the AssetManager, you'll probably crash
    */
    void play(const char* assetName);

    /**
     * Plays a sound
     * @param sound A Raylib Sound to play
    */
    void play(Sound* sound);
};