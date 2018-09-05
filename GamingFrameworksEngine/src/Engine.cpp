#include <chrono>
#include <iostream>
#include "../header/Engine.h"
#include "../header/Constants.h"

Engine::Engine(std::vector<Room*> rooms)
{
	this->rooms = rooms;
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
	sf::View camera = window->getView();
	camera.setCenter(0, 0);
	camera.move(200, 200);
	sf::View fixed = window->getView();
    double loopDeltaTime;
	double accumulator = 0.0;

	previousTime = std::chrono::high_resolution_clock::now();
    while (go) {

		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window->close();
				exit(0);
			}
		}

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

		window->clear(sf::Color::Black);

        // DRAW WORLD
		window->setView(camera);
		rooms[currentRoomIndex]->draw(window, &camera);

        // DRAW GUI
		window->setView(fixed);
        rooms[currentRoomIndex]->drawHUD(window, &fixed);

		window->display();
    }
}
