#pragma once

#include "Actor.h"

// An object that that is drawn to the HUD. Its position is
// in screen coordinates
class Overlay : Actor
{
public:
    // Draws the object to the screen
    void draw();
};