#include <chrono>
#include <iostream>
#include "../header/Engine.h"
#include "../header/Constants.h"

Engine::Engine()
{
	currentRoomIndex = 0;
}


Engine::~Engine()
{
}

// Runs the game loop until complete.
void Engine::run(sf::RenderWindow* window)
{
    bool go = true;
	std::chrono::steady_clock::time_point currentTime;
	std::chrono::steady_clock::time_point previousTime;
    double loopDeltaTime;
	double accumulator = 0.0;

	previousTime = std::chrono::high_resolution_clock::now();
    while (go) {
		// Reset the timer
		currentTime = std::chrono::high_resolution_clock::now();
        // Calculate the time elapsed between the beginning of the last loop and now
		loopDeltaTime = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(currentTime - previousTime).count();
		previousTime = currentTime;

		// Cap the maximum calculated time between frames
		if (loopDeltaTime < engine_constant::MAX_DELTA_TIME)
		{
			loopDeltaTime = engine_constant::MAX_DELTA_TIME;
		}

		// Add calculated time since frame to accumulator
		accumulator += loopDeltaTime;

		// Iterate steps at fixed rate until we can't do another
		while (accumulator >= engine_constant::PHYSICS_DELTA_TIME)
		{
			// Perform iterative game logic
			if (!rooms.empty()) {
				rooms[currentRoomIndex]->step();
				accumulator -= engine_constant::PHYSICS_DELTA_TIME;
			}
			else
			{
				cout << "No rooms exist" << endl;
				exit(1);
			}
		}

		// Interpolate state for a smooth simulation
		// NOTE: technically this makes the simulation lag by one PHYSICS_DELTA_TIME
		//       but it should be unnoticable if our PHYSICS_DELTA_TIME is at least
		//       twice the frame rate
		rooms[currentRoomIndex]->interpolateState(-accumulator / engine_constant::PHYSICS_DELTA_TIME);


        // DRAW WORLD
        rooms[currentRoomIndex]->draw(window);

        // DRAW GUI
        rooms[currentRoomIndex]->drawHUD(window);
    }
}
