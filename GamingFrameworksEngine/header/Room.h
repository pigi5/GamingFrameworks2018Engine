#pragma once

#include <list>
#include "Actor.h"
#include "Music.h"

// Similar to a stage.
class Room
{
private:
    std::list<Actor*> actors;
    std::list<Actor*> overlays;
	Music music;
public:
    Room(std::list<Actor*> actors, std::list<Actor*> overlays, string musicFile);
    ~Room();

    void step();
	void interpolateState(float);
    void draw(sf::RenderWindow* window, sf::View* view);
    void drawHUD(sf::RenderWindow* window, sf::View* view);
};