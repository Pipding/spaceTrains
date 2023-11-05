#include "raylib.h"
#include "src/interfaces/IKeyboardListener.h"
#include <map>
#include <vector>

// The pattern I'm using for handling input is drawn from this StackOverflow answer
// https://gamedev.stackexchange.com/questions/98840/handling-input-with-callback-functions
class InputManager {
private:

    InputManager() {}

    // Maps a given key to all listeners who are interested in its state
    std::map<int, IKeyboardListener*> listeners; // TODO: This limits you to one listener per key. Might be ok but bear in mind for future

    // All keys registered with the InputManager
    std::vector<int> listenedKeys;

public:

    // ==================================================
    // Singleton stuff
    // ==================================================
    static InputManager& getInstance()
    {
        static InputManager instance;
        return instance;
    }

    InputManager(InputManager const&) = delete;
    void operator=(InputManager const&)  = delete;


    /**
     * Add a listener for a keyboard event
     * @param listener  The thing doing the listening
     * @param key       The RayLib key code for the key to listen for
    */
    void addListener(IKeyboardListener* listener, int key);

    /**
     * Add a listener for a keyboard event
     * @param listener  The thing doing the listening
     * @param keys      Vector of RayLib key code for the keys to listen for
    */
    void addListeners(IKeyboardListener* listener, std::vector<int> keys);

    /**
     * Update method
    */
    void update();
};