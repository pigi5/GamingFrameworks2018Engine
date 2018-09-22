#include "../header/Overlay.h"

Overlay::Overlay(const OverlayType* overlayType, State& startState) : Actor(overlayType, startState)
{
    yAcceleration = 0;
}