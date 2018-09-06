#pragma once
#include "../header/Actor.h"
#include "../header/Command.h"
#include "../header/Button.h"
#include <vector>

class Player : public Actor {
private:
	int xSize = 30, ySize = 30;
	vector<Button> buttons;
public:
	Player();
	void step();
	void draw(sf::RenderWindow* window, sf::View* view);
};