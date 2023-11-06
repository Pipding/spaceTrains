#pragma once
#include "raylib.h"
#include "src/interfaces/IKeyboardListener.h"
#include "src/enums/GameState.h"

/**
 * A singleton class for managing game state (paused, playing, etc.)
 * 
 * The implementation of singletone I'm using comes from Martin York on StackOverflow: https://stackoverflow.com/a/1008289
 * and is designed to be an implementation of the Singleton design pattern that is lazy-evaluated,
 * correctly-destroyed, and thread-safe
*/
class GameStateManager: public IKeyboardListener {
private:
    GameStateManager() {}
    GameState gameState = GameState::Invalid;

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

private:

    /**
     * Callback which will be invoked whenever a key pressed event this class listens to is triggered
     * @param key   The key which was pressed
    */
    void onKeyPressed(int key);

    /**
     * Callback which will be invoked whenever a key released event this class listens to is triggered
     * @param key   The key which was released
    */
    void onKeyReleased(int key) { };

public:

    /**
     * Attempts to pause or unpause the game
     * Note: The game can only be paused from the gameplay state. Likewise, unpausing will set the state to Gameplay
     * @return  Returns the new GameState after toggling pause
    */
    GameState togglePaused();

    /**
     * Returns the current GameState
    */
    GameState getState();

    /**
     * Sets the GameState
     * @param newState  The GameState which the game should be in
     * @return  Returns the GameState after attempting to apply the new value
    */
    GameState setState(GameState newState);
};