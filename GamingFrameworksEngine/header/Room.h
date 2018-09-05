#pragma once

#include <list>
#include "Actor.h"
#include "Overlay.h"

// Similar to a stage.
class Room
{
private:
    std::list<Actor*> actors;
    std::list<Overlay*> overlays;
public:
    Room(std::list<Actor*> actors, std::list<Overlay*> overlays);
    ~Room();

    void step();
	void interpolateState(float);
    void draw(sf::RenderWindow* window);
    void drawHUD(sf::RenderWindow* window);
};