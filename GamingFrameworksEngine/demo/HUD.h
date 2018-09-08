#pragma once
#include "../header/Overlay.h"

class HUD : public Overlay 
{
private:
	int xSize;
    int ySize;

public:
	HUD(State startState);
};