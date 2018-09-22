#pragma once

#include "Actor.h"
#include "OverlayType.h"

// An object that that is drawn to the HUD. Its position is
// in screen coordinates
class Overlay : public Actor
{
public:
    Overlay(const OverlayType*, State&);
};