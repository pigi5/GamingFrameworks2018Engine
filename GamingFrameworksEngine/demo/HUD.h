#pragma once
#include "../header/Overlay.h"

class HUD : public Overlay {
private:
	Shape* shape;
	int xPos, yPos, xSize, ySize;

public:
	HUD();
	~HUD();
	void draw(sf::RenderWindow* window, sf::View* view);
};