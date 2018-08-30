#include <chrono>
#include "../header/Engine.h"

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
    double deltaTime;

    loopStartTime = std::chrono::high_resolution_clock::now();
    while (go) {
        // Calculate the time elapsed between the beginning of the last loop and now
        loopElapsedTime = std::chrono::high_resolution_clock::now() - loopStartTime;
        // Reset the timer
        loopStartTime = std::chrono::high_resolution_clock::now();
        // Cast elapsed time to seconds
        // This variable should be passed in to every calculation of game physics and animation
        deltaTime = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(loopElapsedTime).count();

        // DETECT INPUT

        // GAME LOGIC

        // DRAW WORLD

        // DRAW GUI
    }
}
