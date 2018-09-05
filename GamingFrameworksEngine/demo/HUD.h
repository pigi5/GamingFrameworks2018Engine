#pragma once
#include "../header/Overlay.h"

class HUD : public Overlay {
private:
	Shape* shape;
	int xPos, yPos;

public:
	HUD();
	void draw(sf::RenderWindow* window);
};