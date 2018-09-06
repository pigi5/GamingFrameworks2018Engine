#pragma once
#include "../header/Actor.h"

class Wall : public Actor {
private:
	int xSize = 200;
    int ySize = 20;
public:
	Wall();
};