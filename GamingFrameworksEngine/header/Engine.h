#pragma once

#include <map>
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
    std::map<std::string, Room*>::iterator currentRoomIterator;
	sf::RenderWindow window;
    Engine();

public:
    // Runs the main game loop
    void run();

    void changeRoom(int);
    void setRoom(std::string);

    // Functions that should not exist in the Singleton pattern
    Engine(Engine const&) = delete;
    void operator=(Engine const&) = delete;
};

