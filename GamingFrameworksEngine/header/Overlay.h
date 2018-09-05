#pragma once

#include "Actor.h"

// An object that that is drawn to the HUD. Its position is
// in screen coordinates
class Overlay : Actor
{
public:
    // Draws the object to the screen
    virtual void draw(sf::RenderWindow* window) = 0;
	void step();
	void onCollision(const Actor* other);
};