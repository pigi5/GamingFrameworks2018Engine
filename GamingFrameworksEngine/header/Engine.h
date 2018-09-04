#pragma once

#include <vector>
#include "Room.h"

class Engine
{
private:
    std::vector<Room*> rooms;
    int currentRoomIndex;
public:
    Engine();
    ~Engine();

    // Runs the main game loop
    void run(sf::RenderWindow* window);
};

