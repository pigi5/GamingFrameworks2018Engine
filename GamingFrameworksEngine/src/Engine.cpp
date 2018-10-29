#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <deque>
#include <sstream>
#include <numeric>
#include "../header/ButtonStates.h"
#include "../header/Engine.h"
#include "../header/Constants.h"
#include "../header/InputHandler.h"
#include "../header/Text.h"
#include "../header/Configurable.h"
#include "../header/Sprite.h"
#include "../header/Utils.h"

std::unordered_map<std::string, int> Engine::defaultAttributes;

void Engine::loadAttributes(std::string projectDir)
{
    YAML::Node config;
    try 
    {
        config = YAML::LoadFile(projectDir + "/globals.yml");
    }
    catch (const YAML::BadFile& e) 
    {
        // Create file if it doesn't exist
        std::ofstream fout(projectDir + "/globals.yml");
        fout.close();
        return;
    }
    for (auto attribute : config)
    {
        std::string attrName = attribute.first.as<std::string>();

        if (!defaultAttributes.emplace(attrName, attribute.second.as<int>()).second)
        {
            // if key already existed, throw error
            std::stringstream errorMessage;
            errorMessage << "Global attribute name \"" << attrName;
            errorMessage << "\" is not unique.";
            throw ConfigurationError(errorMessage.str());
        }
    }
}

void Engine::saveAttributes(std::string projectDir)
{
    // create YAML emitter
    YAML::Emitter emitter;
    emitter << YAML::BeginDoc << YAML::BeginMap;
    // iterate all attributes in memory
    for (const auto& pair :defaultAttributes)
    {
        // write attribute to emitter
        emitter << YAML::Key << pair.first << YAML::Value << pair.second;
    }
    emitter << YAML::EndMap << YAML::EndDoc;
    // write YAML to file
    std::ofstream fout(projectDir + "/globals.yml");
    fout << emitter.c_str();
    fout.close();
}

Engine::Engine()
{
    
}

// Runs the game loop until complete.
void Engine::run()
{
    // COMMENT THIS OUT FOR LOGGING
    //engine_util::logger.setActive(false);

    engine_util::logger << "Running game " << std::endl;

	sf::VideoMode defaultMode = sf::VideoMode(800, 600);
	this->window.create(defaultMode, "Game Window", sf::Style::Titlebar | sf::Style::Close);
    bool go = true;
	// Initialize input handler
	InputHandler input;

    // Defines the current room locally so that the room cannot be changed during loops
    Room* localCurrentRoom;

    // find default room
    for (auto it = Room::objectMap.begin(); it != Room::objectMap.end(); ++it)
    {
        if (it->second->is_default)
        {
            currentRoomIterator = it;
            currentRoomIterator->second->setEngine(this);
            break;
        }
    }

    // Set global attributes
    globalAttributes.clear();
    globalAttributes.insert(defaultAttributes.begin(), defaultAttributes.end());

    if (currentRoomIterator->second == NULL)
    {
		window.close();
        throw ConfigurationError("No default room declared.");
    }

    // Setup camera
	sf::View camera = window.getView();
	camera.setCenter(0, 0);
	camera.move(200, 200);
	sf::View fixed = window.getView();

	// Disable Repeat Delay
	window.setKeyRepeatEnabled(false);

    // Variables for game loop timing
	std::chrono::steady_clock::time_point currentTime;
	std::chrono::steady_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
    double loopDeltaTime;
	double accumulator = 0.0;
    std::deque<double> loopDeltaTimes;
    double averageLoopDeltaTime;
    double averageFrameRate;
	Text frameCounter("", window.getSize().x - 70, 20);
	frameCounter.setSize(16);

    // Run the game loop
    while (go) 
    {
        localCurrentRoom = currentRoomIterator->second;

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
            // input handling
		    sf::Event event;
		    while (window.pollEvent(event))
		    {
                // Check if window is closed
			    if (event.type == sf::Event::Closed)
                {
				    window.close();
                    go = false;
			    }

			    if (event.type == sf::Event::KeyPressed) 
				{
				    input.handlePress(event.key.code, localCurrentRoom);
				    //cout << "Handling button press" << event.key.code << endl;
			    }
			    if (event.type == sf::Event::KeyReleased) 
				{
				    input.handleRelease(event.key.code, localCurrentRoom);
				    //cout << "Handling button release" << event.key.code << endl;
			    }

				if (event.type == sf::Event::MouseButtonPressed) 
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i mouseCoords(event.mouseButton.x, event.mouseButton.y);
						sf::Vector2f mouseToWorld = window.mapPixelToCoords(mouseCoords, camera);
						input.handleMousePress(mouseToWorld.x, mouseToWorld.y, localCurrentRoom);
					}
				}

				if (event.type == sf::Event::MouseButtonReleased)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i mouseCoords(event.mouseButton.x, event.mouseButton.y);
						sf::Vector2f mouseToWorld = window.mapPixelToCoords(mouseCoords, camera);
						input.handleMouseRelease(mouseToWorld.x, mouseToWorld.y, localCurrentRoom);
					}
				}
		    }
			input.handleHolds(localCurrentRoom);

			// Perform iterative game logic
			localCurrentRoom->step();
			accumulator -= engine_constant::PHYSICS_DELTA_TIME;
		}

		// Interpolate state for a smooth simulation
		// NOTE: technically this makes the simulation lag by one PHYSICS_DELTA_TIME
		//       but it should be unnoticable if our PHYSICS_DELTA_TIME is at least
		//       twice the frame rate
		localCurrentRoom->interpolateState(accumulator / engine_constant::PHYSICS_DELTA_TIME);
        State followedActorState = localCurrentRoom->getFollowedActor()->getCurrentState();
        camera.setCenter(followedActorState.xPosition, followedActorState.yPosition);

        // Clear window
		window.clear(sf::Color::Black);

        // Draw world
		window.setView(camera);
		localCurrentRoom->draw(&window, &camera);

        // Draw GUI
		window.setView(fixed);
        localCurrentRoom->drawHUD(&window, &fixed);
		frameCounter.draw(&window);

        // Refresh window
		window.display();
    }
}

void Engine::changeRoom(int offset)
{
    if (offset < 0)
    {
        for (int i = 0; i > offset; i--)
        {
            if (currentRoomIterator == Room::objectMap.begin())
            {
                throw ConfigurationError("Can't go before first room.");
            }
            currentRoomIterator--;
        }
    }
    else
    {
        for (int i = 0; i < offset; i++)
        {
            if (currentRoomIterator == Room::objectMap.end())
            {
                throw ConfigurationError("Can't go past last room.");
            }
            currentRoomIterator++;
        }
    }
    
    currentRoomIterator->second->setEngine(this);
}

void Engine::setRoom(std::string name)
{
    currentRoomIterator = Room::objectMap.find(name);

    if (currentRoomIterator == Room::objectMap.end())
    {
        throw ConfigurationError("Room " + name + " does not exist.");
    }
    
    currentRoomIterator->second->setEngine(this);
}
