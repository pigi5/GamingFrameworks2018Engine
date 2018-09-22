#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <deque>
#include <sstream>
#include <numeric>
#include "../header/Engine.h"
#include "../header/Constants.h"
#include "../header/InputHandler.h"
#include "../header/Text.h"
#include "../header/Configurable.h"

Engine::Engine()
{
    for (const auto& pair : Room::objectMap)
    {
        if (pair.second->is_default)
        {
            currentRoom = pair.second;
            break;
        }
    }

    if (currentRoom == NULL)
    {
        throw ConfigurationError("No default room declared.");
    }
}

// Runs the game loop until complete.
void Engine::run(sf::RenderWindow* window)
{
    bool go = true;

	// Initialize input handler
	InputHandler input;

    // Defines the current room locally so that the room cannot be changed during loops
    Room* localCurrentRoom;

    /*
    // Load resources
    loadAll<Material>("../resources/materials");
    loadAll<ActorType>("../resources/actor_types");
    TODO load Rooms*/

    // Setup camera
	sf::View camera = window->getView();
	camera.setCenter(0, 0);
	camera.move(200, 200);
	sf::View fixed = window->getView();

	// Disable Repeat Delay
	//window->setKeyRepeatEnabled(false);

    // Variables for game loop timing
	std::chrono::steady_clock::time_point currentTime;
	std::chrono::steady_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
    double loopDeltaTime;
	double accumulator = 0.0;
    std::deque<double> loopDeltaTimes;
    double averageLoopDeltaTime;
    double averageFrameRate;
	Text frameCounter("", window->getSize().x - 70, 20);
	frameCounter.setSize(16);

    // Run the game loop
    while (go) 
    {
        localCurrentRoom = currentRoom;

		// Reset the timer
		currentTime = std::chrono::high_resolution_clock::now();
        // Calculate the time elapsed (in seconds) between the beginning of the last loop and now
		loopDeltaTime = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(currentTime - previousTime).count();
		previousTime = currentTime;

        // Sleep if necessary to cap framerate
        if (engine_constant::MIN_FRAME_TIME > 0 && loopDeltaTime < engine_constant::MIN_FRAME_TIME)
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(engine_constant::MIN_FRAME_TIME - loopDeltaTime));
        }
        
        // Calculate average framerate
        if (loopDeltaTimes.size() >= 10)
        {
            loopDeltaTimes.pop_front();
        }
        loopDeltaTimes.push_back(loopDeltaTime);
        averageLoopDeltaTime = std::accumulate(loopDeltaTimes.begin(), loopDeltaTimes.end(), 0.0) / loopDeltaTimes.size();
        averageFrameRate = 1.0 / averageLoopDeltaTime;

		ostringstream sstr;
		sstr << setprecision(4) << averageFrameRate;
		frameCounter.setText(sstr.str());

		// Cap the maximum calculated time between frames
		if (engine_constant::DELTA_TIME_CAP > 0 && loopDeltaTime > engine_constant::DELTA_TIME_CAP)
		{
			loopDeltaTime = engine_constant::DELTA_TIME_CAP;
		}

		// Add calculated time since frame to accumulator
		accumulator += loopDeltaTime;

		// Iterate steps at fixed rate until we can't do another
		while (accumulator >= engine_constant::PHYSICS_DELTA_TIME)
		{
			// Perform iterative game logic
			currentRoom->step();
			accumulator -= engine_constant::PHYSICS_DELTA_TIME;
		}

		// Interpolate state for a smooth simulation
		// NOTE: technically this makes the simulation lag by one PHYSICS_DELTA_TIME
		//       but it should be unnoticable if our PHYSICS_DELTA_TIME is at least
		//       twice the frame rate
		currentRoom->interpolateState(accumulator / engine_constant::PHYSICS_DELTA_TIME);

        // Clear window
		window->clear(sf::Color::Black);

        // Draw world
		window->setView(camera);
		currentRoom->draw(window, &camera);

        // Draw GUI
		window->setView(fixed);
        currentRoom->drawHUD(window, &fixed);
		frameCounter.draw(window);

        // Refresh window
		window->display();


        // Check if window is closed
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
            {
				window->close();
                go = false;
			}

			if (event.type == sf::Event::KeyPressed) {
				//input.handlePress(event.key.code, true);
			}
			if (event.type == sf::Event::KeyReleased) {
				//input.handlePress(event.key.code, false);
			}
		}
    }

    /*
    // Unload resources
    unloadAll<ActorType>();
    unloadAll<Material>();*/
}
