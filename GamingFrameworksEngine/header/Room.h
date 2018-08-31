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
    Room();
    ~Room();

    void step(double);
    void draw(double);
    void drawHUD(double);
};