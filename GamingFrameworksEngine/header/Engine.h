#pragma once

#include <vector>
#include "Room.h"
#include "Audio.h"

class Engine
{
private:
    std::vector<Room*> rooms;
	Audio demoSound;
	int currentRoomIndex;
public:
    Engine(std::vector<Room*> rooms);
    ~Engine();

    // Runs the main game loop
    void run(sf::RenderWindow* window);
};

