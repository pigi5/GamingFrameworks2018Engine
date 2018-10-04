#pragma once

#include <vector>
#include "Room.h"

// Singleton pattern via Martin York - https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class Engine
{
public:
    static Engine& getInstance()
    {
        // Instance created on first call to function
        static Engine instance;
        return instance;
    }
private:
    Room* currentRoom;
	sf::RenderWindow window;
    Engine();

    void loadConfig();
    void unloadConfig();
public:
    // Runs the main game loop
    void run();

    // Functions that should not exist in the Singleton pattern
    Engine(Engine const&) = delete;
    void operator=(Engine const&) = delete;
};

