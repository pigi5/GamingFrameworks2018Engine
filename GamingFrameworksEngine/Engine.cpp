#include <chrono>
#include "Engine.h"

Engine::Engine()
{
}


Engine::~Engine()
{
}

// Runs the game loop until complete.
void Engine::run()
{
    bool go = true;
    std::chrono::steady_clock::time_point loopStartTime;
    std::chrono::steady_clock::duration loopElapsedTime;
    // Microsecond duration between runs of the game loop
    long long deltaTime;

    loopStartTime = std::chrono::high_resolution_clock::now();
    while (go) {
        // Calculate the time elapsed between the beginning of the last loop and now
        loopElapsedTime = std::chrono::high_resolution_clock::now() - loopStartTime;
        // Reset the timer
        loopStartTime = std::chrono::high_resolution_clock::now();
        // Cast elapsed time to microseconds
        // This variable should be passed in to every calculation of game physics and animation
        deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(loopElapsedTime).count();

        // DETECT INPUT

        // GAME LOGIC

        // DRAW WORLD

        // DRAW GUI
    }
}
